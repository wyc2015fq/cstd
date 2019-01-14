#include <cstddef>
#include <iostream>
#include <algorithm>
#pragma once

#include "ctc_helper.h"
#include "gpu_ctc_kernels.h"
#include "reduce.cuh"

template <typename ProbT>
class GpuCTC
{
public:
  GpuCTC(int alphabet_size,
    int minibatch,
    void* workspace,
    CUstream stream,
    int blank_label) :
    out_dim_(alphabet_size), minibatch_(minibatch),
    gpu_workspace_(workspace), stream_(stream),
    blank_label_(blank_label) {};

  // Noncopyable
  GpuCTC(const GpuCTC &) = delete;
  GpuCTC & operator=(const GpuCTC &) = delete;

  ctcStatus_t
    cost_and_grad(const ProbT* const activations,
      ProbT* grads,
      ProbT* costs,
      const int* const flat_labels,
      const int* const label_lengths,
      const int* const input_lengths);

  ctcStatus_t
    score_forward(const ProbT* const activations,
      ProbT* costs,
      const int* const flat_labels,
      const int* const label_lengths,
      const int* const input_lengths);

private:

  template<int NT, int VT>
  ctcStatus_t launch_alpha_beta_kernels(const ProbT* const probs,
    ProbT* grads,
    bool compute_alpha,
    bool compute_beta);

  ctcStatus_t
    launch_gpu_kernels(const ProbT* const probs,
      ProbT* grads,
      size_t config,
      bool launch_alpha,
      bool launch_beta);

  ctcStatus_t
    setup_gpu_metadata(const int* const flat_labels,
      const int* const label_lengths,
      const int* const input_lengths);

  ctcStatus_t
    create_metadata_and_choose_config(const int* const label_lengths,
      const int* const flat_labels,
      const int* const input_lengths,
      size_t & best_config);

  ctcStatus_t
    compute_probs(const ProbT* const activations);

  ctcStatus_t
    compute_cost_and_score(const ProbT* const activations,
      ProbT* grads,
      ProbT* costs,
      const int* const flat_labels,
      const int* const label_lengths,
      const int* const input_lengths,
      bool compute_alpha,
      bool compute_betas_and_grad);


  int out_dim_; // Number of characters plus blank
  int minibatch_;

  int S_;
  int T_;

  int activation_cols_; // Number of columns in activations

  CUstream stream_;
  int blank_label_;

  void* gpu_workspace_; // Buffer for all temporary GPU memory
  int* utt_length_; // T
  int* label_sizes_; // L
  int* repeats_; // repeats_
  int* label_offsets_;
  int* labels_without_blanks_;
  int* labels_with_blanks_;
  ProbT* alphas_;
  ProbT* nll_forward_;
  ProbT* nll_backward_;
  ProbT* denoms_; // Temporary storage for denoms for softmax
  ProbT* probs_; // Temporary storage for probabilities (softmax output)
};

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::setup_gpu_metadata(const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths)
{
  size_t gpu_bytes_used = 0;
  nll_forward_ =
    reinterpret_cast<ProbT*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(ProbT);
  nll_backward_ =
    reinterpret_cast<ProbT*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(ProbT);
  repeats_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(int);
  label_offsets_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(int);
  // This is the max of all S and T for all valid examples in the minibatch.
  // A valid example is one for which L + repeats <= T
  S_ = 0;
  T_ = 0;
  // This is the max of all timesteps, valid or not. Needed to compute offsets
  int Tmax = 0;
  // This is the max of all labels, valid or not. Needed to compute offsets
  int Lmax = 0;
  int total_label_length = 0;
  constexpr int cpu_buffer_size = 64;
  int repeats[cpu_buffer_size];
  int label_offsets[cpu_buffer_size];
  const int num_passes = ctc_helper::div_up(minibatch_, cpu_buffer_size);
  cudaError_t cuda_status;
  for (int pass = 0; pass < num_passes; ++pass) {
    const int start_idx = pass * cpu_buffer_size;
    const int end_idx = std::min(minibatch_, (pass + 1) * cpu_buffer_size);
    for (int j = start_idx; j < end_idx; ++j) {
      const int L = label_lengths[j];
      const int local_T = input_lengths[j];
      const int* label_ptr = &(flat_labels[total_label_length]);
      label_offsets[j % cpu_buffer_size] = total_label_length;
      total_label_length += L;
      int repeat_counter = 0;
      for (int i = 1; i < L; ++i) {
        repeat_counter += (label_ptr[i] == label_ptr[i - 1]);
      }
      repeats[j % cpu_buffer_size] = repeat_counter;
      const bool valid_label = ((L + repeat_counter) <= local_T);
      // Only update S and T if label is valid
      S_ = (valid_label) ? std::max(S_, L) : S_;
      T_ = (valid_label) ? std::max(T_, local_T) : T_;
      Tmax = std::max(Tmax, local_T);
      Lmax = std::max(Lmax, L);
    }
    cuda_status = cudaMemcpyAsync(&(repeats_[start_idx]), repeats,
      (end_idx - start_idx) * sizeof(int),
      cudaMemcpyHostToDevice, stream_);
    if (cuda_status != cudaSuccess) {
      return CTC_STATUS_MEMOPS_FAILED;
    }
    cuda_status = cudaMemcpyAsync(&(label_offsets_[start_idx]), label_offsets,
      (end_idx - start_idx) * sizeof(int),
      cudaMemcpyHostToDevice, stream_);
    if (cuda_status != cudaSuccess) {
      return CTC_STATUS_MEMOPS_FAILED;
    }
  }
  S_ = 2 * S_ + 1;
  const int Smax = 2 * Lmax + 1;
  activation_cols_ = minibatch_ * Tmax;
  // Allocate memory for T
  utt_length_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(int);
  cuda_status = cudaMemcpyAsync(utt_length_, input_lengths,
    minibatch_ * sizeof(int),
    cudaMemcpyHostToDevice, stream_);
  if (cuda_status != cudaSuccess) {
    return CTC_STATUS_MEMOPS_FAILED;
  }
  label_sizes_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += minibatch_ * sizeof(int);
  cuda_status = cudaMemcpyAsync(label_sizes_, label_lengths,
    minibatch_ * sizeof(int),
    cudaMemcpyHostToDevice, stream_);
  if (cuda_status != cudaSuccess) {
    return CTC_STATUS_MEMOPS_FAILED;
  }
  labels_without_blanks_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += Lmax * minibatch_ * sizeof(int);
  cuda_status = cudaMemcpyAsync(labels_without_blanks_, flat_labels,
    total_label_length * sizeof(int),
    cudaMemcpyHostToDevice, stream_);
  if (cuda_status != cudaSuccess) {
    return CTC_STATUS_MEMOPS_FAILED;
  }
  labels_with_blanks_ =
    reinterpret_cast<int*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += Smax * minibatch_ * sizeof(int);
  alphas_ =
    reinterpret_cast<ProbT*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += (S_ * T_) * minibatch_ * sizeof(ProbT);
  denoms_ =
    reinterpret_cast<ProbT*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += activation_cols_ * sizeof(ProbT);
  probs_ =
    reinterpret_cast<ProbT*>(static_cast<char*>(gpu_workspace_) +
      gpu_bytes_used);
  gpu_bytes_used += out_dim_ * activation_cols_ * sizeof(ProbT);
  return CTC_STATUS_SUCCESS;
}

template<typename ProbT>
template<int NT, int VT>
ctcStatus_t GpuCTC<ProbT>::launch_alpha_beta_kernels(const ProbT* const probs,
  ProbT* grads,
  bool compute_alpha,
  bool compute_beta)
{
  // One thread block per utterance
  const int grid_size = minibatch_;
  // The data is laid out so that the next timestep is minibatch entries
  // away
  const int stride = minibatch_;
  if (compute_alpha)
    compute_alpha_kernel<ProbT, NT, VT> << < grid_size, NT, 0, stream_ >> >
    (probs, label_sizes_, utt_length_,
      repeats_, labels_without_blanks_, label_offsets_,
      labels_with_blanks_, alphas_, nll_forward_,
      stride, out_dim_, S_, T_, blank_label_);
  if (compute_beta) {
    compute_betas_and_grad_kernel<ProbT, NT, VT> << < grid_size, NT, 0, stream_ >> >
      (probs, label_sizes_, utt_length_, repeats_,
        labels_with_blanks_, alphas_, nll_forward_, nll_backward_,
        grads, stride, out_dim_, S_, T_, blank_label_);
    cudaStreamSynchronize(stream_);
  }
  cudaError_t err = cudaGetLastError();
  if (err != cudaSuccess)
    return CTC_STATUS_EXECUTION_FAILED;
  return CTC_STATUS_SUCCESS;
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::create_metadata_and_choose_config(const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths,
  size_t & best_config)
{
  // Setup the metadata for GPU
  ctcStatus_t status = setup_gpu_metadata(flat_labels, label_lengths, input_lengths);
  if (status != CTC_STATUS_SUCCESS) {
    return status;
  }
  constexpr int num_configs = 12;
  int config_NT[num_configs] =
  { 32, 64, 128, 64, 128, 32, 64, 128, 64, 128, 128, 128 };
  int config_VT[num_configs] =
  { 1,  1,   1,  3,   2,  9,  6,   4,  9,   6,   9,  10 };
  best_config = 0;
  for (int i = 0; i < num_configs; ++i) {
    if ((config_NT[i] * config_VT[i]) >= S_) {
      break;
    }
    else {
      best_config++;
    }
  }
  if (best_config >= num_configs) {
    return CTC_STATUS_UNKNOWN_ERROR;
  }
  return CTC_STATUS_SUCCESS;
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::launch_gpu_kernels(const ProbT* const probs,
  ProbT* grads,
  size_t config,
  bool l_a,
  bool l_b)
{
  switch (config) {
  case 0:
  {return launch_alpha_beta_kernels<32, 1>(probs, grads, l_a, l_b); }
  case 1:
  {return launch_alpha_beta_kernels<64, 1>(probs, grads, l_a, l_b); }
  case 2:
  {return launch_alpha_beta_kernels<128, 1>(probs, grads, l_a, l_b); }
  case 3:
  {return launch_alpha_beta_kernels<64, 3>(probs, grads, l_a, l_b); }
  case 4:
  {return launch_alpha_beta_kernels<128, 2>(probs, grads, l_a, l_b); }
  case 5:
  {return launch_alpha_beta_kernels<32, 9>(probs, grads, l_a, l_b); }
  case 6:
  {return launch_alpha_beta_kernels<64, 6>(probs, grads, l_a, l_b); }
  case 7:
  {return launch_alpha_beta_kernels<128, 4>(probs, grads, l_a, l_b); }
  case 8:
  {return launch_alpha_beta_kernels<64, 9>(probs, grads, l_a, l_b); }
  case 9:
  {return launch_alpha_beta_kernels<128, 6>(probs, grads, l_a, l_b); }
  case 10:
  {return launch_alpha_beta_kernels<128, 9>(probs, grads, l_a, l_b); }
  case 11:
  {return launch_alpha_beta_kernels<128, 10>(probs, grads, l_a, l_b); }
  }
  return CTC_STATUS_EXECUTION_FAILED;
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::compute_probs(const ProbT* const activations)
{
  cudaError_t cuda_status;
  cuda_status =
    cudaMemcpyAsync(probs_, activations,
      activation_cols_ * out_dim_ * sizeof(ProbT),
      cudaMemcpyDeviceToDevice, stream_);
  if (cuda_status != cudaSuccess) {
    return CTC_STATUS_MEMOPS_FAILED;
  }
  // Numerically stable SM
  ctcStatus_t ctc_status =
    reduce_max(probs_, denoms_, out_dim_,
      activation_cols_, 1, stream_);
  if (ctc_status != CTC_STATUS_SUCCESS) {
    return ctc_status;
  }
  // Kernel launch to subtract maximum
  const int NT = 128;
  const int VT = 1;
  const int NV = NT * VT;
  const int num_elements = out_dim_ * activation_cols_;
  const int grid_size = ctc_helper::div_up(num_elements, NV);
  prepare_stable_SM_kernel<ProbT, VT> << < grid_size, NT, 0, stream_ >> >
    (ctc_helper::identity<ProbT>(), probs_,
      denoms_, out_dim_, num_elements);
  // Reduce along columns to calculate denominator
  ctc_status =
    reduce_exp(probs_, denoms_, out_dim_,
      activation_cols_, 1, stream_);
  if (ctc_status != CTC_STATUS_SUCCESS)
    return ctc_status;
  // Kernel launch to calculate probabilities
  compute_probs_kernel<ProbT, VT> << < grid_size, NT, 0, stream_ >> >
    (ctc_helper::exponential<ProbT>(), probs_,
      denoms_, out_dim_, num_elements);
  return CTC_STATUS_SUCCESS;
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::compute_cost_and_score(const ProbT* const activations,
  ProbT* grads,
  ProbT* costs,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths,
  bool compute_alpha,
  bool compute_betas_and_grad)
{
  size_t best_config;
  ctcStatus_t status = create_metadata_and_choose_config(flat_labels,
    label_lengths,
    input_lengths,
    best_config);
  if (status != CTC_STATUS_SUCCESS) {
    return status;
  }
  status = compute_probs(activations);
  if (status != CTC_STATUS_SUCCESS) {
    return status;
  }
  launch_gpu_kernels(probs_, grads, best_config,
    compute_alpha, compute_betas_and_grad);
  cudaError_t cuda_status_mem, cuda_status_sync;
  cuda_status_mem = cudaMemcpyAsync(costs, nll_forward_,
    sizeof(ProbT) * minibatch_,
    cudaMemcpyDeviceToHost, stream_);
  cuda_status_sync = cudaStreamSynchronize(stream_);
  if (cuda_status_mem != cudaSuccess || cuda_status_sync != cudaSuccess) {
    return CTC_STATUS_MEMOPS_FAILED;
  }
  return CTC_STATUS_SUCCESS;
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::cost_and_grad(const ProbT* const activations,
  ProbT* grads,
  ProbT* costs,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths)
{
  if (activations == nullptr ||
    grads == nullptr ||
    costs == nullptr ||
    flat_labels == nullptr ||
    label_lengths == nullptr ||
    input_lengths == nullptr
    ) {
    return CTC_STATUS_INVALID_VALUE;
  }
  return compute_cost_and_score(activations, grads, costs, flat_labels,
    label_lengths, input_lengths, true, true);
}

template<typename ProbT>
ctcStatus_t
GpuCTC<ProbT>::score_forward(const ProbT* const activations,
  ProbT* costs,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths)
{
  if (activations == nullptr ||
    costs == nullptr ||
    flat_labels == nullptr ||
    label_lengths == nullptr ||
    input_lengths == nullptr
    ) {
    return CTC_STATUS_INVALID_VALUE;
  }
  return compute_cost_and_score(activations, nullptr, costs, flat_labels,
    label_lengths, input_lengths, true, false);
}



ctcStatus_t FUN(compute_ctc_loss)(const Dtype* const activations,
  Dtype* gradients,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths,
  int alphabet_size,
  int minibatch,
  Dtype *costs,
  void *workspace,
  ctcOptions options) {

  if (activations == nullptr ||
    flat_labels == nullptr ||
    label_lengths == nullptr ||
    input_lengths == nullptr ||
    costs == nullptr ||
    workspace == nullptr ||
    alphabet_size <= 0 ||
    minibatch <= 0)
    return CTC_STATUS_INVALID_VALUE;

  GpuCTC<Dtype> ctc(alphabet_size, minibatch, workspace, options.stream,
    options.blank_label);

  if (gradients != NULL)
    return ctc.cost_and_grad(activations, gradients, costs,
      flat_labels, label_lengths,
      input_lengths);
  else
    return ctc.score_forward(activations, costs, flat_labels,
      label_lengths, input_lengths);

}


ctcStatus_t FUN(get_workspace_size)(const int* const label_lengths,
  const int* const input_lengths,
  int alphabet_size, int minibatch,
  ctcOptions options,
  size_t* size_bytes)
{
  if (label_lengths == nullptr ||
    input_lengths == nullptr ||
    size_bytes == nullptr ||
    alphabet_size <= 0 ||
    minibatch <= 0)
    return CTC_STATUS_INVALID_VALUE;

  // This is the max of all S and T for all examples in the minibatch.
  int maxL = *std::max_element(label_lengths, label_lengths + minibatch);
  int maxT = *std::max_element(input_lengths, input_lengths + minibatch);

  const int S = 2 * maxL + 1;

  *size_bytes = 0;

  // GPU storage
  //nll_forward, nll_backward
  *size_bytes += 2 * sizeof(Dtype) * minibatch;

  //repeats
  *size_bytes += sizeof(int) * minibatch;

  //label offsets
  *size_bytes += sizeof(int) * minibatch;

  //utt_length
  *size_bytes += sizeof(int) * minibatch;

  //label lengths
  *size_bytes += sizeof(int) * minibatch;

  //labels without blanks - overallocate for now
  *size_bytes += sizeof(int) * maxL * minibatch;

  //labels with blanks
  *size_bytes += sizeof(int) * S * minibatch;

  //alphas
  *size_bytes += sizeof(Dtype) * S * maxT * minibatch;

  //denoms
  *size_bytes += sizeof(Dtype) * maxT * minibatch;

  //probs (since we will pass in activations)
  *size_bytes += sizeof(Dtype) * alphabet_size * maxT * minibatch;


  return CTC_STATUS_SUCCESS;
}

void FUN(ExtractInputData)(int T_, int N_, int C_, int blank_index_, const Dtype* seq_ind_data, const Dtype* labels_data, vector<int>* flat_labels, vector<int>* label_lengths, vector<int>* input_lengths) {
  const Dtype* seq_ind = seq_ind_data;
  const Dtype* target_seq = labels_data;
  flat_labels->clear();
  flat_labels->reserve(T_ * N_);  // maximum required
  label_lengths->resize(N_);
  input_lengths->resize(N_);
  // compute the sequence length and label length
  int* seq_len = input_lengths->data();
  int* label_len = label_lengths->data();
  int label_offset = 0;
  //if (blank_index_ == -1) {
  if (blank_index_ == 0) {//modified by jxs
    label_offset = 1;
  }
  for (int n = 0; n < N_; ++n) {
    seq_len[n] = T_;  // default value is maximal allowed length
    label_len[n] = T_;  // default value is maximal allowed length
    const Dtype* seq = seq_ind + n;
    const Dtype* label = target_seq + n;
    // sequence indicators start with seq == 0.0 to indicate the start of a
    // sequence. Skip at t = 0, so start at t = 1
    seq += N_;
    for (int t = 1; t < T_; ++t) {
      if (static_cast<int>(*seq + 0.5) == 0) {
        seq_len[n] = t;
        break;
      }
      seq += N_;
    }
    // label indicators are negative if the sequence has ended
    for (int t = 0; t < T_; ++t) {
      if (*label < 0.0) {
        label_len[n] = t;
        break;
      }
      // Note that the blank label will be 0
      flat_labels->push_back(static_cast<int>(*label + 0.5) + label_offset);
      label += N_;
    }
    // if the label length is 0, the seq_len is 1 (0 following 0)
    // set seq_len to 0 in this case aswell, to skip this example
    if (label_len[n] == 0) {
      CHECK_LE(seq_len[n], 1);
      seq_len[n] = 0;
    }
    CHECK_LE(label_len[n], seq_len[n])
      << "The label length must be smaller or equals the sequence length!";
  }
}

void FUN(warp_ctc_loss_fwd)(int T_, int N_, int C_, int count, int blank_index_,
  const Dtype* bottom0_data, Dtype* bottom0_mdiff, const Dtype* bottom1_data,
  const Dtype* bottom2_data, const Dtype* bottom3_data, Dtype* top) {
  const Dtype* activations = bottom0_data;
  Dtype* gradients = bottom0_mdiff;
  const int alphabet_size = C_;
  const int minibatch = N_;
  int bottom_size = (bottom0_data != NULL) + (bottom1_data != NULL) + (bottom2_data != NULL) + (bottom3_data != NULL);
  vector<int> flat_labels_;
  vector<int> label_lengths_;
  vector<int> input_lengths_;
  label_lengths_.resize(N_);
  input_lengths_.resize(N_);
  vector<Dtype> costs(N_);
  flat_labels_.clear();
  if (bottom_size == 2) {//bottom[0]=activations, bottom[1] is labels, shape: Batchsize*seq len
    const Dtype* label_seq_d = bottom1_data;
    int label_len_per_batch = count / N_;
    for (int n = 0; n < N_; ++n) {
      int curlen = 0;
      for (int l = 0; l < label_len_per_batch; ++l) {
        int label = (int)label_seq_d[n * label_len_per_batch + l];
        if (label <= blank_index_) {
          continue;
        }
        flat_labels_.push_back(label);
        curlen++;
      }
      label_lengths_[n] = curlen;
      input_lengths_[n] = T_;
    }
  }
  else if (bottom_size == 3) {
    FUN(ExtractInputData)(T_, N_, C_, blank_index_, bottom1_data, bottom2_data, &flat_labels_, &label_lengths_, &input_lengths_);
  }
  else if (bottom_size == 4) {
    //Blob* seq_len_blob = bottom1_data;
    //Blob* lab_len_blob = bottom2_data;
    //Blob* label_seq_blob = bottom3_data;
    const Dtype* seq_len_d = bottom1_data;
    const Dtype* lab_len_d = bottom2_data;
    const Dtype* label_seq_d = bottom3_data;
    int accumulated = 0;
    int label_len_per_batch = count / N_;
    //CHECK_EQ(seq_len_blob->count(), lab_len_blob->count());
    for (int i = 0; i < count; ++i) {
      label_lengths_[i] = (int)lab_len_d[i];
      input_lengths_[i] = (int)seq_len_d[i];
      accumulated += (int)lab_len_d[i];
    }
    flat_labels_.clear();
    flat_labels_.reserve(accumulated);
    for (int n = 0; n < N_; ++n) {
      for (int t = 0; t < label_lengths_[n]; ++t) {
        flat_labels_.push_back((int)label_seq_d[n*label_len_per_batch + t]);
      }
    }
  }
  else {
    LOG(FATAL) << "Unsupported blobs shape";
  }
  //remove repeat blank labels
  size_t workspace_alloc_bytes_;
  ctcOptions options;

  cudaStream_t stream;
  CHECK_EQ(cudaStreamCreate(&stream), CUDA_SUCCESS);
  options.loc = CTC_GPU;
  options.stream = stream;

  options.blank_label = blank_index_;
  ctcStatus_t status = FUN(get_workspace_size)(label_lengths_.data(),
    input_lengths_.data(),
    alphabet_size,
    minibatch,
    options,
    &workspace_alloc_bytes_);
  CHECK_EQ(status, CTC_STATUS_SUCCESS) << "CTC Error: " << ctcGetStatusString(status);
  Buffer workspace_[1] = { 0 };
  if (workspace_->size< workspace_alloc_bytes_) {
    gpu_ReAlloc(workspace_, workspace_alloc_bytes_ * sizeof(char));
  }

  //cuda_compute_ctc_loss;
  status = FUN(compute_ctc_loss)(activations,
    gradients,
    flat_labels_.data(),
    label_lengths_.data(),
    input_lengths_.data(),
    alphabet_size,
    minibatch,
    costs.data(),
    workspace_->data,
    options
  );
  CHECK_EQ(status, CTC_STATUS_SUCCESS) << "CTC Error: " << ctcGetStatusString(status);
  // output loss
  Dtype loss;// = top_mdata()[0];
  loss = 0;
  int num = 1;
  for (int n = 0; n < N_; ++n) {
    if (costs[n] < std::numeric_limits<Dtype>::infinity()) {
      loss += costs[n];
      ++num;
    }
  }
  if (num==1) {
    int asdf = 0;
  }
  loss /= num;
  if (isnan(loss)) {
    int asdf = 0;
  }
  *top = loss;
  Free(workspace_);
#if 0
  int gcnt = bottom[0]->count();
  Dtype sumg = 0;
  for (int i = 0; i < gcnt; i++) {
    sumg += fabs(gradients[i]);
  }
  //LOG(INFO) << "mean ctc loss=" << loss << ",N_="<<N_<<",num="<<num << ", mean gradients="<<sumg/gcnt;
#endif
  CHECK_EQ(cudaStreamDestroy(stream), CUDA_SUCCESS);
  return;
}

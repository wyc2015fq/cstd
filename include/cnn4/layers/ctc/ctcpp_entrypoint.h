#include <cstddef>
#include <iostream>
#include <algorithm>
#pragma once

#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

#if !defined(CTC_DISABLE_OMP) && !defined(APPLE)
//#include <omp.h>
#endif

#include "ctc_helper.h"


class CpuCTC
{
public:
  // Noncopyable
  CpuCTC(int alphabet_size, int minibatch, void* workspace, int num_threads,
    int blank_label) :
    alphabet_size_(alphabet_size), minibatch_(minibatch),
    num_threads_(num_threads), workspace_(workspace),
    blank_label_(blank_label) {
#if defined(CTC_DISABLE_OMP) || defined(APPLE)
#else
    if (num_threads > 0) {
      //omp_set_num_threads(num_threads);
    }
    else {
      //num_threads_ = omp_get_max_threads();
    }
#endif
  };

  //CpuCTC(const CpuCTC &) = delete;
  //CpuCTC & operator=(const CpuCTC &) = delete;

  ctcStatus_t cost_and_grad(const Dtype* const activations,
    Dtype* grads,
    Dtype* costs,
    const int* const flat_labels,
    const int* const label_lengths,
    const int* const input_lengths);


  ctcStatus_t score_forward(const Dtype* const activations,
    Dtype* costs,
    const int* const flat_labels,
    const int* const label_lengths,
    const int* const input_lengths);

private:

  class CpuCTC_metadata
  {

  private:
    int setup_labels(const int* const labels, int blank_label, int L, int S);

  public:
    CpuCTC_metadata(int L, int S, int T, int mb, int alphabet_size,
      void* workspace, size_t bytes_used, int blank_label,
      const int* const labels);

    Dtype* alphas;
    Dtype* betas;
    int* labels_w_blanks;
    int* e_inc;
    int* s_inc;
    Dtype* output;
    int repeats;
  };

  int alphabet_size_; // Number of characters plus blank
  int minibatch_;
  int num_threads_;
  int blank_label_;
  void* workspace_;

  void softmax(const Dtype* const activations, Dtype* probs,
    const int* const input_lengths);

  bool cost_and_grad_kernel(Dtype* grad, const Dtype* const probs,
      const int* const labels, int T, int L,
      int mb, size_t bytes_used, Dtype* out);

  Dtype compute_alphas(const Dtype* probs, int repeats, int S, int T,
    const int* const e_inc,
    const int* const s_inc,
    const int* const labels,
    Dtype* alphas);

  Dtype compute_betas_and_grad(Dtype* grad, const Dtype* const probs,
    Dtype log_partition, int repeats,
    int S, int T, const int* const e_inc,
    const int* const s_inc,
    const int* const labels,
    Dtype* alphas,
    Dtype* betas,
    Dtype* output);
};

CpuCTC::CpuCTC_metadata::CpuCTC_metadata(int L, int S, int T, int mb,
  int alphabet_size,
  void* workspace, size_t bytes_used,
  int blank_label,
  const int* const labels)
{
  alphas = reinterpret_cast<Dtype*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(Dtype) * S * T;
  std::fill(alphas, alphas + S * T, ctc_helper::neg_inf<Dtype>());
  betas = reinterpret_cast<Dtype*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(Dtype) * S;
  std::fill(betas, betas + S, ctc_helper::neg_inf<Dtype>());
  labels_w_blanks = reinterpret_cast<int*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(int) * S;
  e_inc = reinterpret_cast<int*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(int) * S;
  s_inc = reinterpret_cast<int*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(int) * S;
  output = reinterpret_cast<Dtype*>(static_cast<char*>(workspace) + bytes_used);
  bytes_used += sizeof(Dtype) * alphabet_size;
  repeats = setup_labels(labels, blank_label, L, S);
}

int CpuCTC::CpuCTC_metadata::setup_labels(const int* const labels, int blank_label, int L, int S)
{
  int e_counter = 0;
  int s_counter = 0;
  s_inc[s_counter++] = 1;
  int repeats = 0;
  int i;
  for (i = 1; i < L; ++i) {
    if (labels[i - 1] == labels[i]) {
      s_inc[s_counter++] = 1;
      s_inc[s_counter++] = 1;
      e_inc[e_counter++] = 1;
      e_inc[e_counter++] = 1;
      ++repeats;
    }
    else {
      s_inc[s_counter++] = 2;
      e_inc[e_counter++] = 2;
    }
  }
  e_inc[e_counter++] = 1;
  for (i = 0; i < L; ++i) {
    labels_w_blanks[2 * i] = blank_label;
    labels_w_blanks[2 * i + 1] = labels[i];
  }
  labels_w_blanks[S - 1] = blank_label;
  return repeats;
}

void CpuCTC::softmax(const Dtype* const activations, Dtype* probs,
  const int* const input_lengths)
{
#pragma omp parallel for
  for (int mb = 0; mb < minibatch_; ++mb) {
    for (int c = 0; c < input_lengths[mb]; ++c) {
      int r;
      int col_offset = (mb + minibatch_ * c) * alphabet_size_;
      Dtype max_activation = -std::numeric_limits<Dtype>::infinity();
      for (r = 0; r < alphabet_size_; ++r) {
        max_activation = MAX(max_activation, activations[r + col_offset]);
      }
      Dtype denom = Dtype(0.);
      for (r = 0; r < alphabet_size_; ++r) {
        probs[r + col_offset] = exp(activations[r + col_offset] - max_activation);
        denom += probs[r + col_offset];
      }
      for (r = 0; r < alphabet_size_; ++r) {
        probs[r + col_offset] /= denom;
      }
    }
  }
}

bool CpuCTC::cost_and_grad_kernel(Dtype* grad, const Dtype* const probs,
  const int* const labels,
  int T, int L, int mb, size_t bytes_used, Dtype* out)
{
  const int S = 2 * L + 1; // Number of labels with blanks
  CpuCTC_metadata ctcm(L, S, T, mb, alphabet_size_, workspace_, bytes_used, blank_label_, labels);
  bool over_threshold = false;
  if (L + ctcm.repeats > T) {
    *out = 0;
    return over_threshold; // TODO, not right to return 0
  }
  Dtype llForward = compute_alphas(probs, ctcm.repeats, S, T, ctcm.e_inc,
    ctcm.s_inc, ctcm.labels_w_blanks,
    ctcm.alphas);
  Dtype llBackward = compute_betas_and_grad(grad, probs, llForward, ctcm.repeats,
    S, T, ctcm.e_inc, ctcm.s_inc,
    ctcm.labels_w_blanks,
    ctcm.alphas,
    ctcm.betas,
    ctcm.output);
  Dtype diff = (Dtype)fabs(llForward - llBackward);
  if (diff > ctc_helper::threshold) {
    over_threshold = true;
  }
  *out = -llForward;
  return over_threshold;
}

// Computes forward probabilities
Dtype CpuCTC::compute_alphas(const Dtype* probs, int repeats, int S, int T,
  const int* const e_inc,
  const int* const s_inc,
  const int* const labels,
  Dtype* alphas)
{
  int i;
  int start = (((S / 2) + repeats - T) < 0) ? 0 : 1,
    end = S > 1 ? 2 : 1;
  for (i = start; i < end; ++i) {
    alphas[i] = log(probs[labels[i]]);
  }
  for (int t = 1; t < T; ++t) {
    int remain = (S / 2) + repeats - (T - t);
    if (remain >= 0) {
      start += s_inc[remain];
    }
    if (t <= (S / 2) + repeats) {
      end += e_inc[t - 1];
    }
    int startloop = start;
    int idx1 = t * S, idx2 = (t - 1) * S, idx3 = t * (alphabet_size_ * minibatch_);
    if (start == 0) {
      alphas[idx1] = alphas[idx2] + log(probs[blank_label_ + idx3]);
      startloop += 1;
    }
    for (i = startloop; i < end; ++i) {
      Dtype prev_sum = ctc_helper::log_plus<Dtype>()(alphas[i + idx2], alphas[(i - 1) + idx2]);
      // Skip two if not on blank and not on repeat.
      if (labels[i] != blank_label_ && i != 1 && labels[i] != labels[i - 2]) {
        prev_sum = ctc_helper::log_plus<Dtype>()(prev_sum, alphas[(i - 2) + idx2]);
      }
      alphas[i + idx1] = prev_sum + log(probs[labels[i] + idx3]);
    }
  }
  Dtype loglike = ctc_helper::neg_inf<Dtype>();
  for (i = start; i < end; ++i) {
    loglike = ctc_helper::log_plus<Dtype>()(loglike, alphas[i + (T - 1) * S]);
  }
  return loglike;
}

// Starting from T, we sweep backward over the alpha array computing one column
// of betas as we go.  At each position we can update product alpha * beta and then
// sum into the gradient associated with each label.
// NOTE computes gradient w.r.t UNNORMALIZED final layer activations.
// Assumed passed in grads are already zeroed!
Dtype CpuCTC::compute_betas_and_grad(Dtype* grad, const Dtype* const probs,
  Dtype log_partition, int repeats,
  int S, int T, const int* const e_inc,
  const int* const s_inc,
  const int* const labels,
  Dtype* alphas,
  Dtype* betas,
  Dtype* output)
{
  int i;
  int start = S > 1 ? (S - 2) : 0,
    end = (T > (S / 2) + repeats) ? S : S - 1;
  std::fill(output, output + alphabet_size_, ctc_helper::neg_inf<Dtype>());
  //set the starting values in the beta column at the very right edge
  for (i = start; i < end; ++i) {
    betas[i] = log(probs[labels[i] + (T - 1) * (alphabet_size_ * minibatch_)]);
    //compute alpha * beta in log space at this position in (S, T) space
    alphas[i + (T - 1) * S] += betas[i];
    //update the gradient associated with this label
    //essentially performing a reduce-by-key in a sequential manner
    output[labels[i]] =
      ctc_helper::log_plus<Dtype>()(alphas[i + (T - 1) * S], output[labels[i]]);
  }
  //update the gradient wrt to each unique label
  for (i = 0; i < alphabet_size_; ++i) {
    int idx3 = (T - 1) * alphabet_size_ * minibatch_ + i;
    if (/*output[i] == 0.0 || */output[i] == ctc_helper::neg_inf<Dtype>() ||
      probs[idx3] == 0.0) {
      grad[idx3] = probs[idx3];
    }
    else {
      grad[idx3] = probs[idx3] - exp(output[i] -
        log(probs[idx3]) - log_partition);
    }
  }
  //loop from the second to last column all the way to the left
  for (int t = T - 2; t >= 0; --t) {
    int remain = (S / 2) + repeats - (T - t);
    if (remain >= -1) {
      start -= s_inc[remain + 1];
    }
    if (t < (S / 2) + repeats) {
      end -= e_inc[t];
    }
    int endloop = end == S ? end - 1 : end;
    int idx1 = t * S, idx3 = t * (alphabet_size_ * minibatch_);
    std::fill(output, output + alphabet_size_, ctc_helper::neg_inf<Dtype>());
    for (int i = start; i < endloop; ++i) {
      Dtype next_sum = ctc_helper::log_plus<Dtype>()(betas[i], betas[(i + 1)]);
      // Skip two if not on blank and not on repeat.
      if (labels[i] != blank_label_ && i != (S - 2) && labels[i] != labels[i + 2]) {
        next_sum = ctc_helper::log_plus<Dtype>()(next_sum, betas[(i + 2)]);
      }
      betas[i] = next_sum + log(probs[labels[i] + idx3]);
      //compute alpha * beta in log space
      alphas[i + idx1] += betas[i];
      //update the gradient associated with this label
      output[labels[i]] =
        ctc_helper::log_plus<Dtype>()(alphas[i + idx1], output[labels[i]]);
    }
    if (end == S) {
      betas[(S - 1)] = betas[(S - 1)] + log(probs[blank_label_ + idx3]);
      alphas[(S - 1) + idx1] += betas[(S - 1)];
      output[labels[S - 1]] =
        ctc_helper::log_plus<Dtype>()(alphas[S - 1 + idx1], output[labels[S - 1]]);
    }
    //go over the unique labels and compute the final grad
    // wrt to each one at this time step
    for (i = 0; i < alphabet_size_; ++i) {
      if (/*output[i] == 0.0 || */output[i] == ctc_helper::neg_inf<Dtype>() ||
        probs[idx3] == 0.0) {
        grad[idx3] = probs[idx3];
      }
      else {
        grad[idx3] = probs[idx3] - exp(output[i] -
          log(probs[idx3]) - log_partition);
      }
      ++idx3;
    }
  }
  Dtype loglike = ctc_helper::neg_inf<Dtype>();
  for (i = start; i < end; ++i) {
    loglike = ctc_helper::log_plus<Dtype>()(loglike, betas[i]);
  }
  return loglike;
}

ctcStatus_t CpuCTC::cost_and_grad(const Dtype* const activations,
  Dtype* grads,
  Dtype* costs,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths)
{
  if (activations == NULL ||
    grads == NULL ||
    costs == NULL ||
    flat_labels == NULL ||
    label_lengths == NULL ||
    input_lengths == NULL
    ) {
    return CTC_STATUS_INVALID_VALUE;
  }
  Dtype* probs = static_cast<Dtype*>(workspace_);
  int maxT = *std::max_element(input_lengths, input_lengths + minibatch_);
  size_t bytes_used = sizeof(Dtype) * minibatch_ * alphabet_size_ * maxT;
  //per minibatch memory
  size_t per_minibatch_bytes = 0;
  int maxL = *std::max_element(label_lengths, label_lengths + minibatch_);;
  int maxS = 2 * maxL + 1;
  //output
  per_minibatch_bytes += sizeof(float) * alphabet_size_;
  //alphas
  per_minibatch_bytes += sizeof(float) * maxS * maxT;
  //betas
  per_minibatch_bytes += sizeof(float) * maxS;
  //labels w/blanks, e_inc, s_inc
  per_minibatch_bytes += 3 * sizeof(int) * maxS;
  softmax(activations, probs, input_lengths);
#pragma omp parallel for
  for (int mb = 0; mb < minibatch_; ++mb) {
    const int T = input_lengths[mb]; // Length of utterance (time)
    const int L = label_lengths[mb]; // Number of labels in transcription
    bool mb_status;
    mb_status =cost_and_grad_kernel(grads + mb * alphabet_size_,
        probs + mb * alphabet_size_,
        flat_labels + std::accumulate(label_lengths, label_lengths + mb, 0),
        T, L, mb,
        bytes_used + mb * per_minibatch_bytes, &costs[mb]);
  }
  return CTC_STATUS_SUCCESS;
}

ctcStatus_t CpuCTC::score_forward(const Dtype* const activations,
  Dtype* costs,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths)
{
  if (activations == NULL ||
    costs == NULL ||
    flat_labels == NULL ||
    label_lengths == NULL ||
    input_lengths == NULL
    ) {
    return CTC_STATUS_INVALID_VALUE;
  }
  Dtype* probs = static_cast<Dtype*>(workspace_);
  int maxT = *std::max_element(input_lengths, input_lengths + minibatch_);
  size_t bytes_used = sizeof(Dtype) * minibatch_ * alphabet_size_ * maxT;
  //per minibatch memory
  size_t per_minibatch_bytes = 0;
  int maxL = *std::max_element(label_lengths, label_lengths + minibatch_);
  int maxS = 2 * maxL + 1;
  //output
  per_minibatch_bytes += sizeof(float) * alphabet_size_;
  //alphas
  per_minibatch_bytes += sizeof(float) * maxS * maxT;
  //betas
  per_minibatch_bytes += sizeof(float) * maxS;
  //labels w/blanks, e_inc, s_inc
  per_minibatch_bytes += 3 * sizeof(int) * maxS;
  softmax(activations, probs, input_lengths);
#pragma omp parallel for
  for (int mb = 0; mb < minibatch_; ++mb) {
    const int T = input_lengths[mb]; // Length of utterance (time)
    const int L = label_lengths[mb]; // Number of labels in transcription
    const int S = 2 * L + 1; // Number of labels with blanks
    CpuCTC_metadata ctcm(L, S, T, mb, alphabet_size_, workspace_,
      bytes_used + mb * per_minibatch_bytes, blank_label_,
      flat_labels + std::accumulate(label_lengths, label_lengths + mb, 0));
    if (L + ctcm.repeats > T) {
      costs[mb] = Dtype(0);
    }
    else {
      costs[mb] = -compute_alphas(probs + mb * alphabet_size_, ctcm.repeats, S, T,
        ctcm.e_inc, ctcm.s_inc, ctcm.labels_w_blanks,
        ctcm.alphas);
    }
  }
  return CTC_STATUS_SUCCESS;
}


int get_warpctc_version()
{
  return 2;
}

const char* ctcGetStatusString(ctcStatus_t status)
{
  switch (status) {
  case CTC_STATUS_SUCCESS:
    return "no error";
  case CTC_STATUS_MEMOPS_FAILED:
    return "cuda memcpy or memset failed";
  case CTC_STATUS_INVALID_VALUE:
    return "invalid value";
  case CTC_STATUS_EXECUTION_FAILED:
    return "execution failed";
  case CTC_STATUS_UNKNOWN_ERROR:
  default:
    return "unknown error";
  }
}

ctcStatus_t compute_ctc_loss_cpu(const Dtype* const activations,
  Dtype* gradients,
  const int* const flat_labels,
  const int* const label_lengths,
  const int* const input_lengths,
  int alphabet_size,
  int minibatch,
  Dtype* costs,
  void* workspace,
  ctcOptions options)
{
  if (activations == NULL ||
    flat_labels == NULL ||
    label_lengths == NULL ||
    input_lengths == NULL ||
    costs == NULL ||
    workspace == NULL ||
    alphabet_size <= 0 ||
    minibatch <= 0) {
    return CTC_STATUS_INVALID_VALUE;
  }
  if (options.loc == CTC_CPU) {
    CpuCTC ctc(alphabet_size, minibatch, workspace, options.num_threads,
      options.blank_label);
    if (gradients != NULL)
      return ctc.cost_and_grad(activations, gradients,
        costs,
        flat_labels, label_lengths,
        input_lengths);
    else
      return ctc.score_forward(activations, costs, flat_labels,
        label_lengths, input_lengths);
  }
  else if (options.loc == CTC_GPU) {
  }
  else {
    return CTC_STATUS_INVALID_VALUE;
  }
  return CTC_STATUS_SUCCESS;
}

ctcStatus_t get_workspace_size(const int* const label_lengths,
  const int* const input_lengths,
  int alphabet_size, int minibatch,
  ctcOptions options,
  size_t* size_bytes)
{
  if (label_lengths == NULL ||
    input_lengths == NULL ||
    size_bytes == NULL ||
    alphabet_size <= 0 ||
    minibatch <= 0) {
    return CTC_STATUS_INVALID_VALUE;
  }
  // This is the MAX of all S and T for all examples in the minibatch.
  int maxL = *std::max_element(label_lengths, label_lengths + minibatch);
  int maxT = *std::max_element(input_lengths, input_lengths + minibatch);
  const int S = 2 * maxL + 1;
  *size_bytes = 0;
  if (options.loc == CTC_GPU) {
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
  }
  else {
    //cpu can eventually replace all minibatch with
    //MAX number of concurrent threads if memory is
    //really tight
    //per minibatch memory
    size_t per_minibatch_bytes = 0;
    //output
    per_minibatch_bytes += sizeof(Dtype) * alphabet_size;
    //alphas
    per_minibatch_bytes += sizeof(Dtype) * S * maxT;
    //betas
    per_minibatch_bytes += sizeof(Dtype) * S;
    //labels w/blanks, e_inc, s_inc
    per_minibatch_bytes += 3 * sizeof(int) * S;
    *size_bytes = per_minibatch_bytes * minibatch;
    //probs
    *size_bytes += sizeof(Dtype) * alphabet_size * maxT * minibatch;
  }
  return CTC_STATUS_SUCCESS;
}

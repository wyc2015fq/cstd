#ifdef USE_NCCL

#include <cuda_runtime.h>

#include <stdio.h>




#include "caffe/caffe.hpp"
#include "caffe/parallel.hpp"
#include "caffe/sgd_solvers.hpp"

enum Op {
  copy,
  replace_cpu,
  replace_gpu,
  replace_cpu_diff,
  replace_gpu_diff
};

template<typename Dtype>
static void apply_buffers(const vector<Blob*>& blobs,
                          Dtype* buffer, size_t total_size, Op op) {
  Dtype* ptr = buffer;
  for (int i = 0; i < blobs.size(); ++i) {
    int size = blobs[i]->count();
    switch (op) {
      case copy: {
        // Init buffer to current values of blobs
        caffe_copy(size,
                   reinterpret_cast<const Dtype*>(blobs[i]->data()->cpu_data()),
                   ptr);
        break;
      }
      case replace_cpu:
        blobs[i]->data()->set_cpu_data(ptr);
        break;
      case replace_gpu:
        blobs[i]->data()->set_gpu_data(ptr);
        break;
      case replace_cpu_diff:
        blobs[i]->diff()->set_cpu_data(ptr);
        break;
      case replace_gpu_diff:
        blobs[i]->diff()->set_gpu_data(ptr);
        break;
    }
    ptr += size;
  }
  // total_size is at least one byte
  CHECK_EQ(total_size, (ptr == buffer ? 1 : ptr - buffer));
}

// Buffer size necessary to store given blobs
template<typename Dtype>
static size_t total_size(const vector<Blob*>& params) {
  size_t size = 0;
  for (int i = 0; i < params.size(); ++i)
    size += params[i]->count();
  // Size have at least one byte, otherwise cudaMalloc fails if net has no
  // learnable parameters.
  return (size > 0) ? size : 1;
}

template<typename Dtype>
Params::Params(shared_ptr<Solver > root_solver)
  : size_(total_size(root_solver->net()->learnable_params())),
    data_(),
    diff_() {
}

template<typename Dtype>
GPUParams::GPUParams(shared_ptr<Solver > root_solver, int device)
  : Params(root_solver) {
  int initial_device;
  CUDA_CHECK(cudaGetDevice(&initial_device));

  // Allocate device buffers
  CUDA_CHECK(cudaSetDevice(device));
  CUDA_CHECK(cudaMalloc(&data_, size_ * sizeof(Dtype)));

  // Copy blob values
  const vector<Blob*>& net =
    root_solver->net()->learnable_params();
  apply_buffers(net, data_, size_, copy);

  CUDA_CHECK(cudaMalloc(&diff_, size_ * sizeof(Dtype)));
  caffe_gpu_set(size_, Dtype(0), diff_);

  CUDA_CHECK(cudaSetDevice(initial_device));
}

template<typename Dtype>
GPUParams::~GPUParams() {
  CUDA_CHECK(cudaFree(data_));
  CUDA_CHECK(cudaFree(diff_));
}

template<typename Dtype>
void GPUParams::Configure(Solver* solver) const {
  const vector<Blob*>& net =
    solver->net()->learnable_params();
  apply_buffers(net, data_, size_, replace_gpu);
  apply_buffers(net, diff_, size_, replace_gpu_diff);
}

static int getDevice() {
  int device = 0;
  CUDA_CHECK(cudaGetDevice(&device));
  return device;
}

template<typename Dtype>
NCCL::NCCL(shared_ptr<Solver > solver)
  : GPUParams(solver, getDevice()),
    comm_(), solver_(solver), barrier_() {
  this->Configure(solver.get());
  Init();
}

template<typename Dtype>
NCCL::NCCL(shared_ptr<Solver > solver, const string& uid)
  : GPUParams(solver, getDevice()),
    solver_(solver), barrier_() {
  this->Configure(solver.get());
  Caffe::set_multiprocess(true);
  ncclUniqueId nccl_uid;
  memcpy(&nccl_uid, &uid[0], NCCL_UNIQUE_ID_BYTES);  // NOLINT(caffe/alt_fn)
  NCCL_CHECK(ncclCommInitRank(&comm_,
                              Caffe::solver_count(),
                              nccl_uid,
                              Caffe::solver_rank()));
  Init();
}

template<typename Dtype>
void NCCL::Init() {
  if (solver_->param().layer_wise_reduce()) {
    CUDA_CHECK(cudaStreamCreateWithFlags(&stream_, cudaStreamNonBlocking));
  }
}

template<typename Dtype>
NCCL::~NCCL() {
  if (solver_->param().layer_wise_reduce()) {
    CUDA_CHECK(cudaStreamDestroy(stream_));
  }
  if (comm_) {
    ncclCommDestroy(comm_);
  }
}

template<typename Dtype>
boost::barrier* NCCL::barrier() {
  return barrier_;
}
template<typename Dtype>
void NCCL::set_barrier(boost::barrier* value) {
  barrier_ = value;
}

template<typename Dtype>
void NCCL::InitSingleProcess(vector<NCCL*>* nccls) {
  ncclComm_t* comms = new ncclComm_t[nccls->size()];
  int* gpu_list = new int[nccls->size()];
  for (int i = 0; i < nccls->size(); ++i) {
    gpu_list[i] = (*nccls)[i]->solver_->param().device_id();
  }
  NCCL_CHECK(ncclCommInitAll(comms, static_cast<int>(nccls->size()), gpu_list));
  for (int i = 0; i < nccls->size(); ++i) {
    (*nccls)[i]->comm_ = comms[i];
  }
}

template<typename Dtype>
string NCCL::new_uid() {
  string uid;
  uid.resize(NCCL_UNIQUE_ID_BYTES);
  ncclUniqueId nccl_uid;
  NCCL_CHECK(ncclGetUniqueId(&nccl_uid));
  memcpy(&uid[0], &nccl_uid, NCCL_UNIQUE_ID_BYTES);  // NOLINT(caffe/alt_fn)
  return uid;
}

template<typename Dtype>
void NCCL::Broadcast() {
  if (barrier_) {  // NULL in multi process case
    barrier_->wait();
  }
  NCCL_CHECK(ncclBcast(data_, static_cast<int>(size_),
                       nccl::dataType::type, 0,
                       comm_, cudaStreamDefault));
  if (barrier_) {
    barrier_->wait();
  }
}

template<typename Dtype>
void NCCL::run(int layer) {
  CHECK(solver_->param().layer_wise_reduce());
  vector<shared_ptr<Blob > >& blobs =
    solver_->net()->layers()[layer]->blobs();
#ifdef DEBUG
  // Assert blobs are contiguous to reduce in one step (e.g. bias often small)
  for (int i = 1; i < blobs.size(); ++i) {
    CHECK_EQ(blobs[i - 1]->gpu_diff() + blobs[i - 1]->count(),
             blobs[i + 0]->gpu_diff());
  }
#endif
  if (blobs.size() > 0) {
    // Make sure default stream is done computing gradients. Could be
    // replaced by cudaEventRecord+cudaStreamWaitEvent to avoid
    // blocking the default stream, but it's actually slower.
    CUDA_CHECK(cudaStreamSynchronize(cudaStreamDefault));

    // Reduce asynchronously
    int size = 0;
    for (int i = 0; i < blobs.size(); ++i) {
      size += blobs[i]->count();
    }
    if (barrier_) {  // NULL in multi process case
      barrier_->wait();
    }
    NCCL_CHECK(ncclAllReduce(blobs[0]->mutable_gpu_diff(),
                             blobs[0]->mutable_gpu_diff(),
                             size,
                             nccl::dataType::type,
                             ncclSum, comm_, stream_));
    caffe_gpu_scal(size, (Dtype) 1.0 / Caffe::solver_count(),
                   blobs[0]->mutable_gpu_diff(), stream_);
  }
}

template<typename Dtype>
void NCCL::on_gradients_ready() {
  if (solver_->param().layer_wise_reduce()) {
    CHECK_EQ(solver_->net()->params().size(),
             solver_->net()->learnable_params().size())
      << "Layer-wise reduce is not supported for nets with shared weights.";

    // Make sure reduction is done before applying gradients
    CUDA_CHECK(cudaStreamSynchronize(stream_));
  } else {
    if (barrier_) {  // NULL in multi process case
      barrier_->wait();
    }
    NCCL_CHECK(ncclAllReduce(diff_, diff_, static_cast<int>(size_),
                             nccl::dataType::type, ncclSum, comm_,
                             cudaStreamDefault));
    caffe_gpu_scal(static_cast<int>(size_),
                   (Dtype) 1.0 / Caffe::solver_count(), diff_);
  }
}

template<typename Dtype>
class Worker : public InternalThread {
 public:
  explicit Worker(shared_ptr<Solver > rank0, int device,
                  boost::barrier* barrier, vector<NCCL*>* nccls,
                  const char* restore)
    : rank0_(rank0), device_(device), barrier_(barrier),
      nccls_(nccls), restore_(restore) {
  }
  virtual ~Worker() {}

 protected:
  void InternalThreadEntry() {
    // Create solver and install callbacks
    SolverParameter param(rank0_->param());
    param.set_device_id(device_);
#ifdef DEBUG
    int device;
    CUDA_CHECK(cudaGetDevice(&device));
    CHECK_EQ(device, device_);
#endif
    param.set_type(rank0_->type());
    shared_ptr<Solver > s(SolverRegistry::CreateSolver(param));
    CHECK_EQ(s->type(), rank0_->type());
    if (restore_) {
      // Could not make NCCL broadcast solver state, it seems to crash
      // if called in a tight loop, regardless of barriers etc. so
      // restore all solvers from file.
      s->Restore(restore_);
    }
    NCCL nccl(s);
    nccl.set_barrier(barrier_);
    s->add_callback(&nccl);
    if (s->param().layer_wise_reduce()) {
      s->net()->add_after_backward(&nccl);
    }
    (*nccls_)[Caffe::solver_rank()] = &nccl;
    // Wait for other threads
    barrier_->wait();
    // Wait for NCCL init
    barrier_->wait();
    // Broadcast rank 0 state
    nccl.Broadcast();
    // Solve
    s->Step(param.max_iter() - s->iter());
    barrier_->wait();
#ifdef DEBUG
    // Check all solvers have same state
    SGDSolver* sa = static_cast<SGDSolver*>(rank0_.get());
    SGDSolver* sb = static_cast<SGDSolver*>(s.get());
    for (int h = 0; h < sa->history().size(); ++h) {
      CUDA_CHECK(cudaSetDevice(sa->param().device_id()));
      const Dtype* a = sa->history()[h]->cpu_data();
      CUDA_CHECK(cudaSetDevice(sb->param().device_id()));
      const Dtype* b = sb->history()[h]->cpu_data();
      for (int v = 0; v < sa->history()[h]->count(); ++v) {
        CHECK_DOUBLE_EQ(a[v], b[v]);
      }
    }
#endif
  }

  shared_ptr<Solver > rank0_;
  int device_;
  boost::barrier* barrier_;
  vector<NCCL*>* nccls_;
  const char* restore_;
};

template<typename Dtype>
void NCCL::Run(const vector<int>& gpus, const char* restore) {
  boost::barrier barrier(static_cast<int>(gpus.size()));
  vector<NCCL*> nccls(gpus.size());
  // Create workers
  vector<shared_ptr<Worker > > workers(gpus.size());
  for (int i = 1; i < gpus.size(); ++i) {
    CUDA_CHECK(cudaSetDevice(gpus[i]));
    Caffe::set_solver_rank(i);
    Worker* w = new Worker(solver_, gpus[i], &barrier,
                                         &nccls, restore);
    w->StartInternalThread();
    workers[i].reset(w);
  }
  CUDA_CHECK(cudaSetDevice(gpus[0]));
  Caffe::set_solver_rank(0);
  barrier_ = &barrier;
  solver_->add_callback(this);
  if (solver_->param().layer_wise_reduce()) {
    solver_->net()->add_after_backward(this);
  }
  nccls[0] = this;
  // Wait for workers
  barrier.wait();
  // Init NCCL
  InitSingleProcess(&nccls);
  barrier.wait();
  // Run first solver on current thread
  Broadcast();
  solver_->Solve();
  barrier.wait();  // Hangs without it when running tests
  // Wait for shutdown
  for (int i = 1; i < gpus.size(); ++i) {
    workers[i]->StopInternalThread();
  }
}

INSTANTIATE_CLASS(Params);
INSTANTIATE_CLASS(GPUParams);
INSTANTIATE_CLASS(Worker);
INSTANTIATE_CLASS(NCCL);

#endif  // USE_NCCL

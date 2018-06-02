#ifndef CAFFE_PARALLEL_HPP_
#define CAFFE_PARALLEL_HPP_

#ifdef USE_NCCL






#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"

#include "caffe/solver.hpp"
#include "caffe/syncedmem.hpp"
#include "blocking_queue.hpp"
#include "nccl.hpp"

// Represents a net parameters. Once a net is created, its parameter buffers can
// be replaced by ones from Params, to allow parallelization. Params ensures
// parameters are allocated in one consecutive array.
template<typename Dtype>
class Params {
 public:
  explicit Params(shared_ptr<Solver > root_solver);
  virtual ~Params() {
  }

  inline size_t size() const {
    return size_;
  }
  inline Dtype* data() const {
    return data_;
  }
  inline Dtype* diff() const {
    return diff_;
  }

 protected:
  const size_t size_;           // Size of buffers
  Dtype* data_;                 // Network parameters
  Dtype* diff_;                 // Gradient

DISABLE_COPY_AND_ASSIGN(Params);
};

// Params stored in GPU memory.
template<typename Dtype>
class GPUParams : public Params {
 public:
  GPUParams(shared_ptr<Solver > root_solver, int device);
  virtual ~GPUParams();

  void Configure(Solver* solver) const;

 protected:
  using Params::size_;
  using Params::data_;
  using Params::diff_;
};

template<typename Dtype>
class NCCL : public GPUParams,
             public Solver::Callback,
             public Net::Callback {
 public:
  /**
   * Single process version.
   */
  explicit NCCL(shared_ptr<Solver > solver);
  /**
   * In multi-process settings, first create a NCCL id (new_uid), then
   * pass it to each process to create connected instances.
   */
  NCCL(shared_ptr<Solver > solver, const string& uid);
  ~NCCL();

  boost::barrier* barrier();
  void set_barrier(boost::barrier* value);

  /**
   * In single process settings, create instances without uids and
   * call this to connect them.
   */
  static void InitSingleProcess(vector<NCCL*>* nccls);

  static string new_uid();

  /**
   * Broadcast weights from rank 0 other solvers.
   */
  void Broadcast();

  /**
   * Single process multi-GPU.
   */
  void Run(const vector<int>& gpus, const char* restore);

 protected:
  void Init();
  void on_start() {}
  void run(int layer);  // Net callback
  void on_gradients_ready();

  ncclComm_t comm_;
  cudaStream_t stream_;

  shared_ptr<Solver > solver_;
  // Should not be necessary, https://github.com/NVIDIA/nccl/issues/37
  boost::barrier* barrier_;
  using Params::size_;
  using Params::data_;
  using Params::diff_;
};

#endif  // USE_NCCL
#endif  // header

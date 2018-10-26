#ifndef CAFFE_INTERNAL_THREAD_HPP_
#define CAFFE_INTERNAL_THREAD_HPP_

#include "common.hpp"
#include <thread>
#include <exception>

/**
 Forward declare boost::thread instead of including boost/thread.hpp
 to avoid a boost/NVCC issues (#1009, #1010) on OSX.
 */

namespace caffe
{

  /**
   * Virtual class encapsulate boost::thread for use in base class
   * The child class will acquire the ability to run a single thread,
   * by reimplementing the virtual function InternalThreadEntry.
   */
  class InternalThread
  {
  public:
    InternalThread() : thread_() { is_must_stop = false;  }
    virtual ~InternalThread()
    {
      StopInternalThread();
    }

    /**
     * Caffe's thread local state will be initialized using the current
     * thread values, e.g. device id, solver index etc. The random seed
     * is initialized using caffe_rng_rand.
     */
    void StartInternalThread()
    {
      CHECK(!is_started()) << "Threads should persist and not be restarted.";
      int device = 0;
#ifndef CPU_ONLY
      CUDA_CHECK(cudaGetDevice(&device));
#endif
      Brew mode = mode();
      int rand_seed = caffe_rng_rand();
      int solver_count = solver_count();
      int solver_rank = solver_rank();
      bool multiprocess = multiprocess();
      try {
        thread_.reset(new std::thread(&InternalThread::entry, this, device, mode,
          rand_seed, solver_count, solver_rank, multiprocess));
      }
      catch (std::exception & e) {
        LOG(FATAL) << "Thread exception: " << e.what();
      }
    }

    /** Will not return until the internal thread has exited. */
    void StopInternalThread()
    {
      if (is_started()) {
        is_must_stop = true;
        //thread_->interrupt();
        try {
          thread_->join();
        }
        catch (std::exception & e) {
          LOG(FATAL) << "Thread exception: " << e.what();
        }
      }
    }

    bool is_started() const
    {
      return thread_ && thread_->joinable();
    }

    /* Implement this method in your subclass
        with the code you want your thread to run. */
    virtual void InternalThreadEntry() {}

    /* Should be tested when running loops to exit when requested. */
    bool must_stop()
    {
      //return thread_ && thread_->interruption_requested();
      return is_must_stop;
    }

    bool is_must_stop;

    void entry(int device, Brew mode, int rand_seed,
               int solver_count, int solver_rank, bool multiprocess)
    {
#ifndef CPU_ONLY
      CUDA_CHECK(cudaSetDevice(device));
#endif
      set_mode(mode);
      set_random_seed(rand_seed);
      set_solver_count(solver_count);
      set_solver_rank(solver_rank);
      set_multiprocess(multiprocess);
      InternalThreadEntry();
    }

    SHARED_PTR(std::thread) thread_;
  };

}  // namespace caffe

#endif  // CAFFE_INTERNAL_THREAD_HPP_

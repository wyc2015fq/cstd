#ifndef CAFFE_UTIL_BLOCKING_QUEUE_HPP_
#define CAFFE_UTIL_BLOCKING_QUEUE_HPP_

#include <queue>
#include <string>
#include <mutex>

namespace caffe
{

  template<typename T>
  class BlockingQueue
  {
  public:
    explicit BlockingQueue()
      : sync_(new sync())
    {
    }

    void push(const T & t)
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      queue_.push(t);
      lock.unlock();
      sync_->condition_.notify_one();
    }

    bool try_pop(T* t)
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      if (queue_.empty()) {
        return false;
      }
      *t = queue_.front();
      queue_.pop();
      return true;
    }

    // This logs a message if the threads needs to be blocked
    // useful for detecting e.g. when data feeding is too slow
    T pop(const string & log_on_wait = "")
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      while (queue_.empty()) {
        if (!log_on_wait.empty()) {
          LOG_EVERY_N(INFO, 1000) << log_on_wait;
        }
        sync_->condition_.wait(lock);
      }
      T t = queue_.front();
      queue_.pop();
      return t;
    }

    bool try_peek(T* t)
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      if (queue_.empty()) {
        return false;
      }
      *t = queue_.front();
      return true;
    }

    // Return element without removing it
    T peek()
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      while (queue_.empty()) {
        sync_->condition_.wait(lock);
      }
      return queue_.front();
    }

    size_t size() const
    {
      std::unique_lock<std::mutex> lock(sync_->mutex_);
      return queue_.size();
    }

  protected:
    /**
     Move synchronization fields out instead of including boost/thread.hpp
     to avoid a boost/NVCC issues (#1009, #1010) on OSX. Also fails on
     Linux CUDA 7.0.18.
     */
    class sync
    {
    public:
      mutable std::mutex mutex_;
      std::condition_variable condition_;
    };

    std::queue<T> queue_;
    SHARED_PTR(sync) sync_;

  };

}  // namespace caffe

#endif

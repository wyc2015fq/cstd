#include <algorithm>
/*
*  Modified for EasyCNN(https://github.com/xylcbd/EasyCNN) by (https://github.com/xylcbd) based on ThreadPool(https://github.com/progschj/ThreadPool)
*  License: WTFPL
*/
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>


//TODO: using wrapper to hidden information of ThreadPool
class ThreadPool
{
public:
  static ThreadPool& instance();
  size_t size() const;
  void resize(const size_t new_size);
  template<class F, class... Args>
  auto enqueue(F && f, Args && ... args)
  ->future<typename result_of<F(Args...)>::type>;
private:
  ThreadPool(const size_t threads);
  virtual ~ThreadPool();
  void shutdown();
  void startup(const size_t threads);
private:
  // need to keep track of threads so we can join them
  vector< thread > workers;
  // the task queue
  queue< function<void()> > tasks;

  // synchronization
  mutex queue_mutex;
  condition_variable condition;
  bool stop = true;
};
// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F && f, Args && ... args)
-> future<typename result_of<F(Args...)>::type> {
  using return_type = typename result_of<F(Args...)>::type;

  auto task = make_shared< packaged_task<return_type()> >(
      bind(forward<F>(f), forward<Args>(args)...)
  );

  future<return_type> res = task->get_future();
  {
    unique_lock<mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if (stop) {
      throw runtime_error("enqueue on stopped ThreadPool");
    }

    tasks.emplace([task]() {
      (*task)();
    });
  }
  condition.notify_one();
  return res;
}

//////////////////////////////////////////////////////////////////////////
//APIs
//get thread number
size_t get_thread_num();
//set thread number, and returned support thread number
size_t set_thread_num(const size_t num);
//dispatcher tasks of layer
void dispatch_worker(function<void(const size_t, const size_t)> func, const size_t number);


ThreadPool& ThreadPool::instance()
{
  static ThreadPool inst(2);
  return inst;
}
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(const size_t threads)
{
  startup(threads);
}
// the destructor joins all threads
ThreadPool::~ThreadPool()
{
  shutdown();
}
void ThreadPool::shutdown()
{
  {
    unique_lock<mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();

for (thread & worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }

  workers.clear();
  tasks = decltype(tasks)();
}
void ThreadPool::startup(const size_t threads)
{
  stop = false;

  for (size_t i = 0; i < threads; ++i)
    workers.emplace_back(
    [this] {
    for (;;) {
      function<void()> task;

      {
        unique_lock<mutex> lock(this->queue_mutex);
        this->condition.wait(lock,
        [this] { return this->stop || !this->tasks.empty(); });

        if (this->stop && this->tasks.empty()) {
          return;
        }

        task = move(this->tasks.front());
        this->tasks.pop();
      }

      task();
    }
  }
  );
}
inline size_t ThreadPool::size() const
{
  return workers.size();
}
void ThreadPool::resize(const size_t new_size)
{
  //stop thread pool
  shutdown();
  //start thread pool
  startup(new_size);
}

//////////////////////////////////////////////////////////////////////////
//APIs
size_t get_thread_num()
{
  return ThreadPool::instance().size();
}
size_t set_thread_num(const size_t num)
{
  easyAssert(num > 0, "number of thread must be larger than 0");

  if (num != get_thread_num()) {
    ThreadPool::instance().resize(num);
  }

  return get_thread_num();
}
void dispatch_worker(function<void(const size_t, const size_t)> func, const size_t number)
{
  if (number <= 0) {
    return;
  }

  const size_t threads_of_pool = ThreadPool::instance().size();

  if (threads_of_pool <= 1 || number <= 1) {
    func(0, number);
  }
  else {
    easyAssert(threads_of_pool > 1, "thread must be larger than 1");
    // 1/4 2/4 /4/4 5/4 => all ok!
    const size_t payload_per_thread = number / threads_of_pool;
    const size_t remainder_payload = number - payload_per_thread * threads_of_pool;
    const size_t remainder_proc_last_idx = remainder_payload;

    size_t start = 0;
    vector<future<void>> futures;

    for (size_t i = 0; i < threads_of_pool; i++) {
      size_t stop = start + payload_per_thread;

      if (i < remainder_proc_last_idx) {
        stop = stop + 1;
      }

      futures.push_back(ThreadPool::instance().enqueue(func, start, stop));
      start = stop;

      if (stop >= number) {
        break;
      }
    }

    for (size_t i = 0; i < futures.size(); i++) {
      futures[i].wait();
    }
  }
}

# srsLTE源码学习：度量中心：metrics_hub.h - Koma Hub - CSDN博客
2019年04月10日 22:06:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：14
个人分类：[LTE																[进程和线程](https://blog.csdn.net/Rong_Toa/article/category/7969469)](https://blog.csdn.net/Rong_Toa/article/category/8766817)
**Table of Contents**
[metrics_hub.h](#metrics_hub.h%C2%A0%C2%A0%20%C2%A0)
PS：《[srsLTE源码学习：绑核创建线程threads.h, threads.c](https://blog.csdn.net/Rong_Toa/article/details/89195281)》
# metrics_hub.h    
lib\include\srslte\common    2456    4/10/2019    38
```cpp
/******************************************************************************
 * File:        metrics_hub.h
 * Description: Centralizes metrics interfaces to allow different metrics clients
 *              to get metrics 
 *****************************************************************************/
#ifndef SRSLTE_METRICS_HUB_H
#define SRSLTE_METRICS_HUB_H
#include <vector>
#include "srslte/common/threads.h"
#include "srslte/srslte.h"
namespace srslte {
template<typename metrics_t>
class metrics_interface 
{
public:
  virtual bool get_metrics(metrics_t &m) = 0; 
}; 
template<typename metrics_t>
class metrics_listener //指标侦听器
{
public: 
  virtual void set_metrics(metrics_t &m, const uint32_t period_usec) = 0;
  virtual void stop() = 0;
};
template<typename metrics_t>
//metrics hub: 指标中心
class metrics_hub : public periodic_thread //periodic 周期
{//hub: 枢纽;    metrics:指标
/*
度量	measurement, metric, metrics, measuring
量度	measure, measuring, metrics, metric
指标	index, indicators, target, parameters, metrics, criteria, markers
*/
public:
  metrics_hub() //hub 枢纽
    :m(NULL)
  {
    for (int i = 0; i < 3; ++i) {
      sleep_period_start[i] = (struct timeval){};
    }
  }
  bool init(metrics_interface<metrics_t> *m_, float report_period_secs_=1.0) {
    m = m_;
    // Start with user-default priority
    start_periodic(report_period_secs_*1e6, -2);
    return true;
  }
  void stop() {
    stop_thread();
    // stop all listeners
    for (uint32_t i=0;i<listeners.size();i++) {
      listeners[i]->stop();
    }
    thread_cancel();
    wait_thread_finish();
  }
  
  void add_listener(metrics_listener<metrics_t> *listener) {
    listeners.push_back(listener);
  }
  
private:
  void run_period(){
    // get current time and check how long we slept
    gettimeofday(&sleep_period_start[2], NULL);
    get_time_interval(sleep_period_start);
    uint32_t period = sleep_period_start[0].tv_sec*1e6 + sleep_period_start[0].tv_usec;
    if (m) {
      metrics_t metric;
      m->get_metrics(metric);
      for (uint32_t i=0;i<listeners.size();i++) {
        listeners[i]->set_metrics(metric, period);
      }
    }
    // store start of sleep period
    gettimeofday(&sleep_period_start[1], NULL);
  }
  metrics_interface<metrics_t> *m;
  std::vector<metrics_listener<metrics_t>*> listeners;
  struct timeval sleep_period_start[3];
};
} // namespace srslte
#endif // SRSLTE_METRICS_HUB_H
```

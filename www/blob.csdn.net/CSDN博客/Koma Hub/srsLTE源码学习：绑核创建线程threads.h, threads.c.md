# srsLTE源码学习：绑核创建线程threads.h, threads.c - Koma Hub - CSDN博客
2019年04月10日 21:34:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：45
**Table of Contents**
[threads.h    lib\include\srslte\common    4228    4/10/2019    83](#threads.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A04228%C2%A0%C2%A0%20%C2%A04%2F10%2F2019%C2%A0%C2%A0%20%C2%A083)
[threads.c    lib\src\common    6122    2/28/2019    59](#threads.c%C2%A0%C2%A0%20%C2%A0lib%5Csrc%5Ccommon%C2%A0%C2%A0%20%C2%A06122%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A059)
# threads.h    lib\include\srslte\common    4228    4/10/2019    83
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#include <pthread.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdio.h>
// Default priority for all threads below UHD threads
#define DEFAULT_PRIORITY 60
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus
/*
   一些创建线程的接口：优先级，绑核，掩码等...
*/
  bool threads_new_rt(pthread_t *thread, void *(*start_routine) (void*), void *arg);
  bool threads_new_rt_prio(pthread_t *thread, void *(*start_routine) (void*), void *arg, int prio_offset);
  bool threads_new_rt_cpu(pthread_t *thread, void *(*start_routine) (void*), void *arg, int cpu, int prio_offset);
  bool threads_new_rt_mask(pthread_t *thread, void *(*start_routine) (void*), void *arg, int mask, int prio_offset);
  void threads_print_self();
#ifdef __cplusplus
}
  
#ifndef SRSLTE_THREADS_H
#define SRSLTE_THREADS_H  
  
class thread //线程类
{
public:
  thread() {
    _thread = 0;
  }
  bool start(int prio = -1) {
    return threads_new_rt_prio(&_thread, thread_function_entry, this, prio);    
  }
  bool start_cpu(int prio, int cpu) {
    return threads_new_rt_cpu(&_thread, thread_function_entry, this, cpu, prio);    
  }
   bool start_cpu_mask(int prio, int mask){
     return threads_new_rt_mask(&_thread, thread_function_entry, this, mask, prio);
}
  void print_priority() {
    threads_print_self();
  }
  void wait_thread_finish() {
    pthread_join(_thread, NULL);
  }
  void thread_cancel() {
    pthread_cancel(_thread);
  }
protected:
  virtual void run_thread() = 0; 
private:
  static void *thread_function_entry(void *_this)  { ((thread*) _this)->run_thread(); return NULL; }
  pthread_t _thread;
};
//periodic 周期
class periodic_thread : public thread 
{
public:
  void start_periodic(int period_us_, int priority = -1) {
    run_enable = true;
    period_us = period_us_; 
    start(priority);
  }
  void stop_thread() {
    run_enable = false;
    wait_thread_finish();
  }
protected:   
  virtual void run_period() = 0; 
private:
  int wakeups_missed; 
  int timer_fd; 
  int period_us;
  bool run_enable;
  void run_thread() {
    if (make_periodic()) {
      return;
    }
    while(run_enable) {
      run_period();
      if (run_enable) {
        wait_period();
      }
    }
  }
  int make_periodic() {
    int ret = -1; 
    unsigned int ns;
    unsigned int sec;
    struct itimerspec itval;
    /* Create the timer */
    ret = timerfd_create (CLOCK_MONOTONIC, 0);
    wakeups_missed = 0;
    timer_fd = ret;
    if (ret > 0) {
      /* Make the timer periodic */
      sec = period_us/1e6;
      ns = (period_us - (sec * 1000000)) * 1000;
      itval.it_interval.tv_sec = sec;
      itval.it_interval.tv_nsec = ns;
      itval.it_value.tv_sec = sec;
      itval.it_value.tv_nsec = ns;
      ret = timerfd_settime (timer_fd, 0, &itval, NULL); 
      if (ret < 0) {
        perror("timerfd_settime");
      }
    } else {
      perror("timerfd_create");
    }
    return ret;
  }
  void wait_period() {
    unsigned long long missed;
    int ret;
    /* Wait for the next timer event. If we have missed any the
        number is written to "missed" */
    ret = read (timer_fd, &missed, sizeof (missed));
    if (ret == -1)
    {
      perror ("read timer");
      return;
    }
    /* "missed" should always be >= 1, but just to be sure, check it is not 0 anyway */
    if (missed > 0) {
      wakeups_missed += (missed - 1);
    }
  }
}; 
#endif // SRSLTE_THREADS_H
#endif // __cplusplus
```
# threads.c    lib\src\common    6122    2/28/2019    59
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "srslte/common/threads.h"
bool threads_new_rt(pthread_t *thread, void *(*start_routine) (void*), void *arg) {
  return threads_new_rt_prio(thread, start_routine, arg, -1);
}
bool threads_new_rt_prio(pthread_t *thread, void *(*start_routine) (void*), void *arg, int prio_offset) {
  return threads_new_rt_cpu(thread, start_routine, arg, -1, prio_offset);
}
bool threads_new_rt_mask(pthread_t *thread, void *(*start_routine) (void*), void *arg,int mask, int prio_offset){
return threads_new_rt_cpu(thread, start_routine, arg, mask*100, prio_offset);// we multiply mask by 100 to distinguish it from a single cpu core id
}
bool threads_new_rt_cpu(pthread_t *thread, void *(*start_routine) (void*), void *arg, int cpu, int prio_offset) {
  bool ret = false; 
  
  pthread_attr_t attr;
  struct sched_param param;
  cpu_set_t cpuset;
  bool attr_enable = false;
#ifdef PER_THREAD_PRIO
  if (prio_offset >= 0) {
    param.sched_priority = sched_get_priority_max(SCHED_FIFO) - prio_offset;
    pthread_attr_init(&attr);
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
      perror("pthread_attr_setinheritsched");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO)) {
      perror("pthread_attr_setschedpolicy");
    }
    if (pthread_attr_setschedparam(&attr, ¶m)) {
      perror("pthread_attr_setschedparam");
      fprintf(stderr, "Error not enough privileges to set Scheduling priority\n");
    }
    attr_enable = true;
  } else if (prio_offset == -1) {
    param.sched_priority = sched_get_priority_max(SCHED_FIFO) - DEFAULT_PRIORITY;
    pthread_attr_init(&attr);
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
      perror("pthread_attr_setinheritsched");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO)) {
      perror("pthread_attr_setschedpolicy");
    }
    if (pthread_attr_setschedparam(&attr, ¶m)) {
      perror("pthread_attr_setschedparam");
      fprintf(stderr, "Error not enough privileges to set Scheduling priority\n");
    }
    attr_enable = true;
  } else if (prio_offset == -2) {
#else
  // All threads have normal priority except prio_offset=0,1,2,3,4
  if (prio_offset >= 0 && prio_offset < 5) {
    param.sched_priority = 50-prio_offset;
    pthread_attr_init(&attr);
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
      perror("pthread_attr_setinheritsched");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO)) {
      perror("pthread_attr_setschedpolicy");
    }
    if (pthread_attr_setschedparam(&attr, ¶m)) {
      perror("pthread_attr_setschedparam");
      fprintf(stderr, "Error not enough privileges to set Scheduling priority\n");
    }
    attr_enable = true;
  } else {
#endif
    param.sched_priority = 0;
    pthread_attr_init(&attr);
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
      perror("pthread_attr_setinheritsched");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_OTHER)) {
      perror("pthread_attr_setschedpolicy");
    }
    if (pthread_attr_setschedparam(&attr, ¶m)) {
      perror("pthread_attr_setschedparam");
      fprintf(stderr, "Error not enough privileges to set Scheduling priority\n");
    }
    attr_enable = true;
  }
  if(cpu > 0) {
    if(cpu > 50) {
      int mask;
      mask = cpu/100;
                  
      CPU_ZERO(&cpuset);	
      for(int i = 0; i < 8;i++){
        if(((mask >> i) & 0x01) == 1){
          CPU_SET((size_t) i , &cpuset);        
        }     
      }  
    } else {
        CPU_ZERO(&cpuset);
        CPU_SET((size_t) cpu, &cpuset);
    }
    
    if(pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset)) {
      perror("pthread_attr_setaffinity_np");
    }
  }
  int err = pthread_create(thread, attr_enable ? &attr : NULL, start_routine, arg);
  if (err) {
    if (EPERM == err) {
      perror("Warning: Failed to create thread with real-time priority. Creating it with normal priority");
      err = pthread_create(thread, NULL, start_routine, arg);
      if (err) {
        perror("pthread_create");
      } else {
        ret = true; 
      }
    } else {
      perror("pthread_create");
    }
  } else {
    ret = true; 
  }
  if (attr_enable) {
    pthread_attr_destroy(&attr);
  }
  return ret; 
}
void threads_print_self() {
  pthread_t thread;
  cpu_set_t cpuset;
  struct sched_param param;
  int policy;
  const char *p;
  int s,j;
  thread = pthread_self();
  s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (s != 0) {
    printf("error pthread_getaffinity_np: %s\n",strerror(s));
  }
  printf("Set returned by pthread_getaffinity_np() contained:\n");
  for (j = 0; j < CPU_SETSIZE; j++) {
    if (CPU_ISSET(j, &cpuset)) {
      printf("    CPU %d\n", j);
    }
  }
  s = pthread_getschedparam(thread, &policy, ¶m);
  if (s != 0) {
    printf("error pthread_getaffinity_np: %s\n", strerror(s));
  }
  switch(policy) {
  case SCHED_FIFO:
    p = "SCHED_FIFO";
    break;
  case SCHED_RR:
    p = "SCHED_RR";
    break;
  default:
    p = "Other";
    break;
  }
  printf("Sched policy is %s. Priority is %d\n",p,param.sched_priority);
}
```

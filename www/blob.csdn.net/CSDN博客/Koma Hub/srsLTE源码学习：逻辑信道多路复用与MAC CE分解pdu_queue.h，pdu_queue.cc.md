# srsLTE源码学习：逻辑信道多路复用与MAC CE分解pdu_queue.h，pdu_queue.cc - Koma Hub - CSDN博客
2019年04月11日 21:57:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：20

> 
Logical Channel Demultiplexing and MAC CE dissassemble -
# 逻辑信道多路复用与MAC CE分解
- CE设备：Customer Edge，客户侧边缘设备，一般用来与PTN网络的边缘设备对接。
- PE设备：Provider Edge，网络侧边缘设备，通常要求具备封装与解封装能力。
- P设备：Provider，核心设备，要求具备标签交换能力。
# pdu_queue.h    
lib\include\srslte\common    2596    4/11/2019    1
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
#ifndef SRSLTE_PDU_QUEUE_H
#define SRSLTE_PDU_QUEUE_H
#include "srslte/common/log.h"
#include "srslte/common/block_queue.h"
#include "srslte/common/buffer_pool.h"
#include "srslte/common/timers.h"
#include "srslte/common/pdu.h"
/* Logical Channel Demultiplexing and MAC CE dissassemble 
    逻辑信道多路复用与MAC CE分解
CE设备：Customer Edge，客户侧边缘设备，一般用来与PTN网络的边缘设备对接。
PE设备：Provider Edge，网络侧边缘设备，通常要求具备封装与解封装能力。
P设备：Provider，核心设备，要求具备标签交换能力。
*/   
namespace srslte {
class pdu_queue
{
public:
  typedef enum {
    DCH,
    BCH,
    MCH
  } channel_t;
  class process_callback
  {
    public:
      virtual void process_pdu(uint8_t *buff, uint32_t len, channel_t channel, uint32_t tstamp) = 0;
  };
  pdu_queue(uint32_t pool_size = DEFAULT_POOL_SIZE) : pool(pool_size), callback(NULL), log_h(NULL) {}
  void init(process_callback *callback, log* log_h_);
  uint8_t* request(uint32_t len);
  void     deallocate(uint8_t* pdu);
  void     push(uint8_t *ptr, uint32_t len, channel_t channel = DCH, uint32_t tstamp = 0);
  bool   process_pdus();
private:
  const static int DEFAULT_POOL_SIZE = 64; // Number of PDU buffers in total
  const static int MAX_PDU_LEN     = 150*1024/8; // ~ 150 Mbps
  typedef struct  {
    uint8_t  ptr[MAX_PDU_LEN];
    uint32_t len;
    uint32_t tstamp;
    channel_t channel;
    #ifdef SRSLTE_BUFFER_POOL_LOG_ENABLED
      char   debug_name[128];
    #endif
  } pdu_t; 
  
  block_queue<pdu_t*> pdu_q; 
  buffer_pool<pdu_t>  pool;
  
  process_callback   *callback;   
  log                *log_h;
};
} // namespace srslte
#endif // SRSLTE_PDU_QUEUE_H
```
# pdu_queue.cc    
lib\src\common    2980    2/28/2019    57
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
#define Error(fmt, ...)   log_h->error(fmt, ##__VA_ARGS__)
#define Warning(fmt, ...) log_h->warning(fmt, ##__VA_ARGS__)
#define Info(fmt, ...)    log_h->info(fmt, ##__VA_ARGS__)
#define Debug(fmt, ...)   log_h->debug(fmt, ##__VA_ARGS__)
#include "srslte/common/pdu_queue.h"
namespace srslte {
    
void pdu_queue::init(process_callback *callback_, log* log_h_)
{
  callback  = callback_;
  log_h     = log_h_;   
}
uint8_t* pdu_queue::request(uint32_t len)
{  
  if (len > MAX_PDU_LEN) {
    fprintf(stderr, "Error request buffer of invalid size %d. Max bytes %d\n", len, MAX_PDU_LEN);
    return NULL; 
  }
  pdu_t *pdu = pool.allocate("pdu_queue::request");
  if (!pdu) {
    if (log_h) {
      log_h->error("Not enough buffers for MAC PDU\n");      
    }
    fprintf(stderr, "Not enough buffers for MAC PDU\n");
  }
  if ((void*) pdu->ptr != (void*) pdu) {
    fprintf(stderr, "Fatal error in memory alignment in struct pdu_queue::pdu_t\n");
    exit(-1);
  }
  
  return pdu->ptr; 
}
void pdu_queue::deallocate(uint8_t* pdu)
{
  if (!pool.deallocate((pdu_t*) pdu)) {
    log_h->warning("Error deallocating from buffer pool in deallocate(): buffer not created in this pool.\n");
  }
}
/* Demultiplexing of logical channels and dissassemble of MAC CE 
 * This function enqueues the packet and returns quicly because ACK 
 * deadline is important here. 
 */ 
void pdu_queue::push(uint8_t *ptr, uint32_t len, channel_t channel, uint32_t tstamp)
{
  if (ptr) {
    pdu_t *pdu  = (pdu_t*) ptr;
    pdu->len    = len;
    pdu->tstamp = tstamp;
    pdu->channel = channel;
    pdu_q.push(pdu);
  } else {
    log_h->warning("Error pushing pdu: ptr is empty\n");
  }
}
bool pdu_queue::process_pdus()
{
  bool have_data = false;
  uint32_t cnt  = 0;
  pdu_t *pdu;
  while(pdu_q.try_pop(&pdu)) {
    if (callback) {
      callback->process_pdu(pdu->ptr, pdu->len, pdu->channel, pdu->tstamp);
    }
    cnt++;
    have_data = true;
  }
  if (cnt > 20) {
    if (log_h) {
      log_h->warning("PDU queue dispatched %d packets\n", cnt);
    }
    printf("Warning PDU queue dispatched %d packets\n", cnt);
  }
  return have_data; 
}
}
```

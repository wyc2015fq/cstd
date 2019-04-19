# srsLTE源码学习：S1接口抓包s1ap_pcap.h，s1ap_pcap.cc - Koma Hub - CSDN博客
2019年04月11日 22:14:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：13
**Table of Contents**
[介绍](#%E4%BB%8B%E7%BB%8D)
[图例](#%E5%9B%BE%E4%BE%8B)
[代码](#%E4%BB%A3%E7%A0%81)
[s1ap_pcap.h](#s1ap_pcap.h%C2%A0%C2%A0)
[s1ap_pcap.cc](#s1ap_pcap.cc%C2%A0%C2%A0%20%C2%A0)
[s1ap_test.cc](#s1ap_test.cc%C2%A0%C2%A0%20%C2%A0)
# 介绍
- S1接口是LTE eNodeB（基站）与 EPC（分组核心网）之间的通讯接口。
- 将LTE系统划分为无线接入网和核心网。
- S1接口沿袭了承载和控制分离的思想，又分成两个接口，一个用于控制平面（S1-MME），一个用于用户平面（S1-U）。
- 控制平面接口S1-MME将基站和移动性管理实体（MME）相连，主要完成S1接口的无线接入承载控制、接口专用的操作维护等功能。
- 用户平面接口S1-U将基站和服务网关（S-GW）连接，用于传送用户数据和相应的用户平面控制帧。
# 图例
![](https://img-blog.csdnimg.cn/20190411221242685.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 代码
## s1ap_pcap.h  
 lib\include\srslte\common    1665    4/11/2019    1
```cpp
#ifndef SRSLTE_S1AP_PCAP_H
#define SRSLTE_S1AP_PCAP_H
#include "srslte/common/pcap.h"
namespace srslte {
//S1接口是LTE eNodeB（基站）与 EPC（分组核心网）之间的通讯接口。
//将LTE系统划分为无线接入网和核心网。
//S1接口沿袭了承载和控制分离的思想，又分成两个接口，一个用于控制平面（S1-MME），一个用于用户平面（S1-U）。
//控制平面接口S1-MME将基站和移动性管理实体（MME）相连，主要完成S1接口的无线接入承载控制、接口专用的操作维护等功能。
//用户平面接口S1-U将基站和服务网关（S-GW）连接，用于传送用户数据和相应的用户平面控制帧。
/*          
                                S6a
                        |     +-----HSS                     Rx
                 S1-MME |     |                    +--PCRF--------------+
                +-------|----MME--+                |                    |
                |       |         |S11             |                    |        =====> 控制面
        Uu      |       |   S1-U  |    S5/S8       |     SGi                    
    UE ----->eNodeB ----|--------S-GW --------> PDN网关 -------->    IP业务          =====> 用户面
                        |
                        |
          E-UTRAN       |               EPC
*/
class s1ap_pcap
{
public:
    s1ap_pcap() {enable_write=false; pcap_file = NULL; }
    void enable();
    void open(const char *filename);
    void close();
    void write_s1ap(uint8_t *pdu, uint32_t pdu_len_bytes);
private:
    bool enable_write;
    FILE *pcap_file;
};
} //namespace srslte
#endif // SRSLTE_NAS_PCAP_H
```
## s1ap_pcap.cc    
lib\src\common    653    2/28/2019    19
```cpp
#include <stdint.h>
#include "srslte/srslte.h"
#include "srslte/common/pcap.h"
#include "srslte/common/s1ap_pcap.h"
namespace srslte {
void s1ap_pcap::enable()
{
  enable_write = true;
}
void s1ap_pcap::open(const char* filename)
{
  pcap_file = LTE_PCAP_Open(S1AP_LTE_DLT, filename);
  enable_write = true;
}
void s1ap_pcap::close()
{
  fprintf(stdout, "Saving S1AP PCAP file\n");
  LTE_PCAP_Close(pcap_file);
}
void s1ap_pcap::write_s1ap(uint8_t *pdu, uint32_t pdu_len_bytes)
{
    if (enable_write) {
      S1AP_Context_Info_t  context;
      if (pdu) {
        LTE_PCAP_S1AP_WritePDU(pcap_file, &context, pdu, pdu_len_bytes);
      }
    }
}
}
```
## s1ap_test.cc    
lib\test\asn1    2025    2/28/2019    22
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2019 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
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
#include "srslte/asn1/liblte_s1ap.h"
#include "srslte/common/log_filter.h"
#include <assert.h>
#include <string.h>
void unpack_test_served_gummeis_with_multiple_plmns() {
  srslte::log_filter log1("Log");
  log1.set_level(srslte::LOG_LEVEL_DEBUG);
  log1.set_hex_limit(1024);
  LIBLTE_S1AP_S1AP_PDU_STRUCT compare_pdu;
  LIBLTE_S1AP_S1AP_PDU_STRUCT output_pdu;
  LIBLTE_BYTE_MSG_STRUCT input_pdu;
  uint8_t pdu[] = {0x20, 0x11, 0x00, 0x26, 0x00, 0x00, 0x02, 0x00, 0x69,
                   0x00, 0x1a, 0x01, 0x40, 0x00, 0xf1, 0x10, 0x00, 0xf1,
                   0x10, 0x00, 0xf1, 0x10, 0x00, 0xf1, 0x10, 0x00, 0xf1,
                   0x10, 0x00, 0xf1, 0x10, 0x00, 0x00, 0x88, 0x88, 0x00,
                   0x7b, 0x00, 0x57, 0x40, 0x01, 0xff};
  input_pdu.N_bytes = sizeof(pdu);
  memcpy(&input_pdu.msg, pdu, sizeof(pdu));
  log1.debug_hex(input_pdu.msg, input_pdu.N_bytes, "Input message len = %d",
                 input_pdu.N_bytes);
  assert(liblte_s1ap_unpack_s1ap_pdu((LIBLTE_BYTE_MSG_STRUCT *)&input_pdu,
                                     &output_pdu) == LIBLTE_SUCCESS);
}
int main(int argc, char **argv) {
  unpack_test_served_gummeis_with_multiple_plmns();
}
```

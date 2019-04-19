# srsLTE源码学习：GTP：GPRS Turning Protocol- GPRS隧道协议 - Koma Hub - CSDN博客
2019年04月01日 21:03:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：31
**Table of Contents**
[gtpc.h](#gtpc.h%C2%A0%C2%A0)
 * GTP-C v2 Header  GPRS Turning Protocol <GPRS Turning Protocol> GPRS隧道协议
 *      1.提供主机间端到端通信
 *      2.通过隧道协议TEI在路径协议上复用
 *      3.GTP协议主要应用场景
 *      4.GTP是一组给予IP的，用于在核心网络中支持通用分组无线服务（GPRS）的通讯协议。
 *  GTP是一组基于IP的高层协议，位于TCP/IP或UDP/IP等协议上，
 *  主要用于在GSM和UMTS和LTE网络中支持通用分组无线服务(GPRS)的通讯协议。
 *  
 * GTP可以分解成三种独立的协议，GTP-C、GTP-U及GTP'。
 * GTP-C用于在GPRS核心网内传输GGSN(网关GPRS支持结点)和SGSN(服务GPRS支持结点)之间的信令，
 * 它允许SGSN代表用户激活一个会话(PDP[分组数据协议]上下文激活)，禁用激活的会话，调整服务质量参数，
 * 或更新刚从其它SGSN切换过来用户的会话。
 * GTP-U用于在GPRS核心网内，无线接入网与核心网之间传送用户数据，用户数据包可以以IPv4, IPv6或PPP中的任何格式传输。
 * GTP'(GTP prime)与GTP-C，GTP-U使用相同的信息结构，但它有独立的功能。
 * 可以用它来传输从GSM或UMTS的CDF(计费数据功能)到CGF(计费网关功能)的数据，
 * 在大部分情况下这表示传输很多独立的网元(如GGSN)的数据到中央电脑中，后者可以更方便的将计费数据传输到运营商的计费中心。
 * 在3GPP网络中实现了不同的GTP变种，包括RNC，SGSN，GGSN及CGF。GRPS移动站(MSs)连接到SGSN时不需要知道GTP的存在。
 * GTP可以用在UDP或TCP上，GTP v1仅用于UDP上。
 * 
 * 应用场合编辑
 * 用于骨干网的SGSN和GGSN之间的Gn接口，为MS和外部分组数据网传输用户数据包。
 * 用于SGSN和RNC之间的Iu-PS接口，用于传输MS和外部PDN的用户数据包
 * 用于源SGSN和目的SGSN之间，用于inter-SGSN路由更新时，或者SRNS重定位时指向MS的未发送完的T-PDU的传输
 * 用于不同PLMN的SGSN和GGSN之间的Gp接口.  PLMN（Public Land Mobile Network，公共陆地移动网络）
# gtpc.h  
 lib\include\srslte\asn1    5452    4/1/2019    
```cpp
/* \section COPYRIGHT
 *
 * Copyright 2013-2017 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of srsLTE.
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
#ifndef SRSLTE_GTPC_H
#define SRSLTE_GTPC_H
#include <stdint.h>
#include "srslte/asn1/gtpc_msg.h"
namespace srslte{
/*GTP-C Version*/
const uint8_t GTPC_V2 = 2;
/****************************************************************************
 * GTP-C v2 Header  GPRS Turning Protocol <GPRS Turning Protocol> GPRS隧道协议
 *      1.提供主机间端到端通信
 *      2.通过隧道协议TEI在路径协议上复用
 *      3.GTP协议主要应用场景
 *      4.GTP是一组给予IP的，用于在核心网络中支持通用分组无线服务（GPRS）的通讯协议。
 *  GTP是一组基于IP的高层协议，位于TCP/IP或UDP/IP等协议上，
 *  主要用于在GSM和UMTS和LTE网络中支持通用分组无线服务(GPRS)的通讯协议。
 *  
 * GTP可以分解成三种独立的协议，GTP-C、GTP-U及GTP'。
 * GTP-C用于在GPRS核心网内传输GGSN(网关GPRS支持结点)和SGSN(服务GPRS支持结点)之间的信令，
 * 它允许SGSN代表用户激活一个会话(PDP[分组数据协议]上下文激活)，禁用激活的会话，调整服务质量参数，
 * 或更新刚从其它SGSN切换过来用户的会话。
 * GTP-U用于在GPRS核心网内，无线接入网与核心网之间传送用户数据，用户数据包可以以IPv4, IPv6或PPP中的任何格式传输。
 * GTP'(GTP prime)与GTP-C，GTP-U使用相同的信息结构，但它有独立的功能。
 * 可以用它来传输从GSM或UMTS的CDF(计费数据功能)到CGF(计费网关功能)的数据，
 * 在大部分情况下这表示传输很多独立的网元(如GGSN)的数据到中央电脑中，后者可以更方便的将计费数据传输到运营商的计费中心。
 * 在3GPP网络中实现了不同的GTP变种，包括RNC，SGSN，GGSN及CGF。GRPS移动站(MSs)连接到SGSN时不需要知道GTP的存在。
 * GTP可以用在UDP或TCP上，GTP v1仅用于UDP上。
 * 
 * 应用场合编辑
 * 用于骨干网的SGSN和GGSN之间的Gn接口，为MS和外部分组数据网传输用户数据包。
 * 用于SGSN和RNC之间的Iu-PS接口，用于传输MS和外部PDN的用户数据包
 * 用于源SGSN和目的SGSN之间，用于inter-SGSN路由更新时，或者SRNS重定位时指向MS的未发送完的T-PDU的传输
 * 用于不同PLMN的SGSN和GGSN之间的Gp接口.  PLMN（Public Land Mobile Network，公共陆地移动网络）
 *
 * Ref: 3GPP TS 29.274 v10.14.0 Section 5
 *
 *        | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 |
 *
 * 1      |  Version  | P | T | S | S | S |
 * 2      |           Message Type        |
 * 3      |         Length (1st Octet)    |
 * 4      |         Length (2nd Octet)    |
 * m      |   If T=1, TEID (1st Octet)    |
 * m+1    |   If T=1, TEID (2nd Octet)    |
 * m+2    |   If T=1, TEID (3st Octet)    |
 * m+3    |   If T=1, TEID (4st Octet)    |
 * n      |           Sequence            |
 * n+1    |           Sequence            |
 * n+2    |           Sequence            |
 * n+3    |            Spare              |
 ***************************************************************************/
typedef struct gtpc_header
{
  uint8_t version;
  bool piggyback;
  bool teid_present;
  uint8_t type;
  uint64_t teid;
  uint64_t sequence;
}gtpc_header_t;
/****************************************************************************
 * GTP-C v2 Payload
 * Ref: 3GPP TS 29.274 v10.14.0 Section 5
 *
 * Union that hold the different structures for the possible message types.
 ***************************************************************************/
typedef union gtpc_msg_choice
{
  struct gtpc_create_session_request create_session_request; //会话session
  struct gtpc_create_session_response create_session_response;
  struct gtpc_modify_bearer_request modify_bearer_request;
  struct gtpc_modify_bearer_response modify_bearer_response;
  struct gtpc_release_access_bearers_request release_access_bearers_request;
  struct gtpc_release_access_bearers_response release_access_bearers_response;
  struct gtpc_delete_session_request delete_session_request;
  struct gtpc_delete_session_response delete_session_response;
}gtpc_msg_choice_t;
/****************************************************************************
 * GTP-C v2 Message
 * Ref: 3GPP TS 29.274 v10.14.0
 *
 * This is the main structure to represent a GTP-C message. It is composed
 * of one GTP-C header and one union of structures, which can hold
 * all the possible GTP-C messages
 ***************************************************************************/
typedef struct gtpc_pdu
{
  struct gtpc_header header;
  union gtpc_msg_choice choice;
}gtpc_pdu_t;
}//namespace
#endif // SRSLTE_GTPC_H
```

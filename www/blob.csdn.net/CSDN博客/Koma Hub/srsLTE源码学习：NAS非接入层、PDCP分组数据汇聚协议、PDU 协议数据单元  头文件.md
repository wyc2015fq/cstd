# srsLTE源码学习：NAS非接入层、PDCP分组数据汇聚协议、PDU 协议数据单元   "头文件" - Koma Hub - CSDN博客
2019年04月05日 11:41:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25
- 非接入层（原名NAS - Non-access stratum）
- 分组数据汇聚协议(PDCP - Packet Data Convergence Protocol)
- 协议数据单元（PDU - Protocol Data Unit）
- 数据链路层的 PDU是数据帧（frame），
- 网络层的PDU是数据包（packet），
- 传输层的 PDU是数据段（segment），
- 其他更高层次的PDU是报文（message）。
**Table of Contents**
[interfaces_common.h](#interfaces_common.h%C2%A0%C2%A0)
# interfaces_common.h  
 lib\include\srslte\common    6490    4/5/2019    3
```cpp
/**
 *
 * \section COPYRIGHT
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
#ifndef SRSLTE_INTERFACES_COMMON_H
#define SRSLTE_INTERFACES_COMMON_H
#include "srslte/common/timers.h"
#include "srslte/common/security.h"
#include <string>
namespace srslte {
class srslte_nas_config_t //非接入层（原名NAS，Non-access stratum）
//非接入层（原名NAS，Non-access stratum）
//非接入层（原名NAS，Non-access stratum）存在于UMTS的无线通信协议栈中，
//作为核心网与用户设备之间的功能层。该层支持在这两者之间的信令和数据传输。
//所谓非接入层 [1]  （NAS）的流程和接入层的流程实际是从协议栈的角度出发的。
//在协议栈中，RRC和RANAP层及其以下的协议层称为接入层，它们之上的MM、SM、CC、SMS 等称为非接入层。//
//简单地说，接入层的流程也就是指无线接入层的设备RNC、NodeB需要参与处理的流程。//
//非接入层的流程就是指只有UE和CN需要处理的信令流程，无线接入网络RNC、NodeB是不需要处理的。//
//举个形象的比喻，接入层的信令是为非接入层的信令交互铺路搭桥的。//
//通过接入层的信令交互在UE和CN之间建立起了信令通路，从而便能进行非接入层信令流程了。
/*
接入层的流程主要包括PLMN 选择、小区选择和无线资源管理流程。
无线资源管理流程就是RRC层面的流程包括RRC连接建立流程、UE和CN之间的信令建立流程、RAB建立流程、呼叫释放流程、
切换流程和SRNS重定位流程。其中切换和SRNS重定位含有跨RNC、跨SGSN/MSC的情况?
此时还需要SGSN/MSC协助完成。所以从协议栈的层面上来说，接入层的流程都是一些底层的流程，
通过它们为上层的信令流程搭建底层的承载。非接入层的流程主要包括电路域的移动性管理、
电路域的呼叫控制、分组域的移动性管理、分组域的会话管理。UMTS的协议栈分为NAS和AS。
NAS协议处理UE和CN之间信息的传输，传输的内容可以是用户信息或控制信息，
如业务的建立、释放或者移动性管理信息。NAS消息一定程度上独立于下面的AS协议结构，
与采取什么样的无线接入网无关，可以是GSM、GPRS、WCDMA。控制平面的NAS消息有CM、MM、SM以及GMM等。
用户平面的网络层NAS协议是IP分组交换，电路交换业务不需要。NAS消息的传输要基于底层的AS协议。
AS是无线接入网采用的协议。UMTS中的AS协议包括无线接口协议、Iub协议以及Iu协议。
其中的无线接口协议是UE与UTRAN间的协议，协议的高层包括MAC、RLC、RRC等，位于UE和RNC之间，而底层PHY位于UE和NodeB之间。
*/
{
public:
  srslte_nas_config_t(uint32_t lcid_ = 0, std::string apn_ = "", std::string apn_protocol_ = "", std::string user_ = "", std::string pass_ = "", bool force_imsi_attach_ = false)
    :lcid(lcid_), //LCID属性的作用是设置或者返回一个用来指定本地或者某个区域性质的整数。
                    //比如某个地区的相关日期、时间以及货币。
     apn(apn_), //APN指一种网络接入技术，
     apn_protocol(apn_protocol_), //APN指一种网络接入技术，是通过手机上网时必须配置的一个参数，
                                   //它决定了手机通过哪种接入方式来访问网络。
     user(user_),
     pass(pass_),
     force_imsi_attach(force_imsi_attach_) //国际移动用户识别码
                                            //（IMSI：International Mobile Subscriber Identification Number）
                                            //是区别移动用户的标志
    {}
  uint32_t    lcid;
  std::string apn;
  std::string apn_protocol;
  std::string user;
  std::string pass;
  bool        force_imsi_attach;
};
class srslte_gw_config_t
{
public:
  srslte_gw_config_t(uint32_t lcid_ = 0)
  :lcid(lcid_)
  {}
  uint32_t lcid;
};
class srslte_pdcp_config_t
/*
PDCP(Packet Data Convergence Protocol)分组数据汇聚协议
PDCP 是对分组数据汇聚协议的一个简称。 
它是UMTS中的一个无线传输协议栈， 
它负责将IP头压缩和解压、传输用户数据并维护为无损的无线网络服务子系统(SRNS)设置的无线承载的序列号。
*/
{
public:
  srslte_pdcp_config_t(uint8_t bearer_id_ = 0, bool is_control_ = false, bool is_data_ = false, uint8_t direction_ = SECURITY_DIRECTION_UPLINK)
    :bearer_id(bearer_id_) //n.  持票人; 送信人; 搬运工人;
    ,direction(direction_)
    ,is_control(is_control_)
    ,is_data(is_data_)
    ,sn_len(12) {}
  uint32_t bearer_id;
  uint8_t  direction;
  bool     is_control;
  bool     is_data;
  uint8_t  sn_len;
  // TODO: Support the following configurations
  // bool do_rohc;
};
class mac_interface_timers
{
public: 
  /* Timer services with ms resolution. 
   * timer_id must be lower than MAC_NOF_UPPER_TIMERS
   */
  virtual timers::timer* timer_get(uint32_t timer_id)  = 0;
  virtual void           timer_release_id(uint32_t timer_id) = 0;
  virtual uint32_t       timer_get_unique_id() = 0;
};
class read_pdu_interface
{
/*
PDU （协议数据单元）
协议数据单元PDU（Protocol Data Unit）是指对等层次之间传递的数据单位。 
协议数据单元(Protocol Data Unit )物理层的 PDU是数据位（bit），
数据链路层的 PDU是数据帧（frame），
网络层的PDU是数据包（packet），
传输层的 PDU是数据段（segment），
其他更高层次的PDU是报文（message）。
*/
public:
  virtual int read_pdu(uint32_t lcid, uint8_t *payload, uint32_t requested_bytes) = 0; 
};
}
#endif // SRSLTE_INTERFACES_COMMON_H
```

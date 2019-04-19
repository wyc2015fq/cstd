# srsLTE源码学习：RRC：(Radio Resource Control):无线资源控制协议 - Koma Hub - CSDN博客
2019年04月01日 21:56:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：21
**Table of Contents**
[rrc_asn1.h](#rrc_asn1.h%C2%A0%C2%A0)
见过一个头文件六万多行的吗？
# rrc_asn1.h  
 lib\include\srslte\asn1    2074866    4/1/2019    15061
给出一小部分
```cpp
/*
Copyright 2013-2017 Software Radio Systems Limited
This file is part of srsLTE
srsASN1 is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.
srsASN1 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.
A copy of the GNU Affero General Public License can be found in
the LICENSE file in the top-level directory of this distribution
and at http://www.gnu.org/licenses/.
*/
#ifndef SRSASN1_RRC_H
#define SRSASN1_RRC_H
#include "asn1_utils.h"
#include <cstdio>
#include <stdarg.h>
/*
ASN.1抽象语法标记（Abstract Syntax Notation One） ASN.1是一种 ISO/ITU-T 标准，
描述了一种对数据进行表示、编码、传输和解码的数据格式。
它提供了一整套正规的格式用于描述对象的结构，而不管语言上如何执行及这些数据的具体指代，
也不用去管到底是什么样的应用程序。
在任何需要以数字方式发送信息的地方，ASN.1 都可以发送各种形式的信息（声频、视频、数据等等）。
ASN.1 和特定的 ASN.1 编码规则推进了结构化数据的传输，尤其是网络中应用程序之间的结构化数据传输，
它以一种独立于计算机架构和语言的方式来描述数据结构。
*/
namespace asn1 { //ASN.1抽象语法标记（Abstract Syntax Notation One）
namespace rrc { // RRC(Radio Resource Control):无线资源控制协议。
/*******************************************************************************
/*                        Functions for external logging
/******************************************************************************/
static log_handler_t log_handler;
static void*         callback_ctx = NULL;
inline void rrc_log_register_handler(void* ctx, log_handler_t handler)
{
  log_handler  = handler;
  callback_ctx = ctx;
}
inline void rrc_log_print(srsasn_logger_level_t log_level, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  vlog_print(log_handler, callback_ctx, log_level, format, args);
  va_end(args);
}
inline void log_invalid_access_choice_id(uint32_t val, uint32_t choice_id)
{
  rrc_log_print(LOG_LEVEL_ERROR, "The access choice id is invalid (%d!=%d)\n", val, choice_id);
}
inline void assert_choice_type(uint32_t val, uint32_t choice_id)
{
  if (val != choice_id) {
    log_invalid_access_choice_id(val, choice_id);
  }
}
inline void assert_choice_type(const std::string& access_type, const std::string& current_type,
                               const std::string& choice_type)
{
  if (access_type != current_type) {
    rrc_log_print(LOG_LEVEL_ERROR, "Invalid field access for choice type \"%s\" (\"%s\"!=\"%s\")\n",
                  choice_type.c_str(), access_type.c_str(), current_type.c_str());
  }
}
template <class NumType>
inline NumType get_enum_number(NumType* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type)
{
  if (enum_val >= nof_types) {
    rrc_log_print(LOG_LEVEL_ERROR, "The provided enum value=%d of type %s cannot be translated into a number\n",
                  enum_val, enum_type);
    return 0;
  }
  return array[enum_val];
}
}
}
```

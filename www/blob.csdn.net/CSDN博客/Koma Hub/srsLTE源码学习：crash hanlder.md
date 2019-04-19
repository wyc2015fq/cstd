# srsLTE源码学习：crash hanlder - Koma Hub - CSDN博客
2019年04月05日 10:28:29[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：16
个人分类：[C/C++																[LTE](https://blog.csdn.net/Rong_Toa/article/category/8766817)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**Table of Contents**
[crash_handler.h](#crash_handler.h%C2%A0%C2%A0)
[lib\include\srslte\common    1395    2/28/2019](#%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A01395%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A0)
[crash_handler.c](#crash_handler.c%C2%A0%C2%A0)
[lib\src\common    2396    2/28/2019    40](#%C2%A0lib%5Csrc%5Ccommon%C2%A0%C2%A0%20%C2%A02396%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A040)
# crash_handler.h  
##  lib\include\srslte\common    1395    2/28/2019    
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
/******************************************************************************
 *  File:         debug.h
 *
 *  Description:  Debug output utilities.
 *
 *  Reference:
 *****************************************************************************/
#ifndef SRSLTE_CRASH_HANDLER_H
#define SRSLTE_CRASH_HANDLER_H
#include <stdio.h>
#include "srslte/config.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void srslte_debug_handle_crash(int argc, char **argv);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SRSLTE_CRASH_HANDLER_H
```
# crash_handler.c  
##  lib\src\common    2396    2/28/2019    40
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
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
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include "srslte/version.h"
#include "srslte/common/crash_handler.h"
const static char crash_file_name[] = "./srsLTE.backtrace.crash";
static int bt_argc;
static char **bt_argv;
static void crash_handler(int sig) {
  void *array[128];
  int size;
  /* Get all stack traces */
  size = backtrace(array, 128);
  FILE *f = fopen(crash_file_name, "a");
  if (!f) {
    printf("srsLTE crashed... we could not save backtrace in '%s'...\n", crash_file_name);
  } else {
    char **symbols = backtrace_symbols(array, size);
    time_t lnTime;
    struct tm *stTime;
    char strdate[32];
    time(&lnTime);
    stTime = localtime(&lnTime);
    strftime(strdate, 32, "%d/%m/%Y %H:%M:%S", stTime);
    fprintf(f, "--- command='");
    for (int i = 0; i < bt_argc; i++) {
      fprintf(f, "%s%s", (i == 0) ? "" : " ", bt_argv[i]);
    }
    fprintf(f, "' version=%s signal=%d date='%s' ---\n", SRSLTE_VERSION_STRING, sig, strdate);
    for (int i = 0; i < size; i++) {
      fprintf(f, "\t%s\n", symbols[i]);
    }
    fprintf(f, "\n");
    printf("srsLTE crashed... backtrace saved in '%s'...\n", crash_file_name);
    fclose(f);
  }
  printf("---  exiting  ---\n");
  exit(1);
}
void srslte_debug_handle_crash(int argc, char **argv) {
  bt_argc = argc;
  bt_argv = argv;
  signal(SIGSEGV, crash_handler);
  signal(SIGABRT, crash_handler);
  signal(SIGILL, crash_handler);
  signal(SIGFPE, crash_handler);
  signal(SIGPIPE, crash_handler);
}
```

# srsLTE源码学习：生成多播信道表gen_mch_tables - Koma Hub - CSDN博客
2019年04月05日 11:19:53[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：142
个人分类：[LTE																[通信技术](https://blog.csdn.net/Rong_Toa/article/category/8758615)](https://blog.csdn.net/Rong_Toa/article/category/8766817)
**Table of Contents**
[gen_mch_tables.h](#gen_mch_tables.h%C2%A0%C2%A0)
[gen_mch_tables.c](#gen_mch_tables.c%C2%A0%C2%A0)
我在代码里做出了注释，如下：
```
sf_alloc -> 1234 4321 2345 5432 3456 6543 4567 7654  -> total 32bits
                      \  1  /\  2  / \  3  /\  4  /
sf_alloc>>18                                23 4554  
table[ 1, 2, 3, 6, 7, 8] = 234554
sf_alloc>>12                                32 3456
table[11,12,13,16,17,18] = 323456
sf_alloc>>6                                 65 4345
table[21,22,23,26,27,28] = 654345
sf_alloc>>0                                 67 7654
table[31,32,33,36,37,38] = 677654
Finally:
table[...] 02340 05540 03230 04560 06540 03450 06770 06540   -> 40 uint8_t
           \    1    / \    2    / \    3    / \    4    /
```
# gen_mch_tables.h  
 lib\include\srslte\common    1581    4/5/2019    
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
#ifndef GEN_MCH_TALBES_H
#define GEN_MCH_TALBES_H
/******************************************************************************
 * Common mch table generation - used in phch_common of UE and ENB for MBMS
 *****************************************************************************/
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void generate_frame_mch_table(uint8_t *table, uint8_t alloc);
void generate_mch_table(uint8_t *table, uint32_t sf_alloc, uint8_t num_frames); //mch multicase channel
void generate_mcch_table(uint8_t *table, uint32_t sf_alloc); //Main Control Channel_主控信道
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SECURITY_H
```
# gen_mch_tables.c  
 lib\src\common    2748    4/5/2019    25
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
#include "srslte/common/gen_mch_tables.h"
/******************************************************************************
 * Key Generation
 *****************************************************************************/
void generate_frame_table(uint8_t *table, uint8_t alloc)
{
/*
    uint8_t -> 012345678
to  table[...] = 3,4,5,6,7,8  -> 0345006780
*/
  table[1] = (alloc >> 5) & 0x01;         
  table[2] = (alloc >> 4) & 0x01;
  table[3] = (alloc >> 3) & 0x01;
  table[6] = (alloc >> 2) & 0x01;
  table[7] = (alloc >> 1) & 0x01;
  table[8] = (alloc >> 0) & 0x01;
}
void generate_mch_table(uint8_t *table, uint32_t sf_alloc, uint8_t num_frames)
{ //mch multicase channel
  if(num_frames == 1){
    uint8_t alloc = (sf_alloc) & 0x3F; // 0x3F -> 0011 1111 for -> generate_frame_table
    generate_frame_table(table, alloc);
  } else if(num_frames == 4){
    for(uint32_t j=0; j<4; j++){
      uint8_t alloc = (sf_alloc >> 6*(3-j)) & 0x3F; 
      generate_frame_table(&table[j*10], alloc);
    /*
        sf_alloc -> 1234 4321 2345 5432 3456 6543 4567 7654  -> total 32bits
                              \  1  /\  2  / \  3  /\  4  /
        sf_alloc>>18                                23 4554  
        table[ 1, 2, 3, 6, 7, 8] = 234554
        
        sf_alloc>>12                                32 3456
        table[11,12,13,16,17,18] = 323456
        
        sf_alloc>>6                                 65 4345
        table[21,22,23,26,27,28] = 654345
        
        sf_alloc>>0                                 67 7654
        table[31,32,33,36,37,38] = 677654
        Finally:
        table[...] 02340 05540 03230 04560 06540 03450 06770 06540   -> 40 uint8_t
                   \    1    / \    2    / \    3    / \    4    /
    */
    }
  }
}
void generate_mcch_table(uint8_t *table, uint32_t sf_alloc)
{
  uint8_t alloc = (sf_alloc) & 0x3F;
  generate_frame_table(table, alloc);
}
```

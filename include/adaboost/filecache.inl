
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef _MT
#include <windows.h>
#endif
#include <stdio.h>
#include "cfile.h"

typedef struct MEMMAP {
  int vcnt; // 访问计数
  size_t size; // 空间大小
  char buf[ 1 ];
}
MEMMAP, *PMEMMAP;

typedef struct tagFILECACHE {
  size_t base_pos;
  size_t max_block_num;
  size_t block_num;
  size_t block_size;
  PMEMMAP* mp;
#ifdef _MT
  
  CRITICAL_SECTION csRecvRead;
#endif
}
FILECACHE;


size_t MAX_MEMORY = (1 << 29);

#ifdef _MT
#define INITIALIZECRITICALSECTION(_CS) InitializeCriticalSection(_CS)
#define DELETECRITICALSECTION(_CS)     DeleteCriticalSection(_CS)
#define ENTERCRITICALSECTION(_CS)      EnterCriticalSection(_CS)
#define LEAVECRITICALSECTION(_CS)      LeaveCriticalSection(_CS)
#else
#define INITIALIZECRITICALSECTION(_CS) {}
#define DELETECRITICALSECTION(_CS)     {}
#define ENTERCRITICALSECTION(_CS)      {}
#define LEAVECRITICALSECTION(_CS)      {}
#endif

FILECACHE* fc_open(size_t base_pos, size_t max_block_num, size_t block_size)
{
  FILECACHE* fmp;
  //assert(fmp->block_size>0);
  fmp = MALLOC(FILECACHE, 1);
  assert(NULL != fmp && "NULL pointer");
  fmp->base_pos = base_pos;
  fmp->max_block_num = max_block_num;
  fmp->block_size = block_size;
  fmp->block_num = 0;
  fmp->mp = MALLOC(PMEMMAP, max_block_num);
  MEMZERO(fmp->mp, max_block_num);
  INITIALIZECRITICALSECTION(&fmp->csRecvRead);     // 临界区初始化
  return fmp;
}

void fc_close(FILECACHE* fmp)
{
  size_t i;

  if (fmp) {
    DELETECRITICALSECTION(&fmp->csRecvRead);     // 临界区初始化

    for (i = 0; i < fmp->block_num; ++i) {
      FREE(fmp->mp[ i ]);
    }

    FREE(fmp->mp);
    FREE(fmp);
  }

  return ;
}

void* fc_get(FILECACHE* fmp, size_t nth_block, HD_FILE hFile)
{
  assert(NULL != fmp && "err pointer");
  assert(nth_block < fmp->max_block_num);
  ENTERCRITICALSECTION(&fmp->csRecvRead);   // 使用临界区变量

  if (NULL == fmp->mp[ nth_block ]) {
    if (
        (fmp->block_num * fmp->block_size) < MAX_MEMORY
        && (fmp->mp[ nth_block ] = (PMEMMAP) malloc(fmp->block_size + sizeof(MEMMAP) - 1))
    ) {
      fmp->mp[ nth_block ] ->vcnt = -1;
      fmp->mp[ nth_block ] ->size = fmp->block_size;
      ++(fmp->block_num);
    }
    else {
      size_t i;
      int minvcnt = 1000000000, maxvcnt = -1, mini = -1, maxi = -1, k;
      MAX_MEMORY = (fmp->block_num * fmp->block_size);

      for (i = 0; i < fmp->max_block_num; ++i) {
        if (NULL != fmp->mp[ i ]) {
          if (fmp->mp[ i ] ->vcnt < minvcnt) {
            minvcnt = fmp->mp[ i ] ->vcnt;
            mini = i;
          }

          if (fmp->mp[ i ] ->vcnt > maxvcnt) {
            maxvcnt = fmp->mp[ i ] ->vcnt;
            maxi = i;
          }
        }
      }

      //if (maxi>=0 && fmp->mp[maxi] && fmp->block_num>10) {
      //  fmp->mp[maxi]->vcnt = 0;
      //}
      // assert(mini>=0 && "no min block!");
      k = maxi;

      if (k >= 0 && fmp->mp[ k ]) {
        fmp->mp[ nth_block ] = fmp->mp[ k ];
        fmp->mp[ nth_block ] ->vcnt = -1;
        fmp->mp[ k ] = NULL;
      }
    }

    if (!INVALID(hFile) && fmp->mp[ nth_block ]) {
      FSEEKSET(hFile, nth_block * fmp->block_size);
      FREAD(fmp->mp[ nth_block ] ->buf, fmp->block_size, hFile);
    }
  }

  assert(NULL != fmp->mp[ nth_block ] && "内存分配失败!");
  fmp->mp[ nth_block ] ->vcnt++;
  LEAVECRITICALSECTION(&fmp->csRecvRead);
  return fmp->mp[ nth_block ] ->buf;
}

size_t fc_write(FILECACHE* fmp, size_t nth_block, HD_FILE hFile)
{
  size_t ret = 0;
  assert(NULL != fmp && "err pointer");
  assert(nth_block < fmp->block_num);
  assert(NULL != fmp->mp[ nth_block ]);
  ENTERCRITICALSECTION(&fmp->csRecvRead);   // 使用临界区变量

  if (NULL != fmp->mp[ nth_block ] && hFile) {
    FSEEKSET(hFile, nth_block * fmp->block_size);
    ret = FWRITE(fmp->mp[ nth_block ] ->buf, fmp->block_size, hFile);
  }

  LEAVECRITICALSECTION(&fmp->csRecvRead);
  return ret;
}

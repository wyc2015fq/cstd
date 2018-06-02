
#ifndef _ICTCLAS_INL_
#define _ICTCLAS_INL_
#include <io.h>
#include "cstd.h"
#include "cfile.h"
#include "Result.h"

bool IsDataExists()
{
  char sDataFiles[][40] = {
    "Data/BigramDict.dct",
      "Data/coreDict.dct",
      "Data/lexical.ctx",
      "Data/nr.dct",
      "Data/nr.ctx",
      "Data/ns.dct",
      "Data/ns.ctx",
      "Data/tr.dct",
      "Data/tr.ctx",
      ""
                          };
  int i = 0;

  while (sDataFiles[i][0] != 0) {
    if ((_access(sDataFiles[i], 0)) == -1) {
      return false;
    }

    i++;
  }

  return true;
}

#undef ELEMENT_TYPE

#endif // _ICTCLAS_INL_

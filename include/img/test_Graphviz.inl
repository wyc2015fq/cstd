
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <assert.h>
#include "cstd.h"
#include "window.h"
#include "imresize.h"
#include "debug.h"

#include "xtypes.h"
#include "macro.h"
#include "fractal.h"

#include "log.h"

int main()
{
  MEM_CHECK_BEGIN()
  test_graphviz();
  MEM_CHECK_END()
  return 0;
}


//#include "gmalloc.h"
#include "iris_detect.inl"
#include "dsp_mem.inl"

void iris_recog_close() {}
void iris_recog_open() {}
int iris_recog_process(void* p0)
{
  iris_detect((irisrecog_t*)p0);
  return 0;
}

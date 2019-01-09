#ifndef _DSP_MEM_INL_
#define _DSP_MEM_INL_
void* dsp_malloc(size_t n)
{
  return malloc(n);
}
void dsp_free(void* buf)
{
  if (buf) {
    free(buf);
  }
}
void* dsp_memclone(void* p, size_t n)
{
  void* a;
  a = dsp_malloc(n);

  if (a) {
    memcpy(a, p, n);
  }
  else {
    printf("memclone fail\n");
  }

  return a;
}
#endif // _DSP_MEM_INL_


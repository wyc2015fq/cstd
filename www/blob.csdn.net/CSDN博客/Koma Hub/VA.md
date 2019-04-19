# VA - Koma Hub - CSDN博客
2019年01月29日 21:55:46[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：46
glibc-2.28：err.c    misc    3818    2018/8/1    98
```cpp
#define VA(call)							      \
{									      \
  va_list ap;								      \
  va_start (ap, format);						      \
  call;									      \
  va_end (ap);								      \
}
void
warn (const char *format, ...)
{
  VA (vwarn (format, ap))
}
libc_hidden_def (warn)
extern void vwarn (const char *__format, __gnuc_va_list)
     __attribute__ ((__format__ (__printf__, 1, 0)));
```

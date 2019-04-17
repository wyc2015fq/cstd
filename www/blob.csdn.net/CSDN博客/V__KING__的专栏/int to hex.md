# int to hex - V__KING__的专栏 - CSDN博客





2016年06月14日 10:54:23[v__king__](https://me.csdn.net/V__KING__)阅读数：1005








int类型，long类型装换成16进制

```
char *printNumber(unsigned long n, uint8_t base)
{
  static char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
  return str;
}
```




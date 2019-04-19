# python string hash 函数 - Arthur的随笔 - CSDN博客
2012年02月03日 15:09:19[largetalk](https://me.csdn.net/largetalk)阅读数：10404
```cpp
static long
string_hash(PyStringObject *a)
{
    register Py_ssize_t len;
    register unsigned char *p;
    register long x;
    if (a->ob_shash != -1)
        return a->ob_shash;
    len = Py_SIZE(a);
    p = (unsigned char *) a->ob_sval;
    x = *p << 7;
    while (--len >= 0)
        x = (1000003*x) ^ *p++;
    x ^= Py_SIZE(a);
    if (x == -1)
        x = -2;
    a->ob_shash = x;
    return x;
}
```
添加几个hash函数
```cpp
#include "stdio.h"
#include "string.h"
unsigned int RS_Hash(char *str)
{
    unsigned int a = 63689;
    unsigned int b = 378551;
    unsigned int v = 0;
    while(*str)
    {
        v = v * a + (*str++);
        a *= b;
    }
    return v;
}
unsigned int JS_Hash(char *str, unsigned int len)
{
    unsigned int hash = 1315423911;
    unsigned int i = 0;
    for(i = 0;i < len;str++, i++)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}
unsigned int ELF_Hash(char *str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;
    for(i=0; i<len; str++,i++)
    {
        hash = (hash << 4) + (*str);
        if((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x>>24);
        }
        hash &= ~x;
    }
    return hash;
}
unsigned int BP_Hash(char *str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = hash << 7 ^ (*str);
    }
    return hash;
}
long PY_Hash(unsigned char *str, int len)
{
    long x;
    int old_len = len;
    x = *str << 7;
    while(--len >= 0)
    {
        x = (1000003*x) ^ (*str++);
    }
    x ^= old_len;
    if( x == -1)
        x == -2;
    return x;
}
int main(void)
{
    char* str = "abc";
    int len = strlen(str);
    printf("%s RS Hash value is %u\n", str, RS_Hash(str));
    printf("%s JS Hash value is %u\n", str, JS_Hash(str, len));
    printf("%s ELF Hash value is %u\n", str, ELF_Hash(str, len));
    printf("%s BP Hash value is %u\n", str, BP_Hash(str, len));
    printf("%s PY Hash value is %ld\n", str, PY_Hash(str, len));
}
```

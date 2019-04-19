# How to print 如何输出 int64_t，uint64_t的值 in C - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月22日 17:29:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1777
For `int64_t` type:
```
int64_t t;
printf("%"PRId64"\n", t);
```
for `uint64_t` type:
```
uint64_t t;
printf("%"PRIu64"\n", t);
```
you can also use `PRIx64` to
 print in hexadecimal.
These macros are defined in `inttypes.h`

# ANSI C: union - Koma Hub - CSDN博客
2019年02月01日 22:07:40[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：14
个人分类：[ANSI C](https://blog.csdn.net/Rong_Toa/article/category/8653945)
```cpp
union bits32_tag {
int whole; /* one 32-bit value*/
struct {char c0,c1,c2,c3;} byte; /* four 8-bit bytes*/
} value;
```

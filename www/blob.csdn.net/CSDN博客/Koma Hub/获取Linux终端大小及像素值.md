# 获取Linux终端大小及像素值 - Koma Hub - CSDN博客
2018年05月11日 18:36:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：260
```cpp
#include <sys/ioctl.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    printf("%i %i %i %i\n", sz.ws_col, sz.ws_row, 
                            sz.ws_xpixel, sz.ws_ypixel);
    return 0;
}
```
编译运行：
```
$ gcc main.c 
$ ./a.out 
62 14 0 0
```

# C/C++搞不懂指针还是数组？用sizeof解释struct大小！ - Koma Hub - CSDN博客
2018年05月08日 14:07:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：237
**数组？指针？**
听说c++打算废弃指针了，谁让指针这么难呢！
我的环境：
```
>uname -a
CYGWIN_NT-10.0-WOW DESKTOP-499IG24 2.10.0(0.325/5/3) 2018-02-02 15:21 i686 Cygwin
```
可见为32bit内核，也就是指针所占空间的sizeof为4.
那么这段程序：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*main函数*/
int main(int argc, char**argv)
{
    char s1[] = "abcdefg";
    char s2[] = {'a','b','c','d','e','f','g'};
    char s3[] = {"abcdefg"};
    char *s4 = "abcdefg";
    //char *s5 = {'a','b','c','d','e','f','g'};//error
    char *s6 = {"abcdefg"};
    
    printf("s:%s, length:%2d, sizeof:%2d\n",s1,strlen(s1),sizeof(s1));
    printf("s:%s, length:%2d, sizeof:%2d\n",s2,strlen(s2),sizeof(s2));
    printf("s:%s, length:%2d, sizeof:%2d\n",s3,strlen(s3),sizeof(s3));
    printf("s:%s, length:%2d, sizeof:%2d\n",s4,strlen(s4),sizeof(s4));
    //printf("s:%s, length:%2d, sizeof:%2d\n",s5,strlen(s5),sizeof(s5));//error
    printf("s:%s, length:%2d, sizeof:%2d\n",s6,strlen(s6),sizeof(s6));
    
    s1[0] = 'z';
    s2[0] = 'z';
    s3[0] = 'z';
    //s4[0] = 'z';//error
    //s5[0] = 'z';//error
    //s6[0] = 'z';//error
    
    return 1;
}
```
的输出为：
```
>gcc string.c
>a.exe
s:abcdefg, length: 7, sizeof: 8
s:abcdefgabcdefg, length:14, sizeof: 7
s:abcdefg, length: 7, sizeof: 8
s:abcdefg, length: 7, sizeof: 4
s:abcdefg, length: 7, sizeof: 4
```
那么这几个的sizeof为什么不一样呢？
32bit系统内核的指针大小为4，后面的两个4不难解释，上面的两个8可以解释为自动补齐（类似于struct，struct是按照最大字节数补齐），而大小为7的则是对应的数组。
**sizeof(结构体)**
一下均采用如下程序打印：
```cpp
printf("%d+%d+%d+%d = %d\n",
        sizeof(int),sizeof(char),sizeof(float),sizeof(double),sizeof(S));
```
需要搞明白的是struct的自动对齐机制：
**例1：**
```cpp
struct{
    int a;
    char ch;
    float f;
    double d;
}S;
```
输出结果为：
```
>gcc string.c
>a.exe
4+1+4+8 = 24
```
![](https://img-blog.csdn.net/20180508140438293)
**例2：**
```cpp
struct{
    int a;
    char ch;
    float f;
}S;
```
结果：
```
>gcc string.c
>a.exe
4+1+4+8 = 12
```
![](https://img-blog.csdn.net/20180508140452854)
**例3：**
```java
struct{
    float f;
    double d;
}S;
```
结果：
```
>gcc string.c
>a.exe
4+1+4+8 = 16
```
![](https://img-blog.csdn.net/20180508140507366)
**例4：**
```cpp
struct{
    int a;
    double d;
}S;
```
结果：
```
>gcc string.c
>a.exe
4+1+4+8 = 16
```
![](https://img-blog.csdn.net/20180508140518969)

# 【编译制导指令】#pragma pack - 字节数基准对齐 - Koma Hub - CSDN博客
2018年06月13日 22:36:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：84

## `#pragma pack(1)`
struct {
    char c;
    int i;
}S;
intmain()
{
    cout<<sizeof(S)<<endl;
}
那么，结果为：
```
$ g++ main.cpp
$ ./a.exe
8
```
如果
#pragma pack(1)
struct {
    char c;
    int i;
}S;
这样的结果为`5`。
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/pragma-pack.md#pragma-pack2)`#pragma pack(2)`
如果是这样
#pragma pack(2)
struct {
    char c;
    int i;
}S;
那么输出结果为`6`
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/pragma-pack.md#pragma-pack3)`#pragma pack(3)`
错误信息
```
$ g++ main.cpp
main.cpp:5:16: 警告：对齐边界必须是 2 的较小次方，而不是 3 [-Wpragmas]
 #pragma pack(64)
                ^
```
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/pragma-pack.md#pragma-packpush1%E4%B8%8Epragma-pack1%E7%9A%84%E5%8C%BA%E5%88%AB)`#pragma pack(push,1)`与`#pragma pack(1)`的区别
`#pragma pack (n)`C编译器将按照n个字节对齐。 `#pragma pack ()`取消自定义字节对齐方式。 `#pragma pack (push,1)`把原来对齐方式设置压栈，并设新的1个字节对齐 `#pragma pack(pop)`恢复对齐状态
例如：
#pragma pack(push) //保存当前对齐状态
#pragma pack(4)    //设定为4字节对齐 
相当于
#pragma  pack (push,4)
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/pragma-pack.md#%E4%BE%8B%E5%AD%90)例子
#include<iostream>usingnamespacestd;
#pragma pack(1)
struct {
    char c;
    int i;
    double d;
}S1;
#pragma pack() //与上面#pragma pack(4)对应
#pragma pack(4)
struct {
    char c;
    int i;
    double d;
}S2;
#pragma pack()
#pragma pack(push,4)
struct {
    char c;
    int i;
    double d;
}S3;
#pragma pack(pop)
intmain()
{
    cout<<sizeof(S1)<<endl;
    cout<<sizeof(S2)<<endl;
    cout<<sizeof(S3)<<endl;
}
结果为：
```
$ g++ main.cpp
$ ./a.exe
13
16
16
```

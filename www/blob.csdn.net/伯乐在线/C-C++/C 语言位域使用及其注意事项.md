# C 语言位域使用及其注意事项 - 文章 - 伯乐在线
原文出处： [zhenhuaqin](http://blog.chinaunix.net/uid-21411227-id-3512376.html)
所谓“位域”是把一个字节中的二进位划分为几个不同的区域，并说明每个区域的位数。每个域有一个域名，允许在程序中按域名进行操作。这样就可以把几个不同的对象用一个字节的二进制位域来表示。位域的定义和位域变量的说明位域定义与结构定义相仿，其形式为：
struct 位域结构名
{ 位域列表 };
eg：


```
struct weiyu {
                  int a:2;
                  int b:5;
                  int   :5;//此五位为空域，不能使用       
                  int c:3;
                   }
```
位域虽然简单好用，但使用时需要注意：
1) 如果相邻位域字段的类型相同，且其位宽之和小于类型的sizeof大小，则后面的字
段将紧邻前一个字段存储，直到不能容纳为止；
2) 如果相邻位域字段的类型相同，但其位宽之和大于类型的sizeof大小，则后面的字
段将从新的存储单元开始，其偏移量为其类型大小的整数倍；
3) 整个结构体的总大小为最宽基本类型成员大小的整数倍。
4) 如果相邻的位域字段的类型不同，则各编译器的具体实现有差异，VC6采取不压缩方
式，Dev-C++采取压缩方式；
5) 如果位域字段之间穿插着非位域字段，则不进行压缩；（不针对所有的编译器）
4 ，5跟编译器有较大的关系，使用时要慎重，尽量避免。


```
#include "stdio.h"
void main(int argn ,char *argv)
{
    struct     test {
        unsigned a:10;
        unsigned b:10;
        unsigned c:6;
        unsigned :2;//this two bytes can't use
        unsigned d:4;
        }data,*pData;
    data.a=0x177;
    data.b=0x111;
    data.c=0x7;
    data.d=0x8;
   
    pData=&data;
    printf("data.a=%x data.b= %x data.c=%x data.d=%xn",pData->a,pData->b,pData->c,pData->d);//位域可以使用指针
    printf("sizeof(data)=%dn",sizeof(data));   //4 bytes ，最常用的情况
    struct testLen{
    char a:5;
    char b:5;
    char c:5;
    char d:5;
    char e:5;
    }len;
   
    printf("sizeof(len)=%dn",sizeof(len));     //5bytes 规则2
    struct testLen1{
        char a:5;
        char b:2;
        char d:3;
        char c:2;
        char e:7;
        }len1;
    printf("sizeof(len1) =%dn",sizeof(len1));    //3bytes 规则1
    struct testLen2{
        char a:2;
        char :3;
        char b:7;
        long d:20; //4bytes
        char e:4;
        }len2;
    printf("sizeof(len2)=%dn",sizeof(len2));  //12 规则3，4,5，总长为4的整数倍，2+3 占1byte，b占1bye 由于与long对其，2+3+7 占4字节，后面 d 与 e进行了优化 占一个4字节
    struct testLen3{
        char a:2;
        char :3;
        char b:7;
        long d:30;
        char e:4;
        }len3;
    printf("sizeof(len3)=%dn",sizeof(len3));//12 规则3，4，5，总长为4的整数倍，2+3 占1byte，b占1bye 由于与long对其，2+3+7 占4字节，后面 d占一个4字节，为了保证与long对其e独占一个4字节
}
```

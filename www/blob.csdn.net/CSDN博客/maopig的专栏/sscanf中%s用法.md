# sscanf中%s用法 - maopig的专栏 - CSDN博客
2015年01月08日 15:35:06[maopig](https://me.csdn.net/maopig)阅读数：6301
                
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char Buffer[512] = {" "};
char IP[512]={""};
char netmask[512];
strcpy(Buffer,"/sbin/ifconfig eth0 172.16.129.150 netmask 255.255.255.0 up");
    sscanf (Buffer, "%*s %*s %s %*s %s ", IP,netmask );//%*s 忽略字符串，直至到空格作为结束字样
    printf ( "ip= %s ,netmask %s\n", IP ,netmask);
  //printf(" IP [%*s ]\n",20,rcSBuffer);
  //sprintf( aa, "%*s", 100, 30 )
}
打印结果：
ip= 172.16.129.150 ,netmask 255.255.255.0
/*
scanf("%*s")表示跳至下一空白字符，这里主要是中间的*字符起的作用。比如说：
C/C++ code
?
1
2
3
4
5
int n;
   scanf("%*d %*d %d",&n);
   printf("%d",n);
   return 0;
你输入的是1 2 3，那么输出的是3，因为前两个已经忽略啦。
*/
/*
sprintf 跟printf 在用法上几乎一样，只是打印的目的地不同而已，前者打印到字符串中，后者则直接在命令行上输出.
sprintf 是个变参函数，定义如下：
int sprintf( char *buffer, const char *format [, argument] ... );
除了前两个参数类型固定外，后面可以接任意多个参数。而它的精华，显然就在第二个参数：
*/
            

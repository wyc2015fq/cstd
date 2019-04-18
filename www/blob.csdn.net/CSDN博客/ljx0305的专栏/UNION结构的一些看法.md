# UNION结构的一些看法 - ljx0305的专栏 - CSDN博客
2008年07月29日 19:48:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1019标签：[motorola																[ibm																[存储																[平台																[include																[x86](https://so.csdn.net/so/search/s.do?q=x86&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=ibm&t=blog)](https://so.csdn.net/so/search/s.do?q=motorola&t=blog)
个人分类：[C](https://blog.csdn.net/ljx0305/article/category/394717)
#include  "stdio.h"    
typedef union 
{    
  char  a[2];    
  int  i;    
}abc;    
void main() 
{    
    abc  in;    
    int  x1,x2;  
    char alx,ahx;/*alx是al寄存器的值，ahx是ah寄存器的值*/    
    in.i=0x128; 
    printf("/na[0]=0x%x",in.a[0]); //a[0]=0x28; 
    printf("/nin.i=%x",in.i); 
    printf("/na[1]=0x%x",in.a[1]);//a[1]=0x1; 
    x1=in.i; 
    printf("/nx1=0x%x",x1);//x1=128 
    printf("/nax=x2=0x%x",x2);//ax=x2=0xcccccccc 
    printf("/nal=0x%x",alx);//al=0xffffffcc 
    printf("/nah=0x%x",ahx);//ah=0xffffffcc 
}    
联合体是所有成员共享内存空间
首先,要搞清楚不同平台上内存存储的字节序(Byte order)是不同的, 在x86平台(Intel, AMD, IBM兼容)上是从低字节向高字节存储/读取, 叫little-endian. 
例如: 
int a = 0x0A0B0C0D 在内存中存储: 
低位 --> 0D 
        0C 
        0B 
高位 --> 0A 
在非x86的平台上(MAC, IBM非兼容机, Motorola)是从高字节向低字节存储/读取, 叫big-endian. 
还是以上面为例: 
高位 --> 0D 
        0C 
        0B 
低位 --> 0A
Windows下的int是32位的，也就是4个字节 而char a[2]，只占两个字节，因些以int类型的4个字节来分配空间
in.i = 0x128 
实际上i就是0x00000128，在内存中的排序0x28 0x01 0x00 0x00 
因为你定义的union中a为2个字符，所以对以的就是0x28和0x01 
至于x2输出0xcccccccc的原因是因为你没有对x2进行初始化，而且在VC的Debug版本程序中未初始化的栈空间都是CC CC CC ... 
alx,ahx同理，也就是0xCC，那为什么它们输出0xffffffcc呢？ 
这是因为你将alx,ahx当作int来输出了，printf传入参数是隐式将其转为int 
又由于char也是有符号的，因此它的符号位就扩展到高字节中，也就是0xffffffcc了 
程序改动如下 
    in.i=0x10111213; 
    printf("/na[0]=0x%x &a[0]=0x%x",in.a[0],&in.a[0]); 
    printf("/na[1]=0x%x &a[1]=0x%x",in.a[1],&in.a[1]); 
    printf("/na[2]=0x%x &a[2]=0x%x",in.a[2],&in.a[2]); 
    printf("/na[3]=0x%x &a[3]=0x%x",in.a[3],&in.a[3]); 
    printf("/nin.i=0x%x &in.i=0x%x",in.i,&in.i); 
当然a[2],a[3]已经越界，不过在自己控制范围内。结果如下： 
a[0]=0x13 &a[0]=0x22ff8c 
a[1]=0x12 &a[1]=0x22ff8d 
a[2]=0x11 &a[2]=0x22ff8e 
a[3]=0x10 &a[3]=0x22ff8f 
in.i=0x10111213 &in.i=0x22ff8c 

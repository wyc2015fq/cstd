# c语言基础知识_结构体访问，共同体，枚举类型 - maopig的专栏 - CSDN博客
2011年09月07日 13:59:12[maopig](https://me.csdn.net/maopig)阅读数：2204
C语言结构体变量的引用：
　　对于定义的结构体类型的普通变量，访问其成员用圆点运算符（“.”），标准访问方式如下：
结构体变量名.成员名
　　对于定义为指向结构体的指针变量，用箭头运算符（“->”）访问，标准的访问形式如下：
指向结构体的指针变量名->成员名
当出现结构体嵌套时，访问结构体成员：
指向结构体的指针变量名
-> 次结构体名.成员名
例如：
定义
typedef struct mystruct{
　　int i;
　　char *p;
　　struct *temp{
　　　　int a;
　　　　int *b;
　　}
}MYDEV;
MYDEV mydev1;
MYDEV *mydev2;
则有以下几种引用方式：
1)组
mydev1.i和mydev1.p
mydev2->i和mydev2->p
2)组
mydev1.temp->a和mydev1.temp->b
mydev2->temp->a和mydev2->temp->b
实例：
#include "stdafx.h"
#include <windows.h>
//#include "log.h"
#include "msglog.h"
#include "stdlib.h"
int b;
typedef  struct  _Atmp
{
    int d;
    int *e;
} Atmp;
typedef struct  _Bdata 
{
    int a;
    char *s;
    struct Ctmp
    {
        int b;
        int c;
    } FF ;
   Atmp *atmp;
   Atmp EE;
} dataA, *dataB;
int _tmain(int argc, _TCHAR* argv[])
{
   int X=3,Y=5;
dataA *A=(dataA *)malloc(sizeof(dataA));
    memset(A,0,sizeof(A));
    A->a=0;
    A->s="Test A!";
    //A.tmp.b=X;
    //A.tmp.c=Y;
    A->atmp = (Atmp *)malloc(sizeof(Atmp));
    A->atmp->d=X;
    A->atmp->e=&Y;
    A->FF.b=X;
    A->EE.d=X;
    A->EE.e=&Y;
dataA B;
    B.a=X;
    B.s="TEST B";
    B.FF.b=X;
    B.FF.c=X;
    B.EE.d=X;
    B.EE.e=&Y;
    B.atmp=(Atmp *)malloc(sizeof(Atmp));
    B.atmp->d=X;
    B.atmp->e=&Y;
dataB C=(dataB)malloc(sizeof(dataB));
    memset(C,0,sizeof(dataB));
    C->a=X;
    C->s="Test C";
    C->FF.b=X;
    C->FF.c=X;
    C->atmp=(Atmp *)malloc(sizeof(Atmp));
    C->atmp->d=X;
    C->atmp->e=&Y;
    C->EE.d=X;
    C->EE.e=&Y;
     return 0;
}
枚举类型：
#include "stdafx.h"
#include "msglog.h"
#define A_TEST_a
typedef enum _CHAR_TYPE 
{
 A_TEST,
 B_TEST,
 C_TEST,
 D_TEST
} CHAR_TYPE;
int _tmain(int argc, _TCHAR* argv[])
{
 int a,b;
 a=3;b=4;
#ifdef A_TEST
   a=5;
#endif
enum _CHAR_TYPE AA=A_TEST;;
switch(AA)
{
case A_TEST:a=10;break;
} 
LOG(LEVEL_INFO,"AA=%d\n",a);
CHAR_TYPE BB=B_TEST;
switch(BB)
{
  case B_TEST: a=15; break;
  case C_TEST: a=20;
  default: a=30;
}
LOG(LEVEL_INFO,"BB=%d\n",a);
int AB=3;
switch(AB)
{
case D_TEST :a=40;break;
}
LOG(LEVEL_INFO,"AB=%d\n",a);
int c=20;
CHAR_TYPE CC=(CHAR_TYPE)20; 
//数值强制转化为枚举类型才能赋值
return 0;
结果：
 AA=10
BB=15
AB=40
结构体重常用的指针转换
#include "stdio.h"
#include "string.h"
#define  NumSize 100
typedef struct _test
{
 int a;
} test;
typedef struct _testA
{
 int c;
 int d;
 char a[NumSize];
} testA;
int main()
{
 testA bb,dd;
 int cc;
 bb.c=10;//bb.d=20;
 bb.a[1]='A';
 // bb.a[]="abc";//错误
 test *aa;
 aa=(test *)&bb;//记住地址需要转换
 printf(" aa->a =%d \n",aa->a);
 static char c[]="BASIC_dBASE"; 
 char b[80]="Study C Programme!\n";
 printf("c[]=%s \n",c);
 printf("b[100]= %s \n",b);
    memset(bb.a,0,sizeof(char)*NumSize);
 memcpy(bb.a,b,sizeof(char)*strlen(b));
 printf("bb.a = %s \n",bb.a);
    // dd.a=b; //错误
 char *ff;
 ff="abc";
 printf(" ff = %s \n",ff);
 int *gg,kk;
 gg =(int *)&bb;
 kk =*(int *)&bb;
 printf(" gg = %d ,kk= %d\n",*gg,kk);
 int *gg,kk;
 gg =(int *)&bb; //指针转换
 kk =*(int *)&bb;
 printf(" gg = %d ,kk= %d\n",*gg,kk);
 char *s="keil is wv \n";
// s[1]='z'; //s为栈内指针，指向常量存储区，所以不能改变常量存储区的值
 printf(" s = %s \n",s);
 char sz[]="keil is wv\n";
    sz[2]='s';//sz为栈内指正，同时指向的字符串同样是在栈内，所以能够用此方式改变变量值
 printf(" sz = %s \n",sz)
 return 0;
 return 0;
}
结果：
aa->a =10
c[]=BASIC_dBASE
b[100]= Study C Programme!
bb.a = Study C Programme!
 ff=abc;
gg = 10 ,kk=10
s = keil is wv
sz = kesl is wv

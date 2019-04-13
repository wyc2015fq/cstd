
# C++动态数组 - forever1dreamsxx--NLP - CSDN博客


2012年08月30日 17:09:14[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：402


本文来自: IT知道网([[url]http://www.itwis.com[/url]](http://www.itwis.com/)) 详细出处参考：[[url]http://www.itwis.com/html/c/ccc/20090226/3431.html[/url]](http://www.itwis.com/html/c/ccc/20090226/3431.html)
[http://daiyanfei.blog.51cto.com/698782/139513](http://daiyanfei.blog.51cto.com/698782/139513)
1.变长一维数组
这里说的变长数组是指在编译时不能确定数组长度，程序在运行时需要动态分配内存空间的数组。实现变长数组最简单的是变长一维数组，你可以这样做：
//文件名:   array01.cpp
\#include<iostream>
using   namespace   std;
int   main()
{
int   len;
cin>>len;
//用指针p指向new动态分配的长度为len*sizeof(int)的内存空间
int   *p=new   int[len];
...........
delete[]   p;
return   0;
}
注意int   *p=new   int[len];这一句，你不能这样做：
int   p[len];
C++编译器会报错说len的大小不能确定，因为用这种形式声明数组，数组的大小需要在编译时确定。而且这样也不行：
int   p[]=new   int[len];
编译器会说不能把int*型转化为int[]型，因为用new开辟了一段内存空间后会返回这段内存的首地址，所以要把这个地址赋给一个指针，所以要用int   *p=new   int[len];
array01.cpp实现了一个变长的一维数组，但是要养成一个好习惯，就是注意要注销指针p，使程序释放用new开辟的内存空间。
当然使用C++标准模版库（STL）中的vector（向量）也可以实现变长数组：
//文件名:   array02.cpp
\#include<iostream>
\#include<vector>
using   namespace   std;
int   main()
{
int   len;
cin>>len;
vector<int>   array(len);//声明变长数组
for(int   i=0;i<len;i++)
{
array[i]=i;
cout<<array[i]<<"\t";
}
return   0;
}
这里的变长数组让我联想到了java的java.util包中的vector和C\#中的ArrayList，它们也可以在各自的语言中实现变长数组。不过C++中的vector不能像C\#一样有托管的垃圾回收机制回收被占用的内存空间，但是你可以在使用完vector后调用~vector()析构函数释放内存。
2.变长n维数组
变长的n维数组实现起来有些麻烦，但是在工程与软件设计应用中常使用的是二维数组，所以在这里着重介绍变长的二维数组，变长的n维数组可以按照类似的方法实现。首先看一个经典的用C实现变长二维数组的例子：
//文件名:   array03.c
\#include     <stdio.h>
\#include     <malloc.h>
void     main()
{
int     x,y,i,j;
float     **a,*b;
printf("请输入你所求解的线性方程组的行数x：x=");
scanf("%d",&x);
printf("请输入你所求解的线性方程组的列数y：y=");
scanf("%d",&y);
a=(float     **)malloc(sizeof(float     *)     *x);
b=(float     *)malloc(sizeof(float)     *x);
for(i=0;i<x;i++)
{
*(a+i)=(float     *)malloc(sizeof(float)     *y);
}
/*读入数据*/
printf("请按行的顺序依次输入系数的值(共%d项)：",x*y);
for(i=0;i<=x-1;i++)
for(j=0;j<=y-1;j++)
scanf("%f",&a[i][j]);
printf("请按列的顺序依次输入常数的值(共%d项)：",x);
for(j=0;j<=x-1;j++)
scanf("%f",&b[j]);
printf("您输入方程组的增广矩阵为：\n");
for(i=0;i<=x-1;i++)
{
for(j=0;j<=y-1;j++)
printf("%.5f         ",a[i][j]);
printf("%.5f         ",b[i]);
printf("\n");
}
free(b);
for(i=0;i<x;i++)
free     (*(a+i));
}
那么用C++怎样实现呢？在C++中可以通过new和delete运算符动态开辟和释放空间，其中new与C中malloc函数的功能相似，delete与C中free函数的功能相似。用C++实现变长二维数组时可以采用两种方法：双指针方法和使用STL中vector（向量）的方法。
首先介绍一下双指针方法，在这里双指针就是指像指针的指针，比如你可以这样声明一个数组：
int   **p   =   new   int*[num1];
而对每一个*p（一共num1个*p）申请一组内存空间：
for(int   i=0;   i<num1;   ++i)
p[i]   =   new   int[num2];
其中，num1是行数，num2是数组的列数。测试的源程序如下：
//文件名:   array04.cpp
\#include   <iostream>
\#include   <iomanip>
using   namespace   std;
int   main()
{
int   num1,//行数
num2;//列数
cout<<"Please   enter   the   number   for   row   and   column:   "<<endl;
cin   >>   num1   >>   num2;
//为二维数组开辟空间
int   **p   =   new   int*[num1];
for(int   i=0;   i<num1;   ++i)
p[i]   =   new   int[num2];
for(int   j=0;j<num1;j++)
{
for(int   k=0;k<num2;k++)
{
p[j][k]=(j+1)*(k+1);
cout<<setw(6)<<p[j][k]<<':'<<setw(8)<<&p[j][k];
}
cout<<endl;
}
//释放二维数组占用的空间
for(int   m=0;m<num1;m++)
delete[]   p[m];
delete[]   p;
return   0;
}
以下是运行结果：
Please   enter   the   number   for   row   and   column:
4   5
1:004915F0           2:004915F4           3:004915F8           4:004915FC           5:00491600
2:00491180           4:00491184           6:00491188           8:0049118C         10:00491190
3:00491140           6:00491144           9:00491148         12:0049114C         15:00491150
4:00491100           8:00491104         12:00491108         16:0049110C         20:00491110
Press   any   key   to   continue
程序清单array04.cpp可以显示分配的内存空间单元的地址，大家可以看到，由于数组空间是动态分配的，数组行之间的地址空间是不连续的，因为不同行的数组元素的地址空间是用不同的new来分配的。而每一行之中列之间的地址空间是连续的。
那么用vector（向量）怎样实现二维数组呢？以下给出源程序：
//文件名:   array05.cpp
\#include   <iostream>
\#include   <vector>
\#include   <iomanip>
using   namespace   std;
int   main()
{
int   i,
j,
m,   //行数
n;   //列数
cout   <<   "input   value   for   m,n:";
cin>>m>>n;
//注意下面这一行：vector<int后两个">"之间要有空格！否则会被认为是重载">>"。
vector<vector<int>   >   vecInt(m,   vector<int>(n));
for   (i   =   0;   i   <   m;   i++)
for   (j   =   0;   j   <   n;   j++)
vecInt[i][j]   =   i*j;
for   (i   =   0;   i   <   m;   i++)
{
for   (j   =   0;   j   <   n;   j++)
cout<<setw(5)<<vecInt[i][j]<<":"<<setw(9)<<&vecInt[i][j];
cout<<endl;
}
return   0;
}
以下是运行结果：
input   value   for   m,n:3   4
0:   00491180         0:   00491184         0:   00491188         0:   0049118C
0:   00491140         1:   00491144         2:   00491148         3:   0049114C
0:   00491100         2:   00491104         4:   00491108         6:   0049110C
Press   any   key   to   continue
大家可以看到，这里vector中元素的内存的地址分配也有同双指针实现的二维数组有同样的特点。不过用vector的方法比使用双指针简单地多，分配内存空间时会更安全，数组初始化代码也更简单，所以本人建议使用STL中的vector来实现变长多维数组。以下是一个变长三维数组：）
//文件名:   array06.cpp
\#include   <iostream>
\#include   <vector>
\#include   <iomanip>
using   namespace   std;
int   main()
{
int   i,
j,
k,
m,   //一维坐标
n,   //二维坐标
l;   //三维坐标
cout   <<   "input   value   for   m,n,l:";
cin>>m>>n>>l;
vector<vector<vector<int>   >   >   vecInt(m,   vector<vector<int>   >(n,   vector<int>(l)));
for   (i   =   0;   i   <   m;   i++)
for   (j   =   0;   j   <   n;   j++)
for(k   =   0;   k   <   l;   k++)
vecInt[i][j][k]   =   i+j+k;
for   (i   =   0;   i   <   m;   i++)
{
for   (j   =   0;   j   <   n;   j++)
{
for(k   =   0;   k<l;   k++)
cout<<setw(5)<<vecInt[i][j][k]<<":"<<setw(9)<<&vecInt[i][j][k];
cout<<endl;
}
cout<<endl;
}
return   0;
}
运行结果：
input   value   for   m,n,l:2   3   4
0:   00492FE0         1:   00492FE4         2:   00492FE8         3:   00492FEC
1:   00492FA0         2:   00492FA4         3:   00492FA8         4:   00492FAC
2:   00492F60         3:   00492F64         4:   00492F68         5:   00492F6C
1:   00492EC0         2:   00492EC4         3:   00492EC8         4:   00492ECC
2:   00492E80         3:   00492E84         4:   00492E88         5:   00492E8C
3:   00492E40         4:   00492E44         5:   00492E48         6:   00492E4C


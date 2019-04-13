
# 希尔排序C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月04日 23:09:32[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1157


希尔排序
// 希尔排序.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
typedef int DataType;
void shell_sort(DataType *data,int n)
{
int i,j,gap;
gap = n/2;
while(gap>0)
{
for(i=gap+1;i<=n;i++)
{
if(data[i]<data[i-gap])
{
data[0] = data[i];
for(j=i-gap;j>0&&data[0]<data[j];j-=gap)
data[j+gap]=data[j];
data[j+gap]=data[0];
}
}
gap/=2;
}
}
int _tmain(int argc, _TCHAR* argv[])
{
DataType data[N];
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
int n;
cout<<"请输入元素的个数：";
cin>>n;
cout<<"请输入这些元素："<<endl;
int i;
//data[0]空出来
for(i=1;i<=n;i++)
{
cin>>data[i];
}
cout<<"希尔排序前："<<endl;
for(i=1;i<=n;i++)
{
cout<<data[i]<<" ";
}
cout<<endl;
shell_sort(data,n);
cout<<"希尔排序后："<<endl;
for(i=1;i<=n;i++)
{
cout<<data[i]<<" ";
}
cout<<endl;
}
system("pause");
return 0;
}
-------------------------------------------程序测试------------------------------------------
请输入案例个数：
2
请输入元素的个数：8
请输入这些元素：
23 1 13 34 113 23 12 19
希尔排序前：
23 1 13 34 113 23 12 19
希尔排序后：
1 12 13 19 23 23 34 113
请输入元素的个数：10
请输入这些元素：
10 9 8 7 6 5 4 3 2 1
希尔排序前：
10 9 8 7 6 5 4 3 2 1
希尔排序后：
1 2 3 4 5 6 7 8 9 10
请按任意键继续. . .


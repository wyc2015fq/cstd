
# C++随机化快速排序 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 00:08:42[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2549


C++随机化快速排序
// 随机化快速排序.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
using namespace std;
double a[100];
int random(int m,int n)            //产生一个随即下标，用其对应的数组元素作为比较标准
{
srand((unsigned)time(NULL));
return m + (rand()%(n-m+1));
}
int qartition(double *a,int begin,int end)    //一趟快速排序
{
int i = begin-1,j=begin;
double x = a[end];
while(j<end)
{
if(a[j]<=x)
{
i++;
double temp = a[i];
a[i]=a[j];
a[j]=temp;
}
j++;
}
double temp = a[end];
a[end]=a[i+1];
a[i+1]=temp;
return i+1;
}
int random_qartition(double *a,int begin,int end)    //一趟随机化快速排序
{
int q = random(begin,end);
double temp = a[end];
a[end]=a[q];
a[q]=temp;
return qartition(a,begin,end);
}
void random_fast_sort(double *a,int begin,int end)   //随机化快速排序
{
if(begin<end)
{
int p = random_qartition(a,begin,end);
random_fast_sort(a,begin,p-1);
random_fast_sort(a,p+1,end);
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入需要排序的案例个数："<<endl;
cin>>cases;
while(cases--)
{
memset(a,0.0,sizeof(a));
int n;
cout<<"请输入需要排序的元素的个数："<<endl;
cin>>n;
cout<<"请输入需要排序的元素："<<endl;
int i = 0;
for(i=0;i<n;i++)
{
cin>>a[i];
}
cout<<"排序前："<<endl;
for(i=0;i<n;i++)
{
cout<<a[i]<<" ";
}
cout<<endl;
random_fast_sort(a,0,n-1);
cout<<"排序后："<<endl;
for(i=0;i<n;i++)
{
cout<<a[i]<<" ";
}
cout<<endl;
}
system("pause");
return 0;
}



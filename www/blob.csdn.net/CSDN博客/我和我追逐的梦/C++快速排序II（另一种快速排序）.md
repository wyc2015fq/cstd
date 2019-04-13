
# C++快速排序II（另一种快速排序） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月28日 19:55:49[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1096标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=ini&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=ini&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=ini&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

另一种 C++快速排序
// 快速排序II.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
double a[N];
int qartition(double *a,int f,int t)
{
double x = a[t];
int i = f-1,j=f;
while(j<t)
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
double temp = a[t];
a[t]=a[i+1];
a[i+1]=temp;
return i+1;
}
void fast_sortII(double *a,int begin,int end)
{
if(begin<end)
{
int q  = qartition(a,begin,end);
fast_sortII(a,begin,q-1);
fast_sortII(a,q+1,end);
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
fast_sortII(a,0,n-1);
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



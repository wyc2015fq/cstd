
# C++快速排序I - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月28日 19:24:54[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：868标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

// 快速排序I.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
double a[N];
void fast_sortI(double *a,int begin,int end)
{
if(begin<end)
{
int i =begin,j=end;
double key = a[begin];
while(i<j)
{
while(i<j&&a[j]>=key) j--;
a[i]=a[j];
while(i<j&&a[i]<=key) i++;
a[j]=a[i];
}
a[i]=key;
fast_sortI(a,begin,i-1);
fast_sortI(a,i+1,end);
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
fast_sortI(a,0,n-1);
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


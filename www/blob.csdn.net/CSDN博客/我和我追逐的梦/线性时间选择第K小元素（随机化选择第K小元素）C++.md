
# 线性时间选择第K小元素（随机化选择第K小元素）C++ - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 00:57:42[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2849


// 随机化产生第K小数.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
using namespace std;
\#define N 100
double a[N];
//产生数组的随机下标，以其对应的数组元素作为比较关键字
int random(int m,int n)
{
//以系统时间作为随机种子
srand((unsigned)time(NULL));
return m + rand()%(n-m+1);
}
//一趟快速排序
int qartition(double *a,int begin,int end)
{
double x = a[end];
int i = begin-1,j=begin;
while(j<end)
{
if(a[j]<=x)
{
i++;
double temp = a[j];
a[j]=a[i];
a[i]=temp;
}
j++;
}
double temp = a[end];
a[end] = a[i+1];
a[i+1]=temp;
return i+1;
}
//一趟随机化快速排序
int random_qartition(double *a,int begin,int end)
{
int k = random(begin,end);
//cout<<"random k="<<k<<endl;
double temp = a[k];
a[k]=a[end];
a[end]=temp;
return qartition(a,begin,end);
}
//随机化选择第i小元素
double random_K_select(double *a,int begin,int end,int i)
{
if(begin>end)
return 65535;
else if(begin==end)
return a[begin];
else
{
int k = random_qartition(a,begin,end);
if(i-1==k)
return a[k];
else if(i-1<k)
return random_K_select(a,begin,k-1,i);
else  return random_K_select(a,k+1,end,i);
}
}
int _tmain(int argc, _TCHAR* argv[])
{
memset(a,0.0,sizeof(a));
int n;
cout<<"请输入这些元素的个数："<<endl;
cin>>n;
cout<<"请输入这些元素："<<endl;
int i = 0;
for(i=0;i<n;i++)
{
cin>>a[i];
}
cout<<"请输入需要查询的第K小元素的K值(以-1结束)："<<endl;
int k;
//double key;
cin>>k;
while(k!=-1)
{      if(k>n)
{
cout<<"没有这个元素，因为输入的k值不能大于n："<<n<<"，请重新输入k值"<<endl;
cin>>k;
continue;
}
//key = random_K_select(a,0,n-1,k);
cout<<"第 "<<k <<" 小元素是："<<endl;
cout<<random_K_select(a,0,n-1,k)<<endl;
cin>>k;
}
system("pause");
return 0;
}



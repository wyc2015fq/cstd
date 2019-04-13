
# 最优二叉查找树的期望搜索代价（动态规划)C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月03日 20:34:38[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1668


// 最优二叉查找树的期望搜索代价.cpp : Defines the entry point for the console application.
//
![](http://hi.csdn.net/attachment/201111/3/0_1320331332ORSb.gif)
\#include "stdafx.h"
\#include<iostream>
\#include<cmath>
\#include<limits>
\#define N 100
using namespace std;
const double MAX = numeric_limits<double>::max();    //double的最大值
int _tmain(int argc, _TCHAR* argv[])
{
//p[j]存储第j关键字的概率(j=1...n)
double p[N];
//存储第j虚拟键的概率(j=0...n)
double q[N];
//存储包含关键字ki....kj的最优子树的搜索代价
double c[N][N];
//存储包含关键字ki....kj和虚拟键的最优子树的概率和
double w[N][N];
//存储存储包含关键字ki....kj的最优子树的根
int root[N][N];
int cases;
cout<<"请输入案例的个数："<<endl;
cin>>cases;
while(cases--)
{
int n;
double sum = 0;
int i,j,l;
cout<<"请输入关键字的个数："<<endl;
cin>>n;
cout<<"请输入每个关键字的概率："<<endl;
for(i=1;i<=n;i++)
{
cin>>p[i];
sum += p[i];
}
cout<<"请输入每个虚拟键的概率："<<endl;
for(i=0;i<=n;i++)
{
cin>>q[i];
sum += q[i];
}
//
if(abs(sum-1)>0.01)
{
cout<<"输入的概率和不为1,请重新输入"<<endl;
cases++;
continue;
}
for(i=1;i<=n+1;i++)
{
c[i][i-1] = q[i-1];
w[i][i-1] = q[i-1];
}
for(l=1;l<=n;l++)
{
for(i=1;i<=n-l+1;i++)
{
j = l+i-1;
c[i][j] = MAX;
w[i][j] = w[i][j-1] + p[j] +q[j];
int r;
for(r=i;r<=j;r++)
{
double k = c[i][r-1] + w[i][j] + c[r+1][j];
if(k<c[i][j])
{
c[i][j] = k;
root[i][j] = k;
}
}
}
}
cout<<"最优二叉查找树的期望搜索代价为："<<c[1][n]<<endl;
}
system("pause");
return 0;
}
---------------------------------------------测试程序----------------------------------------------
请输入案例的个数：
1
请输入关键字的个数：
5
请输入每个关键字的概率：
0.15 0.10 0.05 0.10 0.20
请输入每个虚拟键的概率：
0.05 0.10 0.05 0.05 0.05 0.10
最优二叉查找树的期望搜索代价为：2.75
请按任意键继续. . .


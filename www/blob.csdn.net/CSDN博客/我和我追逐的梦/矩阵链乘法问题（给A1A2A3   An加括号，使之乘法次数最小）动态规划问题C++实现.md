
# 矩阵链乘法问题（给A1A2A3...An加括号，使之乘法次数最小）动态规划问题C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月02日 20:51:16[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2503


// 矩阵链乘法问题.cpp : Defines the entry point for the console application.
//给A1A2A3...An加括号，使之乘法次数最小
//动态规划问题
\#include "stdafx.h"
\#include<iostream>
\#define N 100
\#define Infinity 65535
using namespace std;
struct Matrix //矩阵
{
int row;   //矩阵行数
int col;    //矩阵列数
};
//矩阵
Matrix matrix[N];
//m[i][j]存储Ai到Aj的最小乘法次数
int m[N][N];
//s[i][j]存储Ai到Aj之间加括号的位置
int s[N][N];
//打印加括号后的
void print_it(int i,int j)
{
if(i==j)
cout<<"A"<<i;
else
{
cout<<"(";
print_it(i,s[i][j]);
print_it(s[i][j]+1,j);
cout<<")";
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
//变量初始化
memset(m,0,sizeof(m));
memset(s,0,sizeof(s));
cout<<"请输入矩阵的个数："<<endl;
int n;
cin>>n;
//flag表示输入是否符合要求
bool flag = true;
int i =1;
while(flag)
{
cout<<"请输入每个矩阵行数与列数："<<endl;
for(i=1;i<=n;i++)
{
cout<<"A"<<i<<"行数：";
cin>>matrix[i].row;
cout<<"A"<<i<<"列数：";
cin>>matrix[i].col;
}
//检查Ai的列数是否等于Ai+1的行数
for(i=1;i<n;i++)
{
if(matrix[i].col != matrix[i+1].row)
break;
}
if(i>=n)
flag = false;
else
{
flag = true;
cout<<"输入不符合要求，存在矩阵的列数不等于后面一个矩阵的行数，请重新输入！"<<endl;
}
}
int l,j,k;
for(l=2;l<=n;l++)
{
for(i=1;i<=n-l+1;i++)
{
j = i+l-1;
m[i][j] = Infinity;
int q =0;
for(k=i;k<=j-1;k++)
{
q = m[i][k] + m[k+1][j] + matrix[i].row*matrix[k].col*matrix[j].col;
if(m[i][j]>q)
{
m[i][j] = q;
s[i][j] = k;
}
}
}
}
cout<<"加括号之后："<<endl;
print_it(1,n);
cout<<endl;
cout<<"最小乘法次数为："<<m[1][n]<<endl;;
}
system("pause");
return 0;
}

----------------------------------------------------程序测试-------------------------------------------------
请输入案例个数：
1
请输入矩阵的个数：
6
请输入每个矩阵行数与列数：
A1行数：30
A1列数：35
A2行数：35
A2列数：15
A3行数：15
A3列数：5
A4行数：5
A4列数：10
A5行数：10
A5列数：20
A6行数：20
A6列数：25
加括号之后：
((A1(A2A3))((A4A5)A6))
最小乘法次数为：15125
请按任意键继续. . .


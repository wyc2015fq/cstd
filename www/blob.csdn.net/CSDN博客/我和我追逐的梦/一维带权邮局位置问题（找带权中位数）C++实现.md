
# 一维带权邮局位置问题（找带权中位数）C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 22:43:59[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2403


带权 邮局位置问题： 已知n个点p1,p2,...,pn及与它们相联系的权重w1,w2,...,wn。我们希望能找到一点p（不一定是输入点中的一个），使和式![](http://hi.csdn.net/attachment/201110/29/0_13198981051gBZ.gif)
最小，此处d(a,b)表示点a和点b之间的距离。对于一维带权邮局位置问题即找带权中位数。如下
// 一维邮局选址问题.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
\#include<cmath>
\#define N 100
using namespace std;
struct Node
{
double value;
double weight;
};
Node nodes[N];
//产生一个随即下标，用其对应的数组元素作为比较标准(即一趟快速的主元）
int random(int m,int n)
{
srand((unsigned)time(NULL));
return m + (rand()%(n-m+1));
}
//一趟快速排序
int qartition(Node *nodes,int begin,int end)
{
int i = begin-1,j=begin;
double x = nodes[end].value;
while(j<end)
{
if(nodes[j].value<=x)
{
i++;
Node temp = nodes[i];
nodes[i]=nodes[j];
nodes[j]=temp;
}
j++;
}
Node temp = nodes[end];
nodes[end]=nodes[i+1];
nodes[i+1]=temp;
return i+1;
}
//一趟随机化快速排序
int random_qartition(Node *nodes,int begin,int end)
{
int q = random(begin,end);
Node temp = nodes[end];
nodes[end]=nodes[q];
nodes[q]=temp;
return qartition(nodes,begin,end);
}
//随机化快速排序
void random_fast_sort(Node *nodes,int begin,int end)
{
if(begin<end)
{
int p = random_qartition(nodes,begin,end);
random_fast_sort(nodes,begin,p-1);
random_fast_sort(nodes,p+1,end);
}
}
//得到带权的中位数
Node GetMidWeight(Node *nodes,int begin,int end,double SumWeight)
{
double midSum = 0.0;
int i;
for(i=begin;i<=end;i++)
{
midSum+=nodes[i].weight;
if(midSum>=SumWeight/2)
break;
}
return nodes[i];
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
cout<<"请输入数据个数："<<endl;
int n;
cin>>n;
int i;
double sum = 0.0;
cout<<"请输入每一点值和其权值"<<endl;
for(i=0;i<n;i++)
{
cin>>nodes[i].value>>nodes[i].weight;
sum+=nodes[i].weight;
}
random_fast_sort(nodes,0,n-1);
cout<<"邮局位置为："<<endl;
Node node = GetMidWeight(nodes,0,n-1,sum);
cout<<node.value<<endl;
cout<<"总代价为："<<endl;
double sumValue = 0.0;
for(i=0;i<n;i++)
{
sumValue+=abs(nodes[i].value-node.value);
}
cout<<sumValue<<endl;
}
system("pause");
return 0;
}

找出二维带权邮局位置问题的最佳解答，其中所有的点都是(x,y)坐标对，并且点a(x1,y1)与点b(x2,y2)之间的距离是Manhattan距离：d(a,b)=|x1-x2|+|y1-y2|。
对于二维带权邮局位置问题可以转化为一维邮局位置问题，分别求x、y的带权中位数。见下篇《二维带权邮局位置问题》



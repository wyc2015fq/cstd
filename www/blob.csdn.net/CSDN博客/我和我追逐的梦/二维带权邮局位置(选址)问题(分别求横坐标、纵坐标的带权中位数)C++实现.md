
# 二维带权邮局位置(选址)问题(分别求横坐标、纵坐标的带权中位数)C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 23:12:41[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1995


带权 邮局位置问题： 已知n个点p1,p2,...,pn及与它们相联系的权重w1,w2,...,wn。我们希望能找到一点p（不一定是输入点中的一个），使和式
![](http://hi.csdn.net/attachment/201110/29/0_13198981051gBZ.gif)
最小，此处d(a,b)表示点a和点b之间的距离。
找出二维带权邮局位置问题的最佳解答，其中所有的点都是(x,y)坐标对，并且点a(x1,y1)与点b(x2,y2)之间的距离是Manhattan距离：d(a,b)=|x1-x2|+|y1-y2|。
对于二维带权邮局位置问题可以转化为一维邮局位置问题，分别求x、y的带权中位数。
// 二维带权邮局位置（选址）问题.cpp : Defines the entry point for the console application.
//分别求横坐标、纵坐标的带权中位数
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
\#include<cmath>
\#define N 100
using namespace std;
struct Node
{
double XValue;
double YValue;
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
//flag为0表示求横坐标的排序，为1表示纵坐标的排序
int qartition(Node *nodes,int begin,int end,bool flag)
{
if(!flag)
{
int i = begin-1,j=begin;
double x = nodes[end].XValue;
while(j<end)
{
if(nodes[j].XValue<=x)
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
else
{
int i = begin-1,j=begin;
double y = nodes[end].YValue;
while(j<end)
{
if(nodes[j].YValue<=y)
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
}
//一趟随机化快速排序
int random_qartition(Node *nodes,int begin,int end,bool flag)
{
int q = random(begin,end);
Node temp = nodes[end];
nodes[end]=nodes[q];
nodes[q]=temp;
return qartition(nodes,begin,end,flag);
}
//随机化快速排序
void random_fast_sort(Node *nodes,int begin,int end,bool flag)
{
if(begin<end)
{
int p = random_qartition(nodes,begin,end,flag);
random_fast_sort(nodes,begin,p-1,flag);
random_fast_sort(nodes,p+1,end,flag);
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
cout<<"请输入每一点位置(x,y)和其权值"<<endl;
for(i=0;i<n;i++)
{
cin>>nodes[i].XValue>>nodes[i].YValue>>nodes[i].weight;
sum+=nodes[i].weight;
}
random_fast_sort(nodes,0,n-1,0);
Node node = GetMidWeight(nodes,0,n-1,sum);
double xValue = node.XValue;
random_fast_sort(nodes,0,n-1,1);
node = GetMidWeight(nodes,0,n-1,sum);
double yValue = node.YValue;
cout<<"邮局位置为："<<endl;
cout<<"( "<<xValue<<","<<yValue<<" )"<<endl;
cout<<"总代价为："<<endl;
double sumValue = 0.0;
for(i=0;i<n;i++)
{
sumValue+=abs(nodes[i].XValue-xValue)+abs(nodes[i].YValue-yValue);
}
cout<<sumValue<<endl;
}
system("pause");
return 0;
}



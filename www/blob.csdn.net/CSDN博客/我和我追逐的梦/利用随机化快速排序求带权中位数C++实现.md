
# 利用随机化快速排序求带权中位数C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 22:05:09[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1728


// 利用随机化快速排序求带权中位数.cpp : Defines the entry point for the console application.
//
//中位数：n个元素集合中，第n/2小的元素，如果是偶数个，则选择中间二个的算术平均值。
//带权中位数：对于n个互不相同的元素集合x1、x2……xn，其权重依次为w1、w2……wn。按x值排好序后，
//从第一个元素的权值开始进行权值求和，第一个出现权值和大于等于所有元素的权值和的一半的元素
//先按利用随机化快速排序算法给关键字排序，然后从排序后元素0开始递推累加每个关键字的权，
//如果在某一个i时，累加的权大于等于总权的一半，那么这个i就是带权中位数的序号。
//随机化快速排序的平均时间为nlgn。
\#include "stdafx.h"
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
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
cout<<"请输入每一对值和其权值"<<endl;
for(i=0;i<n;i++)
{
cin>>nodes[i].value>>nodes[i].weight;
sum+=nodes[i].weight;
}
random_fast_sort(nodes,0,n-1);
cout<<"带权中位数为："<<endl;
Node node = GetMidWeight(nodes,0,n-1,sum);
cout<<node.value<<endl;
cout<<"其权值为："<<endl;
cout<<node.weight<<endl;
}
system("pause");
return 0;
}



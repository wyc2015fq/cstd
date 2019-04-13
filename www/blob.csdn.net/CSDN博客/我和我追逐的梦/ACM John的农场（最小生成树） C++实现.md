
# ACM John的农场（最小生成树） C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月01日 21:15:00[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2933


John的农场
Description
John是一个农场主，他有几个牧场，为了好好照顾他的牛，他必须在几个牧场之间来回，可糟糕的天气往往使得道路非常泥泞，为此John准备在牧场之间铺一些石子路，这样在下雨天也能快速地从一个牧场到另外一个牧场。但John的资金有限，为了自己能从任一个牧场都通过石子路到达另外一个牧场，他需要好好设计一下线路。请帮助John设计好线路，使得John能从任一个牧场都通过石子路到达另外一个牧场，且线路的费用最低。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每个测试用例占n+1行。每个测试用例的第一行为一个整数n（3<=n<=20），表示有多少个牧场，从第二行开始为一个n*n的矩阵，矩阵元素aij表示从i个牧场到j个牧场的铺路费用。
输出：
每行输出一个测试用例的最小铺路费用。

Sample Input
2
6
0 6 1 5 0 0
6 0 5 0 3 0
1 5 0 5 6 4
5 0 5 0 0 2
0 3 6 0 0 6
0 0 4 2 6 0
4
0 1 2 3
1 0 3 4
2 3 0 1
3 4 1 0
Sample Output
15
4
\#include<iostream>
using namespace std;
\#define Infinity 65535
bool flag[21];
int  cost[21][21],lowcost[21],mincost;
int main()
{
int k;
cin>>k;
while(k--)
{
int n;
cin>>n;
int i,j;
mincost = 0;
memset(flag,false,sizeof(flag));
memset(lowcost,0,sizeof(lowcost));
for(i=0;i<n;i++)
{
for(j=0;j<n;j++)
{
cin>>cost[i][j];
if(cost[0][i]!=0)
{
lowcost[i]=cost[0][i];
}
else lowcost[i]=Infinity;
}
}
flag[0]=true;
i=0;
int key;
int min;
while(i<n-1)
{
min= Infinity;
key = 0;
for(j=1;j<n;j++)
{
if(!flag[j]&&lowcost[j]!=0&&min>lowcost[j])
{
min = lowcost[j];
key = j;
}
}
flag[key] = true;
mincost+=lowcost[key];
for(j=1;j<n;j++)
{
if(!flag[j]&&cost[key][j]!=0&&cost[key][j]<lowcost[j])
lowcost[j] = cost[key][j];
}
i++;
}
cout<<mincost<<endl;
}
return 0;
}



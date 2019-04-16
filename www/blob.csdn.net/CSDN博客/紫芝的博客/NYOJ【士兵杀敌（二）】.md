# NYOJ【士兵杀敌（二）】 - 紫芝的博客 - CSDN博客





2018年04月10日 15:26:04[紫芝](https://me.csdn.net/qq_40507857)阅读数：38













## 士兵杀敌（二）

时间限制：1000 ms  |  内存限制：65535 KB

难度：5



- 描述

南将军手下有N个士兵，分别编号1到N，这些士兵的杀敌数都是已知的。

小工是南将军手下的军师，南将军经常想知道第m号到第n号士兵的总杀敌数，请你帮助小工来回答南将军吧。

南将军的某次询问之后士兵i可能又杀敌q人，之后南将军再询问的时候，需要考虑到新增的杀敌数。




- 输入
只有一组测试数据
第一行是两个整数N,M，其中N表示士兵的个数(1<N<1000000)，M表示指令的条数。(1<M<100000)
随后的一行是N个整数，ai表示第i号士兵杀敌数目。(0<=ai<=100)
随后的M行每行是一条指令，这条指令包含了一个字符串和两个整数，首先是一个字符串，如果是字符串QUERY则表示南将军进行了查询操作，后面的两个整数m,n，表示查询的起始与终止士兵编号；如果是字符串ADD则后面跟的两个整数I,A(1<=I<=N,1<=A<=100),表示第I个士兵新增杀敌数为A.
- 输出
对于每次查询，输出一个整数R表示第m号士兵到第n号士兵的总杀敌数，每组输出占一行- 样例输入
5 6
1 2 3 4 5
QUERY 1 3
ADD 1 2
QUERY 1 3
ADD 2 3
QUERY 1 2
QUERY 1 5- 样例输出
6
8
8
20



解法一：

树状数组

```cpp
#include<bits/stdc++.h>
using namespace std;
int N,M,c[1000005];
int lowbit(int i)
{
	return i&(-i);
 } 
void add(int i,int value)
{
	while(i<=N){
		c[i]+=value;
		i+=lowbit(i);
	}
}
int sum(int i){
	int sum=0;
	while(i>0){
		sum+=c[i];
		i-=lowbit(i);
	}
	return sum;
}
int main()
{
	ios::sync_with_stdio(false);
	fill(c,c+1000005,0);
	cin>>N>>M;
	int a,b;
	string op;
	for(int i=1;i<=N;i++)
	{
		cin>>a;
		add(i,a);
	}
	while(M--){
		cin>>op>>a>>b;
		if(op[0]=='A'){
			add(a,b);
		}else{
			cout<<sum(b)-sum(a-1)<<endl;
		}
	}
	return 0;
 }
```

解法二：

线段树，更新点，查询区间求和


```cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXNODE =1e5+3;
const int MAX=MAXNODE<<2;
struct NODE{
	int value;//区间和
	int left,right; 
}node[MAXNODE];

int father[MAX];
void BuildTree(int i,int left,int right)
{
	node[i].left=left;
	node[i].right=right;
	node[i].value=0;
	if(left==right){
		father[left]=i;
		return ;
	}
	BuildTree(i<<1,left,(left+right)/2);
	BuildTree(i<<1|1,(left+left)/2+1,right);
}
void UpdateTree(int ri)
{
	if(ri==1)	return;
	int fi=ri/2;
	int a=node[fi<<1].value;
	int b=node[fi<<1|1].value;
	node[fi].value=a+b;
	UpdateTree(ri>>1);
}

int Sum;
void query(int i,int l,int r)
{
	if(node[i].left==l&&node[i].right==r){
		Sum+=node[i].value;
		return;
	}	
	i=i<<1;
	if(l<=node[i].right)
	{
		if(r<=node[i].right)
		query(i,l,r);
		else
		query(i,l,node[i].right);
	}
	i++;
	if(r>=node[i].left){
		if(l>=node[i].left)
		query(i,l,r);
		else query(i,node[i].left,r);
	}
}
int main(){
	int n,m,g;
	ios::sync_with_stdio(false);
	while(cin>>n>>m){
		BuildTree(1,1,n);
		for(int i=1;i<=n;i++)
		{
			cin>>g;
			node[father[i]].value=g;
			UpdateTree(father[i]);
		}
		while(m--)
		{
			int a,b;
			string op;
			//getchar();
			cin>>op>>a>>b;
			if(op[0]=='Q')
			{
				Sum=0;
				query(1,a,b);
				cout<<Sum<<endl;
			}else{
				node[father[a]].value+=b;
				UpdateTree(father[a]);
			}
		}
	}
	return 0;
}
```





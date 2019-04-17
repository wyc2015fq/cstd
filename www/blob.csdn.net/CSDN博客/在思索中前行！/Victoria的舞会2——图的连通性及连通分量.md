# Victoria的舞会2——图的连通性及连通分量 - 在思索中前行！ - CSDN博客





2016年03月30日 12:58:21[_Tham](https://me.csdn.net/txl16211)阅读数：385











## 【Vijos1022]】Victoria的舞会2




### Description


Victoria是一位颇有成就的艺术家，他因油画作品《我爱北京天安门》闻名于世界。现在，他为了报答帮助他的同行们，准备开一个舞会。 

Victoria准备邀请n个已经确定的人，可是问题来了： 

这n个人每一个人都有一个小花名册，名册里面写着他所愿意交流的人的名字。比如说在A的人名单里写了B，那么表示A愿意与B交流；但是B的名单里不见的有A，也就是说B不见的想与A交流。但是如果A愿意与B交流，B愿意与C交流，那么A一定愿意与C交流。也就是说交流有传递性。 

Victoria觉得需要将这n个人分为m组，要求每一组的任何一人都愿意与组内其他人交流。并求出一种方案以确定m的最小值是多少。 

注意：自己的名单里面不会有自己的名字。






### Input


第一行一个数n。 

接下来n行，每i+1行表示编号为i的人的小花名册名单，名单以0结束。1<=n<=200。






### Output


一个数，m。






### Sample Input


18 

0 

18 0 

0 

0 

11 0 

0 

0 

0 

0 

0 

5 0 

0 

0 

0 

0 

0 

0 

2 0






### Sample Output


16




















































































































































问题分析：

问题描述简要概括为：给出舞会成员之间的关系图(有向图)，求解有向图的强连通分量数量。

解法1：

图的连通性+并查集：首先求出任意两点之间的连通性(最短路径)，然后将互相连通的顶点使用并查集合并，并统计合并的堆数，即为所求解。



```cpp
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<iostream>
/*  本题可以构建一个人与人之间的关系图(有向图)，问题转化为求解有向连通图的极大连通子块的数量
    求解过程：
    第一步，构建有向连通图，并使用Floyd求出各点之间的连通性(顶点数量较少)
    第二步，若两点之间互相连通，则把这两个点绑定到一块(分组计数减1)，可以使用并查集将两个互相连通的顶点合并
*/
using namespace std;
#define Max 10000
int people[205][205],Link[205][205],father[205];  //people[][]存储每人的花名册，link[][]存储相互之间的连通性
int n,t,temp,ans;

int Find(int i)
{
	if(i==father[i]) return i;
	return Find(father[i]);
}

void Merge(int a,int b)
{
	a=Find(a);
	b=Find(b);
	if(a!=b)
	{
		father[b]=a;
	}
}

void Floyd()
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			for(int k=1;k<=n;k++)
			{
				if(i!=j||i!=k||j!=k)
				{
					Link[i][j]=min(Link[i][j],Link[i][k]+Link[k][j]);
				}
			}
}

int main()
{
	ios::sync_with_stdio(false);
	freopen("victoria2.in","r",stdin);
	freopen("victoria2.out","w",stdout);
	cin>>n;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
		{
			if(i!=j) Link[i][j]=Max;
		}
	t=1,ans=n;
	while(t<=n)
	{
		while(cin>>temp)
		{
			if(temp==0) break;
			int seq=++people[t][0];
			people[t][seq]=temp;
			Link[t][temp]=1;
		}
		father[t]=t;
		++t;
	}
	Floyd();
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
		{
			if(i!=j&&Link[i][j]<Max&&Link[j][i]<Max&&Find(i)!=Find(j))   //并查集合并关系网中互相连通的顶点
			{
				ans--;
				Merge(i,j);
			}
		}
	cout<<ans<<endl;
	return 0;
}
```


解法2：

采用tarjan求有向图的强连通分量，主要使用栈的结构及访问时间戳，Low[ ]数组标记



```cpp
#include <cstdio>
#include <stack>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

vector<int> point[205];
stack<int>  S;          //栈S用于存储tarja深度搜索过程中顶点的拓扑序列
//InStack[]标记顶点是否在栈中，DFS[]标记顶点的访问时间戳，Low(u)为u或u的子树能追溯到最早的栈中节点次序号
//Belong[]标记顶点属于哪一个强连通分量，index访问时间戳，nun强连通分量计数
int InStack[205]={0},DFS[205],Low[205],Belong[205],Index=0,num=0;

void tarjan(int u)
{
	DFS[u]=Low[u]=++Index;
	S.push(u);
	InStack[u]=1;
	for(vector<int>::iterator v=point[u].begin();v!=point[u].end();++v)
	{
		if(!DFS[*v])
		{
			tarjan(*v);
			Low[u]=min(Low[u],Low[*v]);
		}
		else if(InStack[*v])
		{
			Low[u]=min(Low[u],DFS[*v]);
		}
	}
	if(DFS[u]==Low[u])
	{
		int e;
		++num;
		while(!S.empty())
		{
			e=S.top();
			S.pop();
			InStack[e]=0;
			Belong[e]=num;
			if(u==e) break;
		}
	}
}

int main()
{
//	ios::sync_with_stdio(false);  //注意 cin scanf 与cout printf 在关闭同步输入输出流的时候，混合使用会出错
	freopen("victoria2.in","r",stdin);
//	freopen("victoria2.out","w",stdout);
	int n,t,temp;
//	scanf("%d",&n);
	cin>>n;                            //这个地方用了cin流读入
	t=1;
	while(t<=n)
	{
		while(scanf("%d",&temp))       //这里使用标准scanf()读入，同时存在cin,scanf不能关闭输入输出流同步
		{
			if(!temp) break;
			point[t].push_back(temp);
		}
		++t;
	}
	for(int i=1;i<=n;++i)
	{
		if(!DFS[i])
		{
			tarjan(i);
		}
	}
//	printf("%d\n",num);
	cout<<num<<endl;
	return 0;
}
```


解法3：

求强连通分量算法Kosaraju，正反图双向搜索，寻找解答树



```cpp
#include<cstdio>
#include<stack>
#include<cstring>
/*
	Kosaraju算法求连通分量，采用正反图双向DFS求连通分量。
    根据：有向图的这样一个性质，一个图和他的transpose graph(边全部反向)具有相同的强连通分量。
    算法求解步骤：
	1.对G求解Reverse Post-Order，即得到顶点的"伪拓扑排序"
	2.对G进行转置得到GR
	3.按照第一步得到的集合中顶点出现的顺序，对GR调用DFS得到若干颗搜索树
	4.每一颗搜索树就代表了一个强连通分量
   这个算法的想法很巧妙，为了突出回向边，对图进行转置，然后对转置的图按照之前得到的顶点(拓扑)序列进行DFS调用。
*/
//Map[][]原图关系，rMap[][]反图关系，pcolor[]顶点染色信息，Count连通分量的计数
int Map[205][205],rMap[205][205],pcolor[205]={0},Count=0,N;  
std::stack<int> S;

void dfs(int u)    //正向染色，得到拓扑序列顶点集，存放在栈S
{
	pcolor[u]=1;
	S.push(u);
	for(int v=1;v<=N;++v)
	{
		if(Map[u][v]==1&&!pcolor[v])
			dfs(v);
	}
}

void rdfs(int u)   //反向调用，寻找解的搜索树
{
	pcolor[u]=Count;  //标记u属于连通分量的标号
	for(int v=1;v<=N;++v)
	{
		if(rMap[u][v]==1&&!pcolor[v])
			rdfs(v);
	}
}

int main()
{
	freopen("victoria2.in","r",stdin);
	freopen("victoria2.out","w",stdout);
	scanf("%d",&N);
	int t=1;
	while(t<=N)
	{
		int temp;
		while(scanf("%d",&temp))
		{
			if(temp==0) break;
			Map[t][temp]=1;
			rMap[temp][t]=1;
		}
		++t;
	}
	for(int i=1;i<=N;++i)
	{
		if(!pcolor[i])  dfs(i);
	}
	/*
	while(!S.empty())
	{
		printf("%d ",S.top());
		S.pop();
	}
	printf("\n");
	*/
	memset(pcolor,0,sizeof(pcolor));
	while(!S.empty())    //根据正向生成顶点拓扑序列集，进行反向DFS调用，寻找解答搜索树
	{
		int x=S.top();
		S.pop();
		if(!pcolor[x])
		{
			++Count;
			rdfs(x);
		}
	}
	printf("%d\n",Count);
	return 0;
}
```






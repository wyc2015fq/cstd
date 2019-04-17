# Street Directions（POJ-1515） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 18:56:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：86
个人分类：[图论——图的连通性																[POJ](https://blog.csdn.net/u011815404/article/category/7504500)](https://blog.csdn.net/u011815404/article/category/8236848)








> 
# Problem Description

According to the Automobile Collision Monitor (ACM), most fatal traffic accidents occur on two-way streets. In order to reduce the number of fatalities caused by traffic accidents, the mayor wants to convert as many streets as possible into one-way streets. You have been hired to perform this conversion, so that from each intersection, it is possible for a motorist to drive to all the other intersections following some route. 

You will be given a list of streets (all two-way) of the city. Each street connects two intersections, and does not go through an intersection. At most four streets meet at each intersection, and there is at most one street connecting any pair of intersections. It is possible for an intersection to be the end point of only one street. You may assume that it is possible for a motorist to drive from each destination to any other destination when every street is a two-way street. 

# **Input**

The input consists of a number of cases. The first line of each case contains two integers n and m. The number of intersections is n (2 <= n <= 1000), and the number of streets is m. The next m lines contain the intersections incident to each of the m streets. The intersections are numbered from 1 to n, and each street is listed once. If the pair i j is present, j i will not be present. End of input is indicated by n = m = 0.

# Output

For each case, print the case number (starting from 1) followed by a blank line. Next, print on separate lines each street as the pair i j to indicate that the street has been assigned the direction going from intersection i to intersection j. For a street that cannot be converted into a one-way street, print both i j and j i on two different lines. The list of streets can be printed in any order. Terminate each case with a line containing a single `#' character. 

Note: There may be many possible direction assignments satisfying the requirements. Any such assignment is acceptable. 

# Sample Input

**7 101 21 32 43 44 54 65 76 72 53 67 91 21 31 42 43 44 55 65 77 60 0**

# Sample Output

**1**

**1 22 43 13 64 35 25 46 46 77 5#2**

**1 22 43 14 14 34 55 45 66 77 5#**


题意：给出一个 n 个点 m 条边的无向图，现在要将所有的边变成一条单向边或者两条单向边，使任意两点 i、j 均能可达，要求变成的两条单向边的边最少，输出所有的边

思路：题目实质是要将无向连通图变为有向强连通图，且任意一条边可以被拆分为两条互相反向的有向边，由于任何两点均可达，因此对于连通图来说，要考虑桥的存在，由于将桥变为单向边后，整个图变为弱连通图，因此仅需将图中的所有桥拆为两条反向的单向边即可。对于其他的非桥的边，可以通过 dfs 来锁定边的指向。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
struct Edge{
    int from,to,next;
    int cut;//记录该边是否为桥
    Edge(){}
    Edge(int from,int to,int next,int cut):from(from),to(to),next(next),cut(cut){}
}edge[N*2];//双向边edge[i]与edge[i^2]是两条反向边
int n,m;
int head[N],edgeNum;
int dfn[N],low[N],block_cnt;
bool vis[N];//dfs确定边的方向时的标志数组
bool sig;//判断是否连通
void addEdge(int from,int to){//添边
    edge[edgeNum]=Edge(from,to,head[from],false);
    head[from]=edgeNum++;
}
void tarjin(int x,int father){
	low[x]=dfn[x]=++block_cnt;
	bool flag=true;//阻止双向边的反向边i与i^1
	for(int i=head[x];i!=-1;i=edge[i].next)	{
		int y=edge[i].to;
		if(y==father&&flag){
			flag=false;
			continue;
		}
		if(!dfn[y]){
			tarjin(y,x);
			if(low[x]>low[y])
                low[x]=low[y];
			if(low[y]>dfn[x])
				edge[i].cut=edge[i^1].cut=true;
		}
		else if(low[x]>dfn[y])
            low[x]=dfn[y];
	}
}

void dfs(int x, int father){
	vis[x]=true;
	for(int i=head[x];~i;i=edge[i].next){
		int y=edge[i].to;
		if(edge[i].cut==1)
            continue;
		if(edge[i^1].cut!=-1)
            edge[i].cut=-1;
		if(!vis[y])
            dfs(y,x);
	}
}
int main()
{
    int Case=1;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(dfn,0,sizeof(dfn));
        memset(head,-1,sizeof(head));
        memset(vis,0,sizeof(vis));
        edgeNum=0;

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
            addEdge(y,x);
        }

        tarjin(1,1);
        sig=true;
        for(int i=1;i<=n;i++){
            if(!dfn[i]){
                sig=false;
                break;
            }
        }

        for(int i=1;i<=n;i++)
            if(!vis[i])
                dfs(i,i);

        printf("%d\n\n",Case++);
        for(int i=0;i<edgeNum;i++)
            if(edge[i].cut)
                printf("%d %d\n",edge[i].from,edge[i].to);
        printf("#\n");
    }
    return 0;
}
```







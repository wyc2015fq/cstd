# CodeForces 982F. The Meeting Place Cannot Be Changed - 紫芝的博客 - CSDN博客





2018年07月14日 16:02:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：166








[点击打开链接](https://codeforces.com/problemset/problem/982/F)

# F. The Meeting Place Cannot Be Changed

time limit per test

1 second

memory limit per test

256 megabytes

input

standard input

output

standard output

Petr is a detective in Braginsk. Somebody stole a huge amount of money from a bank and Petr is to catch him. Somebody told Petr that some luxurious car moves along the roads without stopping.

Petr knows that it is the robbers who drive the car. The roads in Braginsk are one-directional and each of them connects two intersections. Petr wants to select one intersection such that if the robbers continue to drive the roads indefinitely, they will sooner or later come to that intersection. The initial position of the robbers is unknown. Find such an intersection that fits the requirements.

Input

The first line of the input contains two integers nn and mm (2≤n≤1052≤n≤105, 2≤m≤5⋅1052≤m≤5⋅105) — the number of intersections and the number of directed roads in Braginsk, respectively.

Each of the next mm lines contains two integers uiui and vivi (1≤ui,vi≤n1≤ui,vi≤n, ui≠viui≠vi) — the start and finish of the ii-th directed road. It is guaranteed that the robbers can move along the roads indefinitely.

Output

Print a single integer kk — the intersection Petr needs to choose. If there are multiple answers, print any. If there are no such intersections, print −1−1.

Examples

input

Copy

```
5 6
1 2
2 3
3 1
3 4
4 5
5 3
```

output

Copy
`3`
input

Copy

```
3 3
1 2
2 3
3 1
```

output

Copy
`1`
Note



In the first example the robbers can move, for example, along the following routes: , , . We can show that if Petr chooses the -rd intersection, he will eventually meet the robbers independently of their route.

题意：

劫匪的初始位置未知，他沿着道路不停的移动，城市的道路是单向的，并且每条道路都连接两个交叉路口，

选择一个交叉路口，如果劫匪继续无限期地开车，他们迟早会到达那个交叉路口。

分析：

其实这个问题可以看做是一个强连通分量里面的所有简单环是否存在至少一个公共点

那么我们假设一个点满足条件就等价于从这个点出发，一定不会滞留在一个死循环中，

而是最后回到了出发点.对所有路径都满足。如果这个点不满足，那么应该再去找哪个点呢？

这个点滞留在死循环中，那么就说明它不属于这个死循环的环，所以接下来的那个点一定是这两个环的交点，

因为该点绝对满足不会进入这两个环的死循环，接下来就是这样找下去，直到遇到交点是已经遇到过的，

那么说明有两个交集点为0的环，则输出-1.由于这样找在有解的情况是最快的，

所以时间一多说明几乎是无解的,则可以限时查询的时间。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=5e5+50;//数据量
int n,m;//节点数、边数
int head[N],to[N],nxt[N],tot;//存储有向图
inline void add(int x,int y)//x为起点，y为终点建边
{
    to[tot]=y;
    nxt[tot]=head[x];
    head[x]=tot++;
}
int vis[N];//标记已经访问过的节点
int in[N],id[N];
bool dt[N],k;
const bool cmp(int a,int b)//间接排序函数
{
    return in[a]<in[b];//按入度排序
}
//计算搜索时间
inline double FIND_TIME()
{return clock()/(double)CLOCKS_PER_SEC;}
void dfs(int x)
{
    vis[x]=1;//标记节点已经访问过
    for(int i=head[x];~i;i=nxt[i])
    {
        if(!vis[to[i]])//下一个节点没有访问过
            dfs(to[i]);
        if(vis[to[i]]==1)//下一个节点访问过
            k=1;
        if(k)   return ;
    }
    vis[x]=2;//DFS回溯
}
inline bool check(int x)//检查每一个点
{
    memset(vis,0,sizeof(vis));
    vis[x]=2;
    k=0;
    for(int i=1;i<=n;i++)
    if(!vis[i]){
        dfs(i);
        if(k){
            for(int i=1;i<=n;i++)
                if(vis[i]!=1)
                dt[i]=1;
            return false;
        }
    }
    return true;
}

int main()
{
    memset(head,-1,sizeof(head));
    scanf("%d%d",&n,&m);
    for(int i=1;i<=m;i++)
    {
        int u,v;
        scanf("%d%d",&u,&v);
        add(u,v);//建边
        ++in[u];//更新度
    }
    for(int i=1;i<=n;i++)
        id[i]=i;//节点编号
    sort(id+1,id+1+n,cmp);//排序
    for(int i=1;i<=n;i++){
         if(FIND_TIME()>0.9) break;//时间限制为1s
    if(!dt[id[i]]&&check(id[i]))//此点合法
    {
        printf("%d\n",id[i]);//输出路口的编号
        return 0;
    }
}
    puts("-1");//不存在这样的点
    return 0;
}
```

用邻接表来存储有向图

```cpp
#include<bits/stdc++.h>
using namespace std;
const int mx = 1e5 + 10;
int n,m,in[mx],ty,type[mx],d,mark;
vector <int> vec[mx];
int dfn[mx],id[mx],size,is,sta[mx];
int vis[mx],flag;
bool vic[mx];
void tarjan(int x)
{
	dfn[x] = id[x] = ++is;
	vis[x] = 1;
	sta[++size] = x;
	for(int i=0;i<vec[x].size();i++){
		int son = vec[x][i];
		if(!dfn[son]){
			tarjan(son);
			id[x] = min(id[x],id[son]);
		}else if(vis[son]) id[x] = min(id[x],dfn[son]);
	}
	if(dfn[x]==id[x]){
		ty++;
		if(sta[size]!=x) d++,mark = ty;
		while(sta[size]!=x){
			type[sta[size]] = ty;
			vis[sta[size]] = 0;
			size--;
		}type[x] = ty,size--,vis[x] = 0;
	}
}
void dfs(int x,int num)
{
	if(flag) return ;
	for(int i=0;i<vec[x].size();i++)
	{
		if(flag) return;
		int son = vec[x][i];
		if(vis[son]==num){
			if(vic[son]) flag = -1;
			else flag = son,vic[son] = 1;
		}
		if(vis[son]<num) vis[son] = num,dfs(son,num);
	}
	vis[x] = num + 1;
}
double TIME(){//获得单位运行时间
	return 1.0*clock()/CLOCKS_PER_SEC;
}
int check(int x,int num)
{
	while(TIME()<0.5){
		vis[x] = num + 1;
		dfs(x,num);
		if(!flag) return x;
		if(flag==-1) return -1;
		x = flag,num += 2,flag = 0;
	}
	return -1;
}
int main()
{
	scanf("%d%d",&n,&m);
	int a,b,c = 0,ans;
	for(int i=1;i<=m;i++){
		scanf("%d%d",&a,&b);
		vec[a].push_back(b);
	}
	for(int i=1;i<=n;i++)
	if(!dfn[i]) tarjan(i);

	if(d>1) puts("-1");
	else{
        for(int i=1;i<=n;i++)
		if(type[i]==mark){
            printf("%d\n",check(i,1));
            break;
		}
	}
	return 0;
}
```






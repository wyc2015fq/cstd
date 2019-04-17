# bzoj3376/poj1988[Usaco2004 Open]Cube Stacking 方块游戏 — 带权并查集 - 在思索中前行！ - CSDN博客





2016年11月04日 11:43:22[_Tham](https://me.csdn.net/txl16211)阅读数：548








题目链接：[http://www.lydsy.com/JudgeOnline/problem.php?id=3376](http://www.lydsy.com/JudgeOnline/problem.php?id=3376)

题目大意：

编号为1到n的n(1≤n≤30000)个方块正放在地上．每个构成一个立方柱．

有P(1≤P≤100000)个指令．指令有两种：

1．移动(M)：将包含X的立方柱移动到包含Y的立方柱上．

2．统计(C):统计名含X的立方柱中，在X下方的方块数目．


题解：

带权并查集

存三个东西，x所在立方柱的最顶端fa[x]，x所在立方柱的最底端d[x]，x上面有多少个立方柱f[x](下面的图画错了不包含x qwq..画的时候一点感觉都没有)。

![](https://img-blog.csdn.net/20161026144241380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


那么要求的x下方的数目就可以用f[d[x]]-f[x]来表示。



```cpp
/*
	题意：
	n块积木，m个操作或询问。每次移动积木的时候，约翰会选择两块积木X，Y，把X搬到Y的上方。如果X已经和其它积
	木叠在一起了，那么应将这叠积木整体移动到Y的上方；如果Y已经和其它积木叠在一起了的，假设在Y上方最高处的
	积木为Z，那么应将X所在的那叠积木移动到Z的上方。每次询问当前时刻，某一块积木的下方有多少块积木。
	n,m<=10^5
    输入第一行 一个整数m，接下来m行，每行命令的格式为： 操作  x  y  ( M 为合并操作，C为查询 )

	题解：
	带权并查集。
	对于每个点x，维护当前所在并查集（也就是这一堆积木中）最下方的积木low[x]，最上方的积木fa[x]，
	x到最上方积木的距离dist[x]，则下方的积木数=dist[low[x]]-dist[x]。
	带权并查集其实就是在Findfa的时候顺便维护一些权值。
*/
#include<cstdio>
#include<cstring>

const int maxn=30000;
int fa[maxn],low[maxn],dist[maxn],N,M;

int Findfa(int x)
{
	if(fa[x]!=x)
	{
		int y=fa[x];
		fa[x]=Findfa(fa[x]);
		low[x]=low[y];
		dist[x]=dist[x]+dist[y];  // x上方木块数=原x积木堆上方的木块数 + Findfa更新后原积木堆x祖先y上方木块数
	}
	return fa[x];
}

int main()
{
	for(int i=1;i<=maxn;++i) fa[i]=i,low[i]=i,dist[i]=0;  // 并查集初始化
	scanf("%d",&M);
	char op[5];
	int x,y;
	while(M>0)
	{
		M--;
		scanf("%s",&op);
		if(op[0]=='M')      //把 x放到y上
		{
			scanf("%d%d",&x,&y);
			int fx=Findfa(x),fy=Findfa(y);
			fa[fy]=fx;
			dist[fy]=dist[low[fx]]+1;
			low[fx]=low[fy];
			Findfa(fy);      // 并查集路径压缩，更新之前积木堆y中的积木最上方的积木编号
		}
		else
		{
			scanf("%d",&x);
			int t=Findfa(x);  
			int d=low[t];
			Findfa(d);   // 路径压缩，更新
			printf("%d\n",dist[d]-dist[x]);
		}
	}
	return 0;
}
```






# 【HDU】4035 Maze - weixin_33985507的博客 - CSDN博客
2015年02月23日 11:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[http://acm.hdu.edu.cn/showproblem.php?pid=4035](http://acm.hdu.edu.cn/showproblem.php?pid=4035)
题意：给一棵n个节点的树，每个节点有值k[i]和e[i]，分别表示k[i]概率走向1号节点，e[i]概率获得胜利（即停止），如果没有进行上边任意操作，则等概率的走向与这个节点连边的点。问走过的边的期望。（n<=10000）
```cpp
#include <cstdio>
#include <cstring>
using namespace std;
const int N=10005;
const double eps=1e-10;
struct dat { double a, b, c, K, E; }D[N];
struct E { int next, to; }e[N<<1];
int n, cnt, ihead[N];
void clr() {
	memset(ihead, 0, sizeof(int)*(n+1));
	cnt=0;
}
void add(int u, int v) {
	e[++cnt].next=ihead[u]; ihead[u]=cnt; e[cnt].to=v;
	e[++cnt].next=ihead[v]; ihead[v]=cnt; e[cnt].to=u;
}
double abs(double x) { return x<0?-x:x; }
void dfs(int x, int fa) {
	bool flag=1;
	double temp=1-D[x].K-D[x].E, suma=0, sumb=0, sumc=0;
	int m=0;
	for(int i=ihead[x]; i; ++m, i=e[i].next) if(e[i].to!=fa) {
		flag=0;
		dfs(e[i].to, x);
		suma+=D[e[i].to].a;
		sumb+=D[e[i].to].b;
		sumc+=D[e[i].to].c;
	}
	if(flag) { D[x].a=D[x].K; D[x].b=D[x].c=temp; return; }
	double di=m-temp*sumb;
	D[x].a=(D[x].K*m+temp*suma)/di;
	D[x].b=temp/di;
	D[x].c=temp*(sumc+m)/di;
}
void work() {
	dfs(1, -1);
	if(abs(1-D[1].a)<=eps) puts("impossible");
	else printf("%.6f\n", D[1].c/(1-D[1].a));
}
int main() {
	int T; scanf("%d", &T);
	for(int TT=1; TT<=T; ++TT) {
		scanf("%d", &n);
		for(int i=0; i<n-1; ++i) { int x, y; scanf("%d%d", &x, &y); add(x, y); }
		for(int i=1; i<=n; ++i) scanf("%lf%lf", &D[i].K, &D[i].E), D[i].K/=100, D[i].E/=100;
		printf("Case %d: ", TT);
		work();
		clr();
	}
	return 0;
}
```
好神的题...做出这题感觉对期望的认识大大加深了呢...
设状态$E[i]$表示当前在$i$节点还需要走$E[i]$的期望边即可结束，答案就是$E[1]$，容易得到：
对于叶子节点：
$$
\begin{align}
E[i] & = k[i]E[1]+e[i]*0+(1-k[i]-e[i])*(E[父亲]+1) \\
      & = k[i]E[1]+(1-k[i]-e[i])E[父亲]+(1-k[i]-e[i])
\end{align}
$$
对于非叶子节点：
$$
\begin{align}
E[i] & = k[i]E[1]+e[i]*0+\frac{(1-k[i]-e[i])(E[父亲]+1+\sum_{j是i孩子} (E[j]+1)}{m} \\
      & = k[i]E[1]+\frac{1-k[i]-e[i]}{m}E[父亲] + \frac{1-k[i]-e[i]}{m} \sum_{j是i孩子} E[j] + (1-k[i]-e[i])
\end{align}
$$
由于是树，我们要深度挖掘他们的性质：
1、我们发现非叶节点里只是多了$\sum_{j是i孩子} E[j]$
2、由于期望本来就是求极限意义，如果裸的这样求显然是递归无限的，我们需要通过数学技巧约掉式子
比如我们可以为了方便，设$E[i]$的一般式子为$E[i]=A[i]E[1]+B[i]E[父亲]+C[i]$（为什么带个父亲未知数呢？那是因为$i$的儿子需要知道自己父亲（即$i$）的期望，那么我们可以通过带入到原式中且移项即可解出$E[i]$，是不是很神奇呢0.0）
那么$\sum_{j是i孩子} E[j] = \sum_{j是i孩子} \left( A[j]E[1] + B[j]E[i] + C[j] \right)$
最终化简超强式子，变为求出这个一般方程的系数！：
对于叶子节点：
$$A[i]=k[i], B[i]=C[i]=1-k[i]-e[i]$$
对于非叶子节点：
设$di=m-(1-k[i]-e[i])\sum_{j} B[j], m=所有与i连边的节点数$
$$
\begin{align}
A[i] & = (k[i]m+(1-k[i]-e[i])\sum_{j} A[j]) / di \\
B[i] & = (1-k[i]-e[i]) / di \\
C[i] & = (1-k[i]-e[i])(m+\sum_{j} C[j]) / di
\end{align}
$$
最后答案：
$E[1]=C[i]/(1-A[1])$，判一下除数是否为0即可判断有无解。

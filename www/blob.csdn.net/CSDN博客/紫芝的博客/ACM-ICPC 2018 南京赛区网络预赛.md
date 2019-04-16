# ACM-ICPC 2018 南京赛区网络预赛 - 紫芝的博客 - CSDN博客





2018年09月03日 21:39:14[紫芝](https://me.csdn.net/qq_40507857)阅读数：49








# [ACM-ICPC 2018 南京赛区网络预赛](https://nanti.jisuanke.com/?kw=ACM-ICPC%202018%20%E5%8D%97%E4%BA%AC%E8%B5%9B%E5%8C%BA%E7%BD%91%E7%BB%9C%E9%A2%84%E8%B5%9B)

## An Olympian Math Problem

看到T最大1000，n的范围是10^18，要么是防AK题，要么是签到题，已经给出n=2，3的结果，算一下n=4，5，6发现都是n-1，然后勇敢的交了一发，就这样AC啦

```cpp
#include<cstdio>
using namespace std;
int main()
{
    long long n;
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%lld",&n);
        printf("%lld\n",n-1);
    }
}
```

Lpl and Energy-saving Lamps

题意：

给n个房间，每个月可以得到m个灯泡，接下来是n个数，代表每个房间的灯泡数，有q次离线询问，问第i个月：已经换过灯泡的房间个数和本月剩余的灯泡数，当现有的灯泡数目大于等于一个房间的灯泡数时，可以将这个房间的灯泡换掉。

分析：

线段树（单点更新，维护区间最小值）

设本月的灯泡个数为lab，使用线段树查询第一个小于等于lab的房间，将这个房间的灯泡换掉，即将这点的值更新为无穷大，若ans=-1，说明当前剩余灯泡不足以换掉一个房间的灯泡，继续下一个月

对于线段树的查询操作，若当前为叶子节点，说明已经找到符合条件的房间，若当前点的左儿子小于等于lab，说明左子树中一定有小于等于lab的房间，查询左子树；若当前点的右儿子小于等于lab，说明右子树中一定有小于等于lab的房间，查询右子树；

```cpp
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
int n,m;
const int maxn=1e5+10;
int a[maxn],d[maxn];
int q;

struct node{
int l,r,Min;
}tree[maxn<<2];

struct point
{
    int num,rem;
};

void build(int m,int l,int r)
{
    tree[m].l=l;
    tree[m].r=r;
    if(l==r)
    {
        tree[m].Min=a[l];
        return;
    }
    int mid=l+(r-l)/2;
    build(m<<1,l,mid);
    build((m<<1)|1,mid+1,r);
    tree[m].Min=min(tree[m<<1].Min,tree[(m<<1)|1].Min);
}

int ans;
void query(int m,int l,int r,int val)
{
    if(l==r)
    {
        ans=tree[m].Min;
        tree[m].Min=inf;
        return ;
    }
    int mid=l+(r-l)/2;
    if(tree[m<<1].Min<=val)
    query(m<<1,l,mid,val);
    else
    if(tree[m<<1|1].Min<=val)
    query((m<<1)|1,mid+1,r,val);
    tree[m].Min=min(tree[m<<1].Min,tree[(m<<1)|1].Min);//跟新操作
}
int main()
{
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);
    build(1,1,n);
    scanf("%d",&q);

    int maxm=-1;
    for(int i=1;i<=q;i++)
    {
        scanf("%d",&d[i]);
        maxm=max(d[i],maxm);
    }

    int lab=m;
    int room=0;
    vector<point> v;
    v.clear();
    point t;
    v.push_back(t);

    int M=inf;
    for(int i=1;i<=maxm;i++)
    {
        if(room==n){
            M=i-1;break;
        }
        ans=-1;
        query(1,1,n,lab);
        while(ans!=-1){
            lab-=ans;
            room++;
            ans=-1;
            query(1,1,n,lab);
        }

        t.num=room;
        t.rem=lab;
        v.push_back(t);
        lab+=m;
    }
    for(int i=1;i<=q;i++)
    {
        if(d[i]>M)
        printf("%d %d\n",v[M].num,v[M].rem);
        else
        printf("%d %d\n",v[d[i]].num,v[d[i]].rem);
    }
    return 0;
}
```

The writing on the wall

题意：

一个n*m的方格矩阵,有的格子被涂成了黑色,问该矩阵中有多少个子矩阵？（子矩阵不包含黑色格子）

分析：

1.不包含黑色小矩形，对于一个长为L, 高为H的无黑点矩阵中，包含的高为H的子矩阵个数为L+(L-1)+(L-2)+...+1个；这是直接算的一种方法；如何程序表示该计算呢？

```cpp
for(int i=1; i<=L; i++){
    for(int j=i; j>0; j--){
        count+=1;
    }
}
```

这样的一个双层循环就表示了上式；那么所有子矩阵个数就是三层循环，高由1->H：



```cpp
for(int h=1; h<=H; h++){
    for(int i=1; i<=L; i++){
        for(int j=i; j>0; j--){
            count+=h;
        }
    }
}
```

如果在某处加了个黑点又如何计算呢？如下图：

![](https://img-blog.csdn.net/20180905185435141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先看高为H(4)的子矩阵个数：以(4, 7)为右下角的高为H的子矩阵个数为3个，由L=4处在向左，就只能构成高为2的子矩阵了；

那么怎么该上边的代码才能得出答案呢？如下：

```cpp
for(int i=1; i<=H; i++){
    for(int j=1; j<=L; j++){
        h=i;
        for(int k=j; k>0; k--){
            h=min(h, i-p[k]);
            count+=h;
        }
    }
}
//p[k]表示第k列中在i行上边的第一个黑点的位置，
```

上边代码就是本题的核心代码了；然后H用n代替，L用m代替，这样复杂度为O(n*m*m)；然后标记黑点的位置每次维护h就可以了；

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int b[100010][110], up[110];
int main(){
	int T, cas=0;
	scanf("%d", &T);
	while(T--){
		int n, m, K;
		scanf("%d%d%d", &n, &m, &K);
		for(int i=0; i<=n; i++){
			for(int j=0; j<=m; j++){
				b[i][j]=0;
				up[j]=0;
			}
		}
		for(int i=0; i<K; i++){
			int x, y;
			scanf("%d%d", &x, &y);
			b[x][y]=1;
		}
		ll ans=0;
		for(int i=1; i<=n; i++){
			for(int j=1; j<=m; j++){
				if(b[i][j]){
					up[j]=i;
				}
			}
			for(int j=1; j<=m; j++){
				ll minn=0x7f7f7f7f7f7f7f7f;
				for(int k=j; k>0; k--){
					minn=min(minn, (ll)(i-up[k]));
					ans+=minn;
				}
			}
		}
		printf("Case #%d: %lld\n", ++cas, ans);
	}
	return 0;
}
```






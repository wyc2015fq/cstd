# Codeforces987F AND Graph - 紫芝的博客 - CSDN博客





2018年06月04日 22:51:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：109









You are given a set of size m with integer elements between 0 and 2n−1 inclusive. Let's build an undirected graph on these integers in the following way: connect two integers x and y with an edge if and only if x&y=0. Here &is the [bitwise AND operation](https://en.wikipedia.org/wiki/Bitwise_operation#AND). Count the number of connected components in that graph.



Input

In the first line of input there are two integers nand m (0≤n≤22, 1≤m≤2n).


In the second line there are m
 integers a1,a2,…,am (0≤ai<2n) — the elements of the set. All ai

 are distinct.



Output

Print the number of connected components.



Examples



Input
Copy

2 3
1 2 3



Output
Copy

2



Input
Copy

5 5
5 19 10 20 12



Output
Copy

2





Note

Graph from first sample:

![](http://codeforces.com/predownloaded/d0/e8/d0e836c5862f20c797eac1d56f25a37a6605cc73.png)

Graph from second sample:

![](http://codeforces.com/predownloaded/32/6d/326db52fed8700dc07e3c1c2ca51534890880eb4.png)

题意：

给出m个大于等于0且小于2^n的不同的整数，如果两个数x&y=0，就把这两个数连接起来，问有多少个连通分支？

错误思路：

并查集，将这m个数两两相与，若x&y=0，则把这两个数放到一个集合，然后标记这m个数的根节点，

最后统计根节点的个数，即连通分支的个数。显然，m非常大时，必定超时


超时代码：

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=(1<<22)+5;
int fa[maxn];
int find(int x)
{
    if(fa[x]==x)    return x;
    return fa[x]=find(fa[x]);
}
void mix(int x,int y)
{
    int fx=find(x),fy=find(y);
    fa[fy]=fx;
}
int n,m,a[maxn];
bool vis[maxn];
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
            bool flag=0;
        for(int i=0;i<m;i++){
            scanf("%d",&a[i]);
            if(a[i]==0)
                flag=1;
        }
            if(flag){
                printf("1\n");
                continue;
            }

            for(int i=0;i<=(1<<n);i++)
                fa[i]=i;

        memset(vis,0,sizeof(vis));

        for(int i=0;i<m;i++)
            for(int j=i+1;j<m;j++)
        {
            if((a[i]&a[j])==0){
                mix(a[i],a[j]);
                //printf("%d %d\n",a[i],a[j]);
            }
        }

        for(int i=0;i<m;i++)
        {
            //printf("%d\n",find(a[i]));
            vis[find(a[i])]=1;
        }
        int ans=0;

        //for(int i=0;i<=(1<<n);i++)
        for(int i=0;i<m;i++)
            if(vis[a[i]])
            ans++;
            printf("%d\n",ans);
    }
    return 0;
}
```
正确思路：

x有边连出的一定是 (2^n-1) ^ x 的一个子集，直接连子集复杂度是n^2,但是我们可以一个1一个1的消去，

最后变成补集的一个子集。
但是必须当且仅当 至少有一个 a 等于 x 的时候， 可以直接dfs(all ^ x) ,否则直接消1连边




AC代码：

```cpp
#include <bits/stdc++.h>

using namespace std;
const int maxn = 1<<22;

int n, m, a[maxn], all;
int vis[maxn];
int have[maxn];

void dfs(int x)
{
    if(vis[x]) return;
    vis[x] = 1;
    if(have[x]) dfs(all ^ x);
    for(int i = 0;i < n;i ++) {
        if(x & 1 << i) dfs(x ^ 1 << i);
    }
}
int main()
{
    ios::sync_with_stdio(false), cin.tie(0);
    cin >> n >> m;
    for(int i = 1;i <= m;i ++) cin >> a[i];
    for(int i = 1;i <= m;i ++) have[a[i]] = 1;
    all = 1<<n;
    all--;
    int res = 0;
    for(int i = 1;i <= m;i ++) {
        if(!vis[a[i]]) {
            vis[a[i]] = 1;
            res ++;
            dfs(all ^ a[i]);
        }
    }
    cout << res << '\n';
    return 0;
}
```


正确思路：

对于一个数 11000 则 他必定可以 与 00111相链接， 那么也一定可以和00111中间少了任何几个1的数相连。
任何我们对于任意一个数都跑出他的父亲， 他的父亲就比他多一个1。

如 00001 他的父亲可以为 10001 01001 00101 00011 这4个， 至于 10101 则是 10001（00101）的父亲。

但是，对于一个数来说，他不能直接和他的父节点链接，必须要父节点先和别的数链接，然后子节点就一定可以和父节点链接的那个数链接。

我们先从小到大跑出所有的可以父节点，开另外一个数组去标记。

然后我们从大的数先开始处理，这样我们就会先访问到祖先节点，再访问到子节点，这样可以保证，我们访问到子节点的时候，他的父节点已经处理过了，我们可以得知这2个点是否可以相连接。

对于处理到每一个点，我们先找他的所有父节点，如果可以链接，就直接链接，如果他不能和父节点链接，就去找他的对立节点（即0->1， 1->0），看看这2个点是否可以链接，可以就相连，不可以就将这个节点标记为-1，即没有链接的节点，从而当该节点的子节点访问的时候，就可以得知不能链接。

然后链接的时候用并查集链接块，最后看有几个联通块就是答案了。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N = (1<<22) + 100;
bool vis[N], vis1[N];
int pre[N];
int Find(int x){
    if(x == pre[x]) return x;
    return pre[x] = Find(pre[x]);
}
int main(){
    int n, m, t, to, u, v;
    scanf("%d%d", &n, &m);
    int Max = (1<<n) - 1;
    for(int i = 1; i <= m; i++){
        scanf("%d", &t);
        vis[t] = 1;
    }
    for(int i = 0; i <= Max; i++){
        vis1[i] |= vis[i];
        if(vis1[i]) {
            for(int j = 0; j < n; j++)
                vis1[i|(1<<j)] = 1;
        }
    }
    bool f = 0;
    for(int i = 0; i <= Max; i++) pre[i] = i;
    for(int i = Max; i >= 0; i--){
        if(!vis1[i]){
            pre[i] = -1;
            continue;
        }
        f = 0;
        for(int j = 0; j < n; j++){
            if(i&(1<<j)) continue;
            to = i|(1<<j);
            if(pre[to] != -1){
                u = Find(i);
                v = Find(to);
                pre[u] = v;
                f = 1;
            }
        }
        if(!f){
            to = Max ^ i;
            if(vis1[to]) {
                u = Find(i);
                v = Find(to);
                pre[u] = v;
            }
            else pre[i] = -1;
        }
    }
    int ans = 0;
    for(int i = 0; i <= Max; i++){
        if(vis[i] && pre[i] == -1) ans++;
        if(pre[i] == i) ans++;
    }
    printf("%d\n", ans);
    return 0;
}
```





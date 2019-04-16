# Codeforces 982 C. Cut 'em all! 图的遍历 - 紫芝的博客 - CSDN博客





2018年06月06日 20:43:25[紫芝](https://me.csdn.net/qq_40507857)阅读数：233
个人分类：[图论](https://blog.csdn.net/qq_40507857/article/category/7659793)










You're given a tree with n vertices.



Your task is to determine the maximum possible number of edges that can be removed 

in such a way that all the remaining connected components will have even size.



Input

The first line contains an integer n (1≤n≤105) denoting the size of the tree.

The next n−1 lines contain two integers u, v (1≤u,v≤n) each, describing the vertices connected by the i-th edge.

It's guaranteed that the given edges form a tree.



Output

Output a single integer k — the maximum number of edges that can be removed to leave all connected components with even size, or −1



if it is impossible to remove edges in order to satisfy this property.



Examples



input
Copy

4
2 4
4 1
3 1



output
Copy

1


input
Copy

3
1 2
1 3



output
Copy

-1


input
Copy

10
7 1
8 4
8 10
4 7
6 5
9 3
3 5
2 10
2 5



output
Copy

4


input
Copy

2
1 2



output
Copy

0




Note

In the first example you can remove the edge between vertices 1 and 4. 

The graph after that will have two connected components with two vertices in each.


In the second example you can't remove edges in such a way that all components have even number of vertices, 

so the answer is −1


.


#### 题目描述：

    给你一个有n个结点的树，其中有n-1条边，询问你最多你可以移动多少条边，使得每个强连通分量的个数均为偶数。

#### 题目分析：

    因为我们知道，如果所给的结点的个数是奇数，那么，因为奇数必定是由一个偶数和一个奇数组成，故总会有一个强连通分量的个数为奇数，因此当n为奇数使，答案为-1。

    那么当n等于偶数的时，我们只需要进行dfs遍历整张图，统计一下某个结点以及其儿子结点的数量，如果递归到最底时统计出的数量为偶数，则答案+1即可。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=2e5+5;
struct edge
{
    int to,next;
}q[maxn];
int head[maxn];
int size[maxn];
int cnt=0;
int ans=0;
void init()
{
    memset(head,-1,sizeof(head));
    cnt=0;
}
void add_edge(int from,int to)
{
    q[cnt].next=head[from];
    q[cnt].to=to;
    head[from]=cnt++;
}
void dfs(int x,int fa)
{
    size[x]=1;
    for(int i=head[x];i!=-1;i=q[i].next)
    {
        if(q[i].to==fa) continue;
        dfs(q[i].to,x);
        size[x]+=size[q[i].to];
    }
    if(size[x]%2==0)
    {
        ans++;size[x]=0;
    }
}
int main()
{
    int n,u,v;
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++)
    {
        scanf("%d%d",&u,&v);
        add_edge(u,v);
        add_edge(v,u);
    }
    if(n&1){
        printf("-1\n");
        return 0;
    }
    dfs(1,-1);
    printf("%d\n",ans-1);
    return 0;
}
```




```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+5;
vector<int>G[maxn];
int son[maxn];
int n,ans;
void dfs(int x,int fa)
{
    son[x]=1;
    for(int i=0;i<G[x].size();i++)
    {
        int k=G[x][i];
        if(k!=fa){
            dfs(k,x);
            son[x]+=son[k];
        }
    }
    if(son[x]%2==0)
    {
        ans++;
        son[x]=0;
    }
}
int main()
{
    ans=0;
    memset(son,0,sizeof(son));
    scanf("%d",&n);
    for(int i=0;i<n-1;i++)
    {
        int a,b;
        scanf("%d%d",&a,&b);
        G[a].push_back(b);
        G[b].push_back(a);
    }
    if(n%2==1){
        printf("-1\n");
        return 0;
    }
    dfs(1,-1);
    printf("%d\n",ans-1);
    return 0;
}
```








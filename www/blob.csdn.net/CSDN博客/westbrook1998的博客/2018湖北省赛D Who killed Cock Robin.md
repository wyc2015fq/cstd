# 2018湖北省赛D. Who killed Cock Robin - westbrook1998的博客 - CSDN博客





2018年04月23日 22:38:15[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：176








> 
链接：[https://www.nowcoder.com/acm/contest/104/C](https://www.nowcoder.com/acm/contest/104/C)

  来源：牛客网 

  Sparrows are a kind of gregarious animals,sometimes the relationship between them can be represented by a tree. 

  The Sparrow is for trial, at next bird assizes,we should select a connected subgraph from the whole tree of sparrows as trial objects. 

  Because the relationship between sparrows is too complex, so we want to leave this problem to you. And your task is to calculate how many different ways can we select a connected subgraph from the whole tree.
> 
输入描述: 

  The first line has a number n to indicate the number of sparrows.  

  The next n-1 row has two numbers x and y per row, which means there is an undirected edge between x and y.
> 
输出描述: 

  The output is only one integer, the answer module 10000007 (107+7) in a line


> 
示例1 

  输入 

  4 

  1 2 

  2 3 

  3 4 

  输出 

  10 

  说明 

  For a chain, there are ten different connected subgraphs: 
题意就是说有n个节点，n-1条边的图（其实就是树了），求这个图的所有子图（或者说所有路径，单个节点也算一个）

应该是深搜加动态规划？ 

这里j为i的儿子节点 
![这里写图片描述](https://img-blog.csdn.net/20180423215220614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9vazE5OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180423215229509?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9vazE5OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码
```cpp
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
#define ll long long
const ll maxn=2e5+10;
const ll mod=1e7+7;
//访问标记数组
int vis[maxn];
vector<ll> mp[maxn];
ll sum,dp[maxn];
void dfs(int x){
    //标记访问
    vis[x]=1;
    ll ans=1;
    int len=mp[x].size();
    for(int i=0;i<len;i++){
        int v=mp[x][i];
        //防止回头访问父节点
        if(vis[v]){
            continue;
        }
        //循环依次递归访问子节点
        dfs(v);
        //访问完返回，取消访问标记
        vis[v]=0;
        //最重要的一步操作
        //dp[v]代表返回回来的上一个函数，即子节点的路径数，
        //ans用来保存该节点的多个儿子节点的路径乘积
        //这里为什么要多个儿子节点路径数相乘呢
        //比如1-2-3这个树 当以2作为根，1 3分别都只有一个路径（也就是本身），
        //所以dp[1]=1,dp[3]=1,而返回给2，之后dp[2]就应该等于（dp[1]+1）*(dp[3]+1)
        //对于x的每个儿子v,你选它时有dp[v]种方法，这是分步过程所以用乘法原理。
        //也就是比如1-2-3-4-5  以3为根，这样就是（2+1）×（2+1）,也就是说3左二子的两种路径情况加上3本身
        //就有了三种情况，而右儿子也是如此，而左二子的每一种路径情况都可以对应右儿子的每一种路径情况，所以用乘法
        ans=ans*(dp[v]+1)%mod;
    }
    //ans代表该节点到叶的路径数
    //每一次递归调用到这个函数，都会将这个ans加到sum里，因为最终的结果就是每个节点的路径数之和
    //因为这道题里面是一棵树，所以不会存在重复访问的情况，所以可以直接加ans
    sum=(sum+ans)%mod;
    //dp[x]代表从叶节点到x这条路径的路径数
    //上面的ans用于加到sum，而这里的ans赋给dp[x]，用于（返回）给上一级函数
    dp[x]=ans;
}
int main(void){
    int n;
    while(scanf("%d",&n)!=EOF){
        //清空vector数组以完成多组测试
        for(int i=1;i<=n;i++){
            mp[i].clear();
        }
        //初始化数据
        sum=0;
        memset(dp,0,sizeof(ll));
        memset(vis,0,sizeof(int));
        for(int i=1;i<n;i++){
            int u,v;
            scanf("%d%d",&u,&v);
            mp[u].push_back(v);
            mp[v].push_back(u);
        }
        dfs(1);
        printf("%lld\n",sum);
    }
    return 0;
}
```











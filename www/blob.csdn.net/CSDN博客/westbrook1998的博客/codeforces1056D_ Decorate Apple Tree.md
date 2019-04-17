# codeforces1056D_ Decorate Apple Tree - westbrook1998的博客 - CSDN博客





2018年12月08日 23:31:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：85








好难的题,队友解释半天才搞懂…

给一棵树,能给叶子染不同的颜色,定义一个节点为happy当且仅当该子树叶子节点(可包括本身)的颜色各不相同,然后求分别有1到n个happy节点的情况下的最小颜色数

反过来想,先考虑n的情况,要n个happy节点,那就是所有叶子都染不同颜色,然后考虑n-1的情况,就去掉一个根节点,那么就变成两棵独立的子树,那只要保证那个叶子节点多的那个子树的叶子染不同的颜色即可,另外的兄弟子树的叶子不管染什么色肯定不会超过这一棵的颜色数,所以其实就是第二大的叶子数的子树,所以答案就是dfs一遍预处理出每个节点对应的子树的叶子节点数,排序输出即可
代码:

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+50;
vector<int> g[N];
int sum[N];
int n,x;
void dfs(int u){
    if(g[u].size()==0){
        sum[u]=1;
        return;
    }
    int siz=g[u].size();
    for(int i=0;i<siz;i++){
        dfs(g[u][i]);
        sum[u]+=sum[g[u][i]];
    }
    return;
}
int main(void){
    scanf("%d",&n);
    for(int i=2;i<=n;i++){
        scanf("%d",&x);
        g[x].push_back(i);
    }
    dfs(1);
    sort(sum+1,sum+1+n);
    for(int i=1;i<=n;i++){
        printf("%d ",sum[i]);
    }
    return 0;
}
```





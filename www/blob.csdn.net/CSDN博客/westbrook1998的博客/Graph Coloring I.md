# Graph Coloring I - westbrook1998的博客 - CSDN博客





2018年10月03日 19:11:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：108标签：[dfs																[图论																[二分图判断																[奇环](https://so.csdn.net/so/search/s.do?q=奇环&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
链接：[https://www.nowcoder.com/acm/contest/203/J](https://www.nowcoder.com/acm/contest/203/J)

来源：牛客网

题目描述

修修在黑板上画了一些无向连通图，他发现他可以将这些图的结点用两种颜色染色，满足相邻点不同色。

澜澜不服气，在黑板上画了一个三个点的完全图。修修跟澜澜说，这个图我能找到一个简单奇环。

澜澜又在黑板上画了一个n个点m条边的无向连通图。很可惜这不是一道数数题，修修做不出来了。

澜澜非常得意，作为一位毒瘤出题人，有了好题当然要跟大家分享，于是他把这道题出给你做了。

输入描述:

第一行两个整数n,m (1≤ n,m≤ 3*105)，接下来m行每行两个整数ai,bi表示一条边 (1≤ ai,bi≤ n)。

保证图连通，并且不存在重边和自环。

输出描述:

如果你能把图二染色，第一行输出0，第二行输出n个整数表示每个点的颜色 (0≤ xi≤ 1)。如果有多种合法方案，你可以输出任意一种。

如果你能找到一个简单奇环，第一行输出环长k，第二行输出k个整数表示环上结点编号 (1≤ yi≤ n)，你需要保证yi和yi+1之间有边，y1和yn之间有边。如果有多种合法方案，你可以输出任意一种。

如果两种情况都是可行的，你只需要输出任意一种。

如果两种情况都是不可行的，请输出一行一个整数-1。

示例1

输入

复制

3 2

1 2

1 3

输出

复制

0

0 1 1

示例2

输入

复制

3 3

1 2

1 3

2 3

输出

复制

3

1 2 3
其实就是判断是否二分图，就是能否染成相邻不同色，如果可以就输出每个点的颜色，如果不是二分图那就是存在奇环，那就输出奇环

然而太菜了最后码不出来

看了题解补题过了

把判二分图和找奇环放在一起就好了，用同一个dfs，判断二分图染色其实就是用不同的颜色dfs，当递归到发现下一个节点已经染色过了，那就不是二分图了，而是存在奇环，每一次遍历下一个节点，不过是不是二分图，都用一个pre数组记录上一个节点，然后递归到奇环的时候记录一下这个位置root，然后要输出奇环就从这里开始，依次pre
代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
const int N=3*1e5+50;
vector<int> g[N];
int color[N];
int pre[N];
vector<int> res;
bool flag;
int root;
void init(){
    memset(color,-1,sizeof(color));
    memset(pre,-1,sizeof(pre));
    res.clear();
    flag=true;
    for(int i=0;i<N;i++){
        g[i].clear();
    }
}
void dfs(int v,int c){
    if(!flag){
        return;
    }
    color[v]=c;
    for(int i=0;i<g[v].size();i++){
        if(!flag){
            return;
        }
        pre[g[v][i]]=v;
        if(color[g[v][i]]==-1){
            dfs(g[v][i],c^1);
        }
        else{
            //不是二分图
            if(color[g[v][i]]==c){
                flag=false;
                //说明奇环就出现在这里
                root=g[v][i];
                return;
            }
        }
    }
}
int main(void){
    init();
    int n,m;
    int u,v;
    scanf("%d%d",&n,&m);
    for(int i=0;i<m;i++){
        scanf("%d%d",&u,&v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1,0);
    if(flag){
        printf("0\n");
        for(int i=1;i<n;i++){
            printf("%d ",color[i]);
        }
        printf("%d\n",color[n]);
    }
    else{
        int now=pre[root];
        res.push_back(root);
        while(root!=now){
            res.push_back(now);
            now=pre[now];
        }
        int cnt=res.size();
        printf("%d\n",cnt);
        for(int i=0;i<cnt-1;i++){
            printf("%d ",res[i]);
        }
        printf("%d\n",res[cnt-1]);
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=二分图判断&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)





# 最小生成树——Kruskal算法 - westbrook1998的博客 - CSDN博客





2018年05月03日 12:49:16[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：50标签：[算法																[最小生成树																[MST](https://so.csdn.net/so/search/s.do?q=MST&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/westbrook1998/article/category/7458454)





直接上代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int maxn=1000001;
//第i条边的左端点
int u[maxn];
//第i条边的右端点
int v[maxn];
//边的权值
int w[maxn];
//并查集,p[x]保存x的父节点
int p[maxn];
//节点（端点）数
int n;
//边数
int m;
//间接排序，判断w[i]的关系，但实际上排序的是p[i]
//比如：
// w 4 2 5 1 3 6
// r 0 1 2 3 4 5
//排序之后，w没有变化，只是用来比较，而位置发生变化的是r
// r 4 1 5 0 2 6
//表示的是最小权值的边是编号为4的边，第二小是编号为1的边......
int cmp(const int i,const int j){
    return w[i]<w[j];
}
//并查集的查找，通过递归查找到根节点来判断是否是同一个连通分量
int find(int x){
    //查找的过程中通过p[x]=find(p[x])这个操作一步一步把节点连接到根节点上
    //可以提高下次查找的效率
    return p[x]==x ? x : p[x]=find(p[x]);
}
int kruskal(){
    int ans=0;
    //初始化并查集
    for(int i-0;i<n;i++){
        p[i]=i;
    }
    //初始化边序号
    for(int i-0;i<m;i++){
        r[i]=i;
    }
    //对边的序号按权值进行排序
    sort(r,r+m,cmp);
    //按边的权值从小到大遍历
    for(int i=0;i<m;i++){
        //按权值从小到大顺序获取边
        int e=r[i];
        //查找节点所属连通分量
        int x=find(u[e]);
        int y=find(v[e]);
        //如果不是同一个连通分量（即无法形成环），那么选择这个边是最优解
        if(x!=y){
            //这里是对该边的操作，不一定是加上权值
            ans+=w[e];
            //将两个连通分量相连
            p[x]=y;
        }
    }
}
int main(void){
    //省略测试代码
    return 0;
}
```

每一步都注释清楚了](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)





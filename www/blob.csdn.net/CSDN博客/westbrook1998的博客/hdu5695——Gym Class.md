# hdu5695——Gym Class - westbrook1998的博客 - CSDN博客





2018年09月01日 22:27:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：25标签：[优先队列																[拓扑排序](https://so.csdn.net/so/search/s.do?q=拓扑排序&t=blog)](https://so.csdn.net/so/search/s.do?q=优先队列&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
众所周知，度度熊喜欢各类体育活动。  

  今天，它终于当上了梦寐以求的体育课老师。第一次课上，它发现一个有趣的事情。在上课之前，所有同学要排成一列， 假设最开始每个人有一个唯一的ID，从1到N，在排好队之后，每个同学会找出包括自己在内的前方所有同学的最小ID，作为自己评价这堂课的分数。麻烦的是，有一些同学不希望某个（些）同学排在他（她）前面，在满足这个前提的情况下，新晋体育课老师——度度熊，希望最后的排队结果可以使得所有同学的评价分数和最大。  

  Input 

  第一行一个整数T，表示T(1≤T≤30) 组数据。  

  对于每组数据，第一行输入两个整数N和M(1≤N≤100000,0≤M≤100000)，分别表示总人数和某些同学的偏好。  

  接下来M行，每行两个整数A 和B(1≤A,B≤N)，表示ID为A的同学不希望ID为B的同学排在他（她）之前。你可以认为题目保证至少有一种排列方法是符合所有要求的。  

  Output 

  对于每组数据，输出最大分数 。 

  Sample Input 

  3 

  1 0 

  2 1 

  1 2 

  3 1 

  3 1 

  Sample Output 

  1 

  2 

  6
用到优先队列和一个拓扑排序的思想，首先要贪心 没有约束条件的话 越大的放越前面越好 

所以先把入度为0 也就是没有约束条件的加入优先队列 然后每次取出就确定一个位置 每确定一个位置 就看这个数有没有约束其他点 如果有的话将这些点入度减1 因为现在位置确定了 肯定不会违反约束条件了 然后减1后如果入度为0就加入优先队列

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#include <vector>
typedef long long ll;
using namespace std;
const int N=100007;
vector<int> g[N];
int ind[N];
int main(void){
    int t;
    int n,m;
    int u,v;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++){
            g[i].clear();
        }
        memset(ind,0,sizeof(ind));
        for(int i=1;i<=m;i++){
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
            ind[v]++;
        }
        //默认从大到小
        priority_queue<int> q;
        for(int i=1;i<=n;i++){
            //加入入度为0的点 表示没有约束条件
            if(!ind[i]){
                q.push(i);
            }
        }
        ll ans=0;
        //minU维护一个排队队列前面的最小值
        int minU=n+1;
        while(!q.empty()){
            int t=q.top();
            q.pop();
            //自身和前面队列的最小值比较
            minU=min(minU,t);
            //printf("%d\n",t);
            ans+=minU;
            //访问以该点作为约束关系的其他点
            //现在这个点已经确定，因此有约束的这些点的入度就减1
            //如果减1后入度为0了，就是没有约束了，加入队列
            for(int i=0;i<g[t].size();i++){
                if(!(--ind[g[t][i]])){
                    q.push(g[t][i]);
                }
            }
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```







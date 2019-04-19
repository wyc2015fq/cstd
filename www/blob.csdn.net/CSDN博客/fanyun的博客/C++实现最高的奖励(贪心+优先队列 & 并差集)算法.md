# C++实现最高的奖励(贪心+优先队列 & 并差集)算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:29:59[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：110
1、 最高的奖励(贪心+优先队列 & 并差集)算法要求
          有N个任务，每个任务有一个最晚结束时间以及一个对应的奖励。在结束时间之前完成该任务，就可以获得对应的奖励。完成每一个任务所需的时间都是1个单位时间。有时候完成所有任务是不可能的，因为时间上可能会有冲突，这需要你来取舍。求能够获得的最高奖励。 
Input ：
       第1行：一个数N，表示任务的数量(2 <= N <= 50000) 
第2 - N + 1行，每行2个数，中间用空格分隔，表示任务的最晚结束时间E[i]以及对应的奖励W[i]。(1 <= E[i] <= 10^9，1 <= W[i] <= 10^9) 
Output ：
输出能够获得的最高奖励。
 2、思路1
        用贪心思想，从0开始，每完成一件任务，消耗时间为1，按最晚时间递增，第n个任务如果最晚时间大于已消耗掉时间量，则可算入总和，若不大于已耗时间量，则可以替换掉总和里最小奖励的一个任务(如果当前任务的奖励更多的话)。 
这个过程可以用堆维护。nlog(n);
3、代码实现
```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
#include<vector>
using namespace std;
struct Node
{
    int l, v;
};
Node node[50009];
bool cmp(const Node &a, const Node &b)
{
    return a.l < b.l;
}
int main()
{
    int n;
    scanf("%d",&n);
    priority_queue<int, vector<int>, greater<int> > q;
    for(int i=0;i<n;i++)
    {
        scanf("%d%d",&node[i].l, &node[i].v);
    }
    sort(node, node+n, cmp);
    long long ans = 0;
    for(int i=0;i<n;i++)
    {
        int num = node[i].v;
        if(node[i].l > q.size())
        {
            ans += num;
            q.push(num);
        }
        else
        {
            ans += num;
            q.push(num);
            ans -= q.top();
            q.pop();
        }
    }
    printf("%lld\n",ans);
}
```
思路2：并差集
       因为每个任务完成需要的时间都是1，即每个点都可以完成一次任务，所以可以使用并差集维护当前未使用的时间点，f[x] = a;表示最晚时间为x的任务可以在a点完成 
       先按奖励降序排列，然后遍历即可，若有可完成任务的时间节点，则完成此任务，奖励算入总和。 
O(n*a(n)) a(n)为并差集的查找过程 接近于1。
代码实现：
```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
#include<vector>
using namespace std;
struct Node
{
    int l, v;
};
Node node[50009];
int f[50009];
bool cmp(const Node &a, const Node &b)
{
    return a.v > b.v;
}
int find(int x)
{
    if(x <= 0)
        return -1;
    if(x == f[x])
        return f[x] = x-1;
    return f[x] = find(f[x]);
}
int main()
{
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        scanf("%d%d",&node[i].l, &node[i].v);
        if(node[i].l > n) node[i].l = n;
        f[i] = i;
    }
    f[n] = n;
    sort(node, node+n, cmp);
    long long ans = 0;
    for(int i=0;i<n;i++)
    {
        if(find(node[i].l) >= 0)
            ans += node[i].v;
    }
    printf("%lld\n",ans);
}
```

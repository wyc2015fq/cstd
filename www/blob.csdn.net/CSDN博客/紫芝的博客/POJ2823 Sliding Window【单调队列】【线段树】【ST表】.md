# POJ2823 Sliding Window【单调队列】【线段树】【ST表】 - 紫芝的博客 - CSDN博客





2018年11月11日 16:02:01[紫芝](https://me.csdn.net/qq_40507857)阅读数：467








## Sliding Window

[POJ - 2823](https://cn.vjudge.net/problem/16694/origin)

### 题意：

给出一个长度为N的序列，通过一个窗口，可以看到序列中连续的K个元素，窗口从最左边出发，每次移动一个单位，对于每次移动，输出当前窗口中的最大值和最小值

**单调递减队列：从队头到队尾单调递减的队列**
- 入队操作，先把队尾元素中小于X的元素删去，将X插入队尾
- 出队操作：队首元素出队

例如，A={9，8，3，5，10，6，5},构造一个单调递减队列
- 初始队列为空，直接将9入队；队头《9】队尾
- 8小于9，将8入队；队头《9，8】队尾
- 3小于8，将3入队；队头《9，8，3】队尾
- 5大于3，将3删去，然后把5入队；队头《9，8，5】队尾
- 把队列中小于10的元素删去，然后把10入队；队头《10】队尾
- 6小于10，将6入队；队头《10，6】队尾
- 5小于10，将5入队；队头《10，6，5】队尾

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#define MAXN 1000010
using namespace std;
int a[MAXN],Q[MAXN],n,k;
//维护一个单调递增队列，从队头到队尾单调递增
void get_min()
{
    int head,tail,i;
    head=1;tail=1;
    for (i=1;i<=n;i++)
    {
        while ((head<tail)&&(a[i]<a[Q[tail-1]]))//删除队尾元素
            tail--;                            //插入队尾元素
        Q[tail++]=i;
        while ((head<tail)&&(Q[head]<i-k+1)) //删除队首过时元素
            head++;
        /*由于每一层循环只添加一个数，因此每次最多删除一次队首，
        故可改成if语句*/
        //Min[i]=a[Q[head]];                         //访问答案
        if(i>=k)
        printf("%d%c",a[Q[head]],i==n?'\n':' ');
    }
}
//维护一个单调递减队列，从队头到队尾单调递增
void get_max()
{
    int head,tail,i;
    head=1;tail=1;
    for (i=1;i<=n;i++)
    {
        while ((head<tail)&&(a[i]>a[Q[tail-1]]))
            tail--;
        Q[tail++]=i;
        while ((head<tail)&&(Q[head]<i-k+1))
            head++;
        if(i>=k)
        printf("%d%c",a[Q[head]],i==n?'\n':' ');
    }
}
int main()
{
    int i;
    scanf("%d%d",&n,&k);
    for (i=1;i<=n;i++)
        scanf("%d",&a[i]);
    get_min();
    get_max();
    return 0;
}
```

超时

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-5
#define long long LL
using namespace std;
const int maxn=1e6+10;
int a[maxn];
struct Node{
    int left,right,Min,Max;
}tree[maxn<<2];
void pushup(int rt){
    tree[rt].Max=max(tree[rt<<1].Max,tree[rt<<1|1].Max);
    tree[rt].Min=min(tree[rt<<1].Min,tree[rt<<1|1].Min);
}
void build(int rt,int l,int r){
    tree[rt].left=l;
    tree[rt].right=r;
    if(l==r){
        tree[rt].Max=tree[rt].Min=a[l];
        return;
    }
    int mid=(tree[rt].left+tree[rt].right)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    pushup(rt);
}
int query_max(int rt,int l,int r){
    if(tree[rt].left==tree[rt].right){
        return tree[rt].Max;
    }
    int mid=(tree[rt].left+tree[rt].right)>>1;
    if(r<=mid)
        return query_max(rt<<1,l,r);
    else if(l>mid)
        return query_max(rt<<1|1,l,r);
    else
        return max(query_max(rt<<1,l,mid),query_max(rt<<1|1,mid+1,r));
}
int query_min(int rt,int l,int r){
    if(tree[rt].left==tree[rt].right)
        return tree[rt].Min;
    int mid=(tree[rt].left+tree[rt].right)>>1;
    if(r<=mid)
        return query_min(rt<<1,l,r);
    else if(l>mid)
        return query_min(rt<<1|1,l,r);
    else
        return min(query_min(rt<<1,l,mid),query_min(rt<<1|1,mid+1,r));
}
int main()
{
    int n,k;
    while(scanf("%d%d",&n,&k)!=EOF){
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
        }
        if(k==1){
            for(int i=1;i<=n;i++)
            printf("%d%c",a[i],i==n?'\n':' ');
            for(int i=1;i<=n;i++)
            printf("%d%c",a[i],i==n?'\n':' ');
            continue;
        }
        build(1,1,n);
        for(int i=1;i<=n;i++){
            int j=i+k-1;
            if(j>n) break;
            printf("%d%c",query_min(1,i,j),j==n?'\n':' ');
        }

        for(int i=1;i<=n;i++){
            int j=i+k-1;
            if(j>n) break;
            printf("%d%c",query_max(1,i,j),j==n?'\n':' ');
        }
    }
    return 0;
}
```






# Naive Operations（HDU-6315） - Alex_McAvoy的博客 - CSDN博客





2019年02月16日 18:47:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：92
个人分类：[HDU																[数据结构——线段树](https://blog.csdn.net/u011815404/article/category/8669176)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In a galaxy far, far away, there are two integer sequence a and b of length n. 

b is a static permutation of 1 to n. Initially a is filled with zeroes. 

There are two kind of operations: 

1. add l r: add one for al,al+1...ar 

2. query l r: query ![\sum^{r}_{i=l}\frac{a_i}{b_i}](https://private.codecogs.com/gif.latex?%5Csum%5E%7Br%7D_%7Bi%3Dl%7D%5Cfrac%7Ba_i%7D%7Bb_i%7D)

# Input

There are multiple test cases, please read till the end of input file. 

For each test case, in the first line, two integers n,q, representing the length of a,b and the number of queries. 

In the second line, n integers separated by spaces, representing permutation b. 

In the following q lines, each line is either in the form 'add l r' or 'query l r', representing an operation. 

1≤n,q≤100000, 1≤l≤r≤n, there're no more than 5 test cases. 

# Output

Output the answer for each 'query', each one line. 

# **Sample Input**

**5 121 5 2 4 3add 1 4query 1 4add 2 5query 2 5add 3 5query 1 5add 2 4query 1 4add 2 5query 2 5add 2 2query 1 5**

# Sample Output

**112446**


题意：两个数组 a、b 的长度都是 n，给出 q 个操作，其实时，给出 b 数组，且 a 数组的所有元素为 0，接下来的 q 组询问针对 a 数组，共有 2 种：add l r 为将 a[l] 到 a[r] 之间每个元素的值 +1，query l r 为求 ![\sum^{r}_{i=l}[\frac{a_i}{b_i}]](https://private.codecogs.com/gif.latex?%5Csum%5E%7Br%7D_%7Bi%3Dl%7D%5B%5Cfrac%7Ba_i%7D%7Bb_i%7D%5D)

思路：

首先，能确定使用线段树来做，唯一的难点就是计算 a[i]/b[i] 下取整的和，即 a[i]/b[i] 什么时候才能 +1

对于 a 来说，需要去维护区间最大值，而对于 b 来说，需要去维护区间最小值，因此可以在线段树中可以去维护 a 的最大值 maxA 与 b 的最小值 minB，当最大值小于最小值时，那么就不需要 +1

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;
    LL sum;
    LL maxx,minn;
    LL lazyAdd;
}tree[N*4];
LL b[N];
LL resSum;//存储结果
void pushDown(int i){//标记下传
    if(tree[i].lazyAdd!=0){
        tree[i*2].lazyAdd+=tree[i].lazyAdd;//打上延迟标记
        tree[i*2+1].lazyAdd+=tree[i].lazyAdd;//打上延迟标记

        tree[i*2].maxx+=tree[i].lazyAdd;//更新
        tree[i*2+1].maxx+=tree[i].lazyAdd;//更新

        tree[i].lazyAdd=0;//清除标记
    }
}

void pushUp(int i){//维护子节点
    tree[i].sum=tree[i*2].sum+tree[i*2+1].sum;
    tree[i].maxx=max(tree[i*2].maxx,tree[i*2+1].maxx);
    tree[i].minn=min(tree[i*2].minn,tree[i*2+1].minn);
}

void build(int i,int l,int r){//建树
    tree[i].l=l;
    tree[i].r=r;
    tree[i].lazyAdd=0;

    if(l==r){//叶结点
        tree[i].sum=0;
        tree[i].maxx=0;
        tree[i].minn=b[l];
        return;
    }

    int mid=(l+r)>>1;

    build(i*2,l,mid);//结点左儿子
    build(i*2+1,mid+1,r);//结点右儿子

    pushUp(i);
}

void updateAdd(int i,int ql,int qr,LL val){//区间修改，整体+val
    if(tree[i].l>=ql&&tree[i].r<=qr){
        tree[i].maxx+=val;
        if(tree[i].maxx<tree[i].minn){
            tree[i].lazyAdd+=val;
            return;
        }
        if(tree[i].l==tree[i].r&&tree[i].maxx>=tree[i].minn){
            tree[i].sum++;
            tree[i].minn+=b[tree[i].l];
            return;
        }
    }

    pushDown(i);//标记下传

    int mid=(tree[i].l+tree[i].r)/2;
    if(ql<=mid)
        updateAdd(i*2,ql,qr,val);
    if(qr>mid)
        updateAdd(i*2+1,ql,qr,val);

    pushUp(i);
}

void query(int i,int ql,int qr){//区间查询
    if(ql<=tree[i].l && tree[i].r<=qr){
        resSum+=tree[i].sum;
        return ;
    }

    pushDown(i);

    int mid=(tree[i].l+tree[i].r)/2;
    if(ql<=mid)
        query(i*2,ql,qr);
    if(qr>mid)
        query(i*2+1,ql,qr);

    pushUp(i);
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++)
            scanf("%lld",&b[i]);

        build(1,1,n);
        while(m--){
            char str[5];
            scanf("%s",str);

            if(str[0]=='a'){
                int x,y;
                int val=1;
                scanf("%d%d",&x,&y);
                updateAdd(1,x,y,val);
            }
            else{
                int x,y;
                scanf("%d%d",&x,&y);
                resSum=0;
                query(1,x,y);
                printf("%lld\n",resSum);
            }
        }
    }
    return 0;
}
```







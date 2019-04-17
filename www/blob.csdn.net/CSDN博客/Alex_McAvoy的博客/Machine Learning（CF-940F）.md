# Machine Learning（CF-940F） - Alex_McAvoy的博客 - CSDN博客





2019年03月07日 19:15:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52








> 
# Problem Description

You come home and fell some unpleasant smell. Where is it coming from?

You are given an array a. You have to answer the following queries:

You are given two integers l and r. Let ci be the number of occurrences of i in al: r, where al: r is the subarray of a from l-th element to r-th inclusive. Find the Mex of {c0, c1, ..., c109}

You are given two integers p to x. Change ap to x.

The Mex of a multiset of numbers is the smallest non-negative integer not in the set.

Note that in this problem all elements of a are positive, which means that c0 = 0 and 0 is never the answer for the query of the second type.

# **Input**

The first line of input contains two integers n and q (1 ≤ n, q ≤ 100 000) — the length of the array and the number of queries respectively.

The second line of input contains n integers — a1, a2, ..., an (1 ≤ ai ≤ 109).

Each of the next q lines describes a single query.

The first type of query is described by three integers ti = 1, li, ri, where 1 ≤ li ≤ ri ≤ n — the bounds of the subarray.

The second type of query is described by three integers ti = 2, pi, xi, where 1 ≤ pi ≤ n is the index of the element, which must be changed and 1 ≤ xi ≤ 109 is the new value.

# Output

For each query of the first type output a single integer  — the Mex of {c0, c1, ..., c109}.

# Sample Input

**10 41 2 3 1 1 2 2 2 9 91 1 11 2 82 7 11 2 8**

# Sample Output

**232**


题意：给出一个长度为 n 的数组以及 m 个询问，对于每个询问 q 有两种操作，q 为 1 时查询区间 [l,r] 中每个数字出现次数的 mex，q 为 2 时将位置 p 的值修改为 x，其中 mex 代表一些数字中最小的未出现的自然数

思路：带修莫队+离散化

这个题实际上是 [数颜色（洛谷-P1903）](https://blog.csdn.net/u011815404/article/details/88317597) 的变种，由于需要统计出现次数需要使用桶排，而数值最大达到 1E9，因此需要离散化，离散化后进行带修莫队即可，最后暴力求 mex

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
#define LL long long
const int MOD=10007;
const int N=200000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;//询问的左右端点
    int time;//时间维度
    int id;//询问的编号
}q[N];
struct Change{
    int pos;//要修改的位置
    int col;//要改成的值
}c[N];
int n,m,a[N],temp[N];
int block;//分块
int numQ,numC;//查询、修改操作的次数
int cnt[N],sum[N];
int res[N];

bool cmp(Node a,Node b){//排序
    return (a.l/block)^(b.l/block) ? a.l<b.l : ((a.r/block)^(b.r/block)?a.r<b.r:a.time<b.time);
}

void add(int x){//统计新的，根据具体情况修改
    sum[cnt[x]]--;
    cnt[x]++;
    sum[cnt[x]]++;
}
void del(int x){//减去旧的，根据具体情况修改
    sum[cnt[x]]--;
    cnt[x]--;
    sum[cnt[x]]++;
}
void change(int x,int ti){//改变时间轴
    if(c[ti].pos>=q[x].l&&c[ti].pos<=q[x].r){
        del(a[c[ti].pos]);//删除指定位置上的值
        add(c[ti].col);//统计新的数
    }
    swap(c[ti].col,a[c[ti].pos]);//直接互换，下次执行时必定是回退这次操作
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF){
        numQ=0;
        numC=0;
        memset(sum,0,sizeof(sum));
        memset(cnt,0,sizeof(cnt));
        block=pow(n,0.66666);//分块

        int numTemp=0;
        for(int i=1;i<=n;++i){
            scanf("%d",&a[i]);
            temp[++numTemp]=a[i];
        }
        for(int i=1;i<=m;i++){
            int op;
            scanf("%d",&op);
            if(op==1){//查询操作
                ++numQ;//查询操作次数+1
                scanf("%d%d",&q[numQ].l,&q[numQ].r);
                q[numQ].id=numQ;//序号
                q[numQ].time=numC;//时间轴
            }
            else{//修改操作
                ++numC;//修改操作次数+1
                scanf("%d%d",&c[numC].pos,&c[numC].col);
                temp[++numTemp]=c[numC].col;
            }
        }

        //离散化
        sort(temp+1,temp+1+numTemp);
        numTemp=unique(temp+1,temp+1+numTemp)-temp;
        for(int i=1;i<=n;i++)
            a[i]=lower_bound(temp+1,temp+1+numTemp,a[i])-temp;
        for(int i=1;i<=numC;i++)
            c[i].col=lower_bound(temp+1,temp+1+numTemp,c[i].col)-temp;

        sort(q+1,q+numQ+1,cmp);//对询问进行排序
        int l=1,r=0,time=0;//左右指针与时间轴
        for(int i=1;i<=numQ;i++){
            int ql=q[i].l,qr=q[i].r;//询问的左右端点
            int qtime=q[i].time;//询问的时间轴
            while(l>ql) add(a[--l]);//[l-1,r,time]
            while(l<ql) del(a[l++]);//[l+1,r,time]
            while(r<qr) add(a[++r]);//[l,r+1,time]
            while(r>qr) del(a[r--]);//[l,r-1,time]
            while(time<qtime) change(i,++time);//[l,r,time+1]
            while(time>qtime) change(i,time--);//[l,r,time-1]

            res[q[i].id]=1;
            while(sum[res[q[i].id]]>0)
                res[q[i].id]++;
        }

        for(int i=1;i<=numQ;i++)
            printf("%d\n",res[i]);
    }
    return 0;
}
```






# Little Elephant and Array（CF-220B） - Alex_McAvoy的博客 - CSDN博客





2019年03月06日 00:48:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：21








> 
# Problem Description

The Little Elephant loves playing with arrays. He has array a, consisting of n positive integers, indexed from 1 to n. Let's denote the number with index i as ai.

Additionally the Little Elephant has m queries to the array, each query is characterised by a pair of integers lj and rj (1 ≤ lj ≤ rj ≤ n). For each query lj, rj the Little Elephant has to count, how many numbers x exist, such that number x occurs exactly x times among numbers alj, alj + 1, ..., arj.

Help the Little Elephant to count the answers to all queries.

# **Input**

The first line contains two space-separated integers n and m (1 ≤ n, m ≤ 105) — the size of array a and the number of queries to it. The next line contains n space-separated positive integers a1, a2, ..., an (1 ≤ ai ≤ 109). Next m lines contain descriptions of queries, one per line. The j-th of these lines contains the description of the j-th query as two space-separated integers lj and rj (1 ≤ lj ≤ rj ≤ n).

# Output

In m lines print m integers — the answers to the queries. The j-th line should contain the answer to the j-th query.

# Examples

**Input**

7 2

3 1 2 2 3 3 7

1 7

3 4

**Output**

3

1


题意：一个长度为 n 的数字序列，m 次询问，每次询问给出一个区间 [l,r]，求这个区间内有多少个数它的值和它出现的次数相同

思路：

普通莫队，修改 O(1) 部分即可

对于 add 操作，如果原本符合，那么 ans--，如果现在符合，那么 ans++

对于 del 操作，如果原本符合，那么 ans--，如果现在符合，那么 ans++

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
const int N=2000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;//询问的左右端点
    int id;//询问的编号
}q[N];
int n,m,a[N];
int block;//分块
LL ans,cnt[N];
LL res[N];

bool cmp(Node a,Node b){//奇偶性排序
    return (a.l/block)^(b.l/block)?a.l<b.l:(((a.l/block)&1)?a.r<b.r:a.r>b.r);
}

void add(int x){//统计新的
    cnt[a[x]]++;
    if(cnt[a[x]]==a[x])
        ans++;
    if(cnt[a[x]]==a[x]+1)
        ans--;
}
void del(int x){//减去旧的
    cnt[a[x]]--;
    if(cnt[a[x]]==a[x])
        ans++;
    if(cnt[a[x]]==a[x]-1)
        ans--;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF){
        for(int i=1;i<=n;++i){
            scanf("%d",&a[i]);
            if(a[i]>n)//大于n肯定不符合
                a[i]=-1;
        }
        for(int i=1;i<=m;i++){
            scanf("%d%d",&q[i].l,&q[i].r);
            q[i].id=i;
        }

        ans=0;
        memset(cnt,0,sizeof(cnt));
        block=sqrt(m*2/3*1.0);//分块，卡常数
        sort(q+1,q+m+1,cmp);//对询问进行排序

        int l=1,r=0;//左右指针
        for(int i=1;i<=m;i++){
            int ql=q[i].l,qr=q[i].r;//询问的左右端点
            while(l>ql) add(--l);//[l-1,r]
            while(l<ql) {l++;del(l-1);}//[l+1,r]
            while(r<qr) add(++r);//[l,r+1]
            while(r>qr) {r--;del(r+1);}//[l,r-1]
            res[q[i].id]=ans;//获取答案
        }

        for(int i=1;i<=m;i++)
            printf("%lld\n",res[i]);
    }
    return 0;
}
```






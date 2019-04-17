# XOR and Favorite Number（CF-617E） - Alex_McAvoy的博客 - CSDN博客





2019年03月06日 00:29:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：24








> 
# Problem Description

Bob has a favorite number k and ai of length n. Now he asks you to answer m queries. Each query is given by a pair li and ri and asks you to count the number of pairs of integers i and j, such that l ≤ i ≤ j ≤ r and the xor of the numbers ai, ai + 1, ..., aj is equal to k.

# **Input**

The first line of the input contains integers n, m and k (1 ≤ n, m ≤ 100 000, 0 ≤ k ≤ 1 000 000) — the length of the array, the number of queries and Bob's favorite number respectively.

The second line contains n integers ai (0 ≤ ai ≤ 1 000 000) — Bob's array.

Then m lines follow. The i-th line contains integers li and ri (1 ≤ li ≤ ri ≤ n) — the parameters of the i-th query.

# Output

Print m lines, answer the queries in the order they appear in the input.

# Examples

**Input**

6 2 3

1 2 1 1 0 3

1 6

3 5

**Output**

7

0

**Input**

5 3 1

1 1 1 1 1

1 5

2 4

1 3

**Output**

9

4

4

# Note

In the first sample the suitable pairs of i and j for the first query are: (1, 2), (1, 4), (1, 5), (2, 3), (3, 6), (5, 6), (6, 6). Not a single of these pairs is suitable for the second query.

In the second sample xor equals 1 for all subarrays of an odd length.


题意：一个长度为 n 的数字序列，m 次询问，以及一个数 k，每次询问给出一个区间 [l,r]，求这个区间内有多少个子区间的所有数异或为 k

思路：

问题实质是求区间 [l,r] 内异或值等于 k 的个数，可转为前缀和来做

在输入过程中令 a[i]^=a[i-1]，那么在区间 [l,r] 内，所有数的异或=k 时，有：a[r]^k=a[l-1]

最后再用莫队处理区间，分块枚举即可

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
int n,m,k,a[N];
int block;//分块
LL ans,cnt[N];
LL res[N];

bool cmp(Node a,Node b){//奇偶性排序
    return (a.l/block)^(b.l/block)?a.l<b.l:(((a.l/block)&1)?a.r<b.r:a.r>b.r);
}

void add(int x){//统计新的
    ans+=cnt[a[x]^k];
    cnt[a[x]]++;
}
void del(int x){//减去旧的
    cnt[a[x]]--;
    ans-=cnt[a[x]^k];
}
int main(){
    while(scanf("%d%d%d",&n,&m,&k)!=EOF){
        a[0]=0;
        for(int i=1;i<=n;++i){
            scanf("%d",&a[i]);
            a[i]=a[i-1]^a[i];
        }
        for(int i=1;i<=m;i++){
            scanf("%d%d",&q[i].l,&q[i].r);
            q[i].id=i;
        }

        memset(cnt,0,sizeof(cnt));
        ans=0;
        cnt[0]=1;
        block=sqrt(m*2/3*1.0);//分块，卡常数
        sort(q+1,q+m+1,cmp);//对询问进行排序

        int l=1,r=0;//左右指针
        for(int i=1;i<=m;i++){
            int ql=q[i].l,qr=q[i].r;//询问的左右端点
            while(l>ql) {l--;add(l-1);}//[l-1,r]
            while(l<ql) {del(l-1);l++;}//[l+1,r]
            while(r<qr) add(++r);//[l,r+1]
            while(r>qr) del(r--);//[l,r-1]
            res[q[i].id]=ans;//获取答案
        }

        for(int i=1;i<=m;i++)
            printf("%lld\n",res[i]);
    }
    return 0;
}
```






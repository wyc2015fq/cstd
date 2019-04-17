# Powerful array（CF-86D） - Alex_McAvoy的博客 - CSDN博客





2019年03月05日 23:24:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：34








> 
# Problem Description

An array of positive integers a1, a2, ..., an is given. Let us consider its arbitrary subarray al, al + 1..., ar, where 1 ≤ l ≤ r ≤ n. For every positive integer s denote by Ks the number of occurrences of s into the subarray. We call the power of the subarray the sum of products Ks·Ks·s for every positive integer s. The sum contains only finite number of nonzero summands as the number of different values in the array is indeed finite.

You should calculate the power of t given subarrays.

# **Input**

First line contains two integers n and t (1 ≤ n, t ≤ 200000) — the array length and the number of queries correspondingly.

Second line contains n positive integers ai (1 ≤ ai ≤ 106) — the elements of the array.

Next t lines contain two positive integers l, r (1 ≤ l ≤ r ≤ n) each — the indices of the left and the right ends of the corresponding subarray.

# Output

Output t lines, the i-th line of the output should contain single positive integer — the power of the i-th query subarray.

Please, do not use %lld specificator to read or write 64-bit integers in C++. It is preferred to use cout stream (also you may use %I64d).

# Examples

**Input**

3 2

1 2 1

1 2

1 3

**Output**

3

6

**Input**

8 3

1 1 2 2 1 3 1 1

2 7

1 6

2 7

**Output**

20

20

20

# Note

Consider the following array (see the second sample) and its [2, 7] subarray (elements of the subarray are colored):

![](https://img-blog.csdnimg.cn/20190305232516727.jpg)

Then K1 = 3, K2 = 2, K3 = 1, so the power is equal to 32·1 + 22·2 + 12·3 = 20.


题意：一个长度为 n 的数字序列，m 次询问，每次询问给出一个区间 [l,r]，求这个区间内出现过的值与其次数平方的乘积的和

思路：普通莫队模版题，要求每个区间出现过的值与其出现次数平方的乘积的和，只需修改 O(1) 部分即可

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
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;//询问的左右端点
    int id;//询问的编号
}q[N];
int n,m,a[N];
int block;//分块
LL ans,cnt[N*2];
LL res[N];

bool cmp(Node a,Node b){//奇偶性排序
    return (a.l/block)^(b.l/block)?a.l<b.l:(((a.l/block)&1)?a.r<b.r:a.r>b.r);
}

void add(int x){
    ans-=cnt[a[x]]*cnt[a[x]]*a[x];//减去旧的
    cnt[a[x]]++;
    ans+=cnt[a[x]]*cnt[a[x]]*a[x];//加上新的
}
void del(int x){
    ans-=cnt[a[x]]*cnt[a[x]]*a[x];//减去旧的
    cnt[a[x]]--;
    ans+=cnt[a[x]]*cnt[a[x]]*a[x];//加上新的
}

int main(){
    while(scanf("%d%d",&n,&m)!=EOF){
        memset(cnt,0,sizeof(cnt));
        ans=0;

        for(int i=1;i<=n;++i)
            scanf("%d",&a[i]);
        for(int i=1;i<=m;i++){
            scanf("%d%d",&q[i].l,&q[i].r);
            q[i].id=i;
        }

        block=sqrt(m*2/3*1.0);//分块，卡常数
        sort(q+1,q+m+1,cmp);//对询问进行排序

        int l=1,r=0;//左右指针
        for(int i=1;i<=m;i++){
            int ql=q[i].l,qr=q[i].r;//询问的左右端点
            while(l>ql) add(--l);
            while(r<qr) add(++r);
            while(l<ql) del(l++);
            while(r>qr) del(r--);
            res[q[i].id]=ans;//获取答案
        }

        for(int i=1;i<=m;i++)
            printf("%lld\n",res[i]);
    }
    return 0;
}
```






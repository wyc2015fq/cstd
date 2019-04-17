# Fast Queries（LightOJ-1188） - Alex_McAvoy的博客 - CSDN博客





2019年03月05日 22:15:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31








> 
# Problem Description

Given an array of N integers indexed from 1 to N, and q queries, each in the form i j, you have to find the number of distinct integers from index i to j (inclusive).

# **Input**

Input starts with an integer T (≤ 5), denoting the number of test cases.

The first line of a case is a blank line. The next line contains two integers N (1 ≤ N ≤ 105), q (1 ≤ q ≤ 50000). The next line contains N space separated integers forming the array. There integers range in [0, 105].

Each of the next q lines will contain a query which is in the form i j (1 ≤ i ≤ j ≤ N).

# Output

For each test case, print the case number in a single line. Then for each query you have to print a line containing number of distinct integers from index i to j.

# Sample Input

**1**

**8 51 1 1 2 3 5 1 21 82 33 64 54 8**

# Sample Output

**Case 1:41424**


思路：莫队算法模版题

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
void add(int x){//根据实际情况进行修改
    if(!cnt[a[x]])
        ans++;
    cnt[a[x]]++;
}
void del(int x){//根据实际情况进行修改
    cnt[a[x]]--;
    if(!cnt[a[x]])
        ans--;
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        memset(cnt,0,sizeof(cnt));
        ans=0;

        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;++i)
            scanf("%d",&a[i]);
        for(int i=1;i<=m;i++){
            scanf("%d%d",&q[i].l,&q[i].r);
            q[i].id=i;
        }

        block=n/sqrt(m*2/3*1.0);//分块
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

        printf("Case %d:\n",Case++);
        for(int i=1;i<=m;i++)
            printf("%lld\n",res[i]);
    }
    return 0;
}
```






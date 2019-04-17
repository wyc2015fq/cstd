# Sona（NBUT-1457） - Alex_McAvoy的博客 - CSDN博客





2019年03月06日 01:55:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：18
个人分类：[数据结构——莫队算法																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/8724401)








> 
# Problem Description

Sona , Maven of the Strings . Of cause, she can play the zither.

Sona can't speak but she can make fancy music. Her music can attack, heal, encourage and enchant.

There're an ancient score(乐谱). But because it's too long, Sona can't play it in a short moment. So Sona decide to just play a part of it and revise it.

A score is composed of notes. There are 109 kinds of notes and a score has 105 notes at most.

To diversify Sona's own score, she have to select several parts of it. The energy of each part is calculated like that:

Count the number of times that each notes appear. Sum each of the number of times' cube together. And the sum is the energy.

You should help Sona to calculate out the energy of each part.

# **Input**

This problem contains several cases. And this problem provides 2 seconds to run. 

The first line of each case is an integer N (1 ≤ N ≤ 10^5), indicates the number of notes. 

Then N numbers followed. Each number is a kind of note. (1 ≤ NOTE ≤ 10^9) 

Next line is an integer Q (1 ≤ Q ≤ 10^5), indicates the number of parts. 

Next Q parts followed. Each part contains 2 integers Li and Ri, indicates the left side of the part and the right side of the part.

# Output

For each part, you should output the energy of that part.

# Sample Input

**81 1 3 1 3 1 3 341 83 85 65 5**

# Sample Output

**1287221**


题意：给出 n 个数和 m 组询问，对于每组询问给出一个区间 [l,r]，求区间内每种数字出现次数的立方和

思路：普通莫队+离散化

很基础的普通莫队，直接修改 O(1) 部分即可，由于数据范围，需要进行离散化

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
const int N=500000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;//询问的左右端点
    int id;//询问的编号
}q[N];
int n,m,a[N],temp[N];
int block;//分块
LL ans,cnt[N];
LL res[N];

bool cmp(Node a,Node b){//奇偶性排序
    return (a.l/block)^(b.l/block)?a.l<b.l:(((a.l/block)&1)?a.r<b.r:a.r>b.r);
}

void add(int x){//统计新的
    ans-=cnt[a[x]]*cnt[a[x]]*cnt[a[x]];
    cnt[a[x]]++;
    ans+=cnt[a[x]]*cnt[a[x]]*cnt[a[x]];
}
void del(int x){//减去旧的
    ans-=cnt[a[x]]*cnt[a[x]]*cnt[a[x]];
    cnt[a[x]]--;
    ans+=cnt[a[x]]*cnt[a[x]]*cnt[a[x]];
}
int main(){
    while(scanf("%d",&n)!=EOF){
        for(int i=1;i<=n;++i){
            scanf("%d",&a[i]);
            temp[i]=a[i];
        }
        //离散化
        sort(temp+1,temp+1+n);
        int len=unique(temp+1,temp+1+n)-temp;
        for(int i=1;i<=n;i++)
            a[i]=lower_bound(temp+1,temp+1+len,a[i])-temp;

        scanf("%d",&m);
        for(int i=1;i<=m;i++){
            scanf("%d%d",&q[i].l,&q[i].r);
            q[i].id=i;
        }

        ans=0;
        memset(cnt,0,sizeof(cnt));
        block=n/sqrt(m*2/3*1.0);//分块，防卡常数
        sort(q+1,q+m+1,cmp);//对询问进行排序

        int l=1,r=0;//左右指针
        for(int i=1;i<=m;i++){
            int ql=q[i].l,qr=q[i].r;//询问的左右端点
            while(l>ql) add(--l);//[l-1,r]
            while(l<ql) del(l++);//[l+1,r]
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







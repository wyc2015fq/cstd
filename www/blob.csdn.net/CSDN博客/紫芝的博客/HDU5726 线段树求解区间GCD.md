# HDU5726 线段树求解区间GCD - 紫芝的博客 - CSDN博客





2018年08月14日 22:06:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：220








# GCD

****Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 5302    Accepted Submission(s): 1908****

**Problem Description**

Give you a sequence of N(N≤100,000) integers : a1,...,an(0<ai≤1000,000,000). There are Q(Q≤100,000) queries. For each query l,r you have to calculate gcd(al,,al+1,...,ar) and count the number of pairs(l′,r′)(1≤l<r≤N)such that gcd(al′,al′+1,...,ar′) equal gcd(al,al+1,...,ar).

**Input**

The first line of input contains a number T, which stands for the number of test cases you need to solve.


The first line of each case contains a number N, denoting the number of integers.


The second line contains N integers, a1,...,an(0<ai≤1000,000,000).


The third line contains a number Q, denoting the number of queries.


For the next Q lines, i-th line contains two number , stand for the li,ri, stand for the i-th queries.

**Output**

For each case, you need to output “Case #:t” at the beginning.(with quotes, t means the number of the test case, begin from 1).


For each query, you need to output the two numbers in a line. The first number stands for gcd(al,al+1,...,ar) and the second number stands for the number of pairs(l′,r′) such that gcd(al′,al′+1,...,ar′) equal gcd(al,al+1,...,ar).

**Sample Input**

```
1 
5 
1 2 4 6 7 
4 
1 5 
2 4 
3 4 
4 4
```

**Sample Output**

```
Case #1: 
1 8 
2 4 
2 4 
6 1
```

**Author**

HIT

**Source**

[2016 Multi-University Training Contest 1](http://acm.hdu.edu.cn/search.php?field=problem&key=2016+Multi-University+Training+Contest+1&source=1&searchmode=source)

**Recommend**

wange2014   |   We have carefully selected several similar problems for you:  [6396](http://acm.hdu.edu.cn/showproblem.php?pid=6396)[6395](http://acm.hdu.edu.cn/showproblem.php?pid=6395)[6394](http://acm.hdu.edu.cn/showproblem.php?pid=6394)[6393](http://acm.hdu.edu.cn/showproblem.php?pid=6393)[6392](http://acm.hdu.edu.cn/showproblem.php?pid=6392)

**线段树求解区间GCD**
题意：

给n个数，q次查询，首先问区间[l,r]的gcd(a[l],a[l+1],a[l+2]....a[r-1],a[r]),然后输出与区间[l,r]的gcd相等的区间的个数

第一问用线段树求解区间，第二问就是先预处理出以i为开头的区间的各自的GCD的值,用线段树来维护区间GCD和查询

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e5+10;
const int inf=0x3f3f3f3f;
int n;
int num[maxn];
struct segtree{
    int g[maxn<<2];
    void build(int i,int l,int r)
    {
        if(l==r){
            g[i]=num[l];
            return ;
        }
        int mid=(l+r)>>1;
        build(i<<1,l,mid);
        build(i<<1|1,mid+1,r);
        g[i]=__gcd(g[i<<1],g[i<<1|1]);
    }
    int query(int i,int l,int r,int L,int R)
    {
        if(L<=l&&r<=R)  return g[i];
        int mid=(l+r)>>1;
        int ls=0,rs=0;
        if(L<=mid)
            ls=query(i<<1,l,mid,L,R);
        if(R>mid)
            rs=query(i<<1|1,mid+1,r,L,R);
        if(!ls) swap(ls,rs);
        return __gcd(ls,rs);
    }
    int search(int i,int l,int r,int L,int R,int cur,int &GCD)
    {
        if(L<=l&&r<=R)
        {
            if(__gcd(g[i],GCD)<cur){
                if(l==r)    return l;
            else{
                int mid=(l+r)>>1,ls=0,rs=0;
                ls=search(i<<1,l,mid,L,R,cur,GCD);
                if(ls)  return ls;
                rs=search(i<<1|1,mid+1,r,L,R,cur,GCD);
                return rs;
            }
        }
        else{
            GCD=__gcd(g[i],GCD);
            return 0;
        }
    }
    int mid=(l+r)>>1,ls=0,rs=0;
    if(L<=mid)  ls=search(i<<1,l,mid,L,R,cur,GCD);
    if(ls)  return ls;
    if(R>mid)
        rs=search(i<<1|1,mid+1,r,L,R,cur,GCD);
    return rs;
}
}seg;

map<int ,ll>M;
int main()
{
    int T,Case=1;
    scanf("%d",&T);
    while(T--){
        M.clear();
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&num[i]);
        }
        seg.build(1,1,n);
        int GCD,nex;
        for(int i=1;i<=n;i++)
        {
            int g=seg.query(1,1,n,i,n);
            for(int now=i,cur=num[i];now<=n;)
            {
                if(g==cur)
                {
                    M[g]+=n-now+1;
                    break;
                }
                GCD=num[i];
                nex=seg.search(1,1,n,i,n,cur,GCD);
                M[cur]+=nex-now;
                now=nex;
                cur=__gcd(num[now],cur);
            }
        }
        int q;
        scanf("%d",&q);
        printf("Case #%d:\n",Case++);
        int a,b,ans;
        for(int i=1;i<=q;i++)
        {
            scanf("%d%d",&a,&b);
            ans=seg.query(1,1,n,a,b);
            printf("%d %lld\n",ans,M[ans]);
        }
    }
    return 0;
}
```






# HDU-6315 多校第二场 Naive Operations - 安得广厦千万间的博客 - CSDN博客





2018年07月25日 21:19:52[独-](https://me.csdn.net/qq_41713256)阅读数：86








# Naive Operations

****Time Limit: 6000/3000 MS (Java/Others)    Memory Limit: 502768/502768 K (Java/Others)Total Submission(s): 481    Accepted Submission(s): 158****



**Problem Description**

In a galaxy far, far away, there are two integer sequence a and b of length n.

b is a static permutation of 1 to n. Initially a is filled with zeroes.

There are two kind of operations:

1. add l r: add one for al,al+1...ar

2. query l r: query ∑ri=l⌊ai/bi⌋





**Input**

There are multiple test cases, please read till the end of input file.

For each test case, in the first line, two integers n,q, representing the length of a,b and the number of queries.

In the second line, n integers separated by spaces, representing permutation b.

In the following q lines, each line is either in the form 'add l r' or 'query l r', representing an operation.

1≤n,q≤100000, 1≤l≤r≤n, there're no more than 5 test cases.





**Output**

Output the answer for each 'query', each one line.





**Sample Input**
` `
5 12 

1 5 2 4 3 

add 1 4 query 1 4 add 2 5 query 2 5 add 3 5 query 1 5 add 2 4 query 1 4 add 2 5 query 2 5 add 2 2 query 1 5





**Sample Output**
` `
1 1 2 4 4 6



求 a/b的下限的和。

我们用c数组记录a/b的值，因为b是一个全排列，c的值变化的次数不大，就算全部都对整个数组更新，

也就是  ![\sum_{i=1}^{100000} 100000/i](https://private.codecogs.com/gif.latex?%5Csum_%7Bi%3D1%7D%5E%7B100000%7D%20100000/i)  大约为100000*log100000.

当对a数组的区间加一时，我们转化为对b数组的区间减一，当b数组中有一个值为0时，我们就对那个位置的c加一，b的值还原。

一直做同样的操作，直到b当前区间的最小值大于0（这里区间中可能不止一个位置要更新，然后我们要维护区间的最小值以及对应的位置）每次找到一个为0的位置，对c数组相应的位置加一（可以用树状数组维护）。

整理一下，这里涉及到线段树的单点更新，区间更新，区间查询。树状数组对的单点更新+区间查询。

```
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL mod = 1e9+7;
const int maxn = 100010;
int a[maxn],b[maxn],n;
int c[maxn<<2],p[maxn<<2],lazy[maxn<<2];
int lowbit(int x){ return x&(-x); }
void add(int x)
{
    while ( x<=n)
        a[x]++, x+=lowbit(x);
}
int sum(int x)
{
    int res=0;
    while ( x)
        res+=a[x], x-=lowbit(x);
    return res;
}
void up(int rt)
{
    if( c[rt<<1]>c[rt<<1|1])
    {
        c[rt]=c[rt<<1|1]; p[rt]=p[rt<<1|1];
    }
    else
    {
        c[rt]=c[rt<<1]; p[rt]=p[rt<<1];
    }
}
void build(int rt,int nowa,int nowb)
{
    lazy[rt]=0;
    if( nowa==nowb )
    {
        c[rt]=b[nowa];
        p[rt]=nowa;
        return;
    }
    int mid=nowa+nowb>>1;
    build(rt<<1,nowa,mid);
    build(rt<<1|1,mid+1,nowb);
    up(rt);
}
void down(int rt)
{
    if( lazy[rt] )
    {
        c[rt<<1]-=lazy[rt];
        c[rt<<1|1]-=lazy[rt];
        lazy[rt<<1]+=lazy[rt];
        lazy[rt<<1|1]+=lazy[rt];
        lazy[rt]=0;
    }
}
void updata(int rt,int l,int r,int nowa,int nowb)
{
    if( r<nowa||l>nowb ) return;
    if( l<=nowa&&nowb<=r )
    {
        c[rt]--; lazy[rt]++;
        return;
    }
    down(rt);
    int mid=nowa+nowb>>1;
    updata(rt<<1,l,r,nowa,mid);
    updata(rt<<1|1,l,r,mid+1,nowb);
    up(rt);
}
int ans,pos;
void change(int rt,int nowa,int nowb,int pp)
{
    if( nowa==nowb&&nowa==pp )
    {
        c[rt]=b[nowa];
        return;
    }
    down(rt);
    int mid=nowa+nowb>>1;
    if( pp<=mid )
        change(rt<<1,nowa,mid,pp);
    else
        change(rt<<1|1,mid+1,nowb,pp);
    up(rt);
}
void query(int rt,int l,int r,int nowa,int nowb)
{
    if( r<nowa||l>nowb ) return;
    if( l<=nowa&&nowb<=r )
    {
        if( ans>c[rt] )
        {
            ans=c[rt];
            pos=p[rt];
        }
        return;
    }
    down(rt);
    int mid=nowa+nowb>>1;
    query(rt<<1,l,r,nowa,mid);
    query(rt<<1|1,l,r,mid+1,nowb);
    up(rt);
}
int main(){
    int  q;
    while ( scanf( "%d%d", &n, &q )==2 )
    {
        for(int i=1;i<=n;i++) scanf( "%d", &b[i] );
        memset(a, 0, sizeof(a) );
        build(1,1,n);
        while ( q-- )
        {
            char op[8];
            int x,y; scanf( "%s%d%d", op, &x, &y );
            if( op[0]=='a' )
            {
                updata(1,x,y,1,n);  //区间更新
                ans=maxn; pos=-1;
                query(1,x,y,1,n);  // 查询区间最小值
                while( ans==0 )
                {
                    add(pos);    // c[pos]加一
                    change(1,1,n,pos); // b[pos]还原
                    ans=maxn; pos=-1;
                    query(1,x,y,1,n);
                }
            }
            else
                printf( "%d\n", sum(y)-sum(x-1) );
        }
    }
    return 0;
}
```






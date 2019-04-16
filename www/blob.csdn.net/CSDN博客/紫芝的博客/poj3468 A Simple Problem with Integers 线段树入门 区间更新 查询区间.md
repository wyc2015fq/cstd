# poj3468 A Simple Problem with Integers 线段树入门 区间更新 查询区间 - 紫芝的博客 - CSDN博客





2018年08月20日 23:38:03[紫芝](https://me.csdn.net/qq_40507857)阅读数：29
个人分类：[线段树](https://blog.csdn.net/qq_40507857/article/category/7569707)









# A Simple Problem with Integers
|**Time Limit:** 5000MS||**Memory Limit:** 131072K|
|----|----|----|
|**Total Submissions:** 140749||**Accepted:** 43645|
|**Case Time Limit:** 2000MS| | |

Description

You have *N* integers, *A*1, *A*2, ... , *AN*. You need to deal with two kinds of operations. One type of operation is to add some given number to each number in a given interval. The other is to ask for the sum of numbers in a given interval.

Input

The first line contains two numbers *N* and *Q*. 1 ≤ *N*,*Q* ≤ 100000.

The second line contains *N* numbers, the initial values of *A*1, *A*2, ... , *AN*. -1000000000 ≤ *Ai* ≤ 1000000000.

Each of the next *Q* lines represents an operation.

"C *a**b**c*" means adding *c* to each of *Aa*, *Aa*+1, ... , *Ab*. -10000 ≤ *c* ≤ 10000.

"Q *a**b*" means querying the sum of *Aa*, *Aa*+1, ... , *Ab*.

Output

You need to answer all *Q* commands in order. One answer in a line.

Sample Input

```
10 5
1 2 3 4 5 6 7 8 9 10
Q 4 4
Q 1 10
Q 2 4
C 3 6 3
Q 2 4
```

Sample Output

```
4
55
9
15
```

Hint

The sums may exceed the range of 32-bit integers.

Source

[POJ Monthly--2007.11.25](http://poj.org/searchproblem?field=source&key=POJ+Monthly--2007.11.25), Yang Yi

```cpp
//poj 3468
#include <iostream>
#include <cstdio>
using namespace std;
const int N = 100005;
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
 
__int64 sum[N<<2],add[N<<2];
struct Node
{
    int l,r;
    int mid()
    {
        return (l+r)>>1;
    }
} tree[N<<2];
 
void PushUp(int rt)
{
    sum[rt] = sum[rt<<1] + sum[rt<<1|1];
}
 
void PushDown(int rt,int m)
{
    if(add[rt])
    {
        add[rt<<1] += add[rt];
        add[rt<<1|1] += add[rt];
        sum[rt<<1] += add[rt] * (m - (m>>1));
        sum[rt<<1|1] += add[rt] * (m>>1);
        add[rt] = 0;
    }
}
 
void build(int l,int r,int rt)
{
    tree[rt].l = l;
    tree[rt].r = r;
    add[rt] = 0;
    if(l == r)
    {
        scanf("%I64d",&sum[rt]);
        return ;
    }
    int m = tree[rt].mid();
    build(lson);
    build(rson);
    PushUp(rt);
}
 
void update(int c,int l,int r,int rt)
{
    if(tree[rt].l == l && r == tree[rt].r)
    {
        add[rt] += c;
        sum[rt] += (__int64)c * (r-l+1);
        return;
    }
    if(tree[rt].l == tree[rt].r) return;
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    if(r <= m) update(c,l,r,rt<<1);
    else if(l > m) update(c,l,r,rt<<1|1);
    else
    {
        update(c,l,m,rt<<1);
        update(c,m+1,r,rt<<1|1);
    }
    PushUp(rt);
}
 
__int64 query(int l,int r,int rt)
{
    if(l == tree[rt].l && r == tree[rt].r)
    {
        return sum[rt];
    }
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    __int64 res = 0;
    if(r <= m) res += query(l,r,rt<<1);
    else if(l > m) res += query(l,r,rt<<1|1);
    else
    {
       res += query(l,m,rt<<1);
       res += query(m+1,r,rt<<1|1);
    }
    return res;
}
 
int main()
{
    int n,m;
    while(~scanf("%d %d",&n,&m))
    {
        build(1,n,1);
        while(m--)
        {
            char ch[2];
            scanf("%s",ch);
            int a,b,c;
            if(ch[0] == 'Q')
            {
                scanf("%d %d", &a,&b);
                printf("%I64d\n",query(a,b,1));
            }
 
            else
            {
                scanf("%d %d %d",&a,&b,&c);
                update(c,a,b,1);
            }
        }
    }
    return 0;
}
```



```cpp
#include<cstdio>
#include<cstring>
const int maxn = 1e5 + 10;
typedef long long int LL;
LL segtree[maxn << 2], lazy[maxn << 2], a[maxn];
 
void pushup(int now) {
  segtree[now] = segtree[now << 1] + segtree[(now << 1)|1];
}
 
void build_tree(int l, int r, int now) {
  if (l == r) {
    segtree[now] = a[l];
    return;
  }
  int mid = l + (r - l)/2;
  build_tree(l, mid, now << 1);
  build_tree(mid + 1, r, (now << 1)|1);
  pushup(now);
}
 
void pushdown(int now, int l, int r) {
  if (lazy[now] == 0) return;
  lazy[now << 1] += lazy[now];
  lazy[(now << 1)|1] += lazy[now];
  int mid = l + (r - l)/2;
  segtree[now << 1] += lazy[now]*(mid - l + 1);
  segtree[(now << 1)|1] += lazy[now]*(r - mid);
  lazy[now] = 0;
}
 
void update(int L, int R, int l, int r, int c, int now) {
  if (L <= l && R >= r) {
    lazy[now] += c;
    segtree[now] += c*(r - l + 1);
    return;
  }
  pushdown(now, l , r);
  int mid = l + (r - l)/2;
  if (L <= mid) update(L, R, l, mid, c, now << 1);
  if (R > mid) update(L, R, mid + 1, r, c, (now << 1)|1);
  pushup(now);
}
 
LL query(int L, int R, int l, int r, int now) {
  if (L <= l && R >= r) {
    return segtree[now];
  }
  pushdown(now, l, r);
  int mid = l + (r - l)/2;
  LL ans = 0;
  if (L <= mid) ans += query(L, R, l, mid, now << 1);
  if (R > mid) ans += query(L, R, mid + 1, r, (now << 1)|1);
  pushup(now);
  return ans;
}
 
int main()
{
  int n, m;
  while (scanf("%d %d", &n, &m) != EOF) {
    memset(segtree, 0, sizeof segtree);
    memset(lazy, 0, sizeof lazy);
 
    for (int i = 1; i <= n; i++) {
      scanf("%lld", &a[i]);
    }
 
    build_tree(1, n, 1);
 
    char s[10];
    int x, y, z;
    for (int i = 0; i < m; i++) {
      scanf("%s", s);
      if (s[0] == 'C') {
        scanf("%d %d %d", &x, &y, &z);
        update(x, y, 1, n, z, 1);
      } else {
        scanf("%d %d", &x, &y);
        printf("%lld\n", query(x, y, 1, n, 1));
      }
    }
  }
 
  return 0;
}
```






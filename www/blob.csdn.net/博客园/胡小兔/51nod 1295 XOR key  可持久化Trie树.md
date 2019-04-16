# 51nod 1295 XOR key | 可持久化Trie树 - 胡小兔 - 博客园







# [51nod 1295 XOR key | 可持久化Trie树](https://www.cnblogs.com/RabbitHu/p/51nod1295.html)





# [51nod 1295](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1295) XOR key

这也是很久以前就想做的一道板子题了……学了一点可持久化之后我终于会做这道题了！

> 
给出一个长度为N的正整数数组A，再给出Q个查询，每个查询包括3个数，L, R, X (L <= R)。求A[L] 至 A[R] 这R - L + 1个数中，与X 进行异或运算(Xor)，得到的最大值是多少？

#### Input

第1行：2个数N, Q中间用空格分隔，分别表示数组的长度及查询的数量(1 <= N <= 50000, 1 <= Q <= 50000)。

第2 - N+1行：每行1个数，对应数组A的元素(0 <= A[i] <= 10^9)。

第N+2 - N+Q+1行：每行3个数X, L, R，中间用空格分隔。（0 <= X <= 10^9，0 <= L <= R < N)

#### Output

输出共Q行，对应数组A的区间[L,R]中的数与X进行异或运算，所能得到的最大值。


可持久化Trie树，就类似于昨天我写的[可持久化线段树](http://www.cnblogs.com/RabbitHu/p/segtree.html)。

每次想要修改一个节点的时候，不直接在原来的节点上进行修改，而是在旁边新建一个点，复制原来点的信息，并在新点上面修改。每次通过访问历史根节点来访问历史版本。

配合离线询问，然后从左往右把数组中的数加入Trie树中，对于每个询问[l, r]，可以通过第r棵Trie树与第l - 1棵Trie树相减得到区间中新建出来的Trie树，在上面进行正常的异或最大查询即可。

时间、空间复杂度都是\(O(32*(n + Q))\)。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef unsigned long long ll;

template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 50005, M = 3000000;
int n, Q, idx = 1, a[N], root[N] = {1}, son[M][2], sze[M];

void insert(int i, int x){
    int now = root[i] = ++idx;
    int old = root[i - 1];
    for(int i = 31; i >= 0; i--){
        sze[now] = sze[old] + 1;
        if((x >> i) & 1) son[now][0] = son[old][0], son[now][1] = ++idx;
        else son[now][1] = son[old][1], son[now][0] = ++idx;
        now = son[now][(x >> i) & 1];
        old = son[old][(x >> i) & 1];
    }
    sze[now] = sze[old] + 1;
}
int query(int l, int r, int x){
    int old = root[l], now = root[r], ans = 0;
    for(int i = 31; i >= 0; i--){
        bool dir = (x >> i) & 1;
        int delta_sze = sze[son[now][!dir]] - sze[son[old][!dir]];
        if(delta_sze) now = son[now][!dir], old = son[old][!dir], ans = ans << 1 | 1;
        else now = son[now][dir], old = son[old][dir], ans = ans << 1;
    }
    return ans;
}

int main(){
    read(n), read(Q);
    for(int i = 1; i <= n; i++)
        read(a[i]);
    for(int i = 1; i <= n; i++)
        insert(i, a[i]);
    int l, r, x;
    while(Q--){
        read(x), read(l), read(r);
        write(query(l, r + 1, x)), enter;
    }
    return 0;
}
```

//P.S. 没看见原题中数组从0开始数，白检查半天……

//错误：数组开小了……要不下次用指针吧……？













# BZOJ 1086 王室联邦 | BFS - 胡小兔 - 博客园







# [BZOJ 1086 王室联邦 | BFS](https://www.cnblogs.com/RabbitHu/p/BZOJ1086.html)





# [BZOJ 1086](http://www.lydsy.com/JudgeOnline/problem.php?id=1086) 王室联邦

## 题意

把一棵树分块，每块大小在[B, 3B]之间（B由输入数据给出），每个块需要对应一个核心点，核心点可以在块内，这个点要满足块内每个点到核心点的路径上的点都属于这个块（核心点本身不算），请输出分块方案。

## 题解

VFK的糖果公园题解让我先来把这道题做了……

做法：直接BFS。

显然我们需要维护一个栈来存储节点编号。

对于一个子树u, 记录刚进入子树时的top，然后BFS所有子树，每当BFS完一个子树并发现新top - 原top >= B时，就把旧top以上的所有点弹出作为一个块，核心点是当前子树的根节点u。

注意最后可能有一块与根节点相连的部分没有归到任何块里，把它归到最后一个块中。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
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
#define space putchar(' ')
#define enter putchar('\n')

const int N = 1005;
int n, B, stk[N], top, bel[N], cap[N], idx;
int ecnt, adj[N], nxt[2*N], go[2*N];

void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void dfs(int u, int pre){
    int st = top;
    for(int e = adj[u], v; e; e = nxt[e])
    if(v = go[e], v != pre){
        dfs(v, u);
        if(top - st >= B){
        cap[++idx] = u;
        while(top > st) bel[stk[top--]] = idx;
        }
    }
    stk[++top] = u;
}

int main(){

    read(n), read(B);
    for(int i = 1, u, v; i < n; i++)
    read(u), read(v), add(u, v), add(v, u);
    dfs(1, 0);
    while(top) bel[stk[top--]] = idx;
    write(idx), enter;
    for(int i = 1; i <= n; i++)
    write(bel[i]), i == n ? enter : space;
    for(int i = 1; i <= idx; i++)
    write(cap[i]), i == idx ? enter : space;

    return 0;
}
```













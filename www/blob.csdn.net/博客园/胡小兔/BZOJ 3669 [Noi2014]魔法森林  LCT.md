# BZOJ 3669 [Noi2014]魔法森林 | LCT - 胡小兔 - 博客园







# [BZOJ 3669 [Noi2014]魔法森林 | LCT](https://www.cnblogs.com/RabbitHu/p/BZOJ3669.html)





迷啊……调了一下午……

为什么我把splay时的update方式改成“splay最后再upt(u)一下”就不RE了……

求大神解答……？

## 题目链接

[BZOJ 3669](http://www.lydsy.com/JudgeOnline/problem.php?id=3669)

## 题解

将边按照a排序，然后从小到大枚举，加入图中。

在图中用lct维护一棵两点之间b最大值尽量小的生成树。

当加入一条边(u, v)时：

如果(u, v)不连通，则直接加入这条边

如果(u, v)连通且它们间路径上的b最大值大于当前边的b，则删除路径上b最大的边，然后加入当前边。

否则这条边没用，啥也不干。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;
typedef unsigned long long ll;
#define enter putchar('\n')
#define space putchar(' ')
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c > '9' || c < '0')
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
const int N = 150005, M = 100005, INF = 0x3f3f3f3f;
int n, m, pcnt, ans = INF;
int fa[N], ls[N], rs[N], val[N], num[N]; //val和num存的都是边的编号
bool rev[N];
struct Edge {
    int u, v, a, b, p;
    bool operator < (const Edge &obj) const{
        return a == obj.a ? b < obj.b : a < obj.a;
    }
} e[M];
#define which(u) (ls[fa[u]] == (u))
#define isroot(u) (!fa[u] || (ls[fa[u]] != (u) && rs[fa[u]] != (u)))
int Max(int A, int B){
    return e[A].b > e[B].b ? A : B;
}
void upt(int u){
    val[u] = Max(Max(val[ls[u]], val[rs[u]]), num[u]);
}
void pushdown(int u){
    if(!rev[u]) return;
    swap(ls[u], rs[u]);
    if(ls[u]) rev[ls[u]] ^= 1;
    if(rs[u]) rev[rs[u]] ^= 1;
    rev[u] = 0;
}
void rotate(int u){
    int v = fa[u], w = fa[v], b = which(u) ? rs[u] : ls[u];
    if(!isroot(v)) (which(v) ? ls[w] : rs[w]) = u;
    which(u) ? (ls[v] = b, rs[u] = v) : (rs[v] = b, ls[u] = v);
    fa[u] = w, fa[v] = u;
    if(b) fa[b] = v;
    upt(v);
}
void splay(int u){
    static int stk[N], top;
    stk[top = 1] = u;
    while(!isroot(stk[top])) stk[top + 1] = fa[stk[top]], top++;
    while(top) pushdown(stk[top--]);
    while(!isroot(u)){
        if(!isroot(fa[u])){
            if(which(u) == which(fa[u])) rotate(fa[u]);
            else rotate(u);
        }
        rotate(u);
    }
    upt(u);
}
void access(int u){
    int v = 0;
    while(u){
        splay(u);
        rs[u] = v;
        v = u;
        u = fa[u];
    }
}
void makeroot(int u){
    access(u);
    splay(u);
    rev[u] ^= 1;
}
int findroot(int u){
    access(u);
    splay(u);
    while(pushdown(u), ls[u]) u = ls[u];
    splay(u);
    return u;
}
void link(int u, int v){
    makeroot(u);
    fa[u] = v;
}
void cut(int u, int v){
    makeroot(u);
    access(v);
    splay(v);
    ls[v] = fa[u] = 0;
    upt(v);
}
void insert_edge(int i){
    e[i].p = ++pcnt;
    num[e[i].p] = val[e[i].p] = i;
    link(e[i].u, e[i].p);
    link(e[i].v, e[i].p);
}
void erase_edge(int i){
    cut(e[i].u, e[i].p);
    cut(e[i].v, e[i].p);
}
int query(int u, int v){
    makeroot(u);
    access(v);
    splay(v);
    return val[v];
}

int main(){
    read(n), read(m), pcnt = n;
    for(int i = 1; i <= m; i++)
        read(e[i].u), read(e[i].v), read(e[i].a), read(e[i].b);
    sort(e + 1, e + m + 1);
    for(int i = 1; i <= m; i++){
        if(findroot(e[i].u) != findroot(e[i].v)) insert_edge(i);
        else{
            int maxe = query(e[i].u, e[i].v);
            if(e[maxe].b > e[i].b)
                erase_edge(maxe), insert_edge(i);
        }
        if(findroot(1) == findroot(n))
            ans = min(ans, e[i].a + e[query(1, n)].b);
    }
    write(ans < INF ? ans : -1), enter;
    return 0;
}
```













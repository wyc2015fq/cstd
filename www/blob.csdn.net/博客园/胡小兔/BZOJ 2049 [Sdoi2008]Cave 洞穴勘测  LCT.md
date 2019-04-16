# BZOJ 2049 [Sdoi2008]Cave 洞穴勘测 | LCT - 胡小兔 - 博客园







# [BZOJ 2049 [Sdoi2008]Cave 洞穴勘测 | LCT](https://www.cnblogs.com/RabbitHu/p/BZOJ2049.html)





今天我遇到了一起灵异事件。

我觉得我没写过无根树lct。

于是我学习了一发。

写了这道例题。

写完觉得不是很稳。

于是决定上洛谷试着提交一发。

然后发现——

“这道题我**怎么A过？！”

好吧。

今天做了一道BZOJ收录的题，可是我的BZOJ做题量并没有增加……

话说我之前做了这道题怎么没写博客啊……

那个人真的是我嘛……

## 传送门

[BZOJ 2049](http://www.lydsy.com/JudgeOnline/problem.php?id=2049)

## 题解

这是一道LCT棵题啦。

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

const int N = 10005;
int n, m;
int fa[N], ls[N], rs[N];
bool rev[N];
#define which(u) (ls[fa[u]] == (u))
#define isroot(u) (!fa[u] || (ls[fa[u]] != (u) && rs[fa[u]] != (u)))

void rotate(int u){
    int v = fa[u], w = fa[v], b = which(u) ? rs[u] : ls[u];
    if(!isroot(v)) (which(v) ? ls[w] : rs[w]) = u;
    which(u) ? (ls[v] = b, rs[u] = v) : (rs[v] = b, ls[u] = v);
    fa[u] = w, fa[v] = u;
    if(b) fa[b] = v;
}
void pushdown(int u){
    if(!rev[u]) return;
    rev[ls[u]] ^= 1;
    rev[rs[u]] ^= 1;
    swap(ls[u], rs[u]);
    rev[u] = 0;
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
    makeroot(v);
    fa[v] = u;
}
void cut(int u, int v){
    makeroot(u);
    access(v);
    splay(v);
    ls[v] = fa[u] = 0;
}

int main(){

    read(n), read(m);
    char op[10];
    int u, v;
    while(m--){
        scanf("%s%d%d", op, &u, &v);
        if(op[0] == 'Q') puts(findroot(u) == findroot(v) ? "Yes" : "No");
        else if(op[0] == 'C') link(u, v);
        else cut(u, v);
    }

    return 0;
}
```













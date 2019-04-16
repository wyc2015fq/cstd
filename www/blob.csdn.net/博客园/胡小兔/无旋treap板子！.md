# 无旋treap板子！ - 胡小兔 - 博客园







# [无旋treap板子！](https://www.cnblogs.com/RabbitHu/p/treap.html)





```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#define space putchar(' ')
#define enter putchar('\n')
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

const int N = 100005, INF = 0x3f3f3f3f;
typedef pair<int, int> par;
#define mp make_pair
int n, tot, root;
int ls[N], rs[N], data[N], key[N], sze[N];

void upt(int u){
    sze[u] = sze[ls[u]] + sze[rs[u]] + 1;
}
par split(int u, int k){
    if(!k) return mp(0, u);
    int l = ls[u], r = rs[u];
    if(k == sze[l]) return ls[u] = 0, upt(u), mp(l, u);
    if(k == sze[l] + 1) return rs[u] = 0, upt(u), mp(u, r);
    if(k < sze[l]){
    par tmp = split(l, k);
    return ls[u] = tmp.second, upt(u), mp(tmp.first, u);
    }
    par tmp = split(r, k - sze[ls[u]] - 1);
    return rs[u] = tmp.first, upt(u), mp(u, tmp.second);
}
int merge(int u, int v){
    if(u == 0 || v == 0) return u + v;
    if(key[u] < key[v]) return rs[u] = merge(rs[u], v), upt(u), u;
    else return ls[v] = merge(u, ls[v]), upt(v), v;
}
int getrank(int u, int x){
    int cnt = 0, ans = INF;
    while(u){
    if(x == data[u]) ans = min(ans, cnt + sze[ls[u]] + 1);
    if(x > data[u]) cnt += sze[ls[u]] + 1, u = rs[u];
    else u = ls[u];
    }
    return ans == INF ? cnt : ans;
}
int getkth(int u, int k){
    while(1){
    if(k == sze[ls[u]] + 1) return data[u];
    if(k <= sze[ls[u]]) u = ls[u];
    else k -= sze[ls[u]] + 1, u = rs[u]; //先修改k再移动u！我居然又写错了
    }
}
int getpre(int u, int x){
    int ans = -INF;
    while(u){
    if(data[u] < x) ans = max(ans, data[u]), u = rs[u];
    else u = ls[u];
    }
    return ans;
}
int getnxt(int u, int x){
    int ans = INF;
    while(u){
    if(data[u] > x) ans = min(ans, data[u]), u = ls[u];
    else u = rs[u];
    }
    return ans;
}
void insert(int x){
    data[++tot] = x, key[tot] = rand(), sze[tot] = 1;
    int k = getrank(root, x);
    par tmp = split(root, k);
    root = merge(tmp.first, tot);
    root = merge(root, tmp.second);
}
void erase(int x){
    int k = getrank(root, x);
    par tmp1 = split(root, k);
    par tmp2 = split(tmp1.first, k - 1);
    root = merge(tmp2.first, tmp1.second);
}

int main(){
    srand(20010424);
    read(n);
    int op, x;
    while(n--){
    read(op), read(x);
    if(op == 1) insert(x);
    else if(op == 2) erase(x);
    else if(op == 3) write(getrank(root, x)), enter;
    else if(op == 4) write(getkth(root, x)), enter;
    else if(op == 5) write(getpre(root, x)), enter;
    else write(getnxt(root, x)), enter;
    }
    return 0;
}
```













# BZOJ 2648 / 2716 K-D Tree 模板题 - 胡小兔 - 博客园







# [BZOJ 2648 / 2716 K-D Tree 模板题](https://www.cnblogs.com/RabbitHu/p/BZOJ2648.html)





```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;
typedef long long ll;
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

const int N = 1000005, D = 2, INF = 0x3f3f3f3f;
int n, m, tot, curD, root, ans;
int co[N][D], mi[N][D], ma[N][D], ls[N], rs[N];
struct point {
    int co[D];
    bool operator < (const point &b) const {
        return co[curD] < b.co[curD];
    }
} a[N];
void update(int fa, int son) {
    for(int i = 0; i < D; i++) {
        mi[fa][i] = min(mi[fa][i], mi[son][i]);
        ma[fa][i] = max(ma[fa][i], ma[son][i]);
    }
}
int build(int l, int r, int d) {
    curD = d;
    int mid = (l + r) >> 1;
    nth_element(a + l, a + mid, a + r + 1);
    for(int i = 0; i < D; i++)
        co[mid][i] = mi[mid][i] = ma[mid][i] = a[mid].co[i];
    if(mid > l) ls[mid] = build(l, mid - 1, (d + 1) % D), update(mid, ls[mid]);
    if(mid < r) rs[mid] = build(mid + 1, r, (d + 1) % D), update(mid, rs[mid]);
    return mid;
}
void insert(int k, int x, int d){
    if(co[x][d] <= co[k][d]){
        if(!ls[k]) ls[k] = x;
        else insert(ls[k], x, (d + 1) % D);
        update(k, ls[k]);
    }
    else{
        if(!rs[k]) rs[k] = x;
        else insert(rs[k], x, (d + 1) % D);
        update(k, rs[k]);
    }
}
int mindis(int k, const point &a) {
    int ret = 0;
    for(int i = 0; i < D; i++)
        ret += max(mi[k][i] - a.co[i], 0) + max(a.co[i] - ma[k][i], 0);
    return ret;
}
int getdis(int k, const point &a) {
    int ret = 0;
    for(int i = 0; i < D; i++)
        ret += abs(co[k][i] - a.co[i]);
    return ret;
}
void query(int k, const point &a) {
    ans = min(ans, getdis(k, a));
    int ld = INF, rd = INF;
    if(ls[k]) ld = mindis(ls[k], a);
    if(rs[k]) rd = mindis(rs[k], a);
    if(ld <= rd){
        if(ld <= ans) query(ls[k], a);
        if(rd <= ans) query(rs[k], a);
    }
    else{
        if(rd <= ans) query(rs[k], a);
        if(ld <= ans) query(ls[k], a);
    }
}
int main() {
    read(n), read(m), tot = n;
    for(int i = 1; i <= n; i++)
        for(int j = 0; j < D; j++)
            read(a[i].co[j]);
    root = build(1, n, 0);
    while(m--){
        int op;
        read(op), read(a[++tot].co[0]), read(a[tot].co[1]);
        if(op == 1) {
            for(int i = 0; i < D; i++)
                co[tot][i] = mi[tot][i] = ma[tot][i] = a[tot].co[i];
            insert(root, tot, 0);
        }
        else {
            ans = INF;
            query(root, a[tot]);
            write(ans), enter;
        }
    }
    return 0;
}
```













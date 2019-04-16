# Luogu 2801 教主的魔法 | 分块模板题 - 胡小兔 - 博客园







# [Luogu 2801 教主的魔法 | 分块模板题](https://www.cnblogs.com/RabbitHu/p/Luogu2801.html)





# [Luogu 2801](https://www.luogu.org/problemnew/show/P2801) 教主的魔法 | 分块模板题

我犯的错误：
- 有一处l打成了1，还看不出来……
- 缩小块大小De完bug后忘了把块大小改回去就提交……还以为自己一定能A了……

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;
typedef long long ll;
#define space putchar(' ')
#define enter putchar('\n')
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
    if(x < 0) x = -x, putchar('-');
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 1000005, B = 1000;
int n, m;
int a[N], s[N], lazy[N];
#define st(x) (((x) - 1) * B + 1)
#define ed(x) min((x) * B, n)
#define bel(x) (((x) - 1) / B + 1)
bool get_op(){
    char c;
    while(c = getchar(), c != 'A' && c != 'M');
    return c == 'M';
}
void pushdown(int b){
    if(!lazy[b]) return;
    for(int i = st(b); i <= ed(b); i++)
    s[i] += lazy[b], a[i] += lazy[b];
    lazy[b] = 0;
}
void single_change(int l, int r, int w){
    int b = bel(l);
    pushdown(b);
    for(int i = l; i <= r; i++) a[i] += w;
    for(int i = st(b); i <= ed(b); i++) s[i] = a[i];
    sort(s + st(b), s + ed(b) + 1);
}
void change(int l, int r, int w){
    if(bel(l) == bel(r))
    return single_change(l, r, w);
    for(int b = bel(l) + 1; b < bel(r); b++)
    lazy[b] += w;
    single_change(l, ed(bel(l)), w), single_change(st(bel(r)), r, w);
}
int block_query(int b, int w){
    return s + ed(b) - upper_bound(s + st(b), s + ed(b) + 1, w - lazy[b] - 1) + 1;
}
int single_query(int l, int r, int w){
    int b = bel(l), ret = 0;
    for(int i = l; i <= r; i++)
    if(a[i] + lazy[b] >= w) ret++;
    return ret;
}
int query(int l, int r, int w){
    if(bel(l) == bel(r))
    return single_query(l, r, w);
    int ret = 0;
    for(int i = bel(l) + 1; i < bel(r); i++)
    ret += block_query(i, w);
    return ret + single_query(l, ed(bel(l)), w) + single_query(st(bel(r)), r, w);
}
int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++) read(a[i]), s[i] = a[i];
    for(int i = 1; st(i) <= n; i++) sort(s + st(i), s + ed(i) + 1);
    while(m--){
    int op, l, r, w;
    op = get_op(), read(l), read(r), read(w);
    if(op) change(l, r, w);
    else write(query(l, r, w)), enter;
    }
    return 0;
}
```













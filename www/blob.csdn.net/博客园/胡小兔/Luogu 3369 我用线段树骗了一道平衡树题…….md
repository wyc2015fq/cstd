# Luogu 3369 我用线段树骗了一道平衡树题…… - 胡小兔 - 博客园







# [Luogu 3369 我用线段树骗了一道平衡树题……](https://www.cnblogs.com/RabbitHu/p/Luogu3369.html)





这篇博客毫无意义……

只是表达一下我仍然会写树状数组和线段树……

[题目链接](https://www.luogu.org/problemnew/show/3369)

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
bool read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
        else if(c == EOF) return 0;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
    return 1;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 100005;
int n;
int lst[N], idx;
int tp[N], val[N];
int data[4*N], sum[N];

int getnum(int x){
    return lower_bound(lst + 1, lst + idx + 1, x) - lst;
}
void add(int p, int x){
    while(p <= idx) sum[p] += x, p += p & -p;
}
int ask(int p){
    int ret = 0;
    while(p) ret += sum[p], p -= p & -p;
    return ret;
}
void change(int k, int l, int r, int p, int x){
    if(l == r){
        data[k] += x;
        add(p, x);
        return;
    }
    int mid = (l + r) >> 1;
    if(p <= mid) change(k << 1, l, mid, p, x);
    else change(k << 1 | 1, mid + 1, r, p, x);
    data[k] = data[k << 1] + data[k << 1 | 1];
}
int query(int k, int l, int r, int x){
    if(l == r) return l;
    int mid = (l + r) >> 1;
    if(x <= data[k << 1]) return query(k << 1, l, mid, x);
    else return query(k << 1 | 1, mid + 1, r, x - data[k << 1]);
}

int main(){
    read(n);
    for(int i = 1; i <= n; i++){
        read(tp[i]), read(val[i]);
        if(tp[i] != 4) lst[++idx] = val[i];
    }
    sort(lst + 1, lst + idx + 1);
    idx = unique(lst + 1, lst + idx + 1) - lst - 1;
    for(int i = 1; i <= n; i++){
        if(tp[i] != 4) val[i] = getnum(val[i]);
        if(tp[i] == 1) change(1, 1, idx, val[i], 1);
        else if(tp[i] == 2) change(1, 1, idx, val[i], -1);
        else if(tp[i] == 3) write(ask(val[i] - 1) + 1), enter;
        else if(tp[i] == 4) write(lst[query(1, 1, idx, val[i])]), enter;
        else if(tp[i] == 5) write(lst[query(1, 1, idx, ask(val[i] - 1))]), enter;
        else write(lst[query(1, 1, idx, ask(val[i]) + 1)]), enter;
    }
    return 0;
}
```













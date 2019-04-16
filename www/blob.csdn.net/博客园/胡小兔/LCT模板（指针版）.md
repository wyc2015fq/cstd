# LCT模板（指针版） - 胡小兔 - 博客园







# [LCT模板（指针版）](https://www.cnblogs.com/RabbitHu/p/9078053.html)





本来是想做THUWC2017的泰勒展开xLCT题的……

然后觉得数组写很麻烦……

然后就决定挑战指针版……

然后写得全是BUG……

与BUG鏖战三千年后，有了这个指针版LCT板子！

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
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

typedef long double ldb;
const int N = 300005;
int n, m;
struct node {
    node *fa, *ch[2];
    int val, sum;
    bool rev;
    node(){
    fa = ch[0] = ch[1] = NULL;
    val = sum = 0;
    rev = 0;
    }
    bool which(){
    return fa->ch[1] == this;
    }
    bool isroot(){
    return fa == NULL || (fa->ch[0] != this && fa->ch[1] != this);
    }
    void upt(){
    sum = val;
    if(ch[0] != NULL) sum ^= ch[0]->sum;
    if(ch[1] != NULL) sum ^= ch[1]->sum;
    }
    void pushdown(){
    if(!rev) return;
    swap(ch[0], ch[1]);
    if(ch[0] != NULL) ch[0]->rev ^= 1;
    if(ch[1] != NULL) ch[1]->rev ^= 1;
    rev = 0;
    }
} s[N];

void rotate(node *u){
    node *v = u->fa, *w = v->fa, *b = u->ch[!u->which()];
    if(!v->isroot()) w->ch[v->which()] = u;
    u->which() ? (u->ch[0] = v, v->ch[1] = b) : (u->ch[1] = v, v->ch[0] = b);
    u->fa = w, v->fa = u;
    if(b != NULL) b->fa = v;
    v->upt();
}
void splay(node *u){
    static node *stk[N];
    int top;
    stk[top = 1] = u;
    while(!stk[top]->isroot()) stk[top + 1] = stk[top]->fa, top++;
    while(top) stk[top--]->pushdown();
    while(!u->isroot()){
    if(!u->fa->isroot()){
        if(u->which() == u->fa->which()) rotate(u->fa);
        else rotate(u);
    }
    rotate(u);
    }
    u->upt();
}
void access(node *u){
    node *v = NULL;
    while(u != NULL){
    splay(u);
    u->ch[1] = v;
    u->upt();
    v = u;
    u = u->fa;
    }
}
void makeroot(node *u){
    access(u);
    splay(u);
    u->rev ^= 1;
}
node *findroot(node *u){
    access(u);
    splay(u);
    while(u->pushdown(), u->ch[0] != NULL)
    u = u->ch[0];
    splay(u);
    return u;
}
void link(node *u, node *v){
    if(findroot(u) == findroot(v)) return;
    makeroot(v);
    v->fa = u;
}
void cut(node *u, node *v){
    makeroot(u);
    access(v);
    splay(v);
    if(v->ch[0] == u)
    v->ch[0] = u->fa = NULL;
}
int query(node *u, node *v){
    makeroot(u);
    access(v);
    splay(v);
    return v->sum;
}
void change(node *u, int x){
    splay(u);
    u->val = x;
    u->upt();
}

int main(){

    read(n), read(m);
    for(int i = 1; i <= n; i++)
    read(s[i].val), s[i].upt();
    int op, x, y;
    while(m--){
    read(op), read(x), read(y);
    if(op == 0) write(query(s + x, s + y)), enter;
    else if(op == 1) link(s + x, s + y);
    else if(op == 2) cut(s + x, s + y);
    else change(s + x, y);
    }

    return 0;
}
```













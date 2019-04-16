# BZOJ 2434 阿狸的打字机 | AC自动机 - 胡小兔 - 博客园







# [BZOJ 2434 阿狸的打字机 | AC自动机](https://www.cnblogs.com/RabbitHu/p/BZOJ2434.html)





[题目戳这里](http://www.lydsy.com/JudgeOnline/problem.php?id=2434)

AC自动机上有神奇的东西叫做fail指针……所有fail指针连起来恰好构成一棵以1为根的树！

而这道题问x在y中出现过多少次，就是问Trie树上根到y的结束节点的路径上有多少节点能通过跳fail跳到x的结束节点上，即，路径上有多少节点在fail树上在x的结束节点的子树中。

那么怎么统计答案呢？可以把询问离线，把fail树DFS一遍得到DFS序列，再把Trie树DFS一遍，DFS到它（入栈）的时候在DFS序列中对应位置+1，离开（出栈）的时候在DFS序列中对应位置-1；中间回答所有当前节点作为y的询问，方法是求对应x的子树和，即序列上的区间和。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
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

const int N = 100005;
char s[N];
int n, m, tot = 1, son[N][26], real_son[N][26], fail[N], ans[N];
int st[N], ed[N], idx, adj[N], nxt[N], nod[N], sum[N];
vector <int> num[N];
struct Query {int id, x;};
vector <Query> q[N];

void add(int u, int v){
    nxt[v] = adj[u], adj[u] = v;
}
void build(){
    static int stk[N], que[N];
    int now = 1, len = strlen(s + 1), top = 0, qr;
    stk[0] = 1;
    for(int i = 1; i <= len; i++)
    if(s[i] == 'P') num[now].push_back(++n), nod[n] = now;
    else if(s[i] == 'B') now = stk[top = max(0, top - 1)];
    else{
        int c = s[i] - 'a';
        if(!son[now][c]) son[now][c] = ++tot;
        now = son[now][c];
        stk[++top] = now;
    }
    for(int i = 1; i <= tot; i++)
    for(int j = 0; j < 26; j++)
        real_son[i][j] = son[i][j];
    for(int i = 0; i < 26; i++) son[0][i] = 1;
    que[qr = 1] = 1;
    for(int ql = 1; ql <= qr; ql++)
    for(int u = que[ql], i = 0; i < 26; i++)
        if(!son[u][i]) son[u][i] = son[fail[u]][i];
        else{
        add(fail[son[u][i]] = son[fail[u]][i], son[u][i]);
        que[++qr] = son[u][i];
        }
}
void dfs_fail(int u){
    st[u] = ++idx;
    for(int v = adj[u]; v; v = nxt[v])
    dfs_fail(v);
    ed[u] = idx;
}
void change(int p, int x){
    while(p <= idx) sum[p] += x, p += p & -p;
}
int ask(int p){
    int ret = 0;
    while(p) ret += sum[p], p -= p & -p;
    return ret;
}
void dfs_trie(int u){
    change(st[u], 1);
    for(vector<int>::iterator i = num[u].begin(); i != num[u].end(); i++)
    for(vector<Query>::iterator j = q[*i].begin(); j != q[*i].end(); j++)
        ans[j -> id] = ask(ed[nod[j -> x]]) - ask(st[nod[j -> x]] - 1);
    for(int i = 0; i < 26; i++)
    if(real_son[u][i])
        dfs_trie(son[u][i]);
    change(st[u], -1);
}

int main(){

    scanf("%s", s + 1);
    read(m);
    for(int i = 1, y; i <= m; i++){
    Query tmp;
    read(tmp.x), read(y), tmp.id = i;
    q[y].push_back(tmp);
    }
    build();
    dfs_fail(1);
    dfs_trie(1);
    for(int i = 1; i <= m; i++)
    write(ans[i]), enter;

    return 0;
}
```













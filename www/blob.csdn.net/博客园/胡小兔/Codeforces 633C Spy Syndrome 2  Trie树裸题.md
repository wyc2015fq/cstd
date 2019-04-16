# Codeforces 633C Spy Syndrome 2 | Trie树裸题 - 胡小兔 - 博客园







# [Codeforces 633C Spy Syndrome 2 | Trie树裸题](https://www.cnblogs.com/RabbitHu/p/CF633C.html)





# [Codeforces 633C](http://codeforces.com/problemset/problem/633/C) Spy Syndrome 2 | Trie树裸题

一个由许多空格隔开的单词组成的字符串，进行了以下操作：把所有字符变成小写，把每个单词颠倒过来，然后去掉单词间的空格。已知操作后的字符串以及可能出现的所有单词，请输出原字符串（多组解只输出一个）。

我犯的错误：数组的trie树……如果根节点是1……则其余节点要从2开始啊啊啊

```
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x3f3f3f3f
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

const int N = 1001000;
int n, m, dp[N], pos[N], len[N], cnt = 1, stk[N], top;
char a[N], s[N];
struct node {
    int son[26], id;
    node(){
        id = 0;
        memset(son, 0, sizeof(son));
    }
} trie[N];
char low(char c){
    return c >= 'a' ? c : c - 'A' + 'a';
}
void insert(int id, char *s){
    int now = 1;
    len[id] = strlen(s);
    for(int i = 0; i < len[id]; i++){
        int c = low(s[i]) - 'a';
        if(!trie[now].son[c]) trie[now].son[c] = ++cnt;
        now = trie[now].son[c];
    }
    trie[now].id = id;
}
int main(){
    read(n);
    scanf("%s", a + 1);
    read(m);
    for(int i = 1; i <= m; i++){
        pos[i] = pos[i - 1] + len[i - 1];
        scanf("%s", s + pos[i]);
        insert(i, s + pos[i]);
    }
    for(int i = 1; i <= n; i++){
        dp[i] = -1;
        int now = 1;
        for(int j = 0; i - j > 0 && now; j++){
            now = trie[now].son[a[i - j] - 'a'];
            if(trie[now].id && dp[i - j - 1] != -1){
                dp[i] = trie[now].id;
                break;
            }
        }
    }
    for(int i = n; i > 0; i -= len[dp[i]])
        stk[++top] = dp[i];
    for(int i = top; i; i--){
        for(int j = 0; j < len[stk[i]]; j++)
            putchar(s[pos[stk[i]] + j]);
        putchar(' ');
    }
    enter;
    return 0;
}
```













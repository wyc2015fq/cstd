# 51nod 1595 回文度 | 马拉车Manacher DP - 胡小兔 - 博客园







# [51nod 1595 回文度 | 马拉车Manacher DP](https://www.cnblogs.com/RabbitHu/p/51nod1595.html)





# [51nod 1595](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1595) 回文度

## 题目描述

> 
如果长度为n的字符串是一个回文串，同时它的长度为floor(n/2)的前缀和后缀是K-1度回文串，则这个字符串被称为K度回文串。根据定义，任何字符串（即使是空字符串）都是0度回文串。

对于一个K度回文串，字符串的回文度就是这个K的最大值，例如，“abaaba”的度为3。

你将得到一个字符串，你的任务是得到它所有前缀的回文度的和。

Input

输入数据的第一行是一个非空字符串，包含拉丁字母和数字。字符串的长度不超过5*10^6 。

Output

输出唯一的数字，即字符串的所有前缀的回文度的和。


## 题解

先用马拉车求一遍哪些前缀是回文串，再DP求出它们的度即可。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
#define INF 0x3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
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

const int N = 5000005;
char a[N], s[2 * N];
int n, deg[N], R[2 * N];
ll ans;

void manacher(){
    s[0] = '@', s[2 * n + 2] = '$', s[2 * n + 1] = '#';
    for(int i = 1; i <= n; i++)
        s[2 * i] = a[i], s[2 * i - 1] = '#';
    int mx = 0, p = 0;
    for(int i = 1; i <= 2 * n; i++){
        if(mx < i) R[i] = 1;
        else R[i] = min(R[2 * p - i], mx - i);
        while(s[i + R[i]] == s[i - R[i]]) R[i]++;
        if(i + R[i] > mx) mx = i + R[i], p = i;
    }
}

int main(){
    gets(a + 1);
    n = strlen(a + 1);
    manacher();
    for(int i = 2; i <= n + 1; i++)
        if(R[i] == i) deg[i - 1] = 1;
    for(int i = 1; i <= n; i++)
        if(deg[i])
            deg[i] = deg[i / 2] + 1, ans = (ll) ans + deg[i];
    write(ans), enter;
    return 0;
}
```













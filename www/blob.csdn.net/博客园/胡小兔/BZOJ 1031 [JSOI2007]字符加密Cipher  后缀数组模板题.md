# BZOJ 1031 [JSOI2007]字符加密Cipher | 后缀数组模板题 - 胡小兔 - 博客园







# [BZOJ 1031 [JSOI2007]字符加密Cipher | 后缀数组模板题](https://www.cnblogs.com/RabbitHu/p/BZOJ1031.html)





# [BZOJ 1031](http://www.lydsy.com/JudgeOnline/problem.php?id=1031) [JSOI2007]字符加密Cipher | 后缀数组模板题

将字符串复制一遍接在原串后面，然后后缀排序即可。

```
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
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

const int N = 400005;
int n, buf1[N], buf2[N], sa[N], buc[N], rnk[N];
char s[N];

void suffix_sort(){
    int *x = buf1, *y = buf2, m = 127;
    for(int i = 0; i <= m; i++) buc[i] = 0;
    for(int i = 1; i <= n; i++) buc[x[i] = s[i]]++;
    for(int i = 1; i <= m; i++) buc[i] += buc[i - 1];
    for(int i = n; i; i--) sa[buc[x[i]]--] = i;
    for(int k = 1, p = 0; k <= n; k <<= 1, m = p, p = 0){
        for(int i = n - k + 1; i <= n; i++) y[++p] = i;
        for(int i = 1; i <= n; i++) if(sa[i] > k) y[++p] = sa[i] - k;
        for(int i = 0; i <= m; i++) buc[i] = 0;
        for(int i = 1; i <= n; i++) buc[x[y[i]]]++;
        for(int i = 1; i <= m; i++) buc[i] += buc[i - 1];
        for(int i = n; i; i--) sa[buc[x[y[i]]]--] = y[i];
        swap(x, y), x[sa[1]] = p = 1;
        for(int i = 2; i <= n; i++)
            if(y[sa[i]] == y[sa[i - 1]] && y[sa[i] + k] == y[sa[i - 1] + k]) x[sa[i]] = p;
            else x[sa[i]] = ++p;
        if(p >= n) break;
    }
    for(int i = 1; i <= n; i++) rnk[sa[i]] = i;
}

int main(){

    scanf("%s", s + 1), n = strlen(s + 1);
    for(int i = 1; i <= n; i++) s[n + i] = s[i];
    n *= 2;
    suffix_sort();
    n /= 2;
    for(int i = 1; i <= 2 * n; i++)
        if(sa[i] <= n) putchar(s[sa[i] + n - 1]);
    enter;
    
    return 0;
}
```













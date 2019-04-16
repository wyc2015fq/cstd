# Luogu 1903 数颜色 | 分块 - 胡小兔 - 博客园







# [Luogu 1903 数颜色 | 分块](https://www.cnblogs.com/RabbitHu/p/Luogu1903.html)





# [Luogu 1903](https://www.luogu.org/problemnew/show/1903) 数颜色 | 分块

莫队不会啊……

这道题直接分块也能卡过！

这道题的做法很有趣：对于每个位置i，记录它的颜色a[i]上一次出现的位置，记为pre[i]。

这样在查询一个区间[l, r]的时候，对于每个区间内的元素，如果pre[i] < l则这个颜色是第一次出现，ans++。

可以分块后把每一块内部的pre[i]都排好序，这样只要二分查找lower_bound(l)就可以知道块内有多少pre[i] < l的元素。

剩下不完整的块只需单独处理。

问题是修改的时候没法修改了，只能暴力把整个数组重构一遍，好在题目中有修改操作较少这条约束，可以卡过。

分块时一定要注意最后一块不完整，需要特判……

跑得贼慢的代码↓

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <set>
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
#define blk(x) ((x - 1) / B + 1)
#define st(x) ((x - 1) * B + 1)
#define ed(x) (x == blk(n) ? n : (x * B + 1))
const int N = 10005, M = 1000005, B = 100;
int n, m, a[N], pre[N], spre[N], lst[M], x, y;
char op;
int query(int x, int y){
    int res = 0;
    if(blk(x) == blk(y)){
        for(int i = x; i <= y; i++)
            if(pre[i] < x) res++;
        return res;
    }
    for(int i = x; i == x || i % B != 1; i++)
        if(pre[i] < x) res++;
    for(int i = y; i == y || i % B != 0; i--)
        if(pre[i] < x) res++;
    for(int i = blk(x) + 1; i < blk(y); i++)
        res += lower_bound(spre + st(i), spre + ed(i), x) - (spre + st(i));
    return res;
}
void build(){
    memset(lst, 0, sizeof(lst));
    for(int i = 1; i <= n; i++)
        spre[i] = pre[i] = lst[a[i]], lst[a[i]] = i;
    for(int i = 1; i <= blk(n); i++)
        sort(spre + st(i), spre + ed(i));
}
int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++)
        read(a[i]);
    build();
    while(m--){
        while(op = getchar(), op != 'Q' && op != 'R');
        read(x), read(y);
        if(op == 'Q') write(query(x, y)), enter;
        else a[x] = y, build();
    }
    return 0;
}
```













# 51nod 1376 最长上升子序列的数量 | DP | vector怒刷存在感！ - 胡小兔 - 博客园







# [51nod 1376 最长上升子序列的数量 | DP | vector怒刷存在感！](https://www.cnblogs.com/RabbitHu/p/51nod1376.html)





# [51nod 1376](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1376) 最长上升子序列的数量

## 题解

我们设lis[i]为以位置i结尾的最长上升子序列长度，dp[i]为以位置i结尾的最长上升子序列数量。

显然，dp[i]要从前面的一些位置（设为位置j）的dp转移过来。

j要满足下面的条件：
- j < i
- a[j] < a[i]
- lis[j] = lis[i] - 1

dp[i]即为所有满足上述条件的dp[j]之和。

如果我们正常从左到右处理序列，第一条显然可以直接满足（因为大于i的位置还都没处理过）。

为了满足第三条，我们可以把lis相同的值放在一起，用vector存起来。我们设lst[i]为所有满足”lis[j] = i“的j所对应的a[j]构成的vector。

这堆vector有个性质——它是单调不减的。

为什么呢？因为这个vector中，前面的元素是序列中先出现的，后面的是序列中后出现的，而它们同在一个vector中，满足lis相同。假如后面的元素反而比前面的大，那么显然以后面的元素结尾的最长上升子序列可以从前面的元素转移过来，后面元素的lis必然比前面的要大，与它们lis相同矛盾。

那么为了满足上面的第二条条件，我们直接在vector中二分，即可求得所有满足条件的a[j]的和了，也就求出了dp[i]。

总复杂度是\(O(n \log n)\)的。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
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

const int N = 50005, P = 1e9+7;
int n, m, a[N], s[N], cnt;
ll dp[N];
vector <ll> lst[N], sum[N];
//lst[i]存储所有以它结尾LIS长度为i的位置上的数值，
//sum存储对应lst数组中dp值的前缀和。
ll find(int p, ll x){
//这个函数通过在lst[p]中找到最后一个>=x的数，
//从而得到所有“比x小且以它结尾的LIS长度为以x结尾的LIS长度-1”的数的dp值之和。
    int l = 0, r = lst[p].end() - lst[p].begin() - 1, mid;
    while(l < r){
    mid = (l + r + 1) >> 1;
    if(lst[p][mid] < x) r = mid - 1;
    else l = mid;
    }
    return ((sum[p].back() - sum[p][l]) % P + P) % P;
}
int main(){
    read(n);
    for(int i = 1; i <= n; i++)
    read(a[i]);
    for(int i = 0; i <= n; i++)
    lst[i].push_back(INF), sum[i].push_back(0);
    lst[0].push_back(-INF), sum[0].push_back(1);
    for(int i = 1; i <= n; i++){
    int lis; //以位置i结尾的LIS长度
    if(!cnt || a[i] > s[cnt]) s[++cnt] = a[i], lis = cnt;
    else {
        int pos = lower_bound(s + 1, s + cnt + 1, a[i]) - s;
        s[pos] = a[i];
        lis = pos;
    }
    lst[lis].push_back(a[i]);
    sum[lis].push_back((sum[lis].back() + find(lis - 1, a[i])) % P);
    }
    write(sum[cnt].back()), enter;
    return 0;
}
```













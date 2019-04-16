# 51nod 1678 lyk与gcd | 容斥原理 - 胡小兔 - 博客园







# [51nod 1678 lyk与gcd | 容斥原理](https://www.cnblogs.com/RabbitHu/p/51nod1678.html)





![](https://images2017.cnblogs.com/blog/1129536/201710/1129536-20171031215721310-2038478756.png)

51nod 200题辣ψ(｀∇´)ψ ！庆祝！

# [51nod 1678](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1678) lyk与gcd | 容斥原理

## 题面

这天，lyk又和gcd杠上了。

它拥有一个n个数的数列，它想实现两种操作。

1：将 ai 改为b。

2：给定一个数i，求所有 gcd(i,j)=1 时的 aj 的总和。

### Input

第一行两个数n，Q(1<=n,Q<=100000)。

接下来一行n个数表示ai(1<=ai<=10^4)。

接下来Q行，每行先读入一个数A(1<=A<=2)。

若A=1，表示第一种操作，紧接着两个数i和b。(1<=i<=n,1<=b<=10^4)。

若B=2，表示第二种操作，紧接着一个数i。(1<=i<=n)。

### Output

对于每个询问输出一行表示答案。

### Input示例

5 3

1 2 3 4 5

2 4

1 3 1

2 4

### Output示例

9

7

## 题解

我们维护sum[j]表示\(\sum_{k = 1}^{n/j} a[k*j]\)，即所有j的倍数位置上的数之和。

题目要求\(gcd(i, j) = 1\), 就是要求总和去掉所有**与i有公约数的位置**上的数。

那么通过sum数组和容斥原理可以求出来“所有与i有公约数的位置上的数”之和：

sum[第一个质因数] + sum[第二个质因数] + ... - sum[第一个质因数*第二个质因数] - sum[第二个质因数 * 第三个质因数] - ... + sum[第一个质因数*第二个质因数*第三个质因数]……

总之，可以枚举i的所有满足**组成该数的每个质因数在该数中只出现一次**的因数，然后如果这个因数中质因数总个数为奇数则加上它的sum，否则减去它的sum，就能求出“所有与i有公约数的位置上的数”之和了。

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
int n, q, op, pos, val;
int sum[N], cnt[N], a[N];
bool np[N], mul[N];

void init(){
    np[0] = np[1] = 1;
    for(int i = 2; i <= n; i++)
        if(!np[i]){
            cnt[i] = 1;
            for(int j = 2; i * j <= n; j++){
                np[i * j] = 1;
                cnt[i * j]++;
                if(j % i == 0) mul[i * j] = 1;
            }
        }
}
void add(int i, int x){
    for(int j = 1; j * j <= i; j++)
        if(i % j == 0){
            sum[j] += x - a[i];
            if(j * j != i) sum[i / j] += x - a[i];
        }
    a[i] = x;
}
void query(int i){
    ll ret = 0;
    for(int j = 1; j * j <= i; j++)
        if(i % j == 0){
            if(j > 1 && !mul[j]) ret += (cnt[j] & 1) ? sum[j] : -sum[j];
            if(j * j != i && !mul[i / j]) ret += (cnt[i / j] & 1) ? sum[i / j] : -sum[i / j];
        }
    write(sum[1] - ret), enter;
}
int main(){
    read(n), read(q);
    init();
    for(int i = 1; i <= n; i++)
        read(val), add(i, val);
    while(q--){
        read(op), read(pos);
        if(op == 1) read(val), add(pos, val);
        else query(pos);
    }
    return 0;
}
```













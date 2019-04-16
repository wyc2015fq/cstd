# OI中的莫比乌斯反演 - 胡小兔 - 博客园







# [OI中的莫比乌斯反演](https://www.cnblogs.com/RabbitHu/p/mobius.html)





# OI中的莫比乌斯反演

## 莫比乌斯函数

想要学习莫比乌斯反演，首先要学习**莫比乌斯函数**。

### 定义

莫比乌斯函数用\(\mu(x)\)表示。如果\(x\)是\(k\)个不同质数的积，则\(\mu(x) = (-1)^k\)，否则\(\mu(x) = 0\)（此时\(x\)一定是某个或某些平方数的倍数）。\(x = 1\)时，相当于\(x\)由\(0\)个不同质数组成，所以\(\mu(1) = 1\)。

\[
\mu(x)=\left\{
\begin{array}{rcl}
1 & & {x = 1}\\
(-1)^k& & {x = p_1p_2...p_k}\\
0 & & {else}\\
\end{array} \right.
\]

### 性质

莫比乌斯函数有如下性质：
- 莫比乌斯函数是一个积性函数，即对于任意互质的两个数\(x, y\)，有\(\mu(xy) = \mu(x) \times \mu(y)\)
- 对于任意正整数\(n\)有\[\sum_{d | n} \mu(d) = \left\{\begin{array}{rcl}1 && {n = 1} \\ 0 && {n > 1} \end{array}\right.\]
- 对于任意正整数\(n\)有\[\sum_{d|n}\frac{\mu(d)}{d} = \frac{\phi(n)}{n}\]

### 证明

// 证明嘛……不看也罢。本文重点在“莫比乌斯反演在OI中的应用”，~~大胆猜想，不用证明，2333~~
- 莫比乌斯函数的积性：显然，若\(x\)、\(y\)中有一个含有平方数因子，则\(\mu(x)\)、\(\mu(y)\)中有一个为\(0\)，相乘得\(0\)，而\(xy\)也含有平方数因子，所以它的\(\mu(xy) = 0 = \mu(x) * \mu(y)\)。若\(x\)、\(y\)中都不含有平方数因子，又因为\(x\)、\(y\)互质，所以乘积\(xy\)也不含有平方数因子。设\(x\)有\(m\)个不同质因子，\(y\)有\(n\)个，则\(xy\)有\(n + m\)个，则\(\mu(xy) = (-1)^{n + m}= (-1)^n \times (-1)^m = \mu(x) \times \mu(y)\)。
- 当\(n = 1\)时显然。当\(n > 1\)时，\(n\)可以分解为\(p_1^{a_1}p_2^{a_2}...p_k^{a_k}\)。在\(n\)的所有因子\(d\)中，\(\mu\)值不为\(0\)的只有没有平方数因子的\(d\)，即由\(p_1...p_k\)中任意多个质因数直接相乘得到的乘积。则有：\[\sum_{d|n}\mu(d) = C_k^0 - C_k^1 + C_k^2 - ... + (-1)^kC_k^k = \sum_{i = 0}^{k}(-1)^iC_k^i\]只需证明这个式子等于\(0\)。根据二项式定理，\((x + y)^n = \sum_{i=0}^{n}C_n^ix^iy^{n-i}\)，令\(x = -1, y = 1\)，代入得证。
- \(\sum_{d|n}\phi(d) = n\)（[证明传送门](http://blog.csdn.net/adjky/article/details/61198817)），而根据下面要讲的莫比乌斯反演定理，直接把\(F(x) = x, f(x) = \phi(x)\)代入即可。

## 莫比乌斯反演定理

### 内容

莫比乌斯反演定理的内容：

\[F(n) = \sum_{d|n}f(d) \Rightarrow f(n) = \sum_{d|n}\mu(d)F(\frac{n}{d})\]
\[F(n) =  \sum_{n|d}f(d) \Rightarrow f(n) = \sum_{n|d}\mu(\frac{d}{n})F(d)\]

### 证明

懒得打这么多公式了……[证明传送门](http://blog.csdn.net/outer_form/article/details/50588307)。

## 莫比乌斯反演的应用

### [BZOJ 2301](http://www.lydsy.com/JudgeOnline/problem.php?id=2301) Problem b

> 
对于给出的n个询问，每次求有多少个数对\((x,y)\)，满足\(a≤x≤b\)，\(c≤y≤d\)，且\(gcd(x,y) = k\)。

\(1≤n≤50000, 1≤a≤b≤50000, 1≤c≤d≤50000, 1≤k≤50000.\)


要求满足条件的\(a≤x≤b\)，\(c≤y≤d\)的数对个数，根据容斥原理，就是求满足条件的“\(1≤x≤b, 1≤y≤d\)的数对个数 - \(1≤x<a\)，\(c≤y≤d\)的数对个数 - \(a≤x≤b\)，\(1≤y<c\)的数对个数 + \(1≤x<a\)，\(1≤y<c\)的数对个数”。

实际上，这道题还有一个更简单的、不用容斥原理的版本——[BZOJ1101](http://www.lydsy.com/JudgeOnline/problem.php?id=1101)，可惜是权限题 =_=|||

而\(1≤x≤a, 1≤y≤b, gcd(x, y) = k\)等价于\(1≤x≤\lfloor\frac{a}{k}\rfloor\)，\(1≤y≤\lfloor\frac{b}{k}\rfloor, gcd(\frac{x}{k}, \frac{y}{k}) = 1\)。

那么现在问题就变得简单了——求有多少数对\((x,y)\)，满足\(1≤x≤a\)，\(1≤y≤b\)且\(gcd(x, y) = 1\)。

设\(F(n)\)为\(1≤x≤a, 1≤y≤b, n | gcd(x, y)\)的点对\((x,y)\)的个数，
\(f(n)\)为\(1≤x≤a, 1≤y≤b, gcd(x, y) = n\)的点对\((x, y)\)个数。

显然，\(F(n) = \lfloor\frac{a}{n}\rfloor\lfloor\frac{b}{n}\rfloor\)，\(n > \min(a, b)\)时，\(F(n) = f(n) = 0\)。

可以看出：\[F(n) = \sum_{n | d}f(d)\]

则根据莫比乌斯反演定理：\[f(n) = \sum_{n|d}\mu(\frac{d}{n})F(d) = \sum_{n|d}\mu(\frac{d}{n}) \lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\]

我们最后要求的是\(f(1)\)，所以代入\(n = 1\)：

\[f(1) = \sum_{d = 1}^{+\infty}\mu(d)F(d) = \sum_{d = 1}^{min(a, b)}\mu(d) \lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\]

枚举\(d\)就可以\(O(n)\)求了，但是还有多组数据，这个复杂度太大。接下来我们要继续优化。

考虑这个问题：\(n\)为正整数，\(d\)为\([1, n]\)整数，那么\(\lfloor\frac{n}{d}\rfloor\)最多有多少种取值？

可以分类讨论：当\(d \le \sqrt n\)，假设每个\(d\)都对应一个\(\lfloor\frac{n}{d}\rfloor\)，则有\(\sqrt n\)个取值；当\(d > \sqrt n\)，则\(\lfloor\frac{n}{d}\rfloor < \sqrt n\)，则最多也只有\(\sqrt n\)个取值，因此取值数不超过\(2\sqrt n\)。

那么\(\lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\)有多少取值呢？

emmm……实际上，最多有\(2(\sqrt a + \sqrt b)\)种取值。

我没有在网上找到严谨的数学证明，于是我在这里写个自创的不是非常数学的证明：

![](https://images2017.cnblogs.com/blog/1129536/201712/1129536-20171218174055490-1171781514.png)

橙色折线是\(\lfloor\frac{b}{d}\rfloor\)随d变化的图像，蓝色折线\(\lfloor\frac{a}{d}\rfloor\)随d变化的图像。橙色水平线段总数\(\sqrt b\)，蓝色水平线段总数是\(\sqrt a\)，设\(a < b\)则橙色折线变化更频繁。

\(\lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\)的取值种数即每一段蓝色水平线段对应的橙色水平线段数之和。假如图中每条绿色虚线对应的都恰好是橙色折线中的拐点，取值总数是橙色水平线段总数，即\(2\sqrt b\)。而实际上不一定总会对齐，每当绿色虚线对应上橙色折线中的一条水平线段中间的某个位置，则相当于右边的那截蓝色水平线段多对应了一截橙色水平线段。绿色线段数即蓝色折线拐点数，为\(2\sqrt a\)。

所以总取值数是\(2\sqrt a + 2\sqrt b\)。

回到上面的那个式子：\(f(1) = \sum_{d = 1}^{min(a, b)}\mu(d) \lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\)，我们枚举\(\lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\)的取值（共\(2\sqrt a + 2\sqrt b\)种），且预处理\(\mu(d)\)的前缀和，则一次询问的复杂度是\(O(\sqrt n)\)。

至于代码实现——

枚举\(\lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\)取值的代码：

```
if(a > b) swap(a, b);
for(int i = 1, last = 0; i <= a; i = last + 1){
    last = min(a / (a / i), b / (b / i));
    ans += (ll)(a / i) * (b / i) * (sum[last] - sum[i - 1]);
}
```

\(O(n)\)预处理\(\mu(d)\)前缀和的代码（参考[贾志鹏 线性筛](https://wenku.baidu.com/view/2d706761aa00b52acec7ca63.html)）：

```
void getmu(){
    mu[1] = sum[1] = 1;
    for(int i = 2; i <= N; i++){
    if(!notprime[i]) mu[i] = -1, prime[++tot] = i;
    for(int j = 1; j <= tot && i * prime[j] <= N; j++){
        notprime[i * prime[j]] = 1;
        if(i % prime[j]) mu[i * prime[j]] = -mu[i];
        else{
        mu[i * prime[j]] = 0;
        break;
        }
    }
    sum[i] = sum[i - 1] + mu[i];
    }
}
```

[BZOJ 1101 完整代码](http://www.cnblogs.com/RabbitHu/p/BZOJ1101.html)

[BZOJ 2301 完整代码](http://www.cnblogs.com/RabbitHu/p/BZOJ2301.html)

### [BZOJ 2820](http://www.lydsy.com/JudgeOnline/problem.php?id=2820) YY的GCD

> 
\(T\)组数据，给定\(a, b\),求\(1<=x<=a, 1<=y<=b\)且\(gcd(x, y)\)为质数的\((x, y)\)有多少对。


这道题和上面的有些类似，也可以设
\(F(n)\)为\(1≤x≤a, 1≤y≤b, n | gcd(x, y)\)的点对\((x,y)\)的个数，
\(f(n)\)为\(1≤x≤a, 1≤y≤b, gcd(x, y) = n\)的点对\((x, y)\)个数。

则仍然有\[f(n) = \sum_{n|d}\mu(\frac{d}{n}) \lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\]

那么枚举质数\(p\)，答案可表示为

\[\begin{align*}
ans &= \sum_{p}^{min(a, b)}\sum_{p|d}\mu(\frac{d}{p}) \lfloor\frac{a}{d}\rfloor\lfloor\frac{b}{d}\rfloor\\
&= \sum_{p}^{min(a, b)}\sum_{i = 1}^{min(a, b)}\mu(i) \lfloor\frac{a}{pi}\rfloor\lfloor\frac{b}{pi}\rfloor\\
\end{align*}\]

设\(t = pi\)，代入得：

\[\begin{align*}
ans &= \sum_{p}^{min(a, b)}\sum_{p|t}^{min(a, b)}\mu(\frac{t}{p}) \lfloor\frac{a}{t}\rfloor\lfloor\frac{b}{t}\rfloor\\
&= \sum_{t = 1}^{min(a, b)}\sum_{p | t}^{min(a, b)}\mu(\frac{t}{p}) \lfloor\frac{a}{t}\rfloor\lfloor\frac{b}{t}\rfloor\\
&= \sum_{t = 1}^{min(a, b)}\lfloor\frac{a}{t}\rfloor\lfloor\frac{b}{t}\rfloor\sum_{p | t}^{min(a, b)}\mu(\frac{t}{p}) 
\end{align*}\]

我们知道\(\lfloor\frac{a}{t}\rfloor\lfloor\frac{b}{t}\rfloor\)的取值不超过\(2(\sqrt a + \sqrt b)\)种，那么如果预处理\(\sum_{p}^{min(a, b)}\mu(\frac{t}{p})\)关于t的前缀和，就可以\(O(\sqrt n)\)出解了。

预处理这个前缀和可以筛出素数后对每个素数枚举它的倍数、更新对应的\(\sum_{p}^{min(a, b)}\mu(\frac{t}{p})\)。因为处理每个素数均摊\(O(\log n)\)，而一共约有\(O(\frac{n}{\log n})\)个素数，所以预处理前缀和的总复杂度是\(O(n)\)。

[BZOJ 2820 完整代码](http://www.cnblogs.com/RabbitHu/p/BZOJ2820.html)

### [BZOJ 3529](http://www.lydsy.com/JudgeOnline/problem.php?id=3529) 数表

> 
令\(F(i)\)为\(i\)的约数和，\(q\)次给定\(n\), \(m\), \(a\), 求
\[\sum_{1 \le i \le n, 1 \le j \le m, F(\gcd(i, j)) \le a} F(\gcd(i, j)) \mod 2^{31}\]

数据范围：\(n, m, \le 10^5, a \le 10^9, q \le 2\times 10^4\)


\(F(gcd(i, j)) \le a\)的限制不好处理，所以我们先假设没有这条限制，问题变为求\[\sum_{1 \le i \le n, 1 \le j \le m} F(gcd(i, j)) \mod 2^{31}\]

和前面几道题一样，我们可以设\(g(x)\)为\(1 \le i \le n, 1 \le j \le m, \gcd(i, j) = 1\)的数对\((i, j)\)的个数，则有：

\[g(i) = \sum_{i|d} \mu(\frac{d}{i}) \lfloor\frac{n}{d}\rfloor\lfloor\frac{m}{d}\rfloor\]

\(F(i)\)可以用线性筛预处理出来，则答案\(ans\)就是
\[\begin{align*}
ans &= \sum_{i = 1}^{min(n, m)}F(i)g(i)\\
&= \sum_{i = 1}^{min(n, m)} F(i)\sum_{i|d}\lfloor\frac{n}{d}\rfloor\lfloor\frac{m}{d}\rfloor\\
&= \sum_{d=1}^{min(n, m)}\lfloor\frac{n}{d}\rfloor\lfloor\frac{m}{d}\rfloor\sum_{i|d}F(i)\mu(\frac{d}{i})
\end{align*}\]

现在只需求出\(\sum_{i|d}F(i)\mu(\frac{d}{i})\)关于d的前缀和就好了。

和上道题一样，枚举\(i\)，暴力用它的\(F(i)\)更新它的倍数\(d\)们的\(\sum_{i|d}F(i)\mu(\frac{d}{i})\)，复杂度\(O(n\log n)\)。

现在我们还有一个问题没有解决，那就是\(F(i) \le a\)的限制。

如果只有一次询问，这很简单——大于\(a\)的\(F(i)\)都等于0就好了。

现在有好多询问，这也不难——只需离线读入询问，将询问按照\(a\)排序，将所有\(F(i)\)也排序，然后处理每个询问前，把小于等于\(a\)的\(F(i)\)加入树状数组即可。

[BZOJ 3529 完整代码](http://www.cnblogs.com/RabbitHu/p/BZOJ3529.html)













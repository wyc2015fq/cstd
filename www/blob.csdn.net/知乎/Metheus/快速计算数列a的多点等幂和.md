# 快速计算数列a的多点等幂和 - 知乎
# 

## 问题概述：

> 给定数列 ![\{a_n\}](https://www.zhihu.com/equation?tex=%5C%7Ba_n%5C%7D) ,上界 ![m](https://www.zhihu.com/equation?tex=m) ,对于所有的 ![k \leq m](https://www.zhihu.com/equation?tex=k+%5Cleq+m) , 求![\sum_{i=1}^{n}{a_i^k}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7Ba_i%5Ek%7D) 的值。

## 更新一个新的方法：

直接写出答案的生成函数： ![H(x)=\sum_{i\ge0}\sum_{j=1}^n {a_j}^ix^i=\sum_{j=1}^n\sum_{i\ge0} {(a_jx)}^i=\sum_{i=1}^n\frac1{1-a_ix}=\frac{\sum_{i=1}^n{\prod_{j\not =i}(1-a_jx)}}{\prod_{i=1}^n(1-a_ix)}](https://www.zhihu.com/equation?tex=H%28x%29%3D%5Csum_%7Bi%5Cge0%7D%5Csum_%7Bj%3D1%7D%5En+%7Ba_j%7D%5Eix%5Ei%3D%5Csum_%7Bj%3D1%7D%5En%5Csum_%7Bi%5Cge0%7D+%7B%28a_jx%29%7D%5Ei%3D%5Csum_%7Bi%3D1%7D%5En%5Cfrac1%7B1-a_ix%7D%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5En%7B%5Cprod_%7Bj%5Cnot+%3Di%7D%281-a_jx%29%7D%7D%7B%5Cprod_%7Bi%3D1%7D%5En%281-a_ix%29%7D)

设 ![F(x)=\prod_{i=1}^n(1-a_ix),G(x)=\sum_{i=1}^n{\prod_{j\not =i}(1-a_jx)}](https://www.zhihu.com/equation?tex=F%28x%29%3D%5Cprod_%7Bi%3D1%7D%5En%281-a_ix%29%2CG%28x%29%3D%5Csum_%7Bi%3D1%7D%5En%7B%5Cprod_%7Bj%5Cnot+%3Di%7D%281-a_jx%29%7D) ， ![F(x)](https://www.zhihu.com/equation?tex=F%28x%29) 可以利用分治FFT在 ![O(n\log^2 n)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2+n%29) 算出来。而 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 的 ![x^i](https://www.zhihu.com/equation?tex=x%5Ei) 的系数恰好就是 ![F(x)](https://www.zhihu.com/equation?tex=F%28x%29) 对应项系数乘上 ![n-i](https://www.zhihu.com/equation?tex=n-i) ，即 ![[x^i]G(x)=(n-i)[x^i]F(x)](https://www.zhihu.com/equation?tex=%5Bx%5Ei%5DG%28x%29%3D%28n-i%29%5Bx%5Ei%5DF%28x%29) 。

考虑式子的组合意义， ![F(x)](https://www.zhihu.com/equation?tex=F%28x%29) 的 ![x^i](https://www.zhihu.com/equation?tex=x%5Ei) 的系数表示从这 ![n](https://www.zhihu.com/equation?tex=n) 个数中选出 ![i](https://www.zhihu.com/equation?tex=i) 个数的乘积之和再乘上 ![(-1)^i](https://www.zhihu.com/equation?tex=%28-1%29%5Ei) ，稍微观察一下 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 的形式，就会发现对于一个选取了 ![j](https://www.zhihu.com/equation?tex=j) 个数的方案，它会在 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 中重复出现 ![n-j](https://www.zhihu.com/equation?tex=n-j) 次，所有选取数量相同的方案被算重的方案数是相同的，那么直接对 ![F(x)](https://www.zhihu.com/equation?tex=F%28x%29) 的系数乘上被算重次数 ![n-i](https://www.zhihu.com/equation?tex=n-i) ，就是 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 的系数了。

设数列长度为 ![n](https://www.zhihu.com/equation?tex=n) ，需要计算的幂和的上限为 ![m](https://www.zhihu.com/equation?tex=m) ，那么分治FFT时间复杂度 ![O(n\log^2n)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2n%29)  ,求逆+乘法时间复杂度 ![O(m\log m)](https://www.zhihu.com/equation?tex=O%28m%5Clog+m%29) ，所以时间复杂度 ![O(n\log^2n+m\log m)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2n%2Bm%5Clog+m%29) 。与方法二相同但是不需要多项式取对。

## 原文：

注意到直接计算该和式需要 ![O(n\log m)](https://www.zhihu.com/equation?tex=O%28n%5Clog+m%29) ， ![m](https://www.zhihu.com/equation?tex=m) 次计算则需要花费 ![O(nm\log m)](https://www.zhihu.com/equation?tex=O%28nm%5Clog+m%29) 的时间。

如果我们改变枚举顺序，时间复杂度可降至 ![O(nm)](https://www.zhihu.com/equation?tex=O%28nm%29) 。

对于数列 ![\{a_n\}](https://www.zhihu.com/equation?tex=%5C%7Ba_n%5C%7D) ,我们可以设出一个生成函数 ![E(x)=\prod{(1+a_ix)}](https://www.zhihu.com/equation?tex=E%28x%29%3D%5Cprod%7B%281%2Ba_ix%29%7D)。似乎这个生成函数也没有什么特殊的性质，尝试对其求对数:

令 ![G(x)=\ln E(x)](https://www.zhihu.com/equation?tex=G%28x%29%3D%5Cln+E%28x%29) ,那么有： 

![G(x)=\ln \prod_{i=1}^{n}(1+a_ix)=\sum_{i=0}^{n} \ln(1+a_ix)\\](https://www.zhihu.com/equation?tex=G%28x%29%3D%5Cln+%5Cprod_%7Bi%3D1%7D%5E%7Bn%7D%281%2Ba_ix%29%3D%5Csum_%7Bi%3D0%7D%5E%7Bn%7D+%5Cln%281%2Ba_ix%29%5C%5C)

利用 ![\ln (x+1)](https://www.zhihu.com/equation?tex=%5Cln+%28x%2B1%29) 在 ![x=0](https://www.zhihu.com/equation?tex=x%3D0) 处展开的麦克劳林展开式 ![\ln (1+ax)=\sum_{n \geq 1}{\frac{(-1)^{n+1}}{n} x^n}](https://www.zhihu.com/equation?tex=%5Cln+%281%2Bax%29%3D%5Csum_%7Bn+%5Cgeq+1%7D%7B%5Cfrac%7B%28-1%29%5E%7Bn%2B1%7D%7D%7Bn%7D+x%5En%7D) ，可得： ![\begin{align} G(x)&=\sum_{i=1}^{n}{\ln(1+a_ix)}\\ &=\sum_{i=1}^{n}{\sum_{k\geq1}{\frac{(-1)^{k+1}}{k}}a_i^kx^k}\\ \end{align}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+G%28x%29%26%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7B%5Cln%281%2Ba_ix%29%7D%5C%5C+%26%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7B%5Csum_%7Bk%5Cgeq1%7D%7B%5Cfrac%7B%28-1%29%5E%7Bk%2B1%7D%7D%7Bk%7D%7Da_i%5Ekx%5Ek%7D%5C%5C+%5Cend%7Balign%7D%5C%5C)

发现生成函数中出现了 ![a_i^k](https://www.zhihu.com/equation?tex=a_i%5Ek) 的结构。接下来尝试变换枚举顺序：

![\begin{align} G(x)&=\sum_{k \geq 1}\sum_{i=1}^{n}{\frac{(-1)^{k+1}}{k}a_i^kx^k}\\ &=\sum_{k \geq 1}\frac{(-1)^{k+1}}{k}\sum_{i=1}^{n}{a_i^kx^k}\\ \end{align}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+G%28x%29%26%3D%5Csum_%7Bk+%5Cgeq+1%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7B%5Cfrac%7B%28-1%29%5E%7Bk%2B1%7D%7D%7Bk%7Da_i%5Ekx%5Ek%7D%5C%5C+%26%3D%5Csum_%7Bk+%5Cgeq+1%7D%5Cfrac%7B%28-1%29%5E%7Bk%2B1%7D%7D%7Bk%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7Ba_i%5Ekx%5Ek%7D%5C%5C+%5Cend%7Balign%7D%5C%5C)

式子中出现了我们求和的东西： ![\sum_{i=1}^{n}{a_i^k}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%7Ba_i%5Ek%7D) 。接下来的难点便在于如何快速求出 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29)。

首先 ![E(x)=\prod_{i=1}^{n}(1+a_ix)](https://www.zhihu.com/equation?tex=E%28x%29%3D%5Cprod_%7Bi%3D1%7D%5E%7Bn%7D%281%2Ba_ix%29) 。

令 ![P(x)=\prod_{i=1}^{\lceil \frac{n}{2} \rceil}(1+a_ix), Q(x)=\prod_{i=\lceil \frac{n}{2} \rceil+1}^{n}(1+a_ix)](https://www.zhihu.com/equation?tex=P%28x%29%3D%5Cprod_%7Bi%3D1%7D%5E%7B%5Clceil+%5Cfrac%7Bn%7D%7B2%7D+%5Crceil%7D%281%2Ba_ix%29%2C+Q%28x%29%3D%5Cprod_%7Bi%3D%5Clceil+%5Cfrac%7Bn%7D%7B2%7D+%5Crceil%2B1%7D%5E%7Bn%7D%281%2Ba_ix%29)，则 ![E(x)=P(x)Q(x)](https://www.zhihu.com/equation?tex=E%28x%29%3DP%28x%29Q%28x%29) 。

结合FFT以及分治算法,![E(x) ](https://www.zhihu.com/equation?tex=E%28x%29+)可以在 ![O(n\log^2n)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2n%29) 时间内被算出。

接下来计算 ![G(x)=\ln E(x)](https://www.zhihu.com/equation?tex=G%28x%29%3D%5Cln+E%28x%29) 。由于题目给定了K，只需要知道 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 的前K+1项，那么我们把式子放在取模的意义下：

![G(x) \equiv \ln E(x)\mod(x^{m+1})\\](https://www.zhihu.com/equation?tex=G%28x%29+%5Cequiv+%5Cln+E%28x%29%5Cmod%28x%5E%7Bm%2B1%7D%29%5C%5C)

两边同时对 ![x](https://www.zhihu.com/equation?tex=x) 求导：

![G'(x)\equiv \frac{E'(x)}{E(x)}\mod(x^{m+1})\\](https://www.zhihu.com/equation?tex=G%27%28x%29%5Cequiv+%5Cfrac%7BE%27%28x%29%7D%7BE%28x%29%7D%5Cmod%28x%5E%7Bm%2B1%7D%29%5C%5C)

同时积分：

![G(x)\equiv \int\frac{E'(x)}{E(x)}dx\mod(x^{m+1})\\](https://www.zhihu.com/equation?tex=G%28x%29%5Cequiv+%5Cint%5Cfrac%7BE%27%28x%29%7D%7BE%28x%29%7Ddx%5Cmod%28x%5E%7Bm%2B1%7D%29%5C%5C)

我们得到了 ![G(x)](https://www.zhihu.com/equation?tex=G%28x%29) 的表达式，其中多项式求逆 ![O(m\log m)](https://www.zhihu.com/equation?tex=O%28m%5Clog+m%29) ,求导以及积分 ![O(m)](https://www.zhihu.com/equation?tex=O%28m%29) 。

至此问题得以全部解决，总时间复杂度 ![O(n\log^2n+m\log m)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2n%2Bm%5Clog+m%29) 。

## **例题：玩游戏（**[【P4705】玩游戏 - 洛谷](https://link.zhihu.com/?target=https%3A//www.luogu.org/problemnew/show/P4705)**）**

> Alice 和 Bob 又在玩游戏。
对于一次游戏，首先 Alice 获得一个长度为 ![n](https://www.zhihu.com/equation?tex=n) 的序列 ![\{a_n\}](https://www.zhihu.com/equation?tex=%5C%7Ba_n%5C%7D) ，Bob 获得一个长度为 ![m](https://www.zhihu.com/equation?tex=m) 的序列 ![\{b_n\}](https://www.zhihu.com/equation?tex=%5C%7Bb_n%5C%7D) 。之后他们各从自己的序列里随机取出一个数，分别设为 ![a_x,b_y](https://www.zhihu.com/equation?tex=a_x%2Cb_y) ，定义这次游戏的 ![k](https://www.zhihu.com/equation?tex=k) 次价值为 ![(a_x+b_y)^k](https://www.zhihu.com/equation?tex=%28a_x%2Bb_y%29%5Ek) 。
由于他们发现这个游戏实在是太无聊了，所以想让你帮忙计算对于 ![i=1,2,3,...,t](https://www.zhihu.com/equation?tex=i%3D1%2C2%2C3%2C...%2Ct) ，一次游戏 ![i](https://www.zhihu.com/equation?tex=i) 次价值的期望是多少。
由于答案可能很大，只需要求出模 ![ 998244353](https://www.zhihu.com/equation?tex=+998244353) 下的结果即可。

**输入格式：**

> 第一行两个整数 ![n,m(1≤n,m≤10^5)](https://www.zhihu.com/equation?tex=n%2Cm%281%E2%89%A4n%2Cm%E2%89%A410%5E5%29) ，分别表示 Alice 和 Bob 序列的长度。
接下来一行 ![n](https://www.zhihu.com/equation?tex=n) 个数，第 *i*![i](https://www.zhihu.com/equation?tex=i) 个数为 ![a_i​(0≤a_i​<998244353)](https://www.zhihu.com/equation?tex=a_i%E2%80%8B%280%E2%89%A4a_i%E2%80%8B%3C998244353%29) ，表示 Alice 的序列。
接下来一行 ![m](https://www.zhihu.com/equation?tex=m) 个数，第 ![j](https://www.zhihu.com/equation?tex=j) 个数为![b_j​(0≤b_j​<998244353)](https://www.zhihu.com/equation?tex=b_j%E2%80%8B%280%E2%89%A4b_j%E2%80%8B%3C998244353%29) ，表示 Bob 的序列。
接下来一行一个整数  ![t (1≤t≤10^5)](https://www.zhihu.com/equation?tex=t+%281%E2%89%A4t%E2%89%A410%5E5%29) ，意义如上所述。

**输出格式：**

> 共 ![t](https://www.zhihu.com/equation?tex=t) 行，第 ![i](https://www.zhihu.com/equation?tex=i) 行表示一次游戏 ![i](https://www.zhihu.com/equation?tex=i) 次价值的期望。

**样例输入：**

> 2 8
764074134 743107904
663532060 183287581 749169979 7678045 393887277 27071620 13482818 125504606
6

**样例输出：**

> 774481679
588343913
758339354
233707576
36464684
461784746

对于某一个 ![k](https://www.zhihu.com/equation?tex=k) ， ![k](https://www.zhihu.com/equation?tex=k) 次价值期望值为 ![E_k=\frac{\sum_{i=1}^{n}\sum_{i=1}^{m}{(a_i+b_y)^k}}{nm}](https://www.zhihu.com/equation?tex=E_k%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%7B%28a_i%2Bb_y%29%5Ek%7D%7D%7Bnm%7D) 。那么我们只需要计算分子和式，最后除以定值 ![nm](https://www.zhihu.com/equation?tex=nm) 就行了。

![\begin{align} nmE_k&=\sum_{i=1}^{n}\sum_{j=1}^{m}(a_i+b_j)^k\\ &=\sum_{i=1}^{n}\sum_{j=1}^{m}\sum_{p=1}^{p}\binom{k}{p}a_i^pb_j^{k-p}\\ &=\sum_{p=1}^{k}\binom{k}{p}\sum_{i=1}^{n}a_i^p\sum_{j=1}^{m}b_j^{k-p}\\ &=\sum_{p=1}^{k}\frac{k!}{p!(k-p)!}\sum_{i=1}^{n}a_i^p\sum_{j=1}^{m}b_j^{k-p}\\ &=k!\sum_{p=1}^{k}\frac{\sum_{i=1}^{n}a_i^p}{p!}\frac{\sum_{j=1}^{m}b_j^{k-p}}{(k-p)!}\\ \end{align}\\ ](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+nmE_k%26%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Csum_%7Bj%3D1%7D%5E%7Bm%7D%28a_i%2Bb_j%29%5Ek%5C%5C+%26%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Csum_%7Bj%3D1%7D%5E%7Bm%7D%5Csum_%7Bp%3D1%7D%5E%7Bp%7D%5Cbinom%7Bk%7D%7Bp%7Da_i%5Epb_j%5E%7Bk-p%7D%5C%5C+%26%3D%5Csum_%7Bp%3D1%7D%5E%7Bk%7D%5Cbinom%7Bk%7D%7Bp%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Da_i%5Ep%5Csum_%7Bj%3D1%7D%5E%7Bm%7Db_j%5E%7Bk-p%7D%5C%5C+%26%3D%5Csum_%7Bp%3D1%7D%5E%7Bk%7D%5Cfrac%7Bk%21%7D%7Bp%21%28k-p%29%21%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Da_i%5Ep%5Csum_%7Bj%3D1%7D%5E%7Bm%7Db_j%5E%7Bk-p%7D%5C%5C+%26%3Dk%21%5Csum_%7Bp%3D1%7D%5E%7Bk%7D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7Da_i%5Ep%7D%7Bp%21%7D%5Cfrac%7B%5Csum_%7Bj%3D1%7D%5E%7Bm%7Db_j%5E%7Bk-p%7D%7D%7B%28k-p%29%21%7D%5C%5C+%5Cend%7Balign%7D%5C%5C+)

发现这是一个卷积的形式，于是令 ![A(x)=\sum_{p=1}^{k}{\frac{\sum_{i=1}^{n}a_i^p}{p!}}x^p, B(x)=\sum_{p=1}^{k}{ \frac{\sum_{i=1}^{m}b_i^{p}}{p!}x^p }\\ \\](https://www.zhihu.com/equation?tex=A%28x%29%3D%5Csum_%7Bp%3D1%7D%5E%7Bk%7D%7B%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7Da_i%5Ep%7D%7Bp%21%7D%7Dx%5Ep%2C+B%28x%29%3D%5Csum_%7Bp%3D1%7D%5E%7Bk%7D%7B+%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bm%7Db_i%5E%7Bp%7D%7D%7Bp%21%7Dx%5Ep+%7D%5C%5C+%5C%5C)

可以利用上文快速求和的方法，计算 ![A(x),B(x)](https://www.zhihu.com/equation?tex=A%28x%29%2CB%28x%29) 的系数。

答案 ![nmE_k=k![x^k]A(x)B(x)](https://www.zhihu.com/equation?tex=nmE_k%3Dk%21%5Bx%5Ek%5DA%28x%29B%28x%29) ，可以用FFT快速算出。整体时间复杂度 ![O(n\log^2n+k\log k)](https://www.zhihu.com/equation?tex=O%28n%5Clog%5E2n%2Bk%5Clog+k%29) 。

```cpp
#include<cmath>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define loop(n,i) for(register int i=1;i<=(n);++i)
#define MAX 6662333
using namespace std;
/*玩游戏*/
inline int getint(){
    char c=getchar();int s=;bool sign=;
    while(!isdigit(c)&&c^'-')c=getchar();
    if(c=='-')c=getchar(),sign=;
    while(isdigit(c))s=(s<<)+(s<<)+c-'0',c=getchar();
    return sign?-s:s;
}
#define P 998244353
#define int long long
struct Poly{
    int a[MAX];
    int length;
    int& operator [](const int x){return a[x];}
    const int operator [](const int x)const{return a[x];}
    inline void operator *=(const Poly& X){
        for(register int i=,j=<<max(length,X.length);i<j;++i) a[i]=a[i]*X[i]%P;
    }
    Poly(int x=):length(x){}
};
ostream& operator <<(ostream& out,Poly& X){
    for(register int i=,j=<<X.length;i<j;++i) out<<X[i]<<' ';return out;
}
int R[MAX];
inline int Quick(int a,int m){int ans=;for(;m;m>>=,a=a*a%P) if(m&) ans=ans*a%P;return ans;}
inline void NTT(Poly& a,int type){
    int x=a.length,n=<<x;
    for(register int i=;i<n;++i) if((R[i]=R[i>>]>>|(i&)<<x-)>i) swap(a[i],a[R[i]]);
    for(register int s=;s<=x;++s){
        int len=<<s,mid=len>>;
        int w=Quick(,P-+type*(P-)/len);
        for(register int k=;k<n;k+=len){
            int d=;
            for(register int j=;j<mid;++j){
                int u=a[j+k],v=a[j+k+mid]*d%P;
                a[j+k]=(u+v)%P;
                a[j+k+mid]=(u-v+P)%P;
                d=d*w%P;
            }
        }
    }
    if(type==-) for(register int i=,inv=Quick(n,P-);i<n;++i) a[i]=a[i]*inv%P;
}

Poly A;
inline void Inverse(Poly& a,Poly& b){
    for(register int i=,j=<<a.length+;i<j;++i) b[i]=A[i]=;
    b[]=Quick(a[],P-);
    for(register int s=;s<=a.length;++s){
        int len=<<s,Len=len<<;
        for (register int i=;i<len;++i) A[i]=a[i];
        b.length=A.length=s+;
        NTT(b,),NTT(A,);
        for(register int i=;i<Len;++i) b[i]=(*b[i]%P-b[i]*b[i]%P*A[i]%P+P)%P;
        NTT(b,-);
        for(register int i=len;i<Len;++i) b[i]=;
    }
    b.length=a.length;
}

inline void Derivation(Poly& a,Poly& b){
    for(register int i=,j=<<a.length+;i<j;++i) b[i]=;
    for(register int i=,j=<<a.length;i<j;++i) b[i]=a[i+]*(i+)%P;
    b[(<<(b.length=a.length))-]=;
}
inline void Integrate(Poly& a,Poly& b){
    for(register int i=,j=<<a.length+;i<j;++i) b[i]=;
    R[]=;for(register int i=,j=<<a.length;i<j;++i) R[i]=(P-P/i)*R[P%i]%P;
    for(register int i=,j=<<a.length;i<j;++i) b[i+]=a[i]*R[i+]%P;
    b[]=;b.length=a.length+;
}

Poly C;
inline void Ln(Poly& a,Poly& b){
    for(register int i=,j=<<a.length+;i<j;++i) b[i]=C[i]=;
    Derivation(a,b);
    Inverse(a,C);
    C.length++,b.length++;
    NTT(C,),NTT(b,);C*=b;NTT(C,-);
    C.length--,b.length--;
    for(register int i=<<C.length,j=<<C.length+;i<j;++i) b[i]=;
    Integrate(C,b);
    b.length=a.length;
}

int memo[MAX];
Poly AA,BB;
int Calculate(int l,int r,const Poly& a){
    if(l==r) return memo[l]=a[l],;
    int mid=l+r>>;
    AA[]=Calculate(l,mid,a);
    BB[]=Calculate(mid+,r,a);
    AA.length=;
    while((<<AA.length)<=(r-l+)) AA.length++;BB.length=AA.length;
    for(register int i=,j=<<AA.length;i<j;++i) AA[i]=BB[i]=;
    for(register int i=l;i<=mid;++i) AA[i-l+]=memo[i];
    for(register int i=mid+;i<=r;++i) BB[i-mid]=memo[i];
    NTT(AA,),NTT(BB,);AA*=BB;NTT(AA,-);
    for(register int i=l;i<=r;++i) memo[i]=AA[i-l+];
    return AA[];
}

int n,m,t;
Poly a,b,temp;
main(){
    n=getint(),m=getint();
    loop(n,i) a[i]=getint();
    loop(m,i) b[i]=getint();
    t=getint(); 
    while((<<a.length)<=t) a.length++;b.length=a.length;
    
    memo[]=Calculate(,n,a);
    for(register int i=;i<=n;++i) a[i]=memo[i];
    Ln(a,temp);a=temp;
    memo[]=Calculate(,m,b);
    for(register int i=;i<=m;++i) b[i]=memo[i];
    Ln(b,temp);b=temp;
    
    R[]=;for(register int i=;i<=t;++i) R[i]=(P-P/i)*R[P%i]%P;
    for(register int k=,sign=,fact=;k<=t;++k,sign=P-sign,fact=fact*R[k]%P){
        a[k]=a[k]*k%P*sign%P*fact%P;
        b[k]=b[k]*k%P*sign%P*fact%P;
    }
    a[]=n,b[]=m;
    a.length++,b.length++;
    for(register int i=t+,j=<<a.length;i<j;++i) a[i]=b[i]=;
    NTT(a,),NTT(b,);a*=b;NTT(a,-);
    int inv=Quick(n*m%P,P-);
    for(register int i=,fact=;i<=t;++i,fact=fact*i%P){
        cout<<fact*a[i]%P*inv%P<<'\n';
    }
}
```


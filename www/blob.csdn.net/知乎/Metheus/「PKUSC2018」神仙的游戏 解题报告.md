# 「PKUSC2018」神仙的游戏 解题报告 - 知乎
# 

**问题描述**

> 小 D 和小H是两位神仙。他们经常在一起玩神仙才会玩的一些游戏，比如 “口算一个 4 位数是不是完全平方数” 。
今天他们发现了一种新的游戏：首先称 s 长度为 len的前缀成为 border 当且仅当 *s*[1…len]=s[∣s∣−len+1…∣s∣] 。给出一个由 01? 组成的字符串 s, 将 s 中的问号用 0,1 替换，对每个 len 口算是否存在替换问号的方案使得 s长度为len的前缀成为 border，把这个结果记做 ![f(len)∈{0,1}](https://www.zhihu.com/equation?tex=f%28len%29%E2%88%88%7B0%2C1%7D) 。如果 s 长度为len的前缀能够成为 border,则 ![f(len) = 1](https://www.zhihu.com/equation?tex=f%28len%29+%3D+1) ，否则 ![f(len) = 0](https://www.zhihu.com/equation?tex=f%28len%29+%3D+0) 。
由于小 D和小 H是神仙，所以他们计算的 s 的长度很长，因此把计算的结果一一比对会花费很长的时间。为了方便比对，他们规定了一个校验值： ![(f(1)×1^2) xor (f(2)×2^2) xor (f(3)×3^2) xor … xor (f(n)×n^2)](https://www.zhihu.com/equation?tex=%28f%281%29%C3%971%5E2%29+xor+%28f%282%29%C3%972%5E2%29+xor+%28f%283%29%C3%973%5E2%29+xor+%E2%80%A6+xor+%28f%28n%29%C3%97n%5E2%29) 来校验他们的答案是否相同。xor 表示按位异或。但是不巧，在某一次游戏中，他们口算出的校验值并不一样，他们希望你帮助他们来计算一个正确的校验值。当然，他们不强迫你口算，可以编程解决。

**输入格式**

> 一个串s, 保证每个字符都是 0，1，或者?.

**输出格式**

> 输出字符串的校验值， 即 ![(f(1)×1^2) xor (f(2)×2^2) xor (f(3)×3^2) xor … xor (f(n)×n^2)](https://www.zhihu.com/equation?tex=%28f%281%29%C3%971%5E2%29+xor+%28f%282%29%C3%972%5E2%29+xor+%28f%283%29%C3%973%5E2%29+xor+%E2%80%A6+xor+%28f%28n%29%C3%97n%5E2%29)

**样例输入**

> 1?0?

**样例输出**

> 17

**数据范围**
![](https://pic2.zhimg.com/v2-5b1b701f4952384f665fbb3e562315ad_b.jpg)
稍微摆弄一下题目的条件可以发现，如果存在长为 ![len ](https://www.zhihu.com/equation?tex=len+) 的前缀等于后缀，则原串一定是以若干长度为 ![n-len](https://www.zhihu.com/equation?tex=n-len) 的子串连接而成（也就是说子串有长度为 ![n-len](https://www.zhihu.com/equation?tex=n-len) 的循环节，最后一个循环节不一定完整）。

如果存在两个位置 ![a_i,a_j](https://www.zhihu.com/equation?tex=a_i%2Ca_j) 上的数字不相等（一个0，一个1），且下标之差为 ![|i-j|=d](https://www.zhihu.com/equation?tex=%7Ci-j%7C%3Dd) ，那么![d](https://www.zhihu.com/equation?tex=d)不可能成为原串的循环节长度，显然 ![d](https://www.zhihu.com/equation?tex=d) 的所有因子 ![x|d](https://www.zhihu.com/equation?tex=x%7Cd) 也不能（若干个 ![x](https://www.zhihu.com/equation?tex=x) 可以连接成一个 ![d](https://www.zhihu.com/equation?tex=d) ）。

根据这个性质，我们可以得到一个朴素解法：枚举所有的0,1对，计算他们的位置差，统计答案。时间复杂度 ![O(n^2)](https://www.zhihu.com/equation?tex=O%28n%5E2%29) 。

算法的瓶颈在于枚举01对。如何快速枚举？不难想到加速枚举利器——FFT。

我们需要统计是否存在位置差为某个值的01对。设 ![f(d)](https://www.zhihu.com/equation?tex=f%28d%29) 为位置差为 ![d](https://www.zhihu.com/equation?tex=d) 的01对数量，则 ![f(d)=\sum_{i-j=d}[a_i\ne a_j]](https://www.zhihu.com/equation?tex=f%28d%29%3D%5Csum_%7Bi-j%3Dd%7D%5Ba_i%5Cne+a_j%5D)。设 ![A(x)=\sum_{i=0}^{n-1}[a_i=1]x^i,B(x)=\sum_{i=0}^{n-1}[b_i=1]x^{-i}](https://www.zhihu.com/equation?tex=A%28x%29%3D%5Csum_%7Bi%3D0%7D%5E%7Bn-1%7D%5Ba_i%3D1%5Dx%5Ei%2CB%28x%29%3D%5Csum_%7Bi%3D0%7D%5E%7Bn-1%7D%5Bb_i%3D1%5Dx%5E%7B-i%7D) ,那么 ![f(d)=[x^d]A(x)B(x)](https://www.zhihu.com/equation?tex=f%28d%29%3D%5Bx%5Ed%5DA%28x%29B%28x%29) 。在这里 ![-n\lt d \lt n](https://www.zhihu.com/equation?tex=-n%5Clt+d+%5Clt+n) ，那么只需要对式子加上 ![n](https://www.zhihu.com/equation?tex=n) 的偏离量就行了。时间复杂度 ![O(nlogn)](https://www.zhihu.com/equation?tex=O%28nlogn%29) 。

算出 ![f(d)](https://www.zhihu.com/equation?tex=f%28d%29) 之后，可以直接枚举 ![len](https://www.zhihu.com/equation?tex=len) ，再检查是否存在位置差为 ![n-len](https://www.zhihu.com/equation?tex=n-len) 的倍数的01对，时间复杂度为调和级数 ![O(nlogn)](https://www.zhihu.com/equation?tex=O%28nlogn%29) 。

算法总时间复杂度 ![O(nlogn)](https://www.zhihu.com/equation?tex=O%28nlogn%29) ，可以通过所有数据。

```cpp
#include<algorithm> 
#include<iostream>
#include<cstring>
#include<cstdio>
#define loop(n,i) for(regsiter int i=1;i<=(n);++i)
#define MAX 2100009
#define zxc(x) cerr<<(#x)<<'='<<(x)<<'\n'
#define zxcv(x) cerr<<(#x)<<'='<<(x)<<','
using namespace std;
/*神仙游戏*/
inline int getint(){
    char c=getchar();int s=;bool sign=;
    while(!isdigit(c)&&c^'-')c=getchar();
    if(c=='-')c=getchar(),sign=;
    while(isdigit(c)) s=(s<<)+(s<<)+c-'0',c=getchar();
    return sign?-s:s;
}

#define P 998244353
#define int long long
struct Poly{
    int a[MAX];int length;
    int& operator [](const int x){return a[x];}
    inline void operator *=(Poly& X){
        for(register int i=,n=<<max(length,X.length);i<n;++i) a[i]=a[i]*X[i]%P;
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

char s[MAX];
int len;
Poly a,b;

bool exist[MAX];
main(){
    scanf("%s",s);
    len=strlen(s);
    while((<<a.length)<=*len) a.length++;b.length=++a.length;
    for(register int i=;i<len;++i) a[i+len]=s[i]=='1';
    for(register int i=;i<len;++i) b[len-i]=s[i]=='0';
    NTT(a,),NTT(b,);a*=b;NTT(a,-);
    for(register int i=;i<len;++i) exist[i]=a[*len-i]|a[*len+i];
    int ans=;
    for(register int L=;L<len;++L){
        int delta=len-L;
        bool _=;
        for(register int j=delta;j<len;j+=delta) if(exist[j]) _=;
        if(_) ans^=(L*L);
    }
    ans^=(len*len);
    cout<<ans;
}
```

- 封面图源见水印。
- 题目[Loj 6436 神仙游戏](https://link.zhihu.com/?target=https%3A//loj.ac/problem/6436)


# GCD LCM 欧几里得算法 扩展欧几里得算法 - 紫芝的博客 - CSDN博客





2018年04月12日 23:35:20[紫芝](https://me.csdn.net/qq_40507857)阅读数：46








**欧几里得算法：**

辗转相除法的关键恒等式：**gcd(a,b)=gcd(b,a mod b);**

边界条件：**gcd(a,0)=a;**

```cpp
//最大公约数 
int gcd(int a,int b)
{
	return b==0?a:gcd(b,a%b);
 }
```

 公式： 
**gcd(a,b)*lcm(a,b) = a*b；**

最小公倍数：

lcm(a,b)=a/gcd(a,b)*b;//先除后除
lcm(a,b)=a*b/gcd(a,b); //错误，a*b可能会溢出 

**扩展欧几里得算法：**

```cpp
//求整数x和y，使得a*x+b*y=d,且|x|+|y|最小，d=gcd(x,y)
void exgcd(ll a,ll b,ll &d,ll &x,ll &y)
{
    if(!b) {d=a; x=1; y=0;}
    else {
        exgcd(b,a%b,d,y,x);
        y-=x*(a/b);
    }
}
```

**给一个不定方程Ax+By+C=0. 这个方程是否存在整数解？**

**如果存在，就输出一组x和y，否则输出-1**

## 输入

有多组样例，每行三个数字A，B，C

-2×10^9 < A，B，C < 2×10^9 

## 输出

若有解，输出一行x和y，以空格隔开。否则输出-1

## 样例输入

```cpp
2 5 3
931480234 -1767614767 -320146190
-1548994394 -1586527767 -1203252104
1906842444 749552572 -1693767003
-1183748658 875864960 -1315510852
200000003 200000001 1
```

## 样例输出

```cpp
6 -3
-98880374013340920 -52107006370101410
-878123061596147680 857348814150663048
-1
-97498198168399474 -131770725522871624
100000000 -100000001
```

 一个二元一次方程如果存在整数解，那么就有多组（x，y）满足方程，用扩展欧几里得定理求出来的（x，y）是确定的

如果  C%gcd(A,B)不等于0，说明方程 Ax + By = C 无整数解。

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
void exgcd(ll a,ll b,ll &d,ll &x,ll &y)//扩展欧几里得
{
    if(!b){//若b=0，gcd(a,b)=gcd(a,0)=a;
        d=a;//a,b的最大公约数
        x=1;y=0;
    }else{
        exgcd(b,a%b,d,y,x);
        y-=x*(a/b);
    }
}
int main()
{
    ll a,b,c;
    while(scanf("%lld%lld%lld",&a,&b,&c)!=EOF){
        ll x=0,y=0,d=0;
        exgcd(a,b,d,x,y);
        //printf("%d\n",d);
        if(c%d==0)
        printf("%lld %lld\n",-x*(c/d),-y*(c/d));//求不定方程Ax+By+C=0的一组整数解
        //printf("%lld %lld\n",x*(c/d),y*(c/d));//求不定方程Ax+By=c的一组整数解
        else//不存在整数解
            printf("-1\n");
    }
    return 0;
}
```






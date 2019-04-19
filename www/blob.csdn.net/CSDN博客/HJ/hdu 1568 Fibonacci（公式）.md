# hdu  1568  Fibonacci（公式） - HJ - CSDN博客
2016年08月02日 18:53:05[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：225
个人分类：[====ACM====																[----------数学----------																[递推公式&结论题																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308108)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
2007年到来了。经过2006年一年的修炼，数学神童zouyu终于把0到100000000的Fibonacci数列 
(f[0]=0,f[1]=1;f[i] = f[i-1]+f[=2”>i-2]("i))的值全部给背了下来。 
接下来，CodeStar决定要考考他，于是每问他一个数字，他就要把答案说出来，不过有的数字太长了。所以规定超过4位的只要说出前4位就可以了，可是CodeStar自己又记不住。于是他决定编写一个程序来测验zouyu说的是否正确。
Input 
输入若干数字n(0 <= n <= 100000000)，每个数字一行。读到文件尾。
Output 
输出f[n]的前4个数字（若不足4个数字，就全部输出）。
Sample Input 
0 
1 
2 
3 
4 
5 
35 
36 
37 
38 
39 
40
Sample Output 
0 
1 
1 
2 
3 
5 
9227 
1493 
2415 
3908 
6324 
1023
解析： 
先看对数的性质
```
loga(b^c)=c*loga(b),loga(b*c)=loga(b)+loga(c);
```
假设给出一个数10234432,那么
```
log10(10234432)=log10(1.0234432*10^7)=log10(1.0234432)+7;
```
log10(1.0234432)就是log10(10234432)的小数部分.
```
log10(1.0234432)=0.010063744
10^0.010063744=1.023443198
```
那么要取几位就很明显了吧~ 
先取对数(对10取),然后得到结果的小数部分bit,pow(10.0,bit)以后如果答案还是<1000那么就一直乘10。 
注意偶先处理了0~20项是为了方便处理~ 
这题要利用到数列的公式:
```
an=(1/√5) * [((1+√5)/2)^n-((1-√5)/2)^n](n=1,2,3.....)
```
取完对数
```
log10(an)=-0.5*log10(5.0)+((double)n)*log(f)/log(10.0)+log10(1-((1-√5)/(1+√5))^n)
其中f=(sqrt(5.0)+1.0)/2.0;
```
因为
```
log10(1-((1-√5)/(1+√5))^n)趋近于0
```
所以可以写成
```
log10(an)=-0.5*log10(5.0)+((double)n)*log(f)/log(10.0);
```
最后取其小数部分。
```cpp
#include<iostream>
#include<cmath>
#include<cstdio> 
using namespace std;
const double s = (sqrt(5.0)+1.0)/2;
int main()
{
    int n,i;
    double bit;
    int fac[21] = { 0 , 1 };
    for(i = 2; i < 21; i++) 
        fac[i] = fac[i-1] + fac [i-2];
    while(cin >> n)
    {
        if(n <= 20) {
            cout << fac[n] << endl;
            continue;
        }
        else{
            bit = -0.5*log(5.0)/log(10.0)+((double)n)*log(s)/log(10.0);//调用公式 
            bit = bit - floor(bit); //取小数部分└(^o^)┘ 
            bit = pow(10.0,bit); 
            while(bit < 1000) //要求四位，所以要将小数点右边的数移到左边直到符合要求 
                bit = 10.0 * bit; 
            cout << (int)bit << endl;    
        }
    }
    return 0;
```

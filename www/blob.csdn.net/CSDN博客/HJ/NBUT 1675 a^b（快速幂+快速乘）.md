# NBUT  1675  a^b（快速幂+快速乘） - HJ - CSDN博客
2016年08月19日 20:02:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：323
Description 
求a的b次方，取模mod(1<=a,b,mod<=1e18)
Input 
多组输入，每组数据一行，3个正整数，分别为a,b,mod
Output 
每组数据输出一行，为答案
Sample Input
2 10 10000000 
5 100 1 
0 2 37
Sample Output
1024 
0 
0
解析：这是个很明显的快速幂取模题，不过还需要注意的是中间两个数据相乘可能会超边界。
提示：为了防止中间两个long long 相乘的时候会超long long，所以对乘法再写一个快速乘法的函数，将乘法变为加法，每加一次就进行取模，代码如下：
```cpp
#include<iostream>
using namespace std;
long long mul_mod(long long a,long long b,long long c)    //快速乘取模
{
    long long ans=0;
    a%=c;
    b%=c;
    while(b)
    {
        if(b&1)
        {
            ans+=a;
            ans%c;
        }
        a<<=1;
        a%=c;
        b>>=1;
    }
    return ans;
}
long long quick_mod(long long a,long long b,long long c)    //快速幂取模 
{
    long long sum=1;
    while(b)
    {
        if(b&1)
        {
            sum=mul_mod(sum,a,c);
            sum%=c;
        }
        a=mul_mod(a,a,c);
        b>>=1;
    }
    return sum;
}
int main()
{
    long long a,b,c;
    while(cin>>a>>b>>c)
    {
        if(a==0||c==0)
          cout<<0<<endl;
        else
          cout<<quick_mod(a,b,c)<<endl;
    }
    return 0;
}
```
可作为模板直接使用！！！

# hdu  2503  a/b+c/d（基础数论） - HJ - CSDN博客
2017年03月06日 23:48:41[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：282
给你2个分数，求他们的和，并要求和为最简形式。
Input 
    输入首先包含一个正整数T（T< =1000），表示有T组测试数据，然后是T行数据，每行包含四个正整数a,b,c,d（0< a,b,c,d<1000），表示两个分数a/b 和 c/d。
Output 
    对于每组测试数据，输出两个整数e和f，表示a/b + c/d的最简化结果是e/f，每组输出占一行。
Sample Input
```
2
1 2 1 3
4 3 2 3
```
Sample Output
```
5 6
2 1
```
实际上就是考察gcd和lcm，很简单的一道题。
```cpp
#include<iostream>
using namespace std;
int gcd(int a,int b)
{
    return a%b==0?b:gcd(b,a%b);
} 
int main()
{
    int a,b,c,d,t;
    cin>>t;
    while(t--)
    {
        cin>>a>>b>>c>>d;
        int k,sum,cnt,p,q;
        k=d*b/gcd(b,d);        //计算b跟d的最小公倍数即通分后的分母 
        sum=a*k/b+c*k/d;      //计算通分后的分子 
        cnt=gcd(sum,k);       //计算分子和分母的最大公约数 
        p=sum/cnt,q=k/cnt;   //最后再分别化简 
        cout<<p<<" "<<q<<endl;
    }
    return 0;
}
```

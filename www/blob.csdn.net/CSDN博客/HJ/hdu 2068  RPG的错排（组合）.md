# hdu  2068   RPG的错排（组合） - HJ - CSDN博客
2016年07月18日 01:35:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：385
hdu  2068  RPG的错排
Description 
今年暑假杭电ACM集训队第一次组成女生队,其中有一队叫RPG,但做为集训队成员之一的野骆驼竟然不知道RPG三个人具体是谁谁。RPG给他机会让他猜猜，第一次猜：R是公主，P是草儿，G是月野兔；第二次猜：R是草儿，P是月野兔，G是公主；第三次猜：R是草儿，P是公主，G是月野兔；……可怜的野骆驼第六次终于把RPG分清楚了。由于RPG的带动，做ACM的女生越来越多，我们的野骆驼想都知道她们，可现在有N多人，他要猜的次数可就多了，为了不为难野骆驼，女生们只要求他答对一半或以上就算过关，请问有多少组答案能使他顺利过关。
Input 
输入的数据里有多个case,每个case包括一个n，代表有几个女生，（n<=25）, n = 0输入结束。
Sample Input 
1 
2 
0 
Sample Output 
1 
1 
解析：这个题的意思呢是在1~N的全排列中，ai = i达到一半或一半以上的全排列数量。由题目看得出这是一个组合数学问题，而且是一个关于错置的问题。n的全排列中错置的个数为
```
Dn=n!(1-1/1!+1/2!-1/3!+…+(-1)n1/n!)
```
又有
```
Dn=(n-1)(Dn-2+Dn-1)
```
而n给出最大为25，所以把前13个算出来即可。接着每次只要枚举1~n/2之间每个的正确个数即可
(注意:涉及组合个数)。
最后求和就可以了，代码如下：
```
#include<cstdio>
#include<iostream>
using namespace std;
double jc(int x)         
{
    int i,sum=1;
    for(i=1;i<=x;i++)
    sum*=i;
    return sum;
}
double C(int fz,int fm)
{
    double x=1;
    int i;
    for(i=fz+1;i<=fm;i++)
    x*=i;
    double y=jc(fm-fz);
    return x/y;
}
int main()
{
    int n;
    long long int s[15],sum;
    s[0]=1,s[1]=0,s[2]=1;
    for(int i=3;i<=13;i++)
        s[i]=(i-1)*(s[i-1]+s[i-2]);  
    while(1)
    {sum=0;
        scanf("%d",&n);
        if(n==0)
        break;
        for(int i=0;i<=n/2;i++)
            sum=sum+C(n-i,n)*s[i];
        printf("%lld\n",sum);
    }
    return 0;
}
```

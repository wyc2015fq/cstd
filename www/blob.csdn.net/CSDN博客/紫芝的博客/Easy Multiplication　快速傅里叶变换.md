# Easy Multiplication　快速傅里叶变换 - 紫芝的博客 - CSDN博客





2018年08月17日 17:24:48[紫芝](https://me.csdn.net/qq_40507857)阅读数：44
个人分类：[动态规划](https://blog.csdn.net/qq_40507857/article/category/7565832)









## Easy Multiplication

时间限制: 1 Sec  内存限制: 128 MB

## 题目描述

乘法就是加法的连续运算，同一个数若干次连加，其运算结果称为积

![\underbrace {a+a+a+\cdots +a}_{{n}}=a\times n](https://wikimedia.org/api/rest_v1/media/math/render/svg/390adfe6f4fee771b5555ea7c2385677260afea9)

给出两个n位10进制整数x和y，你需要计算x*y。 

## 输入

第一行一个正整数n（n<=200000)。 第二行描述一个位数为n的正整数x。 第三行描述一个位数为n的正整数y。 

## 输出

输出一行，即x*y的结果。 

## 样例输入

```
5
12345
78945
```

## 样例输出
`974576025`
分析：

首先ｎ的范围是２＊１０＾５，ｌｏｎｇ　ｌｏｎｇ　ｉｎｔ是没有办法计算的，考虑大数乘法

FFT模板题，直接进行高精度乘法是O(n＾2)的，于是我们采用FFT来O(nlogn)实现： 

1.我们把乘数的每一位看作多项式的系数，得到多项式A(x)（因为高精度乘法的本质就是多项式乘法）

2.首先求出![](https://img-blog.csdn.net/20180817165237174?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，其中k∈[0,n−1]，![](https://img-blog.csdn.net/20180817165412799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)是n次单位复根。 

由于n次单位复根的一些奇妙性质： 

***相消引理***

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20150406231625782)

***折半引理***

![](https://img-blog.csdn.net/20150406232145692)

我们可以采用分治O(nlogn)的时间求出这nn项的值，但是递归实现常数较大，我们采用*蝴蝶算法*来迭代实现。 

![](https://img-blog.csdn.net/20150406232419199)

如图，把原来顺次排列的数列变成叶子中的顺序就可以迭代了~ 

（叶子中的顺序就是原序列的二进制逆序）

```cpp
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <complex>
#define pi acos(-1)
#define N 200005
using namespace std;
complex<double> a[N],b[N],p[N];
int n,c[N];
char s[N];
void FFT(complex<double> x[],int n,int p)
{
//把原来依次排列的数变成叶子中的顺序
    for (int i=0,t=0;i<n;i++)
    {
        if (i>t) swap(x[i],x[t]);
        for (int j=n>>1;(t^=j)<j;j>>=1);
    }
    for (int m=2;m<=n;m<<=1)  //枚举每一层
    {
        complex<double>       wn(cos(p*2*pi/m),sin(p*2*pi/m));
        for (int i=0;i<n;i+=m)
        {
            complex<double> w(1,0),u;
            int k=m>>1;
            for (int j=0;j<k;j++,w*=wn)
            {
            //蝴蝶操作
                u=x[i+j+k]*w;
                x[i+j+k]=x[i+j]-u;
                x[i+j]=x[i+j]+u;
            }
        }
    }
}
int main()
{
    scanf("%d",&n);
    scanf("%s",s);
    for (int i=0;i<n;i++)
        a[i]=s[n-i-1]-'0';
    scanf("%s",s);
    for (int i=0;i<n;i++)
        b[i]=s[n-i-1]-'0';
    //把长度变为2的幂次，方便FFT中的迭代
    for (int j=n,i=1;(i>>2)<j;i<<=1)
        n=i;
    FFT(a,n,1),FFT(b,n,1);
    for (int i=0;i<n;i++)
        p[i]=a[i]*b[i];
    //插值
    FFT(p,n,-1);
    for (int i=0;i<n;i++)
        c[i]=p[i].real()/n+0.1;
    int len=0;
    //进位
    for (int i=0;i<n;i++)
        if (c[i])
            len=i,c[i+1]+=c[i]/10,c[i]%=10;
    for (int i=len;i>=0;i--)
        printf("%d",c[i]);
    printf("\n");
    return 0;
}
```

暴力算法

```cpp
#include<bits/stdc++.h>
using namespace std;
int ans[200000*2+20];
char f[200000],g[200000];
int main()
{
    int n;
    while(~scanf("%d",&n)){
        fill(ans,ans+n*2+2,0);
        fill(f,f+(n+2),NULL);
        fill(g,g+(n+2),NULL);
        scanf("%s",f);
        scanf("%s",g);
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            ans[i+j-1]+=(f[n-i]-'0')*(g[n-j]-'0');
            //printf("%d  ",ans[i+j-2]);
        }
    }
//  for(int i=1;i<=n*2;i++)
//  printf("%d  ",ans[i]);
    for(int i=1;i<=n*2+2;i++)
    {
        int a;
        a=ans[i];
        ans[i]=a%10;
        ans[i+1]+=a/10;
    }
//  for(int i=1;i<=n*2;i++)
//  printf("%d\n",ans[i]);
    int s=n*2+2;
    while(s--)
    {
        if(ans[s-1]!=0)
        {
            while(s--&&s>0)
            printf("%d",ans[s]);
            printf("\n");
            break;
        }
    }
}
    return 0;
}
```






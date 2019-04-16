# 1061 Rightmost Digit（快速幂取模） - PeterBishop - CSDN博客





2018年09月11日 14:42:15[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









### [快速幂取模看这里！](https://blog.csdn.net/qq_40061421/article/details/82625338)

```cpp
#include<stdio.h>
int power(long long a,int n)
{
    long long ans=1;
    while(n>0)
    {
        if(n&1)
        {
            ans*=a;
            ans%=10;
        }
        a*=a%10;
        a%=10;
        n/=2;
    }
    return ans%10;
}
int main(int argc, char const *argv[])
{
    int T;
    while(~scanf("%d",&T))
    {
        while(T--)
        {
            int ans,t;
            scanf("%d",&t);
            ans=power(t,t);
            printf("%d\n",ans);
        }
    }
    return 0;
}
```






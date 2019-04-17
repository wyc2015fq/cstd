# 1757 A Simple Math Problem(矩阵快速幂入门题) - PeterBishop - CSDN博客





2018年09月13日 21:40:07[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









### [矩阵快速幂看这里](https://blog.csdn.net/qq_40061421/article/details/82625338)

题意：求下图公式的结果     这一题 还是个 递推关系式 的矩阵就非常的好推了  和之前的几乎一样  这里就不解释了

![](https://img-blog.csdn.net/20170814155949214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcl9raXJpdG8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```cpp
#include<cstdio>
#include<cstring>
using namespace std;
const int N=10;
int n,mod;
int temp[N][N];
int res[N][N],a[N][N];
void mul(int a[][N],int b[][N])
{
    memset(temp,0,sizeof(temp));
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            for(int k=0;k<N;k++)
                temp[i][j]=(temp[i][j]+a[i][k]*b[k][j]%mod)%mod;
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            a[i][j]=temp[i][j];
    return ;
}
void fun(int nn)
{
    memset(res,0,sizeof(res));
    for(int i=0;i<N;i++)
        res[i][i]=1;
    while(nn){
        if(nn&1)
            mul(res,a);
        mul(a,a);
        nn>>=1;
    }
    return ;
}
int main()
{
    while(~scanf("%d %d",&n,&mod)){
        memset(a,0,sizeof(a));
        for(int i=0;i<N;i++)
            scanf("%d",&a[0][i]);
        for(int i=1;i<N;i++)
            a[i][i-1]=1;
        if(n<10) printf("%d\n",n%mod);
        else {
            fun(n-9);
            int ans=0;
            for(int i=0;i<N;i++)
                ans+=res[0][i]*(9-i)%mod;
            printf("%d\n",ans%mod);
        }
    }
    return 0;
}
```






# HDU 5391 Zball in Tina Town （打表，水） - xcw0754 - 博客园
# [HDU 5391 Zball in Tina Town （打表，水）](https://www.cnblogs.com/xcw0754/p/4733308.html)
题意：
　　Tina有一个球，它的名字叫zball。zball很神奇，它会每天变大。在第一天的时候，它会变大1倍。在第二天的时候，它会变大2倍。在第n天的时候，它会变大n倍。
zball原来的体积是1。Tina想知道，zball在第n−1天时的体积对n取模是多大呢？
思路：
　　如果n是一个合数，那它肯定是有因子的，即有两个比它小的数字a和b，使得a*b=n，那么答案必为0。如果是质数，打表可知是n-1。数字小的直接算，防止意外。数字都不会超过int。
```
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#define LL long long
#define INF 0x7f7f7f7f
using namespace std;
int ans[10000];
bool isPrimeNumber(int N) 
{
    if(N==2)    return true;
    if(N==1||N&1==0)    return false;
    int tmp = sqrt(N);
    for (int j = 2; j <= tmp; j++)
        if (N % j == 0)
            return false;
    return true;
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int t, n;
    LL sum=0;
    for(int i=2; i<=1000; i++)
    {
        sum=1;
        for(int j=1; j<i; j++)
        {
            sum=sum*j%i;
        }
        ans[i]=sum;
    }
    cin>>t;
    while(t--)
    {
        scanf("%d",&n);
        if(n<1000)    
        {
            printf("%d\n",ans[n]);
            continue;
        }
        if(isPrimeNumber(n))    printf("%d\n",n-1);
        else    printf("0\n");
    }
    return 0;
}
AC代码
```


# ZOJ  3607 Lazier Salesgirl（贪心） - HJ - CSDN博客
2017年08月23日 19:16:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：219
[http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3607](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3607)
题目大意： 
有一位卖面包的女孩她很想睡觉，但是她想在睡觉之前得到最大的平均价值。现在有n个顾客会来买面包，请问在保证平均每次卖出的面包平均价值最大的前提下她至少需要等待多少分钟才能去睡着？
解题思路： 
贪心求一遍最大的平均价值。需要注意的是，小女孩如果确定好的最少分钟数大于两个顾客之间的间隔时间，她就必须卖面包给下一个顾客，要一直到下一个顾客来的时间与当前顾客来的时间间隔超过最少分钟数，小女孩才会去睡觉。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
#define INF 0x3f3f3f3f
#define maxn 1005
double s[maxn],p[maxn];
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        p[0]=s[0]=0;
        for(int i=1;i<=n;i++)
          cin>>s[i];
        for(int i=1;i<=n;i++)
          cin>>p[i];
        double ans=0,min_time=0,qq_time=0,sum=0;
        p[n+1]=INF;
        for(int i=1;i<=n;i++)
        {           
            qq_time=max(qq_time,p[i]-p[i-1]);         //qq_time 保存当前为止小女孩所需要等待的最长时间 
            sum+=s[i];
            double res=sum/i;
            if(res>ans&&qq_time<p[i+1]-p[i])           //如果下一个顾客到来的间隔时间比当前需要等待的最长时间要长并且价值也比当前价值大 
            {
                min_time=qq_time;            //更新当前值 
                ans=res;
            }
        }
        printf("%.6lf %.6lf\n",min_time,ans);
    }
    return 0;
}
```

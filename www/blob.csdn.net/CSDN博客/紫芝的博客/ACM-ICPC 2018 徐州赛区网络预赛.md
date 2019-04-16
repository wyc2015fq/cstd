# ACM-ICPC 2018 徐州赛区网络预赛 - 紫芝的博客 - CSDN博客





2018年09月11日 15:30:46[紫芝](https://me.csdn.net/qq_40507857)阅读数：32








# [BE, GE or NE](https://nanti.jisuanke.com/t/31454)

## 题意：

每一轮有三种操作， 加上a 减去b 或者 取负 当且仅当 a, b, c 不为0时，对应的操作有效;

给出一个上界和一个下界 大于等于上界就是 Good Ending 小于等于下界 就是 Bad Ending 否则就是 Normal Ending

两个人轮流操作，第一个人想要Good Ending 第二个人想要 Bad Ending  两个人操作最优，求最后的结局

## 思路：

dp[i][j] 表示 第几轮 数字是多少的时候 ，记忆化爆搜 ，因为数字在[−100,100]

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+7;
int n,m,k,l,a[maxn],b[maxn],c[maxn];
int dp[maxn][250];//dp[i][id[j]]表示第i轮的数字为j
map<int,int> id;
int up,down;
int dfs(int pos,int now)
{
    if(pos==n+1)
    {
        if(now>=k)
            return 2;
        else if(now>l)
            return 1;
        else
            return 0;
    }
    if(dp[pos][id[now]]!=-1)    return dp[pos][id[now]];
    if(pos&1)//奇数
    {
        int f=0;
        if(a[pos])  f=max(f,dfs(pos+1,min(now+a[pos],up)));
        if(b[pos])  f=max(f,dfs(pos+1,max(now+b[pos],down)));
        if(c[pos])  f=max(f,dfs(pos+1,-now));
        return dp[pos][id[now]]=f;
    }else{
        int f=2;
        if(a[pos])  f=min(f,dfs(pos+1,min(now+a[pos],up)));
        if(b[pos])  f=min(f,dfs(pos+1,max(now+b[pos],down)));
        if(c[pos])  f=min(f,dfs(pos+1,-now));
        return dp[pos][id[now]]=f;
    }
}
int main()
{
    int tot=0;
    up=100;
    down=-100;
    for(int i=-100;i<=100;i++)
        id[i]=++tot;
    scanf("%d%d%d%d",&n,&m,&k,&l);
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d%d",&a[i],&b[i],&c[i]);
        b[i]=-b[i];
    }
    memset(dp,-1,sizeof dp);
    int f=dfs(1,m);
    if(f==2)
        printf("Good Ending\n");
    else if(f==1)
        printf("Normal Ending\n");
    else
        printf("Bad Ending\n");
    return 0;
}
```










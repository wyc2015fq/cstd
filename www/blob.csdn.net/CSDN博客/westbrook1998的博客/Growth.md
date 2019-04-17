# Growth - westbrook1998的博客 - CSDN博客





2018年10月06日 19:59:52[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[dp](https://so.csdn.net/so/search/s.do?q=dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
链接：[https://www.nowcoder.com/acm/contest/206/E](https://www.nowcoder.com/acm/contest/206/E)

来源：牛客网

题目描述

弱弱有两个属性a和b，这两个属性初始的时候均为0，每一天他可以通过努力，让a涨1点或b涨1点。

为了激励弱弱努力学习，我们共有n种奖励，第i种奖励有xi，yi，zi三种属性，若a≥ xi且b≥ yi，则弱弱在接下来的每一天都可以得到zi的分数。

问m天以后弱弱最多能得到多少分数。

输入描述:

第一行一个两个整数n和m（1≤ n≤ 1000，1≤ m≤ 2000000000）。

接下来n行，每行三个整数xi，yi，zi（1≤ xi,yi≤ 1000000000，1≤ zi ≤ 1000000）。

输出描述:

一行一个整数表示答案。

示例1

输入

复制

2 4

2 1 10

1 2 20

输出

复制

50

备注:

在样例中，弱弱可以这样规划：第一天a涨1，第二天b涨1，第三天b涨1，第四天a涨1。

共获得0+0+20+30=50分。
dp太难了…

首先是离散化，这步很关键，然后z值对应的是一个x,y对，而不是特定的第几个奖励

定义两个状态和转移
$add_{i,j}$表示当ab满足i,j所对应的x[i]和y[j]时之后的每天能奖励多少
$dp_{i,j}$表示ab满足i,j所对应的x[i]和y[j]时最大能获得的奖励

然后状态转移就是
$add_{i,j}=add_{i-1,j}+add_{i,j-1}-add_{i-1,j-1}+mp[{x[i],y[j]}]$

前面三个就是容斥原理，然后最后一个加的就是对应的z值
$dp_{i,j}=add_{i,j}+max(dp_{i-1,j}+(x_i-x_{i-1}-1)*add_{i-1,j},dp_{i,j-1}+(y_j-y_{j-1}-1)*add_{i,j-1})$

对应就等于从{i,j}开始后面每天获得的奖励（这里其实就是一天）+max(分别从{i-1 j}转移和从{i,j-1}转移过来的)
代码：

```
#include <cstdio>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
using namespace std;
const int N=1050;
typedef long long ll;
int n,m;
ll x[N],y[N],z[N];
ll add[N][N],dp[N][N];
ll xx[N],yy[N];
set<int> xs,ys;
map<pair<int,int>,int> mp;
int main(void){
    scanf("%d%d",&n,&m);
    int xn=0,yn=0;
    for(int i=1;i<=n;i++){
        scanf("%lld%lld%lld",&x[i],&y[i],&z[i]);
        mp[{x[i],y[i]}]+=z[i];
    }
    sort(x+1,x+1+n);
    sort(y+1,y+1+n);
    xn=unique(x+1,x+1+n)-(x);
    yn=unique(y+1,y+1+n)-(y);
    printf("%d %d\n",xn,yn);
    for(int i=1;i<=xn;i++){
        for(int j=1;j<=yn;j++){
            add[i][j]+=add[i-1][j]+add[i][j-1]-add[i-1][j-1]+mp[{x[i],y[j]}];
        }
    }
    // for(int i=1;i<=xn;i++){
    //     for(int j=1;j<=yn;j++){
    //         printf("%lld ",add[i][j]);
    //     }
    //     printf("\n");
    // }
    for(int i=1;i<=xn;i++){
        for(int j=1;j<=yn;j++){
            dp[i][j]=add[i][j]+max(dp[i-1][j]+(x[i]-x[i-1]-1)*add[i-1][j],dp[i][j-1]+(y[j]-y[j-1]-1)*add[i][j-1]);
        }
    }
    // for(int i=1;i<=xn;i++){
    //     for(int j=1;j<=yn;j++){
    //         printf("%lld ",dp[i][j]);
    //     }
    //     printf("\n");
    // }
    ll ans=0;
    for(int i=1;i<xn;i++){
        for(int j=1;j<yn;j++){
            if(x[i]+y[j]<=m){
                ans=max(ans,dp[i][j]+(m-x[i]-y[j])*add[i][j]);
            }
        }
    }
    printf("%lld\n",ans);
    return 0;
}
```







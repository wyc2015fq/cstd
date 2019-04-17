# hdu2844——Coins - westbrook1998的博客 - CSDN博客





2018年08月25日 18:29:13[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26








> 
Whuacmers use coins.They have coins of value A1,A2,A3…An Silverland dollar. One day Hibix opened purse and found there were some coins. He decided to buy a very nice watch in a nearby shop. He wanted to pay the exact price(without change) and he known the price would not more than m.But he didn’t know the exact price of the watch.  

  You are to write a program which reads n,m,A1,A2,A3…An and C1,C2,C3…Cn corresponding to the number of Tony’s coins of value A1,A2,A3…An then calculate how many prices(form 1 to m) Tony can pay use these coins. 

  Input 

  The input contains several test cases. The first line of each test case contains two integers n(1 ≤ n ≤ 100),m(m ≤ 100000).The second line contains 2n integers, denoting A1,A2,A3…An,C1,C2,C3…Cn (1 ≤ Ai ≤ 100000,1 ≤ Ci ≤ 1000). The last test case is followed by two zeros. 

  Output 

  For each test case output the answer on a single line. 

  Sample Input 

  3 10 

  1 2 4 2 1 1 

  2 5 

  1 4 2 1 

  0 0 

  Sample Output 

  8 

  4
给n中硬币的面额和个数 和最高的金额 

求有多少种钱数的组合方式

一开始用了二进制优化的多重背包,,,还兴奋的以为过了 结果超时了 

百度了一下发现不能优化成01背包 数据太大  

等学会了背包再来改
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <set>
#include <cstring>
using namespace std;
const int N=100005;
int n,m;
int dp[100005];
int v[105];
int c[105];
//数据量太大不能用二进制优化成01背包
int va[105];
//记录到达j元时i种钱用的次数？？
int vis[100005];
set<int> res;
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d",&n,&m)){
        if(n==0 && m==0){
            break;
        }
        for(int i=0;i<n;i++){
            scanf("%d",&va[i]);
        }
        for(int i=0;i<n;i++){
            scanf("%d",&c[i]);
        }
        int cnt=0;
        memset(dp,0,sizeof(dp));
        dp[0]=1;
        for(int i=0;i<n;i++){
            memset(vis,0,sizeof(vis));
            for(int j=va[i];j<=m;j++){
                if(dp[j-va[i]] && !dp[j] && vis[j-va[i]]<c[i]){
                    dp[j]=1;
                    vis[j]=vis[j-va[i]]+1;
                    cnt++;
                }
            }
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```







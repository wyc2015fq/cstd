# hdu2955——Robberies - westbrook1998的博客 - CSDN博客





2018年08月25日 18:05:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35








> 
The aspiring Roy the Robber has seen a lot of American movies, and knows that the bad guys usually gets caught in the end, often because they become too greedy. He has decided to work in the lucrative business of bank robbery only for a short while, before retiring to a comfortable job at a university.  
![这里写图片描述](https://odzkskevi.qnssl.com/931cc59be308ad18cd98b5c8caec6454?v=1535029868)

  For a few months now, Roy has been assessing the security of various banks and the amount of cash they hold. He wants to make a calculated risk, and grab as much money as possible.  

  His mother, Ola, has decided upon a tolerable probability of getting caught. She feels that he is safe enough if the banks he robs together give a probability less than this. 

  Input 

  The first line of input gives T, the number of cases. For each scenario, the first line of input gives a floating point number P, the probability Roy needs to be below, and an integer N, the number of banks he has plans for. Then follow N lines, where line j gives an integer Mj and a floating point number Pj .  

  Bank j contains Mj millions, and the probability of getting caught from robbing it is Pj . 

  Output 

  For each test case, output a line with the maximum number of millions he can expect to get while the probability of getting caught is less than the limit set.  

  Notes and Constraints  

  0 < T <= 100  

  0.0 <= P <= 1.0  

  0 < N <= 100  

  0 < Mj <= 100  

  0.0 <= Pj <= 1.0  

  A bank goes bankrupt if it is robbed, and you may assume that all probabilities are independent as the police have very low funds. 

  Sample Input 

  3 

  0.04 3 

  1 0.02 

  2 0.03 

  3 0.05 

  0.06 3 

  2 0.03 

  2 0.03 

  3 0.05 

  0.10 3 

  1 0.03 

  2 0.02 

  3 0.05 

  Sample Output 

  2 

  4 

  6
经典的01背包 而且是概率 浮点数的问题 

这题几个问题要搞清楚，第一就是这个概率是被抓的概率 要求的也就是被抓的概率 但是被抓的概率不能直接简单相乘 但是反过来，不被抓的概率却可以直接相乘，所以要先用1减去被抓概率得到不被抓概率，相乘得到抢两个银行的不被抓概率，再用1减得到被抓概率 

然后一个就是背包容量和价值的确定，一开始就以为是浮点数概率作为价值，但是枚举不了啊，看了题解发现钱要做容量，而概率做价值 dp[i]表示抢i的钱被抓的概率
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=105;
//被抓的概率不能简单相乘，而不被抓的概率却可以直接相乘
//dp[i]表示抢i元抓的概率
double dp[N*100];
double prob[N];
int money[N];
int main(void){
    //freopen("data.txt","r",stdin);
    int t,n;
    double p;
    scanf("%d",&t);
    while(t--){
        scanf("%lf%d",&p,&n);
        int sum=0;
        for(int i=0;i<n;i++){
            scanf("%d%lf",&money[i],&prob[i]);
            sum+=money[i];
        }
        dp[0]=0;
        for(int i=1;i<=sum;i++){
            dp[i]=i;
        }
        int ans=0;
        for(int i=0;i<n;i++){
            for(int j=sum;j>=money[i];j--){
                dp[j]=min(dp[j],1-(1-dp[j-money[i]])*(1-prob[i]));
                if(dp[j]<p){
                    ans=max(ans,j);
                }
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```







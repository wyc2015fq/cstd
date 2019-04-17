# hdu1171——Big Event in HDU - westbrook1998的博客 - CSDN博客





2018年08月14日 19:54:34[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：25标签：[01背包																[dp](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=01背包&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Nowadays, we all know that Computer College is the biggest department in HDU. But, maybe you don’t know that Computer College had ever been split into Computer College and Software College in 2002.  

  The splitting is absolutely a big event in HDU! At the same time, it is a trouble thing too. All facilities must go halves. First, all facilities are assessed, and two facilities are thought to be same if they have the same value. It is assumed that there is N `(0<N<1000)` kinds of facilities (different value, different kinds).  

  Input 

  Input contains multiple test cases. Each test case starts with a number N `(0 < N <= 50 -- the total number of different facilities)`. The next N lines contain an integer V `(0<V<=50 --value of facility)` and an integer M`(0<M<=100 --corresponding number of the facilities)` each. You can assume that all V are different.  

  A test case starting with a negative integer terminates input and this test case is not to be processed.  

  Output 

  For each case, print one line containing two integers A and B which denote the value of Computer College and Software College will get respectively. A and B should be as equal as possible. At the same time, you should guarantee that A is not less than B.  

  Sample Input 

  2 

  10 1 

  20 1 

  3 

  10 1  

  20 2 

  30 1 

  -1 

  Sample Output 

  20 10 

  40 40
题意是要将价值不同的设备分给两个学院，然后设备可能有多个 

网上一个处理很巧妙，先是将有多个的拆分成单个的，然后看作只有价值没有重量（或者说价值就是重量）的01背包进行操作 

这题参考了题解仍然卡了很久，第一个坑点是数组大小，首先设备种类N是到100，然后每台设备价值v是到50，然后数量m最多是50台，因此dp应该开250000大小的 

然后第二个坑点是没看清楚题意，n小于0退出，我以为是等于-1退出
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int w[50500];
int dp[250050];
int n;
int main(void){
    while(~scanf("%d",&n)){
        if(n<0){
            break;
        }
        memset(dp,0,sizeof(dp));
        int v,m;
        int cnt=0;
        int sum=0;
        for(int i=0;i<n;i++){
            scanf("%d%d",&v,&m);
            sum+=v*m;
            while(m--){
                w[cnt++]=v;
            }
        }
        for(int i=0;i<cnt;i++){
            for(int j=sum/2;j>=w[i];j--){
                dp[j]=max(dp[j],dp[j-w[i]]+w[i]);
            }
        }
        printf("%d %d\n",max(dp[sum/2],sum-dp[sum/2]),min(dp[sum/2],sum-dp[sum/2]));
    }
    return 0;
}
```








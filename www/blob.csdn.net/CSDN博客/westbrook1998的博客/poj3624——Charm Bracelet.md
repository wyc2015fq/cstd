# poj3624——Charm Bracelet - westbrook1998的博客 - CSDN博客





2018年08月15日 08:21:47[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：18标签：[01背包																[dp](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=01背包&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Bessie has gone to the mall’s jewelry store and spies a charm bracelet. Of course, she’d like to fill it with the best charms possible from the N (1 ≤ N ≤ 3,402) available charms. Each charm i in the supplied list has a weight Wi (1 ≤ Wi ≤ 400), a ‘desirability’ factor Di (1 ≤ Di ≤ 100), and can be used at most once. Bessie can only support a charm bracelet whose weight is no more than M (1 ≤ M ≤ 12,880). 

  Given that weight limit as a constraint and a list of the charms with their weights and desirability rating, deduce the maximum possible sum of ratings. 

  Input 

  * Line 1: Two space-separated integers: N and M 

  * Lines 2..N+1: Line i+1 describes charm i with two space-separated integers: Wi and Di 

  Output 

  * Line 1: A single integer that is the greatest sum of charm desirabilities that can be achieved given the weight constraints 

  Sample Input 

  4 6 

  1 4 

  2 6 

  3 12 

  2 7 

  Sample Output 

  23
非常基础的一道01背包

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=15000;
int v[N];
int w[N];
int dp[N];
int main(void){
    //freopen("data.txt","r",stdin);
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        _clr(v,0);
        _clr(w,0);
        _clr(dp,0);
        for(int i=0;i<n;i++){
            scanf("%d%d",&v[i],&w[i]);
        }
        for(int i=0;i<n;i++){
            for(int j=m;j>=v[i];j--){
                dp[j]=max(dp[j],dp[j-v[i]]+w[i]);
            }
        }
        printf("%d\n",dp[m]);
    }
    return 0;
}
```







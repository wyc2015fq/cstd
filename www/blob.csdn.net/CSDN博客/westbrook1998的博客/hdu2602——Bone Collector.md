# hdu2602——Bone Collector - westbrook1998的博客 - CSDN博客





2018年05月13日 22:42:09[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30标签：[动态规划																[01背包](https://so.csdn.net/so/search/s.do?q=01背包&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Many years ago , in Teddy’s hometown there was a man who was called “Bone Collector”. This man like to collect varies of bones , such as dog’s , cow’s , also he went to the grave … 

  The bone collector had a big bag with a volume of V ,and along his trip of collecting there are a lot of bones , obviously , different bone has different value and different volume, now given the each bone’s value along his trip , can you calculate out the maximum of the total value the bone collector can get ?  

  Input 

  The first line contain a integer T , the number of cases.  

  Followed by T cases , each case three lines , the first line contain two integer N , V, (N <= 1000 , V <= 1000 )representing the number of bones and the volume of his bag. And the second line contain N integers representing the value of each bone. The third line contain N integers representing the volume of each bone. 

  Output 

  One integer per line representing the maximum of the total value (this number will be less than 2 31). 

  Sample Input 

  1 

  5 10 

  1 2 3 4 5 

  5 4 3 2 1 

  Sample Output 

  14
最简单的01背包题目，因为多组测试样例忘记memset而wa了两次…

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1050;
int v,n;
int V[MAXN];
int W[MAXN];
int dp[MAXN];
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(V,0,sizeof(V));
        memset(W,0,sizeof(W));
        memset(dp,0,sizeof(dp));
        scanf("%d%d",&n,&v);
        for(int i=1;i<=n;i++){
            scanf("%d",&V[i]);
        }
        for(int i=1;i<=n;i++){
            scanf("%d",&W[i]);
        }
        for(int i=1;i<=n;i++){
            for(int j=v;j>=0;j--){
                if(j>=W[i]){
                    dp[j]=max(dp[j],dp[j-W[i]]+V[i]);
                }
            }
        }
        printf("%d\n",dp[v]);
    }
    return 0;
}
```







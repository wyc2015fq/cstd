# hdu1423——Greatest Common Increasing Subsequence - westbrook1998的博客 - CSDN博客





2018年06月17日 15:41:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：48标签：[LICS																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=LICS&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
This is a problem from ZOJ 2432.To make it easyer,you just need output the length of the subsequence. 

  Input 

  Each sequence is described with M - its length (1 <= M <= 500) and M integer numbers Ai (-2^31 <= Ai < 2^31) - the sequence itself. 

  Output 

  output print L - the length of the greatest common increasing subsequence of both sequences. 

  Sample Input 

  1 

  5 

  1 4 2 5 -12 

  4 

  -12 1 2 4 

  Sample Output 

  2
学了新的模板LICS 最大公共递增子序列 

领悟了一天才大概懂了 妈蛋 

代码：
```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=550;
int a[MAXN];
int b[MAXN];
int t;
int al;
int bl;
int dp[MAXN];
int main(void){
    scanf("%d",&t);
    while(t--){
        memset(a,0,sizeof(a));
        memset(b,0,sizeof(b));
        scanf("%d",&al);
        for(int i=1;i<=al;i++){
            scanf("%d",&a[i]);
        }
        scanf("%d",&bl);
        for(int i=1;i<=bl;i++){
            scanf("%d",&b[i]);
        }
        memset(dp,0,sizeof(dp));
        int ans=0;
        for(int i=1;i<=al;i++){
            int len=0;
            for(int j=1;j<=bl;j++){
            //这部分更新dp[j]
            //因为是公共子序列所以只要用b[j]来保存这个公共子序列的最后一个元素
            //所以这里a[i]和b[j]比较其实也是和整个公共子序列比较，如果是递增
            //则暂时增加len，然后接下来的b[j]循环中如果出现相同的，该LICS就成立，dp就等于len+1
                if(a[i]>b[j]){
                    len=max(len,dp[j]);
                }
                //如果两个数相同，说明在前一步的len的基础上再加1，用max保证更优
                else if(a[i]==b[j]){
                    dp[j]=max(dp[j],len+1);
                }
                //更新答案
                ans=max(ans,dp[j]);
            }
        }
        printf("%d\n",ans);
        if(t!=0){
            printf("\n");
        }
    }
    return 0;
}
```








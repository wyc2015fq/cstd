# hdu1003——Max Sum - westbrook1998的博客 - CSDN博客





2018年05月10日 12:51:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[算法																[动态规划																[dp																[简答题](https://so.csdn.net/so/search/s.do?q=简答题&t=blog)
个人分类：[ACM练习题																[hdu](https://blog.csdn.net/westbrook1998/article/category/7459916)](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Given a sequence a[1],a[2],a[3]……a[n], your job is to calculate the max sum of a sub-sequence. For example, given (6,-1,5,4,-7), the max sum in this sequence is 6 + (-1) + 5 + 4 = 14.  

  Input 

  The first line of the input contains an integer T(1<=T<=20) which means the number of test cases. Then T lines follow, each line starts with a number N(1<=N<=100000), then N integers followed(all the integers are between -1000 and 1000).  

  Output 

  For each test case, you should output two lines. The first line is “Case #:”, # means the number of the test case. The second line contains three integers, the Max Sum in the sequence, the start position of the sub-sequence, the end position of the sub-sequence. If there are more than one result, output the first one. Output a blank line between two cases.  

  Sample Input 

  2 

  5 6 -1 5 4 -7 

  7 0 6 -1 1 -6 7 -5 

  Sample Output 

  Case 1: 

  14 1 4 

   Case 2: 

  7 1 6
经典的dp啊..而且是简单题，但是细节总是没处理好，硬是wa了6次，mmp 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long dp[100010];
int main(void){
    int t;
    int c=0;
    scanf("%d",&t);
    while(t--){
        c++;
        int n;
        int maxn=0;
        int l=1,r=1,s=1;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%lld",&dp[i]);
            if(i==0){
                maxn=dp[0];
            }
            else{
                if(dp[i-1]>=0){
                    dp[i]=dp[i-1]+dp[i];
                }
                else{
                    s=i+1;
                }
            }
            if(dp[i]>maxn){
                maxn=dp[i];
                l=s;
                r=i+1;
            }
        }
        if(c!=1){
            printf("\n");
        }
        printf("Case %d:\n%d %d %d\n",c,maxn,l,r);
    }
    return 0;
}
```

无需多一个dp数组，用原数组就行，也无需先读取再遍历，直接边读取边遍历即可](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)





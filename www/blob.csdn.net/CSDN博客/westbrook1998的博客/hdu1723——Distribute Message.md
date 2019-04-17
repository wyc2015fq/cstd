# hdu1723——Distribute Message - westbrook1998的博客 - CSDN博客





2018年02月19日 18:59:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：73标签：[hdu																[算法																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)
个人分类：[hdu](https://blog.csdn.net/westbrook1998/article/category/7459916)





## 问题：

> 
Problem Description 

  The contest’s message distribution is a big thing in prepare. Assuming N students stand in a row, from the row-head start transmit message, each person can transmit message to behind M personals, and how many ways could row-tail get the message?

Input 

  Input may contain multiple test cases. Each case contains N and M in one line. (0<=M

## 题解：

```
#include <stdio.h>
#include <string.h>
int dp[40];
int main(void)
{
    //分析最后一个人，消息传递给他有可能是经过1人、2人...到m人
    //所以状态转移方程dp[n]=dp[n-1]+dp[n-2]+...+dp[n-m]
    int n,m;
    while(scanf("%d %d",&n,&m)!=EOF)
    {
        if(0==n && 0==m)
            break;
        memset(dp,0,40*sizeof(int));
        dp[1]=1;
        int i,j;
        for(i=2;i<=n;i++)
        {
            for(j=1;j<=m && i-j>0;j++)   //这里内循环的判断条件卡了一段时间
            {                            //可以从单步调试的结果反推判断条件
                dp[i]+=dp[i-j];  
            }
        }
        printf("%d\n",dp[n]);
    }
    return 0;
}
```

算是独立做出来了一道动态规划的水题，真叫人头大 

类似题目（hdu2041更水）](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=hdu&t=blog)





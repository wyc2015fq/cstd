# Max Sum（HDU-1003） - Alex_McAvoy的博客 - CSDN博客





2018年03月26日 20:31:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75








# Problem Description

    Given a sequence a[1],a[2],a[3]......a[n], your job is to calculate the max sum of a sub-sequence. 

    For example, given (6,-1,5,4,-7), the max sum in this sequence is 6 + (-1) + 5 + 4 = 14.

# Input

    The first line of the input contains an integer T(1<=T<=20) which means the number of test cases. Then T lines follow, each line starts with a number N(1<=N<=100000), then N integers followed(all the integers are between -1000 and 1000).

# Output

    For each test case, you should output two lines. The first line is "Case #:", # means the number of the test case. The second line contains three integers, the Max Sum in the sequence, the start position of the sub-sequence, the end position of the sub-sequence. If there are more than one result, output the first one. Output a blank line between two cases.

# Sample Input

**2**

**5 6 -1 5 4 -77 0 6 -1 1 -6 7 -5**

# Sample Output

**Case 1:14 1 4Case 2:7 6 6**

# Source Program

```cpp
#include<cstdio>
#include<iostream>
#define N 100001
using namespace std;
int main()
{
    int t;
    int n;
    int num[N],dp[N];
    int max_num,sum;
    int pos_begin,pos_end;
    int k,i;

    scanf("%d",&t);
    for(k=1;k<=t;k++)
    {
        scanf("%d",&n);
        for(i=1;i<=n;i++)
            scanf("%d",&num[i]);

        max_num=-1001;
        for(i=1;i<=n;i++)
        {
            dp[i]=max(num[i],dp[i-1]+num[i]);
            if(max_num<dp[i])
            {
                max_num=dp[i];//记录最大值
                pos_end=i;//记录最后位置
            }
        }

        sum=max_num;
        for(i=pos_end;i>0;i--)//从最后位置开始
        {
            sum-=num[i];//逐个递减
            if(sum==0)//减到0停止
            {
                pos_begin=i;//记录开始位置
                break;
            }
        }
        printf("Case %d:\n%d %d %d\n",k,max_num,pos_begin,pos_end);
        if(k<t)//格式控制
            printf("\n");
    }
    return 0;
}
```






# I NEED A OFFER!（HDU-1203） - Alex_McAvoy的博客 - CSDN博客





2018年03月31日 21:39:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    Speakless很早就想出国，现在他已经考完了所有需要的考试，准备了所有要准备的材料，于是，便需要去申请学校了。要申请国外的任何大学，你都要交纳一定的申请费用，这可是很惊人的。Speakless没有多少钱，总共只攒了n万美元。他将在m个学校中选择若干的（当然要在他的经济承受范围内）。每个学校都有不同的申请费用a（万美元），并且Speakless估计了他得到这个学校offer的可能性b。不同学校之间是否得到offer不会互相影响。“I NEED A OFFER”，他大叫一声。

    帮帮这个可怜的人吧，帮助他计算一下，他可以收到至少一份offer的最大概率。（如果Speakless选择了多个学校，得到任意一个学校的offer都可以）。

# Input

    输入有若干组数据，每组数据的第一行有两个正整数n,m(0<=n<=10000,0<=m<=10000) 

    后面的m行，每行都有两个数据ai(整型),bi(实型)分别表示第i个学校的申请费用和可能拿到offer的概率。 

    输入的最后有两个0。

# Output

    每组数据都对应一个输出，表示Speakless可能得到至少一份offer的最大概率。用百分数表示，精确到小数点后一位。

# Sample Input

**10 34 0.14 0.25 0.30 0**

# Sample Output

**44.0%**

*Hint*

You should use printf("%%") to print a '%'.

————————————————————————————————————————————

思路：01背包变种

求的是可收到至少一份offer的最大概率，即：求拿不到offer的最小概率。

处理数据时不能够把概率机械地相加，同时状态转移后要求的是拿不到的最小概率，而非求拿到offer的最大概率。

如果把状态转移方程写成dp[j]=max(dp[j],dp[j -a[i]]*b[i])，那么就变成：求拿到所有offer的最大概率。（拿到所有offer和拿到至少一份offer是不同的）

故：最大背包因为状态转移变成了最小背包

因此，状态转移方程：dp[j]=min(dp[j],dp[j-a[i]]*(1.0–b[i]));



# **Source Program**

```cpp
#include<cstdio>
#include<iostream>
#define N 10001
using namespace std;

double a[N],b[N];
double dp[N];

int main()
{
    int n,m;
    int i,j;

    while(cin>>n>>m)
    {
        if(n==0&&m==0)
            break;
        for(i=1;i<=m;i++)
            cin>>a[i]>>b[i];

        /*初始化*/
        for(i=0;i<=n;i++)
            dp[i]=1;

        for(i=1;i<=m;i++)
            for(j=n;j>=a[i];j--)
                dp[j]=min(dp[j],dp[j-(int)a[i]]*(1-b[i]));
        printf("%.1lf%%\n",(1-dp[n])*100);
    }
    return 0;
}
```







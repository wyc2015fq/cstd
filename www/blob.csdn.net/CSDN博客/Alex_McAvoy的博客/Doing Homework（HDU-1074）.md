# Doing Homework（HDU-1074） - Alex_McAvoy的博客 - CSDN博客





2018年10月03日 23:11:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[HDU																[动态规划——状压 DP](https://blog.csdn.net/u011815404/article/category/7955244)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

Ignatius has just come back school from the 30th ACM/ICPC. Now he has a lot of homework to do. Every teacher gives him a deadline of handing in the homework. If Ignatius hands in the homework after the deadline, the teacher will reduce his score of the final test, 1 day for 1 point. And as you know, doing homework always takes a long time. So Ignatius wants you to help him to arrange the order of doing homework to minimize the reduced score.

# **Input**

The input contains several test cases. The first line of the input is a single integer T which is the number of test cases. T test cases follow.

Each test case start with a positive integer N(1<=N<=15) which indicate the number of homework. Then N lines follow. Each line contains a string S(the subject's name, each string will at most has 100 characters) and two integers D(the deadline of the subject), C(how many days will it take Ignatius to finish this subject's homework). 

Note: All the subject names are given in the alphabet increasing order. So you may process the problem much easier.

# Output

For each test case, you should output the smallest total reduced score, then give out the order of the subjects, one subject in a line. If there are more than one orders, you should output the alphabet smallest one.

# Sample Input

**23Computer 3 3English 20 1Math 3 23Computer 3 3English 6 3Math 6 3**

# Sample Output

**2ComputerMathEnglish3ComputerEnglishMath**

————————————————————————————————————————————————————

题意：给出 n 种作业以及作业的提交时间和完成时间，每超过一天罚1分，要求输出最少罚几分，输入按最小字典序顺序

思路：状压DP

n小于15，全部做完有 n！种，因此可以考虑状压DP，对于完成相同的作业来说，以任意顺序完成，完成的天数是一样的，只是完成的顺序不同而扣分不同，因此可以将完成相同的作用的状态压缩成一种并记录所扣的最小分数即可。

用 dp[i] 储存作业的状态，time[i] 表示经过的时间，pre[i] 表示状态的前驱，枚举所有的作业，假设对于作业 ，若作业 i 中含有作业 x 已完成，那么 i 可以由与 i 状态相同状态，但作业 x 未完成的状态 j=i-(1<<x) 来完成，若状态从 0 枚举到 2^n-1 那么 j 一定是在 i 之前统计过的

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 15
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
char str[20][101];
int dp[1<<N],pre[1<<N],times[1<<N];
int dead[N],last[N];
void print(int x){//递归输出作业顺序
    if(!x)
        return;
    print(x-(1<<pre[x]));
    printf("%s\n",str[pre[x]]);
}
int main()
{
    int t;
    scanf("%d",&t);
	while(t--)
    {
        int n;
        scanf("%d",&n);
        for(int i=0;i<n;i++)
            scanf("%s%d%d",&str[i],&dead[i],&last[i]);
        memset(times,0,sizeof(times));
        memset(pre,0,sizeof(pre));

        for(int i=1;i<(1<<n);i++){//枚举所有状态
            dp[i]=INF;//初始化状态i的扣分
            for(int j=n-1;j>=0;j--){//从后向前找
                int temp1=1<<j;
                if(!(i&temp1))//状态i不存在作业j完成的情况
                    continue;

                int temp2=i-temp1;//未完成作业j的状态
                int score=times[temp2]+last[j]-dead[j];//计算扣分

                if(score<0)//最小扣分为0
                    score=0;
                if(dp[i]>dp[temp2]+score){
                    dp[i]=dp[temp2]+score;//记录最小扣分
                    times[i]=times[temp2]+last[j];//到达状态i所花费的时间
                    pre[i]=j;//状态i的前驱
                }
            }
        }
        printf("%d\n",dp[(1<<n)-1]);
        print((1<<n)-1);
    }
	return 0;
}
```







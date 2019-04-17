# Monthly Expense（ POJ-3273 ） - Alex_McAvoy的博客 - CSDN博客





2018年06月16日 00:02:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71
个人分类：[POJ																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John is an astounding accounting wizard and has realized he might run out of money to run the farm. He has already calculated and recorded the exact amount of money (1 ≤ moneyi ≤ 10,000) that he will need to spend each day over the next N (1 ≤ N ≤ 100,000) days.

FJ wants to create a budget for a sequential set of exactly M (1 ≤ M ≤ N) fiscal periods called "fajomonths". Each of these fajomonths contains a set of 1 or more consecutive days. Every day is contained in exactly one fajomonth.

FJ's goal is to arrange the fajomonths so as to minimize the expenses of the fajomonth with the highest spending and thus determine his monthly spending limit.



# **Input**

Line 1: Two space-separated integers: N and M

Lines 2.. N+1: Line i+1 contains the number of dollars Farmer John spends on the ith day

# Output

Line 1: The smallest possible monthly limit Farmer John can afford to live with.


# Sample Input

**7 5**

**100400300100500101400**

# Sample Output

**500**

——————————————————————————————————————————————————————————————————————————————————————————





题意：给出n天每天的花费，将n天分成m组，且分组的天数是连续的，要求分完各组的花费和尽量小，输出各组花费和的最大值。

思路：一开始以为是 dp ，写了写没写出来突然想到要求各组最小和的最大值应该用二分做，顺利AC。。。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int n,m;
int a[N];
bool judge(int value)//判断当前花费可把n分成几组
{
    int sum=0;//花费总和
    int cnt=1;//初始时只有一组
    for(int i=0;i<n;i++)//枚举每天花费
    {
        if(sum+a[i]<=value)
            sum+=a[i];
        else
        {
            sum=a[i];
            cnt++;
        }
    }
    if(cnt>m)//若分组数比规定数要多，则mid偏小，需调整左值
        return false;
    else//若分组数比规定数要少，则mid偏大，需调整右值
        return true;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        int left=0,right=0;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
            left=max(left,a[i]);
            right+=a[i];
        }
        int mid;
        while(left<=right)
        {
            mid=(left+right)/2;
            if(judge(mid))
                right=mid-1;
            else
                left=mid+1;
        }
        printf("%d\n",mid);
    }
    return 0;
}
```







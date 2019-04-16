# hdu1059 Dividing 多重背包+二进制优化 - 紫芝的博客 - CSDN博客





2018年08月19日 00:07:02[紫芝](https://me.csdn.net/qq_40507857)阅读数：37








# Dividing

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 31037    Accepted Submission(s): 8805****

**Problem Description**

Marsha and Bill own a collection of marbles. They want to split the collection among themselves so that both receive an equal share of the marbles. This would be easy if all the marbles had the same value, because then they could just split the collection in half. But unfortunately, some of the marbles are larger, or more beautiful than others. So, Marsha and Bill start by assigning a value, a natural number between one and six, to each marble. Now they want to divide the marbles so that each of them gets the same total value. 

Unfortunately, they realize that it might be impossible to divide the marbles in this way (even if the total value of all marbles is even). For example, if there are one marble of value 1, one of value 3 and two of value 4, then they cannot be split into sets of equal value. So, they ask you to write a program that checks whether there is a fair partition of the marbles.

**Input**

Each line in the input describes one collection of marbles to be divided. The lines consist of six non-negative integers n1, n2, ..., n6, where ni is the number of marbles of value i. So, the example from above would be described by the input-line ``1 0 1 2 0 0''. The maximum total number of marbles will be 20000. 

The last line of the input file will be ``0 0 0 0 0 0''; do not process this line.

**Output**

For each colletcion, output ``Collection #k:'', where k is the number of the test case, and then either ``Can be divided.'' or ``Can't be divided.''. 

Output a blank line after each test case.

**Sample Input**

```
1 0 1 2 0 0 
1 0 0 0 1 1 
0 0 0 0 0 0
```

**Sample Output**

```
Collection #1: 
Can't be divided. 

Collection #2: 
Can be divided.
```

**Source**

[Mid-Central European Regional Contest 1999](http://acm.hdu.edu.cn/search.php?field=problem&key=Mid-Central+European+Regional+Contest+1999&source=1&searchmode=source)

**Recommend**

JGShining   |   We have carefully selected several similar problems for you:  [1114](http://acm.hdu.edu.cn/showproblem.php?pid=1114)[1025](http://acm.hdu.edu.cn/showproblem.php?pid=1025)[2191](http://acm.hdu.edu.cn/showproblem.php?pid=2191)[1024](http://acm.hdu.edu.cn/showproblem.php?pid=1024)[1081](http://acm.hdu.edu.cn/showproblem.php?pid=1081)

题意：

有价值分别为1到6的六种物品，数量分别是n1，n2，n3，n4，n5，n6，问是否可以把这些物品分成价值相等的两份？

分析：

典型的多重背包+二进制优化

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=2e5+10;
int dp[maxn];
int ave;//六件物品总价值的一半
int a[10];//价值为i的物品的数量为a[i]
void zero(int cost)//01背包
{
    for(int i=ave;i>=cost;i--)//从大到小枚举背包容量
        dp[i]=max(dp[i],dp[i-cost]+cost);
}
void complet(int cost)//完全背包
{
    for(int i=cost;i<=ave;i++)//从小到大枚举背包容量
        dp[i]=max(dp[i],dp[i-cost]+cost);
}
void multi(int cost,int amount)//多重背包
{
    if(cost*amount>=ave)//价值为cost的物品总价值大于六件物品总价值的一半
    {
        complet(cost);//相当于这种物品有无数件，完全背包
        return ;
    }
    int k=1;
    while(k<amount)//多重背包转换为01背包
    {
        zero(k*cost);
        amount-=k;
        k=k<<1;//二进制优化，时间复杂度log(n)
    }
    zero(amount*cost);
}
int main()
{
    int Case=1;
    while(1){
        memset(dp,0,sizeof(dp));
        int sum=0;
        for(int i=1;i<=6;i++)
        {
            scanf("%d",&a[i]);
            sum+=i*a[i];
        }
        if(!sum)  return 0;
        printf("Collection #%d:\n",Case++);
        if(sum&1)   printf("Can't be divided.\n\n");
        else{
            ave=sum>>1;
            for(int i=1;i<=6;i++)//将每种物品抽象成混合背包
                multi(i,a[i]);//i为价值，a[i]为数量
            if(dp[ave]==ave)//恰好可以分成相等的两份
                printf("Can be divided.\n\n");
            else
                printf("Can't be divided.\n\n");
        }
    }
}
```






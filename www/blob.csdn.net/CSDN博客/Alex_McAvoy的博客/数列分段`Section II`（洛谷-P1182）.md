# 数列分段`Section II`（洛谷-P1182） - Alex_McAvoy的博客 - CSDN博客





2018年08月19日 22:21:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：98








## 题目描述

对于给定的一个长度为N的正整数数列 A−i ，现要将其分成 M(M≤N) 段，并要求每段连续，且每段和的最大值最小。

关于最大值最小：

例如一数列 4 2 4 5 1 要分成 3 段

将其如下分段：

[4 2][4 5][1]

第一段和为 6 ，第 2 段和为 9 ，第 3 段和为 1 ，和最大值为 9 。

将其如下分段：

[4][2 4][5 1]

第一段和为 4 ，第 2 段和为 6 ，第 3 段和为 6 ，和最大值为 6 。

并且无论如何分段，最大值不会小于 6 。

所以可以得到要将数列 4 2 4 5 1 要分成 3 段，每段和的最大值最小为 6 。

## 输入输出格式

**输入格式：**

第 1 行包含两个正整数N，M。

第 2 行包含 N 个空格隔开的非负整数 Ai​ ，含义如题目所述。

**输出格式：**

一个正整数，即每段和最大值最小为多少。

## 输入输出样例

**输入样例#1：**

```
5 3
4 2 4 5 1
```

**输出样例#1：**
`6`
## 源代码

```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int n,m,a[1000000];
bool judge(int x)
{
    int sum=0,group=1;//组数和清零，组数归1
    for(int i=1;i<=n;i++)
    {
        sum+=a[i];//累加和
        if(sum>x)//当前和>当前值
        {
            sum=a[i];//和=当前值
            group++;//分组
        }
    }
    if(group<=m)//当前组数<需要组数
        return true;//需要减小右值
    else
        return false;
}
int main()
{
    int l=0,r=0,mid;
    int i;
    
    cin>>n>>m;
    for(i=1;i<=n;i++)
    {
        cin>>a[i];
        l=max(l,a[i]);//设置左值为数列最大值
        r+=a[i];//设置右值为数列和
    }
    
    while(l+1<r)
    {
        mid=(l+r)/2;//取中间值
        if(judge(mid))//进行判断
            r=mid;
        else 
            l=mid;
    }

    if(judge(l))//判断当前值是否满足条件
        cout<<l<<endl;
    else
        cout<<r<<endl;
    return 0;
}
```






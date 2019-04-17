# Speed Reading（POJ-3619 ） - Alex_McAvoy的博客 - CSDN博客





2018年06月02日 02:41:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：87
个人分类：[POJ																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

All K (1 ≤ K ≤ 1,000) of the cows are participating in Farmer John's annual reading contest. The competition consists of reading a single book with N (1 ≤ N ≤ 100,000) pages as fast as possible while understanding it.

Cow i has a reading speed Si (1 ≤ Si ≤ 100) pages per minute, a maximum consecutive reading time Ti (1 ≤ Ti ≤ 100) minutes, and a minimum rest time Ri (1 ≤ Ri ≤ 100) minutes. The cow can read at a rate ofSi pages per minute, but only for Ti minutes at a time. After she stops reading to rest, she must rest for Ri minutes before commencing reading again.

Determine the number of minutes (rounded up to the nearest full minute) that it will take for each cow to read the book.

# **Input**

Line 1: Two space-separated integers: N and K

Lines 2..K+1: Line i+1 contains three space-separated integers: Si , Ti , and Ri

# Output

Lines 1...K: Line i should indicate how many minutes (rounded up to the nearest full minute) are required for cow i to read the whole book.

# Sample Input

**10 3**

**2 4 16 1 53 3 3**

# Sample Output

**677**

———————————————————————————————————————————————————————

题意：一本书，页数为n，每秒读s页，可连读t秒，但读t秒后需休息r秒，求读完书所需时间。

思路：水题一枚，读的懂题就行。。。

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
#define MOD 2520
#define E 1e-12
using namespace std;
int main()
{
    int n,k;
    scanf("%d%d",&n,&k);
    for(int i=0;i<k;i++)
    {
        int s,t,r;
        scanf("%d%d%d",&s,&t,&r);

        int num=n;//未读页数
        int time=0;//用时

        while(num>s*t)//当未读页数>连读t秒的页数时
        {
            time+=t+r;//总时间=读书时间+休息时间
            num-=s*t;//未读页数=原页数-已读页数
        }

        /*处理未计算页数*/
        time+=num/s;
        if(num%s)
            time++;

        cout<<time<<endl;
    }
    return 0;
}
```







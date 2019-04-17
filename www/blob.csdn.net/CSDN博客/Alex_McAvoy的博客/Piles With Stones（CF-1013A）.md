# Piles With Stones（CF-1013A） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 15:52:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

There is a beautiful garden of stones in Innopolis.

Its most beautiful place is the nn piles with stones numbered from 1 to n.

EJOI participants have visited this place twice.

When they first visited it, the number of stones in piles was x1,x2,…,xn, correspondingly. One of the participants wrote down this sequence in a notebook.

They visited it again the following day, and the number of stones in piles was equal to y1,y2,…,yn. One of the participants also wrote it down in a notebook.

It is well known that every member of the EJOI jury during the night either sits in the room 108108 or comes to the place with stones. Each jury member who comes there either takes one stone for himself or moves one stone from one pile to another. We can assume that there is an unlimited number of jury members. No one except the jury goes to the place with stones at night.

Participants want to know whether their notes can be correct or they are sure to have made a mistake

# Input

The first line of the input file contains a single integer n, the number of piles with stones in the garden (1≤n≤50).

The second line contains n integers separated by spaces x1,x2,…,xn, the number of stones in piles recorded in the notebook when the participants came to the place with stones for the first time (0≤xi≤1000).

The third line contains n integers separated by spaces y1,y2,…,yn, the number of stones in piles recorded in the notebook when the participants came to the place with stones for the second time (0≤yi≤1000).

# Output

f the records can be consistent output "Yes", otherwise output "No" (quotes for clarity).

# Examples

**Input**

5

1 2 3 4 5

2 1 4 3 5

**Output**

Yes

**Input**

5

1 1 1 1 1

1 0 1 0 1

**Output**

Yes

**Input**

3

2 3 9

1 7 9

**Output**

No

————————————————————————————————————————————

题意：给出一个整数 n ，以及两串数列，每串数列有 n 个数，每个数代表一堆石头，每次可取走一个石头或者移动一个石头，问最后能否让第一串数列变成第二串数列

思路：巨水的题。。。统计两串数列的石头数，然后比较两串石头总数，如果第一串的石头总数比第二串要小说明无法达到，输出 No 即可，反之输出 Yes

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
#define N 10001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N],b[N];
int main()
{
    int n;
    cin>>n;

    int sum1=0;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        sum1+=a[i];
    }

    int sum2=0;
    for(int i=1;i<=n;i++)
    {
        cin>>b[i];
        sum2+=b[i];
    }

    if(sum1<sum2)
        cout<<"No"<<endl;
    else
        cout<<"Yes"<<endl;


    return 0;
}
```







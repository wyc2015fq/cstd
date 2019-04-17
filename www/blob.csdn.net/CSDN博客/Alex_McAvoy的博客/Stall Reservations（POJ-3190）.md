# Stall Reservations（POJ-3190） - Alex_McAvoy的博客 - CSDN博客





2018年07月21日 18:42:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67








> 
# Problem Description

Oh those picky N (1 <= N <= 50,000) cows! They are so picky that each one will only be milked over some precise time interval A..B (1 <= A <= B <= 1,000,000), which includes both times A and B. Obviously, FJ must create a reservation system to determine which stall each cow can be assigned for her milking time. Of course, no cow will share such a private moment with other cows. 

Help FJ by determining:

The minimum number of stalls required in the barn so that each cow can have her private milking period

An assignment of cows to these stalls over time

Many answers are correct for each test dataset; a program will grade your answer.

# **Input**

Line 1: A single integer, N

Lines 2..N+1: Line i+1 describes cow i's milking interval with two space-separated integers.

# Output

Line 1: The minimum number of stalls the barn must have.

Lines 2..N+1: Line i+1 describes the stall to which cow i will be assigned for her milking period.

# Sample Input

**51 102 43 65 84 7 **

# Sample Output

**412324**


题意：n 头牛，每头牛会在 A~B 的时间间隔内挤奶，要求为每头牛分配栏位来挤奶，最后输出最小的栏位以及为每头牛分配的栏位。

思路：先将所有牛按照挤奶时间进行排序，将牛按照结束挤奶的时间依次加入优先队列，如果结束时间相同，则开始时间早的优先级高，枚举所有牛，如果存在牛的挤奶时间开始值大于优先队列头部的结束值，说明两头牛可以共用一个栏位，从优先队列中删除这头牛，最后按要求输出即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 50001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int start;
    int endd;
    int num;
    friend bool operator < (Node x,Node y)
    {
        if(x.endd==y.endd)//如果结束时间相等，开始时间早的优先级高
            return x.start<y.start;
        else//如果结束时间不相等，越早结束的优先级越高
            return x.endd>y.endd;
    }
}cow[N];
int num[N];
priority_queue<Node> q;
int cmp(Node x,Node y)
{
    if(x.start==y.start)//如果开始时间相同，则按照结束时间排序
        return x.endd<y.endd;
    else//如果开始时间不同，则按照开始时间排序
        return x.start<y.start;
}
int main()
{
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d",&cow[i].start,&cow[i].endd);//输入开始时间与结束时间
        cow[i].num=i;//记录牛的编号
    }

    sort(cow+1,cow+n+1,cmp);//将所有牛排序

    int sum=1;
    q.push(cow[1]);
    num[cow[1].num]=1;
    for(int i=2;i<=n;i++)
    {
        if( !q.empty() && q.top().endd<cow[i].start)//如果牛的挤奶时间开始值大于优先队列头部的结束值
        {
            num[cow[i].num]=num[q.top().num];//两头牛可共用一个栏位，记录序号
            q.pop();//从优先队列中删除这头牛
        }
        else
        {
            sum++;//栏位数+1
            num[cow[i].num]=sum;//记录栏位号
        }
        q.push(cow[i]);
    }

    printf("%d\n",sum);
    for(int i=1;i<=n;i++)
        printf("%d\n",num[i]);

    return 0;
}
```






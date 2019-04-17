# City Skyline（POJ-3044） - Alex_McAvoy的博客 - CSDN博客





2018年07月26日 01:58:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52
个人分类：[POJ																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The best part of the day for Farmer John's cows is when the sun sets. They can see the skyline of the distant city. Bessie wonders how many buildings the city has. Write a program that assists the cows in calculating the minimum number of buildings in the city, given a profile of its skyline. 

The city in profile is quite dull architecturally, featuring only box-shaped buildings. The skyline of a city on the horizon is somewhere between 1 and W units wide (1 <= W <= 1,000,000) and described using N (1 <= N <= 50,000) successive x and y coordinates (1 <= x <= W, 0 <= y <= 500,000), defining at what point the skyline changes to a certain height. 

An example skyline could be: 

.......................... 

.....XX.........XXX....... 

.XXX.XX.......XXXXXXX..... 

XXXXXXXXXX....XXXXXXXXXXXX 

and would be encoded as (1,1), (2,2), (5,1), (6,3), (8,1), (11,0), (15,2), (17,3), (20,2), (22,1). 

This skyline requires a minimum of 6 buildings to form; below is one possible set of six buildings whose could create the skyline above: 

.......................... .......................... 

.....22.........333....... .....XX.........XXX....... 

.111.22.......XX333XX..... .XXX.XX.......5555555..... 

X111X22XXX....XX333XXXXXXX 4444444444....5555555XXXXX .......................... 

.....XX.........XXX....... 

.XXX.XX.......XXXXXXX..... 

XXXXXXXXXX....666666666666

# **Input**

* Line 1: Two space separated integers: N and W 

* Lines 2..N+1: Two space separated integers, the x and y coordinate of a point where the skyline changes. The x coordinates are presented in strictly increasing order, and the first x coordinate will always be 1.

# Output

* Line 1: The minimum number of buildings to create the described skyline.

# Sample Input

**10 261 12 25 16 38 111 015 217 320 222 1**

# Sample Output

**6**


题意：用坐标的形式给出一些楼房的高度，求楼房最小个数。

思路：

读了半天题图还是看不懂，看了下题解，原来给的是楼房正视图每次高度改变时的坐标，实质上是维护一个递增的单调栈。

先将 0 压栈，然后遍历所有高度，如果栈顶元素大于当前高度，说明栈顶元素所代表的高度的楼找到了，楼数+1，不断弹出栈顶元素，直到栈顶元素小于当前高度，如果栈顶元素小于当前高度的话，就将当前高度压栈，继续向下寻找。

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
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int x[N],y[N];
int main()
{
    int n,w;
    scanf("%d%d",&n,&w);

    for(int i=1;i<=n;i++)
        scanf("%d%d",&x[i],&y[i]);
    y[n+1]=0;

    stack<int> s;
    int cnt=0;
    s.push(0);
    for(int i=1;i<=n+1;i++)
    {
        while(!s.empty()&&s.top()>y[i])
        {
            s.pop();
            cnt++;
        }
        if(y[i]!=s.top())
            s.push(y[i]);
    }

    printf("%d\n",cnt);

    return 0;
}
```







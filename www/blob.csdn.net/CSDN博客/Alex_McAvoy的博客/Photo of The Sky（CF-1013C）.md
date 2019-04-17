# Photo of The Sky（CF-1013C） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 18:50:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Pavel made a photo of his favourite stars in the sky. His camera takes a photo of all points of the sky that belong to some rectangle with sides parallel to the coordinate axes.

Strictly speaking, it makes a photo of all points with coordinates (x,y), such that x1≤x≤x2 and y1≤y≤y2, where (x1,y1) and (x2,y2) are coordinates of the left bottom and the right top corners of the rectangle being photographed. The area of this rectangle can be zero.

After taking the photo, Pavel wrote down coordinates of nn of his favourite stars which appeared in the photo. These points are not necessarily distinct, there can be multiple stars in the same point of the sky.

Pavel has lost his camera recently and wants to buy a similar one. Specifically, he wants to know the dimensions of the photo he took earlier. Unfortunately, the photo is also lost. His notes are also of not much help; numbers are written in random order all over his notepad, so it's impossible to tell which numbers specify coordinates of which points.

Pavel asked you to help him to determine what are the possible dimensions of the photo according to his notes. As there are multiple possible answers, find the dimensions with the minimal possible area of the rectangle.

# Input

The first line of the input contains an only integer n (1≤n≤100000), the number of points in Pavel's records.

The second line contains 2⋅n integers a1, a2, ..., a2⋅n (1≤ai≤109), coordinates, written by Pavel in some order.

# Output

Print the only integer, the minimal area of the rectangle which could have contained all points from Pavel's records.

# Examples

**Input**

4

4 1 3 2 3 2 1 3

**Output**

1

**Input**

3

5 8 5 5 7 5

**Output**

0

————————————————————————————————————————————

题意：给一个整数 n 以及 2n 个整数，要求任意排成 n 个坐标，求包含这 n 个坐标的矩形的面积的最小值

思路：

这个题说白了就求一个包含所有点的最小的矩形面积，就要考虑最左下角的坐标(x1,y1)与最右上角的坐标(xn,yn)，那么面积就是(xn-x1)*(yn-y1)

假设将横纵坐标各放在一集合中，则有两种情况：

当最大值和最小值在同一个集合时，要想使面积最小，就要使另一个集合里面的最大值和最小值差最小

当最大值和最小值不在同一个集合时，要想使面积最小，就要使最小值集合里的最大值和最小值的差尽量小，最大值集合里的最小值和最大值的差尽量小，然后相乘即可

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
#define N 200001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int main()
{
    int n;
    cin>>n;

    int sum1=0;
    for(int i=1;i<=2*n;i++)
        cin>>a[i];

    sort(a+1,a+n*2+1);

    int minn=INF;
    for(int i=2;i<=n;i++)//枚举第一种情况
        minn=min(minn,a[i+n-1]-a[i]);

    int x=(a[2*n]-a[n+1])*(a[n]-a[1]);//第二种情况
    int num=a[2*n]-a[1];
    x=min(x,num*minn);//第一、二种情况取最优
    cout<<x<<endl;

    return 0;
}
```







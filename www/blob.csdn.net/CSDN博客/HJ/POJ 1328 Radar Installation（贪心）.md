# POJ 1328 Radar Installation（贪心） - HJ - CSDN博客
2017年08月03日 10:39:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：185
个人分类：[---------OJ---------																[====ACM====](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)
Description 
Assume the coasting is an infinite straight line. Land is in one side of coasting, sea in the other. Each small island is a point locating in the sea side. And any radar installation, locating on the coasting, can only cover d distance, so an island in the sea can be covered by a radius installation, if the distance between them is at most d.
We use Cartesian coordinate system, defining the coasting is the x-axis. The sea side is above x-axis, and the land side below. Given the position of each island in the sea, and given the distance of the coverage of the radar installation, your task is to write a program to find the minimal number of radar installations to cover all the islands. Note that the position of an island is represented by its x-y coordinates. 
![这里写图片描述](http://poj.org/images/1328_1.jpg)
Figure A Sample Input of Radar Installations
Input 
The input consists of several test cases. The first line of each case contains two integers n (1<=n<=1000) and d, where n is the number of islands in the sea and d is the distance of coverage of the radar installation. This is followed by n lines each containing two integers representing the coordinate of the position of each island. Then a blank line follows to separate the cases.
The input is terminated by a line containing pair of zeros
Output 
For each test case output one line consisting of the test case number followed by the minimal number of radar installations needed. “-1” installation means no solution for that case.
Sample Input
3 2 
1 2 
-3 1 
2 1
1 2 
0 2
0 0
Sample Output
Case 1: 2 
Case 2: 1
题目大意： 
    x轴为海洋和陆地的分界线（假设x轴无限长），海洋上面有n个小岛，现在你可以在陆地上安装一些雷达，每个雷达能够探测的范围是一个半径为d的圆。问，至少在陆地上安装多少个雷达， 才能把所有的小岛探测到？
解题思路： 
用贪心的思想，雷达安在x轴上是最优解（x轴是陆地的最靠北的地方）。 
然后把每个小岛以小岛为圆心，半径为d的圆与x轴的左右两个相交的横坐标的交点值求出来。按照左交点的x值从小到大排序。
考虑用最少的次数，把所有的左焦点的值包括在内。这个最少次数就是所求值
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
struct node{
    double l,r;
}s[1005],k;
double cmp(node x,node y)
{
    return x.l<y.l;
}
int main()
{
    int n,d,a,b,tt=0;
    while(cin>>n>>d)
    {
        if(n==0&&d==0)
          break;
        int blag=0;
        for(int i=0;i<n;i++)
        {
            cin>>a>>b;
            if(b>d)
              blag=1;
            else                       //计算出以每一个点为圆心d的值为半径与x轴的左右两个交点坐标 
            {
                s[i].l=a*1.0-sqrt(d*d-b*b);    
                s[i].r=a*1.0+sqrt(d*d-b*b);
            }
        }
        if(blag)
          printf("Case %d: -1\n",++tt);
        else
        {
            sort(s,s+n,cmp);
            int ans=1;
            k=s[0]; 
            for(int i=1;i<n;i++)               //每次贪心比较，更新k的左右两个点的范围 
            {
                if(k.r<s[i].l)            //如果在范围之外，重新赋值 雷达个数+1 
                {
                    ans++;
                    k=s[i];
                }
                else if(k.r>s[i].r)          //如果在范围之内，更新k的范围  因为s[i].x<k.x 因此可以直接更新 
                  k=s[i];
            }
            printf("Case %d: %d\n",++tt,ans);
        }
    }
    return 0;
}
```

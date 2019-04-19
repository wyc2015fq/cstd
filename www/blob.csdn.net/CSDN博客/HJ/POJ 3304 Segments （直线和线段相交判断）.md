# POJ 3304 Segments （直线和线段相交判断） - HJ - CSDN博客
2017年01月14日 19:57:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：349
Description
Given n segments in the two dimensional space, write a program, which determines if there exists a line such that after projecting these segments on it, all projected segments have at least one point in common.
Input
Input begins with a number T showing the number of test cases and then, T test cases follow. Each test case begins with a line containing a positive integer n ≤ 100 showing the number of segments. After that, n lines containing four real numbers x1 y1 x2 y2 follow, in which (x1, y1) and (x2, y2) are the coordinates of the two endpoints for one of the segments.
Output
For each test case, your program must output “Yes!”, if a line with desired property exists and must output “No!” otherwise. You must assume that two floating point numbers a and b are equal if |a - b| < 10-8.
Sample Input
3 
2 
1.0 2.0 3.0 4.0 
4.0 5.0 6.0 7.0 
3 
0.0 0.0 0.0 1.0 
0.0 1.0 0.0 2.0 
1.0 1.0 2.0 1.0 
3 
0.0 0.0 0.0 1.0 
0.0 2.0 0.0 3.0 
1.0 1.0 2.0 1.0
Sample Output
Yes! 
Yes! 
No!
Source 
Amirkabir University of Technology Local Contest 2006
题目大意： 
    给出n条线段两个端点的坐标，问所有线段投影到一条直线上，如果这些所有投影至少相交于一点就输出Yes！，否则输出No！。 
解题思路： 
    如果有存在这样的直线，过投影相交区域作直线的垂线，该垂线必定与每条线段相交，问题转化为问是否存在一条线和所有线段相交
直线肯定经过两个端点。 
枚举端点，判断直线和线段是否相交。 
注意是枚举所有的两个端点，包括同一条直线的。
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
#define eps 1e-8
int m;
struct point{
    double x,y;
};
struct node{
    point a,b;
}s[105];
double multi(point p0,point p1,point p2)
{
    return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}
int Check(point a,point b)
{
    if(fabs(a.x-b.x)<eps&&fabs(a.y-b.y)<eps)
      return 0;
    for(int i=0;i<m;i++)
      if(multi(s[i].a,b,a)*multi(s[i].b,b,a)>eps)
        return 0;
    return 1;
}
int main()
{
    int n;
    cin>>n;
    while(n--)
    {
        cin>>m;
        for(int i=0;i<m;i++)
          cin>>s[i].a.x>>s[i].a.y>>s[i].b.x>>s[i].b.y;
        int blag=0;
        if(m<3)
          blag=1;
        for(int i=0;i<m&&!blag;i++)
        {
            if(Check(s[i].a,s[i].b))
              blag=1;
            for(int j=i+1;j<m&&!blag;j++)
            {
                if(Check(s[i].a,s[j].a)||Check(s[i].a,s[j].b)||Check(s[i].b,s[j].a)||Check(s[i].b,s[j].b))
                  blag=1;
            }
        }
        if(blag)
          printf("Yes!\n");
        else
          printf("No!\n");
    }
    return 0;
}
```

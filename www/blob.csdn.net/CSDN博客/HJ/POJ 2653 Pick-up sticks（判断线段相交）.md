# POJ 2653 Pick-up sticks（判断线段相交） - HJ - CSDN博客
2017年01月14日 20:06:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：239
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------计算几何--------																[点和直线的关系](https://blog.csdn.net/feizaoSYUACM/article/category/7238016)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
Stan has n sticks of various length. He throws them one at a time on the floor in a random way. After finishing throwing, Stan tries to find the top sticks, that is these sticks such that there is no stick on top of them. Stan has noticed that the last thrown stick is always on top but he wants to know all the sticks that are on top. Stan sticks are very, very thin such that their thickness can be neglected.
Input 
Input consists of a number of cases. The data for each case start with 1 <= n <= 100000, the number of sticks for this case. The following n lines contain four numbers each, these numbers are the planar coordinates of the endpoints of one stick. The sticks are listed in the order in which Stan has thrown them. You may assume that there are no more than 1000 top sticks. The input is ended by the case with n=0. This case should not be processed.
Output 
For each input case, print one line of output listing the top sticks in the format given in the sample. The top sticks should be listed in order in which they were thrown.
The picture to the right below illustrates the first case from input.
Sample Input
5 
1 1 4 2 
2 3 3 1 
1 -2.0 8 4 
1 4 8 2 
3 3 6 -2.0 
3 
0 0 1 1 
1 0 2 1 
2 0 3 1 
0
Sample Output
Top sticks: 2, 4, 5. 
Top sticks: 1, 2, 3.
Hint 
Huge input,scanf is recommended.
Source 
Waterloo local 2005.09.17
题目大意： 
    枚举每条线段，如果它上面没有和它相交的，则输出它的序号
```
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
#define eps 1e-8
int vis[100001],num[100001];
struct point{
    double x,y;
}p[100001],q[100001];
double m(point a,point p1,point p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
bool judge(point u1,point u2,point v1,point v2)
{
    if(min(u1.x,u2.x)>max(v1.x,v2.x)||
       min(u1.y,u2.y)>max(v1.y,v2.y)||
       min(v1.x,v2.x)>max(u1.x,u2.x)||
       min(v1.y,v2.y)>max(u1.y,u2.y))
      return false;                                         //快速排斥试验 
//  double a,b,c,d;
//  a=(u2.x-u1.x)*(v1.y-u1.y)-(u2.y-u1.y)*(v1.x-u1.x);
//    b=(u2.x-u1.x)*(v2.y-u1.y)-(u2.y-u1.y)*(v2.x-u1.x);
//    c=(v2.x-v1.x)*(u1.y-v1.y)-(v2.y-v1.y)*(u1.x-v1.x);
//    d=(v2.x-v1.x)*(u2.y-v1.y)-(v2.y-v1.y)*(u2.x-v1.x);
//  return a*b<=eps&&c*d<=eps;                             //跨立试验 
    return m(u1,u2,v1)*m(u1,u2,v2)<=eps&&m(v1,v2,u1)*m(v1,v2,u2)<=eps; 
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        if(n==0)
          break;
        memset(vis,0,sizeof(vis));
        for(int i=1;i<=n;i++)
          scanf("%lf%lf%lf%lf",&p[i].x,&p[i].y,&q[i].x,&q[i].y);
        for(int i=1;i<=n;i++)
          for(int j=i+1;j<=n;j++)
            if(judge(p[i],q[i],p[j],q[j]))
            {
                vis[i]=1;
                break;
            }
        printf("Top sticks:");
        for(int i=1;i<n;i++)
          if(!vis[i]) 
            printf(" %d,",i);
        printf(" %d.\n",n);
    }
    return 0;
}
```

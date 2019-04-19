# hdu  1007 Quoit Design（最近点对） - HJ - CSDN博客
2017年05月03日 20:52:38[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：239
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------计算几何--------																[最近点对](https://blog.csdn.net/feizaoSYUACM/article/category/7238019)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Have you ever played quoit in a playground? Quoit is a game in which flat rings are pitched at some toys, with all the toys encircled awarded. 
In the field of Cyberground, the position of each toy is fixed, and the ring is carefully designed so it can only encircle one toy at a time. On the other hand, to make the game look more attractive, the ring is designed to have the largest radius. Given a configuration of the field, you are supposed to find the radius of such a ring.
Assume that all the toys are points on a plane. A point is encircled by the ring if the distance between the point and the center of the ring is strictly less than the radius of the ring. If two toys are placed at the same point, the radius of the ring is considered to be 0.
Input 
The input consists of several test cases. For each case, the first line contains an integer N (2 <= N <= 100,000), the total number of toys in the field. Then N lines follow, each contains a pair of (x, y) which are the coordinates of a toy. The input is terminated by N = 0.
Output 
For each test case, print in one line the radius of the ring required by the Cyberground manager, accurate up to 2 decimal places.
Sample Input
2 
0 0 
1 1 
2 
1 1 
1 1 
3 
-1.5 0 
0 0 
0 1.5 
0
Sample Output
0.71 
0.00 
0.75
裸的最近点对题目。
详细介绍：[http://blog.csdn.net/lonelycatcher/article/details/7973046/](http://blog.csdn.net/lonelycatcher/article/details/7973046/)
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
#define N 100005
struct point{
    double x,y;
}s[N];
int a[N];
double dis(point a,point b)   //两点间距离 
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool cmp_y(int a,int b)
{
    return s[a].y<s[b].y;
}
bool cmp_x(point a,point b)
{
    return a.x<b.x;
}
double closest_pair(int l,int r)        //求最近点对 
{
    if(r==l+1)          //两个点的情况 
      return dis(s[l],s[r]);
    else if(r==l+2)        //三个点的情况 
      return min(dis(s[l],s[r]),min(dis(s[l],s[l+1]),dis(s[l+1],s[r])));
    int mid=(l+r)/2,cnt=0;
    double ans=min(closest_pair(l,mid),closest_pair(mid+1,r));
    for(int i=l;i<=r;i++)
      if(fabs(s[i].x-s[mid].x)<=ans)    
        a[cnt++]=i;       //把所有范围在[s[mid].x-ans,s[mid].x+ans]之内的点都记录下来 
    sort(a,a+cnt,cmp_y);         //对a数组内点按照纵坐标从小到大排序 
    for(int i=0;i<cnt;i++)
      for(int j=i+1;j<cnt&&fabs(s[a[j]].y-s[a[i]].y)<ans;j++)
        ans=min(ans,dis(s[a[i]],s[a[j]]));    //将符合条件的点对距离值跟ans不断比较，更新ans 
    return ans;
}
int main()
{
    int n;
    while(~scanf("%d",&n)&&n)
    {
        for(int i=0;i<n;i++)
          scanf("%lf%lf",&s[i].x,&s[i].y);
        sort(s,s+n,cmp_x);      //首先将点按照横坐标的从小到大排序 
        printf("%.2lf\n",closest_pair(0,(n-1))/2.0);
    }
    return 0;
}
```

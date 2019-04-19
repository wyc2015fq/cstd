# POJ 3714 Raid（最近点对） - HJ - CSDN博客
2017年05月03日 20:56:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：452
Description
After successive failures in the battles against the Union, the Empire retreated to its last stronghold. Depending on its powerful defense system, the Empire repelled the six waves of Union’s attack. After several sleepless nights of thinking, Arthur, General of the Union, noticed that the only weakness of the defense system was its energy supply. The system was charged by N nuclear power stations and breaking down any of them would disable the system.
The general soon started a raid to the stations by N special agents who were paradroped into the stronghold. Unfortunately they failed to land at the expected positions due to the attack by the Empire Air Force. As an experienced general, Arthur soon realized that he needed to rearrange the plan. The first thing he wants to know now is that which agent is the nearest to any power station. Could you, the chief officer, help the general to calculate the minimum distance between an agent and a station?
Input
The first line is a integer T representing the number of test cases. 
Each test case begins with an integer N (1 ≤ N ≤ 100000). 
The next N lines describe the positions of the stations. Each line consists of two integers X (0 ≤ X ≤ 1000000000) and Y (0 ≤ Y ≤ 1000000000) indicating the positions of the station. 
The next following N lines describe the positions of the agents. Each line consists of two integers X (0 ≤ X ≤ 1000000000) and Y (0 ≤ Y ≤ 1000000000) indicating the positions of the agent. 　
Output
For each test case output the minimum distance with precision of three decimal placed in a separate line.
Sample Input
2 
4 
0 0 
0 1 
1 0 
1 1 
2 2 
2 3 
3 2 
3 3 
4 
0 0 
0 0 
0 0 
0 0 
0 0 
0 0 
0 0 
0 0
Sample Output
1.414 
0.000
稍微有点不一样。求的是两个集合内找出两个距离最短的点（要求距离最短的两个点必须属于两个不同的集合）
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
#define N 200005
#define INF 1e100 
struct point{
    double x,y;
    int blag;    
}s[N];
int a[N];
double dis(point a,point b)
{
    if(a.blag==b.blag)    //如果两个点属于同一个集合，距离返回无穷大 
      return INF; 
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool cmpy(int a,int b)
{
    return s[a].y<s[b].y;
}
bool cmpx(point a,point b)
{
    return a.x<b.x;
}
double closest_pair(int l,int r)
{
    if(r==l+1)
      return dis(s[l],s[r]);
    if(r==l+2)
      return min(dis(s[l],s[r]),min(dis(s[l],s[l+1]),dis(s[l+1],s[r])));
    int mid=(l+r)/2,cnt=0;
    double ans=min(closest_pair(l,mid),closest_pair(mid+1,r));
    for(int i=l;i<=r;i++)
      if(fabs(s[i].x-s[mid].x)<=ans)
        a[cnt++]=i;
    sort(a,a+cnt,cmpy);
    for(int i=0;i<cnt;i++)
      for(int j=i+1;j<cnt&&s[a[j]].y-s[a[i]].y<ans;j++)
          ans=min(ans,dis(s[a[i]],s[a[j]]));
    return ans;
}
int main()
{
    int t,n;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        for(int i=0;i<n;i++)
        {
            scanf("%lf%lf",&s[i].x,&s[i].y);
            s[i].blag=0;     //前一半属于集合A   用0表示 
        }
        for(int i=0;i<n;i++)
        {
            scanf("%lf%lf",&s[i+n].x,&s[i+n].y);
            s[i+n].blag=1;    //后一半属于集合B   用1表示 
        }
        n*=2;
        sort(s,s+n,cmpx);
        printf("%.3lf\n",closest_pair(0,n-1));
    }
    return 0;
}
```

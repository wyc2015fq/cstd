# hdu 5120 Intersection（求圆面积交） - HJ - CSDN博客
2018年08月12日 19:59:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：54
[http://acm.hdu.edu.cn/showproblem.php?pid=5120](http://acm.hdu.edu.cn/showproblem.php?pid=5120)
简单的模板题。最后答案为
S（两个大圆） + S（两个小圆） - S（小圆1与大圆2） -  S（小圆2与大圆1）
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
#define mod 1000000007
const double PI=acos(-1.0);  
const int maxn=1e5+10;
struct Round{
    double x,y;
    double r;
}s[4];
double dis(Round a,Round b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double solve(Round a,Round b)
{
    double d=dis(a,b);
    if(d>=a.r+b.r)
      return 0;
    if(d<=abs(a.r-b.r))
    {
        double r=a.r<b.r?a.r:b.r;
        return PI*r*r; 
    }
    double ang1=acos((a.r*a.r+d*d-b.r*b.r)/2.0/a.r/d);
    double ang2=acos((b.r*b.r+d*d-a.r*a.r)/2.0/b.r/d);
    double ret=ang1*a.r*a.r+ang2*b.r*b.r-d*a.r*sin(ang1);
    return ret;
}
int main()
{
    int t,tt=0;
    scanf("%d",&t);
    while(t--)
    {
        double r,R;
        //s[0] 小圆a 
        //s[1] 大圆a 
        //s[2] 小圆b
        //s[3] 大圆b 
        scanf("%lf%lf",&s[0].r,&s[1].r);
        scanf("%lf%lf%lf%lf",&s[0].x,&s[0].y,&s[2].x,&s[2].y);
        s[2].r=s[0].r,s[3].r=s[1].r;
        s[1].x=s[0].x,s[1].y=s[0].y;
        s[3].x=s[2].x,s[3].y=s[2].y;
        double res=solve(s[1],s[3])+solve(s[0],s[2])-solve(s[0],s[3])-solve(s[1],s[2]);
        printf("Case #%d: %.6lf\n",++tt,res);
    } 
    return 0;
}
```

# FZU  2273 Triangles（判断线段相交和点在多变形内） - HJ - CSDN博客
2017年08月23日 16:39:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：256
[http://acm.fzu.edu.cn/problem.php?pid=2273](http://acm.fzu.edu.cn/problem.php?pid=2273)
解题思路： 
先判断两个三角形是否存在一些点在另一个三角形内。如果有，则它们相交，否则判断它们线段是否相交（具体可以看看六芒星的特例），如果不相交，则相离，否则包含。
判断线段相交的模板：
```
bool Judge(point u1,point u2,point v1,point v2)        //判断两条线段相交情况 
{
   return (max(u1.x,u2.x)>=min(v1.x,v2.x)&&
           max(u1.y,u2.y)>=min(v1.y,v2.y)&&
           max(v1.x,v2.x)>=min(u1.x,u2.x)&&
           max(v1.y,v2.y)>=min(u1.y,u2.y)&&
           m(u1,v1,u2)*m(u1,u2,v2)>=0&&
           m(v1,u1,v2)*m(v1,v2,u2)>=0);
}
```
面积法判断点是否三角形内（可以推广到多边形）
```cpp
int point_check(int p,int q)        // 面积法判断点在三角形内
{
    double res=fabs(m(s[q][0],s[q][1],s[q][2])); 
    int ans=0;
    for(int i=0;i<3;i++)
    {
        double res1=fabs(m(s[q][0],s[q][1],s[p][i]));
        double res2=fabs(m(s[q][1],s[q][2],s[p][i]));
        double res3=fabs(m(s[q][0],s[q][2],s[p][i]));
        if(res1+res2+res3==res)
          ans++;
    }
    return ans;
}
```
吐槽一下：题目后台数据应该比较水！而且题目意思也不太明了（比如两个三角形如果有一条边重合或者两个三角形重合时相交还是包含呢？）
完整代码：
```cpp
#include<iostream>
#include<cmath>
using namespace std;
struct point{
    int x,y;
}s[5][5];
double m(point a,point b,point c)            //叉积 
{
    return ((b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x));
}
bool Judge(point u1,point u2,point v1,point v2)        //判断两条线段相交情况 
{
   return (max(u1.x,u2.x)>=min(v1.x,v2.x)&&
           max(u1.y,u2.y)>=min(v1.y,v2.y)&&
           max(v1.x,v2.x)>=min(u1.x,u2.x)&&
           max(v1.y,v2.y)>=min(u1.y,u2.y)&&
           m(u1,v1,u2)*m(u1,u2,v2)>=0&&
           m(v1,u1,v2)*m(v1,v2,u2)>=0);
}
int line_check(int p,int q)                     //判断两个三角形是否相交 
{
    return (Judge(s[p][0],s[p][1],s[q][0],s[q][1])||
           Judge(s[p][0],s[p][1],s[q][1],s[q][2])||
           Judge(s[p][0],s[p][1],s[q][0],s[q][2])||
           Judge(s[p][1],s[p][2],s[q][0],s[q][1])||
           Judge(s[p][1],s[p][2],s[q][1],s[q][2])||
           Judge(s[p][1],s[p][2],s[q][0],s[q][2])||
           Judge(s[p][0],s[p][2],s[q][0],s[q][1])||
           Judge(s[p][0],s[p][2],s[q][1],s[q][2])||
           Judge(s[p][0],s[p][2],s[q][0],s[q][2]));
} 
int point_check(int p,int q)        // 面积法判断点在三角形内
{
    double res=fabs(m(s[q][0],s[q][1],s[q][2])); 
    int ans=0;
    for(int i=0;i<3;i++)
    {
        double res1=fabs(m(s[q][0],s[q][1],s[p][i]));
        double res2=fabs(m(s[q][1],s[q][2],s[p][i]));
        double res3=fabs(m(s[q][0],s[q][2],s[p][i]));
        if(res1+res2+res3==res)
          ans++;
    }
    return ans;
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        for(int i=0;i<2;i++)
          for(int j=0;j<3;j++)
            cin>>s[i][j].x>>s[i][j].y;
        int ans1=point_check(0,1),ans2=point_check(1,0);
        if(ans1==0&&ans2==0)                     //如果两个三角形没有一个点在另一个三角形内 
        {
            int ans=line_check(0,1);
            if(ans==0)
              cout<<"disjoint"<<endl;       // 相离
            else
               cout<<"intersect"<<endl;        //相交
        }         
        else if(ans1==3||ans2==3)
          cout<<"contain"<<endl;                //否则包含 
        else 
          cout<<"intersect"<<endl;        //相交 
    }
    return 0;
}
```

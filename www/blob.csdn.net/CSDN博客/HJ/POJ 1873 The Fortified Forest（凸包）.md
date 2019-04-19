# POJ 1873 The Fortified Forest（凸包） - HJ - CSDN博客
2017年08月10日 18:53:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：180
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------计算几何--------																[凸包](https://blog.csdn.net/feizaoSYUACM/article/category/7238021)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://poj.org/problem?id=1873](http://poj.org/problem?id=1873)
题目大意： 
给你n棵树每棵树的坐标，高度，价值，要求砍掉一些树，用那些木材，将其它树围起来，要求花最小的代价，代价相同，要求砍掉最少的树。
解题思路： 
枚举2^15的所有情况，算出最少的代价即可。
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<vector>
#include<cmath>
using namespace std;
#define INF 0x3f3f3f3f
#define maxn 55
#define eps 1e-8
struct point{
    int x,y,v,l;
}p[maxn];
int n;
vector<point> a;
int multi(point p0,point p1,point p2)       //叉积 
{
    return (p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
} 
double dot(point p1,point p2)                     //点积 
{ 
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); 
}
bool cmp(point p1,point p2)                 //极角排序 
{
    if(multi(a[0],p1,p2)>0)
      return true;
    else if(multi(a[0],p1,p2)==0&&dot(a[0],p1)<dot(a[0],p2))
      return true;
    return false;
}
double Grham_scan(int len)              //求凸包周长 
{
    if(a.size()==1)
      return len;
    else if(a.size()==2)
      return len-2*dot(a[0],a[1]);
    for(int i=1;i<a.size();i++)
      if(a[i].y<a[0].y||(a[i].y==a[0].y&&a[i].x<a[0].x))
        swap(a[i],a[0]);
    sort(a.begin()+1,a.end(),cmp);
    vector<point> s;
    s.push_back(a[0]),s.push_back(a[1]),s.push_back(a[2]);
    for(int i=3;i<a.size();i++) 
    {
        while(s.size()>=2&&multi(s[s.size()-2],s[s.size()-1],a[i])<eps)         //凸包判断操作 
          s.pop_back();
        s.push_back(a[i]);
    }
    s.push_back(s[0]);
    double ans=0;
    for(int i=0;i<s.size()-1;i++)
      ans+=dot(s[i],s[i+1]);
    return len-ans;  
}
int main()
{
    int t=0;
    while(scanf("%d",&n)&&n)
    {
        for(int i=0;i<n;i++)
          scanf("%d%d%d%d",&p[i].x,&p[i].y,&p[i].v,&p[i].l);
        int best_val=INF,best_num,best_state;
        double best_extra;
        for(int i=1;i<(1<<n)-1;i++)          //枚举2^15种情况 
        {
            int tmp_val=0,tmp_len=0;
            a.clear();
            for(int j=0;j<n;j++)
            {
                if(i&(1<<j))                   //砍下来的树木 
                {
                    tmp_len+=p[j].l;
                    tmp_val+=p[j].v; 
                }
                else                    //保留着的树木 
                  a.push_back(p[j]);
            } 
            if(tmp_val>best_val)               //如果代价已经比最小代价高 可以直接跳过求凸包 
              continue;
            double extra=Grham_scan(tmp_len);
            if(extra>=0)              //围着凸包周长的长度如果够用 
            {
                if(tmp_val<best_val)              //代价更小 
                {
                    best_val=tmp_val;
                    best_state=i;
                    best_num=n-a.size();
                    best_extra=extra;
                } 
                else if(tmp_val==best_val&&n-a.size()<best_num)      //相同代价 但砍掉的树更少 
                {
                    best_state=i;
                    best_num=n-a.size();
                    best_extra=extra;
                } 
            }
        }           
        printf("Forest %d\nCut these trees:",++t);  
        for(int i=0;i<n;i++)  
          if((1<<i)&best_state)  
            printf(" %d",i+1);  
        printf("\nExtra wood: %.2f\n\n",best_extra);  
    } 
    return 0;
}
```

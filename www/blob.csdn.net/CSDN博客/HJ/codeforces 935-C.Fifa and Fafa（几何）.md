# codeforces 935-C.Fifa and Fafa（几何） - HJ - CSDN博客
2018年08月14日 11:14:12[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：57
个人分类：[====ACM====																[--------计算几何--------																[点和直线的关系](https://blog.csdn.net/feizaoSYUACM/article/category/7238016)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://codeforces.com/problemset/problem/935/C](http://codeforces.com/problemset/problem/935/C)
题目大意：
给出一个圆的圆心和半径，另外再给出一个点，问在这个圆内求一个最大的圆保证这个点不在这个圆内（可以在圆上），求圆的圆心和半径。
解题思路：
利用相似三角形，先判断圆心和点的距离大于半径和两点重合的情况，再结合相似求出圆心即可。
```cpp
#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
#include<set>
#include<cstring>
#include<cstdio>
#include<cmath>
using namespace std;
#define mod 1000000007
typedef long long ll;
const int maxn=1e5+10;
int main()
{
    double R,x1,x2,y1,y2;
    while(cin>>R>>x1>>y1>>x2>>y2)
    {
        double dist=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
        if(dist>=R*R)
          printf("%lf %lf %lf\n",x1,y1,R);
        else if(dist==0)
          printf("%lf %lf %lf\n",x1+R/2.0,y1,R/2.0);
        else
        {
            double r=(sqrt(dist)+R)/2.0;
            double x3=x1<x2?(x2-(x2-x1)*r/(sqrt(dist)*1.0)):(x2+(x1-x2)*r/(sqrt(dist)*1.0));
            double y3=y1<y2?(y2-(y2-y1)*r/(sqrt(dist)*1.0)):(y2+(y1-y2)*r/(sqrt(dist)*1.0)); 
            printf("%lf %lf %lf\n",x3,y3,r);
        }
    }
    return 0;
}
```

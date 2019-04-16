# ZOJ - 1450 Minimal Circle  HDU - 3007 Buried memory 最小圆覆盖模板 【随机函数】【增量法】 - 紫芝的博客 - CSDN博客





2018年11月23日 10:07:28[紫芝](https://me.csdn.net/qq_40507857)阅读数：74








## 题意

 给N个点，求最小的圆将这N个点全部覆盖，输出圆心坐标和半径

## 分析

最小的圆肯定落在三个点上，因此暴力枚举圆上的三个点即可，点增量算法O(n ^ 3),加入随机化,平均复杂度可以降到O(n^2)

```cpp
#include<iostream>
#include<map>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<set>
#include<sstream>
#include<cstdio>
#include<cmath>
#include<climits>
#include<cstdlib>
using namespace std;
//ZOJ1450 最小圆覆盖
struct POINT{
    double x,y;
}p[110];
int n;
inline double dist(POINT &a,POINT &b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
//求三点外接圆的圆心
POINT circumcenter(POINT &a,POINT &b,POINT &c){
    POINT ret;
    double a1=b.x-a.x,b1=b.y-a.y,c1=(a1*a1+b1*b1)/2;
    double a2=c.x-a.x,b2=c.y-a.y,c2=(a2*a2+b2*b2)/2;
    double d=a1*b2-a2*b1;
    ret.x=a.x+(c1*b2-c2*b1)/d;
    ret.y=a.y+(a1*c2-a2*c1)/d;
    return ret;
}
//最小的圆肯定落在三个点上，因此暴力枚举圆上的三个点即可
//点增量算法O(n^3),加入随机化,平均复杂度可以降到O(n^2)
void solve(){
    random_shuffle(p,p+n);//随机化序列，std里的随机函数
    POINT c;
    double r=0;
    for(int i=1;i<n;i++){//第一层
        if(dist(p[i],c)<=r) continue;
        c=p[i];
        r=0;
        for(int j=0;j<i;j++){//第二层
            if(dist(p[j],c)<=r) continue;
            c.x=(p[i].x+p[j].x)/2;
            c.y=(p[i].y+p[j].y)/2;
            r=dist(p[j],c);
            for(int k=0;k<j;k++){//第三层
                if(dist(p[k],c)<=r) continue;
                c=circumcenter(p[i],p[j],p[k]);
                r=dist(p[i],c);
            }
        }
    }
    printf("%.2lf %.2lf %.2lf\n",c.x,c.y,r);
}
int main(){
    while(scanf("%d",&n)!=EOF&&n){
        for(int i=0;i<n;i++){
            scanf("%lf%lf",&p[i].x,&p[i].y);
        }
        solve();
    }
    return 0;
}
```




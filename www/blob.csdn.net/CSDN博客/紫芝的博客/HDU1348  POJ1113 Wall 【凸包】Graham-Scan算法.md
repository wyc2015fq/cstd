# HDU1348 / POJ1113 Wall 【凸包】Graham-Scan算法 - 紫芝的博客 - CSDN博客





2018年10月31日 15:12:22[紫芝](https://me.csdn.net/qq_40507857)阅读数：27








## Wall

Once upon a time there was a greedy King who ordered his chief Architect to build a wall around the King's castle. The King was so greedy, that he would not listen to his Architect's proposals to build a beautiful brick wall with a perfect shape and nice tall towers. Instead, he ordered to build the wall around the whole castle using the least amount of stone and labor, but demanded that the wall should not come closer to the castle than a certain distance. If the King finds that the Architect has used more resources to build the wall than it was absolutely necessary to satisfy those requirements, then the Architect will loose his head. Moreover, he demanded Architect to introduce at once a plan of the wall listing the exact amount of resources that are needed to build the wall. 

Your task is to help poor Architect to save his head, by writing a program that will find the minimum possible length of the wall that he could build around the castle to satisfy King's requirements. 

![](https://vj.e949.cn/a1a8c07239b01a6ef723f1b5390a3672?v=1540957471)

The task is somewhat simplified by the fact, that the King's castle has a polygonal shape and is situated on a flat ground. The Architect has already established a Cartesian coordinate system and has precisely measured the coordinates of all castle's vertices in feet. 

Input

The first line of the input file contains two integer numbers N and L separated by a space. N (3 <= N <= 1000) is the number of vertices in the King's castle, and L (1 <= L <= 1000) is the minimal number of feet that King allows for the wall to come close to the castle. 


Next N lines describe coordinates of castle's vertices in a clockwise order. Each line contains two integer numbers Xi and Yi separated by a space (-10000 <= Xi, Yi <= 10000) that represent the coordinates of ith vertex. All vertices are different and the sides of the castle do not intersect anywhere except for vertices. 

Output

Write to the output file the single number that represents the minimal possible length of the wall in feet that could be built around the castle to satisfy King's requirements. You must present the integer number of feet to the King, because the floating numbers are not invented yet. However, you must round the result in such a way, that it is accurate to 8 inches (1 foot is equal to 12 inches), since the King will not tolerate larger error in the estimates. 


This problem contains multiple test cases! 


The first line of a multiple input is an integer N, then a blank line followed by N input blocks. Each input block is in the format indicated in the problem description. There is a blank line between input blocks. 


The output format consists of N output blocks. There is a blank line between output blocks. 

Sample Input

1

9 100
200 400
300 400
300 300
400 300
400 400
500 400
500 200
350 200
200 200
Sample Output

1628
### 题意：

**给N个点和圆的半径，求N个点构成的凸包的周长以及圆的周长**

```cpp
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <math.h>

#define ll long long
using namespace std;
const int maxn=1009;
const double PI=acos(-1.0);
struct point{
    double x,y;
};
int n;
double R;
point p[maxn];//N个点

int tot;//凸包点的个数
point poly[maxn];//凸包的点，编号：0~tot；

//向量AB和向量AC的叉积
double cross(point a,point b,point c)
{
    return (b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
}

double dist(point a,point b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

//根据P[0]进行极角排序函数
//P[0]为最左下角的点
bool cmp(point a,point b){
    double pp=cross(p[0],a,b);
    if(pp>0)    return true;
    if(pp<0)    return false;
    return dist(p[0],a)<dist(p[0],b);
}

void Graham_scan(){
 //P[0]为Y轴最小的点
    for(int i=1;i<n;i++){
        if(p[i].y<p[0].y)
            swap(p[i],p[0]);
        if(p[i].y==p[0].y&&p[i].x<p[0].x)
            swap(p[i],p[0]);
    }
    //printf("%lf %lf\n",p[0].x,p[0].y);
    sort(p+1,p+n,cmp);//不包含第一个点
    poly[0]=p[0];
    poly[1]=p[1];
    tot=1;
    for(int i=2;i<n;i++){
        while(tot>0&&cross(poly[tot-1],poly[tot],p[i])<=0)
        tot--;//模拟栈
            tot++;
        poly[tot]=p[i];
    }
}
int main()
{

    int T;
    int ca=1;
    //scanf("%d",&T);
    while(scanf("%d%lf",&n,&R)!=EOF){
        /*
        if(ca!=1)   printf("\n");
        ca++;
        */
        double ans=PI*R*2;//圆的周长
        for(int i=0;i<n;i++){
            scanf("%lf%lf",&p[i].x,&p[i].y);
        }
        if(n==1){
            printf("%.0f\n",ans);
            continue;
        }

        if(n==2){//直线
            printf("%.0f\n",ans+dist(p[0],p[1]));
            continue;
        }

        Graham_scan();//求凸包
        //printf("%d\n",tot);
        //求凸包的周长
        for(int i=0;i<tot;i++)
            ans+=dist(poly[i],poly[i+1]);

        ans+=dist(poly[0],poly[tot]);
        printf("%.0f\n",ans);
    }
}
```






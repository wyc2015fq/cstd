# POJ 1113 Wall （凸包） - HJ - CSDN博客
2017年01月14日 20:19:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：234
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------计算几何--------																[凸包](https://blog.csdn.net/feizaoSYUACM/article/category/7238021)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
Once upon a time there was a greedy King who ordered his chief Architect to build a wall around the King’s castle. The King was so greedy, that he would not listen to his Architect’s proposals to build a beautiful brick wall with a perfect shape and nice tall towers. Instead, he ordered to build the wall around the whole castle using the least amount of stone and labor, but demanded that the wall should not come closer to the castle than a certain distance. If the King finds that the Architect has used more resources to build the wall than it was absolutely necessary to satisfy those requirements, then the Architect will loose his head. Moreover, he demanded Architect to introduce at once a plan of the wall listing the exact amount of resources that are needed to build the wall.
![这里写图片描述](http://poj.org/images/1113/1113_1.gif?_=2445633)
Your task is to help poor Architect to save his head, by writing a program that will find the minimum possible length of the wall that he could build around the castle to satisfy King’s requirements.
The task is somewhat simplified by the fact, that the King’s castle has a polygonal shape and is situated on a flat ground. The Architect has already established a Cartesian coordinate system and has precisely measured the coordinates of all castle’s vertices in feet.
Input 
The first line of the input file contains two integer numbers N and L separated by a space. N (3 <= N <= 1000) is the number of vertices in the King’s castle, and L (1 <= L <= 1000) is the minimal number of feet that King allows for the wall to come close to the castle.
Next N lines describe coordinates of castle’s vertices in a clockwise order. Each line contains two integer numbers Xi and Yi separated by a space (-10000 <= Xi, Yi <= 10000) that represent the coordinates of ith vertex. All vertices are different and the sides of the castle do not intersect anywhere except for vertices.
Output 
Write to the output file the single number that represents the minimal possible length of the wall in feet that could be built around the castle to satisfy King’s requirements. You must present the integer number of feet to the King, because the floating numbers are not invented yet. However, you must round the result in such a way, that it is accurate to 8 inches (1 foot is equal to 12 inches), since the King will not tolerate larger error in the estimates.
Sample Input
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
Hint 
结果四舍五入就可以了
Source 
Northeastern Europe 2001
题目大意： 
    给定多边形城堡的n个顶点，绕城堡外面建一个围墙，围住所有点，并且墙与所有点的距离至少为L，求这个墙最小的长度。
解法：
推导公式
```
城堡围墙长度最小值 = 城堡顶点坐标构成的散点集的凸包总边长 + 半径为L的圆周长
```
由于数据规模较大，必须用GrahamScan Algorithm构造凸包（详细的算法可以参考我的POJ2187，这里就不再啰嗦了），然后顺序枚举凸包相邻的两点并计算其距离，得到凸包的总边长，最后加上圆周长2πL。
根据圆形的性质,其实就相当于多加了一个r=L的圆，把该圆根据凸包的边数（假设有k条）划分为k段弧，分别用来连接凸包上所有边。这样做的目的就是为了在保证围墙距离城堡至少为L的同时，使得转角处为圆角而不是直角，减少建造围城所需的资源。
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
#define PI acos(-1.0)
int stack[1000],top;
struct point{
    int x,y;
}list[1000];
int multi(point a,point p1,point p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
double d(point a,point b)                              //两点间距离 
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool cmp(point p1,point p2)                            //从小到大极角排序 角度相同则按从近到远的顺序 
{
    int tmp=multi(list[0],p1,p2);
    if(tmp>0)
      return true;
    else if(tmp==0&&d(list[0],p1)<d(list[0],p2))
      return true;
    return false;
}
void init(int n)                                    //控制输入 找出最左下的点放在list[0]中 进行极角排序 
{
    int k=0;
    point p0;                                      //中间搜索变量，相当于t 
    scanf("%d%d",&list[0].x,&list[0].y);
    p0.x=list[0].x;
    p0.y=list[0].y;
    for(int i=1;i<n;i++)
    {
        scanf("%d%d",&list[i].x,&list[i].y);
        if(p0.y>list[i].y||(p0.y==list[i].y&&p0.x>list[i].x))          //找到纵坐标最小的点 纵坐标相同则取横坐标最小的点 
        {
            p0.x=list[i].x;
            p0.y=list[i].y;
            k=i;
        }
    }
    list[k]=list[0];
    list[0]=p0;
    sort(list+1,list+n,cmp);
}
void Graham(int n)
{
    stack[0]=0;stack[1]=1;
    if(n==1)
      top=0;
    else
    {
        top=1;                                                                       //至少保存三个点(p0,p1,p2) 
        for(int i=2;i<n;i++)                                                         //从开始往后寻找 
        {
            while(top>0&&multi(list[stack[top-1]],list[stack[top]],list[i])<=0)      //模拟栈功能  直到出现一个点满足顺时针即跳出循环 
              top--;
            stack[++top]=i;                                                           //找到这个点之后压入"栈"中 
        }
    }
}
int main()
{
    int n,l;
    while(scanf("%d %d",&n,&l)!=EOF)
    {
        init(n);
        Graham(n);
        double res=0;
        for(int i=0;i<top;i++)
          res+=d(list[stack[i]],list[stack[i+1]]);        //累加凸包周长 
        res+=d(list[stack[0]],list[stack[top]]);
        res+=2*PI*l;                                      //最后加上圆的周长 
        printf("%d\n",int(res+0.5));                      //保留整数的时候注意精度问题 
    }
    return 0;
}
```

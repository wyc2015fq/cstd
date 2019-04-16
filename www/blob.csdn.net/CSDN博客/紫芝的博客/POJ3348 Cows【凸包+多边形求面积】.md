# POJ3348 Cows【凸包+多边形求面积】 - 紫芝的博客 - CSDN博客





2018年11月01日 09:39:19[紫芝](https://me.csdn.net/qq_40507857)阅读数：30








# [POJ3348Cows 凸包+多边形求面积](http://so.csdn.net/so/search/s.do?q=POJ3348Cows%E5%87%B8%E5%8C%85+%E5%A4%9A%E8%BE%B9%E5%BD%A2%E6%B1%82%E9%9D%A2%E7%A7%AF&t=blog)

个人分类： [计算几何](https://blog.csdn.net/R1986799047/article/category/5687035)[凸包](https://blog.csdn.net/R1986799047/article/category/5793491)
|Language: DefaultCows|**Time Limit:** 2000MS||**Memory Limit:** 65536K||----|----|----||**Total Submissions:** 7633||**Accepted:** 3467|DescriptionYour friend to the south is interested in building fences and turning plowshares into swords. In order to help with his overseas adventure, they are forced to save money on buying fence posts by using trees as fence posts wherever possible. Given the locations of some trees, you are to help farmers try to create the largest pasture that is possible. Not all the trees will need to be used.However, because you will oversee the construction of the pasture yourself, all the farmers want to know is how many cows they can put in the pasture. It is well known that a cow needs at least 50 square metres of pasture to survive.InputThe first line of input contains a single integer, *n* (1 ≤ *n* ≤ 10000), containing the number of trees that grow on the available land. The next *n* lines contain the integer coordinates of each tree given as two integers *x* and *y*separated by one space (where -1000 ≤ x, y ≤ 1000). The integer coordinates correlate exactly to distance in metres (e.g., the distance between coordinate (10; 11) and (11; 11) is one metre).OutputYou are to output a single integer value, the number of cows that can survive on the largest field you can construct using the available trees.Sample Input40 00 10175 075 101Sample Output151|**Time Limit:** 2000MS||**Memory Limit:** 65536K|**Total Submissions:** 7633||**Accepted:** 3467|
|----|----|----|----|----|----|----|
|**Time Limit:** 2000MS||**Memory Limit:** 65536K| | | | |
|**Total Submissions:** 7633||**Accepted:** 3467| | | | |

Description

Your friend to the south is interested in building fences and turning plowshares into swords. In order to help with his overseas adventure, they are forced to save money on buying fence posts by using trees as fence posts wherever possible. Given the locations of some trees, you are to help farmers try to create the largest pasture that is possible. Not all the trees will need to be used.

However, because you will oversee the construction of the pasture yourself, all the farmers want to know is how many cows they can put in the pasture. It is well known that a cow needs at least 50 square metres of pasture to survive.

Input

The first line of input contains a single integer, *n* (1 ≤ *n* ≤ 10000), containing the number of trees that grow on the available land. The next *n* lines contain the integer coordinates of each tree given as two integers *x* and *y*separated by one space (where -1000 ≤ x, y ≤ 1000). The integer coordinates correlate exactly to distance in metres (e.g., the distance between coordinate (10; 11) and (11; 11) is one metre).

Output

You are to output a single integer value, the number of cows that can survive on the largest field you can construct using the available trees.

Sample Input

4
0 0
0 101
75 0
75 101
Sample Output

151

题意：给出一些点，圈出一个最大面积，每50平方养一头牛，问最多能养多少牛

**求凸包 + (int)求面积 / 50**

```cpp
//POJ--1228
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#define  eps 1e-8
using namespace std;

struct  point
{
    double x,y;
};
point p[1010],stack[1010];
int N,top;
//叉积
double multi(point p1, point p2, point p3)
{
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}
//距离公式
double dis(point a, point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
//极角排序比较器
bool cmp(point c, point d)
{
    double k = multi(p[0], c, d);
    if(k>0) return true;
    if(k<0) return false;
    return dis(c,p[0])<dis(d,p[0]);
}
//求凸包
void Convex()
{
    //第一个点p[0]为左下角的点
    for(int i = 1; i < N; i++)
    {
        point temp;
        if(p[i].y < p[0].y || ( p[i].y == p[0].y && p[i].x < p[0].x))
        {
            temp = p[i];
            p[i] = p[0];
            p[0] = temp;
        }
    }

    sort(p + 1, p+N , cmp);//不包括第一个点
    stack[0] = p[0];
    stack[1] = p[1];
    top = 1;
    for(int i = 2; i < N; i++)
    {
        while(top >= 1 && multi(stack[top - 1], stack[top], p[i]) < 0)     top--;
        //共线的点也压入凸包内;
        top++;
        stack[top] = p[i];
    }
}
//判断每条边是否有至少三个点;
bool judge()
{
    for(int  i=1;i<top;i++)
    {
        if((multi(stack[i-1],stack[i+1],stack[i]))!=0&&
        (multi(stack[i],stack[i+2],stack[i+1]))!=0)
            return false;
    }
    return true;
}
double area()
{
    double res=0;
    for(int i=1;i<top;i++){
        res+=(multi(stack[0],stack[i],stack[i+1])/2.0);
    }
    return res;
}
int main()
{
    while(scanf("%d",&N)!=EOF)
    {
        for(int i=0;i<N;i++)
        scanf("%lf%lf",&p[i].x,&p[i].y);
        Convex();
        int num=(int)floor(area()/50.0);
        printf("%d\n",num);
    }
    return 0;
}
```






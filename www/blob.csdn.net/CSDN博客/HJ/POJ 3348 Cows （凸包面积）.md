# POJ  3348 Cows （凸包面积） - HJ - CSDN博客
2017年01月14日 20:26:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：298
Your friend to the south is interested in building fences and turning plowshares into swords. In order to help with his overseas adventure, they are forced to save money on buying fence posts by using trees as fence posts wherever possible. Given the locations of some trees, you are to help farmers try to create the largest pasture that is possible. Not all the trees will need to be used.
However, because you will oversee the construction of the pasture yourself, all the farmers want to know is how many cows they can put in the pasture. It is well known that a cow needs at least 50 square metres of pasture to survive.
Input 
The first line of input contains a single integer, n (1 ≤ n ≤ 10000), containing the number of trees that grow on the available land. The next n lines contain the integer coordinates of each tree given as two integers x and y separated by one space (where -1000 ≤ x, y ≤ 1000). The integer coordinates correlate exactly to distance in metres (e.g., the distance between coordinate (10; 11) and (11; 11) is one metre).
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
题目大意： 
    草地上有些树，用树做篱笆围一块最大的面积来养牛，每头牛要50平方米才能养活，问最多能养多少只羊
解法： 
    凸包求面积，分解成三角形用叉积求面积。
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
struct point{
    int x,y;
}list[10005],t;
int top,k,n,stack[10005];
double area;
double multi(point a,point p1,point p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
double d(point p1,point p2)
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}
bool cmp(point a,point b)
{
    int tmp=multi(list[0],a,b);
    if(tmp==0)
      return d(list[0],a)<d(list[0],b);
    return tmp>0;
}
int main()
{   
    while(cin>>n)
    {
        k=0;
        for(int i=0;i<n;i++)
          cin>>list[i].x>>list[i].y;
        for(int i=1;i<n;i++)
          if(list[i].y<list[k].y||(list[i].y==list[k].y&&list[i].x<list[k].x))
            k=i;
        swap(list[0],list[k]);                                             //找到最小坐标并与第一个交换 
        sort(list+1,list+n,cmp);                                           //极角排序 
        stack[0]=0,stack[1]=1,top=1,area=0;                                 //栈内至少要有两个坐标 面积初始值为0 
        for(int i=2;i<n;i++)
        {
            while(top>=1&&multi(list[stack[top-1]],list[i],list[stack[top]])>=0)
              top--;
            stack[++top]=i;
        }
        for(int i=1;i<top;i++)
          area+=fabs(multi(list[stack[0]],list[stack[i]],list[stack[i+1]]));
        printf("%d\n",(int)(area/100));                                   //面积应该是叉积除2，放在这里和50一起除 
    }
    return 0;
}
```

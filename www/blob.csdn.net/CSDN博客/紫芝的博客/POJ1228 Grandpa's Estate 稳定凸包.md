# POJ1228 Grandpa's Estate  稳定凸包 - 紫芝的博客 - CSDN博客





2018年10月31日 23:03:27[紫芝](https://me.csdn.net/qq_40507857)阅读数：1802








Being the only living descendant of his grandfather, Kamran the Believer inherited all of the grandpa's belongings. The most valuable one was a piece of convex polygon shaped farm in the grandpa's birth village. The farm was originally separated from the neighboring farms by a thick rope hooked to some spikes (big nails) placed on the boundary of the polygon. But, when Kamran went to visit his farm, he noticed that the rope and some spikes are missing. Your task is to write a program to help Kamran decide whether the boundary of his farm can be exactly determined only by the remaining spikes.

Input

The first line of the input file contains a single integer t (1 <= t <= 10), the number of test cases, followed by the input data for each test case. The first line of each test case contains an integer n (1 <= n <= 1000) which is the number of remaining spikes. Next, there are n lines, one line per spike, each containing a pair of integers which are x and y coordinates of the spike.

Output

There should be one output line per test case containing YES or NO depending on whether the boundary of the farm can be uniquely determined from the input.

Sample Input

```
1
6 
0 0
1 2
3 4
2 0
2 4 
5 0
```

Sample Output
`NO`
### 题意：

**给N个点，是某个凸包上的部分点，问这N个点是否可以确定一个唯一的凸包，这种凸包叫做稳定凸包；**

比如，有4个点，他们连起来确实是一个凸包；

![](https://pic002.cnblogs.com/images/2012/408767/2012062000201251.png)

这4个点是某个凸包上的部分点，但原始的凸包可能不是这样的，比如：即这4个点构成的凸包不是稳定的

![](https://pic002.cnblogs.com/images/2012/408767/2012062000220247.png)

**结论：当凸包的一条边上只有2个点时（即两个端点），这个凸包是不稳定的；**

**原因：可以在这条边外引入一个点，构成一个新的凸包。**

**如果一条边上点数大于等于3个，则不可能在找到一个点，使扩展成一个新的凸包，否则构成的多边形是凹的**

这是一个典型的稳定凸包

![](https://pic002.cnblogs.com/images/2012/408767/2012062000284828.png)

**这道题的做法是先求出N个点的凸包，然后判断每条边的点数，如果每条边的点数都大于等于3，输出YES，否则输出NO**

**如果N小于6，直接输出NO；先按极角进行排序，然后使用包括共线点的凸包模板求解；至于判断一条边上至少有3个点，假设要判断的边i，那么判断边i和边i-1，边i和边i+1的向量叉积是否都为0（夹角180度）。**

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

int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>N;
        for(int i=0;i<N;i++)
        scanf("%lf%lf",&p[i].x,&p[i].y);
        if(N<6)   puts("NO");
        else
        {
                Convex();
                if(judge())  puts("YES");
                else puts("NO");
        }
    }
    return 0;
}
```






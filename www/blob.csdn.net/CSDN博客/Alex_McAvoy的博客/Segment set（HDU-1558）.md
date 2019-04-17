# Segment set（HDU-1558） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 18:57:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A segment and all segments which are connected with it compose a segment set. The size of a segment set is the number of segments in it. The problem is to find the size of some segment set.

![](https://img-blog.csdn.net/20180523185812823)



# **Input**

In the first line there is an integer t - the number of test case. For each test case in first line there is an integer n (n<=1000) - the number of commands. 

There are two different commands described in different format shown below:

    P x1 y1 x2 y2 - paint a segment whose coordinates of the two endpoints are (x1,y1),(x2,y2).<br>Q k - query the size of the segment set which contains the k-th segment.

    k is between 1 and the number of segments in the moment. There is no segment in the plane at first, so the first command is always a P-command.

# Output

For each Q-command, output the answer. There is a blank line between test cases.

# Sample Input

**1**

**10P 1.00 1.00 4.00 2.00P 1.00 -2.00 8.00 4.00Q 1P 2.00 3.00 3.00 1.00Q 1Q 3P 1.00 4.00 8.00 2.00Q 2P 3.00 3.00 6.00 -2.00Q 5 **

# Sample Output

**12225**


———————————————————————————————————————————————————————

题意：给出N个命令，如果命令为P，则向平面内添加一条线段，并判断是否与之前的相交，若相交则并入相应集合内。如果命令为Q ，则输出线段号为Q后面数字所在集合的线段总数。

思路：首先要判断线段相交，其次用并查集判断集合内个数，注意每次合并时，主集合将自己和附属集合的个数和存在主集合内即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000001
#define MOD 2520
#define E 1e-12
using namespace std;
struct point
{
    double x;
    double y;
};
struct Line
{
    point a;
    point b;
}line[N];
int father[N],num[N];
bool Judge(Line l1,Line l2)
{
    point p1=l1.a,p2=l1.b,p3=l2.a,p4=l2.b;

    if( min(p1.x,p2.x)>max(p3.x,p4.x) || min(p1.y,p2.y)>max(p3.y,p4.y) || min(p3.x,p4.x)>max(p1.x,p2.x) || min(p3.y,p4.y)>max(p1.y,p2.y) )
        return 0;

    double k1,k2,k3,k4;
    k1=(p2.x-p1.x)*(p3.y-p1.y) - (p2.y-p1.y)*(p3.x-p1.x);
    k2=(p2.x-p1.x)*(p4.y-p1.y) - (p2.y-p1.y)*(p4.x-p1.x);
    k3=(p4.x-p3.x)*(p1.y-p3.y) - (p4.y-p3.y)*(p1.x-p3.x);
    k4=(p4.x-p3.x)*(p2.y-p3.y) - (p4.y-p3.y)*(p2.x-p3.x);
    return (k1*k2<=E && k3*k4<=E);
}

int Find(int x)
{
    while(x!=father[x])
        x=father[x];
    return x;
}
void Union(int x,int y)
{
    int a,b;
    a=Find(x);
    b=Find(y);
    if(a!=b)
    {
        father[a]=b;
        num[b]+=num[a];
    }
}

int main()
{
    int t,n,k,js;
    scanf("%d",&t);
    while(t--)
    {
        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        {
            father[i]=i;
            num[i]=1;
        }

        int i=0;
        while(n--)
        {
            char ch[2];
            scanf("%s",ch);
            if(ch[0]=='P')
            {
                i++;
                scanf("%lf%lf%lf%lf",&line[i].a.x,&line[i].a.y,&line[i].b.x,&line[i].b.y);

                for(int j=1;j<i;j++)//判断该线段与之前线段是否相交，相交则合并
                    if( Judge(line[i],line[j]) )
                        Union(i,j);
            }
            else
            {
                int k;
                scanf("%d",&k);
                printf("%d\n",num[Find(k)]);
            }
        }
        if(t!=0)//注意格式！
            printf("\n");
    }
    return 0;
}
```







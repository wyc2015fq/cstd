# Zjnu Stadium（HDU-3047） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 18:54:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46








> 
# Problem Description

In 12th Zhejiang College Students Games 2007, there was a new stadium built in Zhejiang Normal University. It was a modern stadium which could hold thousands of people. The audience Seats made a circle. The total number of columns were 300 numbered 1--300, counted clockwise, we assume the number of rows were infinite.

These days, Busoniya want to hold a large-scale theatrical performance in this stadium. There will be N people go there numbered 1--N. Busoniya has Reserved several seats. To make it funny, he makes M requests for these seats: A B X, which means people numbered B must seat clockwise X distance from people numbered A. For example: A is in column 4th and X is 2, then B must in column 6th (6=4+2).

Now your task is to judge weather the request is correct or not. The rule of your judgement is easy: when a new request has conflicts against the foregoing ones then we define it as incorrect, otherwise it is correct. Please find out all the incorrect requests and count them as R.

# **Input**

There are many test cases:

For every case: 

The first line has two integer N(1<=N<=50,000), M(0<=M<=100,000),separated by a space.

Then M lines follow, each line has 3 integer A(1<=A<=N), B(1<=B<=N), X(0<=X<300) (A!=B), separated by a space.

# Output

For every case: 

Output R, represents the number of incorrect request.

# Sample Input

**10 101 2 1503 4 2001 5 2702 6 2006 5 804 7 1508 9 1004 8 501 7 1009 2 100**

# Sample Output

**2**


———————————————————————————————————————————————————————

题意：有n个人在体育馆里面，然后给出m个他们之间的距离， 输入A B X，代表B的座位比A多X，然后求m个关系之间有多少个错误，错误就是当前关系与之前的有冲突

思路

使用带权并查集，对于并查集中每一课树，设树根距离为0，以树根为参考，每个结点的权值代表与根节点的距离，合并两个元素时。

假设A、B属于不同的树，如果合并这两棵树，把A树合并到B树上，就需要给A树跟他的根结点赋值，假设于A、B的权值Wa、Wb，由于两权值代表的都是与根结点的距离，分析可知，给根结点所赋的值=Wa-Wb+x。

此时，对于原来的结点A，只更新了A与他的根结点的权值，因此其他结点的更新在查找中实现即可。

![](https://img-blog.csdn.net/20180523191256313)

                                                                                                （图源网络，侵权致歉）

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
#define N 1000001
#define MOD 2520
#define E 1e-12
using namespace std;
int father[N];
int dis[N];
int Find(int x)
{
    if(father[x]==x)
        return x;
    int temp=father[x];
    father[x]=Find(father[x]);
    dis[x]+=dis[temp];//结点a到根的距离
    return father[x];
}

int Union(int x,int y,int w)
{
    int a=Find(x);
    int b=Find(y);

    if(a==b)//如果当前两点与之前距离有冲突
        if(dis[y]!=dis[x]+w)
            return 1;

    father[b]=a;
    dis[b]=dis[x]-dis[y]+w;//计算两点距离

    return 0;
}

int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        int cnt=0;
        memset(dis,0,sizeof(dis));
        for(int i=0;i<=n;i++)
            father[i]=i;

        while(m--)
        {
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            if(Union(x,y,w))//合并x、y两点，并计算权值以及判断是否有冲突
                cnt++;
        }

        printf("%d\n",cnt);
    }
    return 0;
}
```






# A Bug's Life（HDU-1829） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 18:47:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Professor Hopper is researching the sexual behavior of a rare species of bugs. He assumes that they feature two different genders and that they only interact with bugs of the opposite gender. In his experiment, individual bugs and their interactions were easy to identify, because numbers were printed on their backs. 

Problem 

Given a list of bug interactions, decide whether the experiment supports his assumption of two genders with no homosexual bugs or if it contains some bug interactions that falsify it.

# Input

The first line of the input contains the number of scenarios. Each scenario starts with one line giving the number of bugs (at least one, and up to 2000) and the number of interactions (up to 1000000) separated by a single space. In the following lines, each interaction is given in the form of two distinct bug numbers separated by a single space. Bugs are numbered consecutively starting from one.

# Output

The output for every scenario is a line containing "Scenario #i:", where i is the number of the scenario starting at 1, followed by one line saying either "No suspicious bugs found!" if the experiment is consistent with his assumption about the bugs' sexual behavior, or "Suspicious bugs found!" if Professor Hopper's assumption is definitely wrong.

# Sample Input

**23 31 22 31 34 21 23 4 **

# Sample Output

**Scenario #1:**

**Suspicious bugs found!Scenario #2:No suspicious bugs found!**


———————————————————————————————————————————————————————

题意：同性别的合并在一个集合中，然后每次输入看两个值看是否在同一个集合中，裸的并查集应用

思路：利用一个数组来标记性别关系，然后把性别相同的合并到一个集合，如果发现在同一个集合便返回。

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
#define N 2001
#define MOD 2520
#define E 1e-12
using namespace std;
int father[N];
int a[N];
int Find(int k)
{
    if(father[k]==k)
        return k;
    father[k]=Find(father[k]);
    return father[k];
}
void Union(int x,int y)
{
    x=Find(x);
    y=Find(y);
    if(x==y)
        return ;
    father[x]=y;
}
int main()
{
    int t;

    scanf("%d",&t);
    for(int l=1;l<=t;l++)
    {
        int n,m;
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            father[i]=i;

        memset(a,0,sizeof(a));
        bool flag=false;
        while(m--)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            if(flag)
                continue;
            if(Find(x)==Find(y))
            {
                flag=true;
                continue;
            }


            if(a[x]==0)
                a[x]=y;
            else
                Union(a[x],y);

            if(a[y]==0)
                a[y]=x;
            else
                Union(a[y],x);
        }

        printf("Scenario #%d:\n",l);
        if(flag)
            printf("Suspicious bugs found!\n\n");
        else
            printf("No suspicious bugs found!\n\n");
    }
    return 0;
}
```







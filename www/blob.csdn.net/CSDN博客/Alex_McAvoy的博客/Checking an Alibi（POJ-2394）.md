# Checking an Alibi（POJ-2394） - Alex_McAvoy的博客 - CSDN博客





2018年07月27日 21:48:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：218








> 
# Problem Description

A crime has been comitted: a load of grain has been taken from the barn by one of FJ's cows. FJ is trying to determine which of his C (1 <= C <= 100) cows is the culprit. Fortunately, a passing satellite took an image of his farm M (1 <= M <= 70000) seconds before the crime took place, giving the location of all of the cows. He wants to know which cows had time to get to the barn to steal the grain. 

Farmer John's farm comprises F (1 <= F <= 500) fields numbered 1..F and connected by P (1 <= P <= 1,000) bidirectional paths whose traversal time is in the range 1..70000 seconds (cows walk very slowly). Field 1 contains the barn. It takes no time to travel within a field (switch paths). 

Given the layout of Farmer John's farm and the location of each cow when the satellite flew over, determine set of cows who could be guilty. 

NOTE: Do not declare a variable named exactly 'time'. This will reference the system call and never give you the results you really want.

# **Input**

* Line 1: Four space-separated integers: F, P, C, and M 

* Lines 2..P+1: Three space-separated integers describing a path: F1,F2, and T. The path connects F1 and F2 and requires T seconds to traverse. 

* Lines P+2..P+C+1: One integer per line, the location of a cow. The first line gives the field number of cow 1, the second of cow 2, etc.

# Output

* Line 1: A single integer N, the number of cows that could be guilty of the crime. 

* Lines 2..N+1: A single cow number on each line that is one of the cows that could be guilty of the crime. The list must be in ascending order.

# Sample Input

**7 6 5 81 4 21 2 12 3 63 5 55 4 61 7 914537**

# Sample Output

**41234**


题意：给出 f 个点，p 条边以及每条边的起点终点和通过时间，给出 c 个有牛的点，求在规定时间内能到达 m 点的牛的数量，并按升序输出牛的编号。

思路：裸的 Floyd 求最短路问题，套模版即可。

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
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int dis[N][N];
int num[N];
int main()
{
    int f,p,c,m;
    scanf("%d%d%d%d",&f,&p,&c,&m);

    memset(dis,INF,sizeof(dis));
    for(int i=1;i<=f;i++)
        dis[i][i]=0;
    for(int i=1;i<=p;i++)
    {
        int x,y,t;
        scanf("%d%d%d",&x,&y,&t);
        if(t<=m)
        {
            dis[x][y]=min(dis[x][y],t);
            dis[y][x]=dis[x][y];
        }
    }

    for(int k=1;k<=f;k++)
        for(int i=1;i<=f;i++)
            for(int j=1;j<=f;j++)
                if(dis[i][k]+dis[k][j]<dis[i][j]&&i!=j)
                    dis[i][j]=dis[i][k]+dis[k][j];

    int k=0;
    for(int i=1;i<=c;i++)
    {
        int x;
        scanf("%d",&x);
        if(dis[x][1]>m)
            continue;
        else
        {
            k++;
            num[k]=i;
        }
    }

    for(int i=1;i<k;i++)
        for(int j=i+1;j<=k;j++)
            if(num[i]>num[j])
                swap(num[i],num[j]);

    printf("%d\n",k);
    for(int i=1;i<=k;i++)
        printf("%d\n",num[i]);

    return 0;
}
```






# POJ 1325 Machine Schedule 二分图最小点集覆盖 - 紫芝的博客 - CSDN博客





2018年08月28日 14:31:38[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








Machine Schedule
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 17026||**Accepted:** 7211|

Description

As we all know, machine scheduling is a very classical problem in computer science and has been studied for a very long history. Scheduling problems differ widely in the nature of the constraints that must be satisfied and the type of schedule desired. Here we consider a 2-machine scheduling problem. 


There are two machines A and B. Machine A has n kinds of working modes, which is called mode_0, mode_1, ..., mode_n-1, likewise machine B has m kinds of working modes, mode_0, mode_1, ... , mode_m-1. At the beginning they are both work at mode_0. 


For k jobs given, each of them can be processed in either one of the two machines in particular mode. For example, job 0 can either be processed in machine A at mode_3 or in machine B at mode_4, job 1 can either be processed in machine A at mode_2 or in machine B at mode_4, and so on. Thus, for job i, the constraint can be represent as a triple (i, x, y), which means it can be processed either in machine A at mode_x, or in machine B at mode_y. 


Obviously, to accomplish all the jobs, we need to change the machine's working mode from time to time, but unfortunately, the machine's working mode can only be changed by restarting it manually. By changing the sequence of the jobs and assigning each job to a suitable machine, please write a program to minimize the times of restarting machines. 

Input

The input file for this program consists of several configurations. The first line of one configuration contains three positive integers: n, m (n, m < 100) and k (k < 1000). The following k lines give the constrains of the k jobs, each line is a triple: i, x, y. 


The input will be terminated by a line containing a single zero. 

Output

The output should be one integer per line, which means the minimal times of restarting machine.

Sample Input

5 5 10
0 1 1
1 1 2
2 1 3
3 1 4
4 2 1
5 2 2
6 2 3
7 2 4
8 3 3
9 4 3
0

Sample Output

3
Source

[Beijing 2002](http://poj.org/searchproblem?field=source&key=Beijing+2002)

**题意：**A机器有n个模式，B机器有m个模式，有k个任务，第i个任务可以用A机器的ai模式或者B机器的bi模式，换模式需要重启，开始两个机器都在模式0，问最少需要重启几次。

**分析：**要求最小的重启次数，也就是求出除了0模式，最少要工作在几个模式

**建图：**A的模式为X集，B的模式为Y集，每个任务看做一条线，连接X集和Y集，则问题转化为求X、Y中最少的点，使得每条线至少有一个端点被选。即最小点集覆盖。

根据最小点集覆盖=二分图最大匹配。

```cpp
#include<cstdio>
#include<cstring>
using namespace std;
int n1,n2,k;
const int maxn=1e3+10;
int map[maxn][maxn],vis[maxn],link[maxn];
int find(int x)
{
    for(int i=1;i<=n2;i++)
    {
        if(map[x][i]&&!vis[i])
        {
            vis[i]=1;
            if(link[i]==0||find(link[i]))
            {
                link[i]=x;
                return 1;
            }
        }
    }
    return 0;
}
int main()
{
    while(scanf("%d",&n1)!=EOF&&n1){
        scanf("%d%d",&n2,&k);
        memset(map,0,sizeof map);
        memset(link,0,sizeof link);
        for(int i=0;i<k;i++)
        {
            int num,x,y;
            scanf("%d%d%d",&num,&x,&y);
            if(x*y)
            map[x][y]=1;
        }
        int ans=0;
        for(int i=1;i<=n1;i++)
        {
            memset(vis,0,sizeof vis);
            if(find(i)) ans++;
        }
        printf("%d\n",ans);
    }
    return 0;
}
```






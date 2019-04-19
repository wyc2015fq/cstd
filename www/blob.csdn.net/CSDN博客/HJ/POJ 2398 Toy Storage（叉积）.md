# POJ  2398  Toy Storage（叉积） - HJ - CSDN博客
2017年01月14日 19:52:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：205
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------计算几何--------																[点和直线的关系](https://blog.csdn.net/feizaoSYUACM/article/category/7238016)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
Mom and dad have a problem: their child, Reza, never puts his toys away when he is finished playing with them. They gave Reza a rectangular box to put his toys in. Unfortunately, Reza is rebellious and obeys his parents by simply throwing his toys into the box. All the toys get mixed up, and it is impossible for Reza to find his favorite toys anymore. 
Reza’s parents came up with the following idea. They put cardboard partitions into the box. Even if Reza keeps throwing his toys into the box, at least toys that get thrown into different partitions stay separate. The box looks like this from the top: 
![这里写图片描述](http://poj.org/images/2398_1.jpg)
We want for each positive integer t, such that there exists a partition with t toys, determine how many partitions have t, toys.
Input 
The input consists of a number of cases. The first line consists of six integers n, m, x1, y1, x2, y2. The number of cardboards to form the partitions is n (0 < n <= 1000) and the number of toys is given in m (0 < m <= 1000). The coordinates of the upper-left corner and the lower-right corner of the box are (x1, y1) and (x2, y2), respectively. The following n lines each consists of two integers Ui Li, indicating that the ends of the ith cardboard is at the coordinates (Ui, y1) and (Li, y2). You may assume that the cardboards do not intersect with each other. The next m lines each consists of two integers Xi Yi specifying where the ith toy has landed in the box. You may assume that no toy will land on a cardboard.
A line consisting of a single 0 terminates the input.
Output 
For each box, first provide a header stating “Box” on a line of its own. After that, there will be one line of output per count (t > 0) of toys in a partition. The value t will be followed by a colon and a space, followed the number of partitions containing t toys. Output will be sorted in ascending order of t for each box.
Sample Input
4 10 0 10 100 0 
20 20 
80 80 
60 60 
40 40 
5 10 
15 10 
95 10 
25 10 
65 10 
75 10 
35 10 
45 10 
55 10 
85 10 
5 6 0 10 60 0 
4 3 
15 30 
3 1 
6 8 
10 10 
2 1 
2 8 
1 5 
5 5 
40 10 
7 9 
0
Sample Output
Box 
2: 5 
Box 
1: 4 
2: 1
Source 
Tehran 2003 Preliminary
题目大意： 
    一个矩形，有被若干直线分成N个格子，给出一个点的坐标，问你该点位于哪个点中。
解法： 
   其实就是点在凸四边形内的判断，若利用叉积的性质，可以二分求解。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int a[5005],num[5005];
struct node{
    double x,y;
}k,s[5005];
double Check(double a,double b,double x1,double y1,double x2,double y2)
{
    return (x1-a)*(y2-b)-(y1-b)*(x2-a);
}
int cmp(node a,node b)                 //需要排序
{
    if(a.x==b.x)
      return a.y<b.y;
    return a.x<b.x;
}
int main()
{
    int n,m;
    double x1,y1,x2,y2;
    while(scanf("%d",&n))
    {
        if(n==0) break;     
        scanf("%d%lf%lf%lf%lf",&m,&x1,&y1,&x2,&y2);
        memset(a,0,sizeof(a));
        memset(num,0,sizeof(num));
        for(int i=0;i<n;i++)
          scanf("%lf %lf",&s[i].x,&s[i].y);
        s[n].x=s[n].y=x2;
        sort(s,s+n+1,cmp);
        while(m--)
        {
            scanf("%lf %lf",&k.x,&k.y);
            int l=0,r=n,cnt;
            while(l<=r)
            {
                int mid=(l+r)/2;
                double p=Check(k.x,k.y,s[mid].x,y1,s[mid].y,y2);
                if(p<0)
                {
                    cnt=mid;
                    r=mid-1;
                }
                else  l=mid+1;
            }
            a[cnt]++;
        }    
        for(int i=0;i<=n;i++)
          if(a[i])
            num[a[i]]++; 
        printf("Box\n");
        for(int i=0;i<=n;i++)
          if(num[i])
            printf("%d: %d\n",i,num[i]);
    }
    return 0;
}
```

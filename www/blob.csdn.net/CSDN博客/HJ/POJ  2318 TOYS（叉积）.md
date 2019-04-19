# POJ   2318  TOYS（叉积） - HJ - CSDN博客
2017年01月14日 19:47:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：284
Calculate the number of toys that land in each bin of a partitioned toy box. 
   Mom and dad have a problem - their child John never puts his toys away when he is finished playing with them. They gave John a rectangular box to put his toys in, but John is rebellious and obeys his parents by simply throwing his toys into the box. All the toys get mixed up, and it is impossible for John to find his favorite toys.
John’s parents came up with the following idea. They put cardboard partitions into the box. Even if John keeps throwing his toys into the box, at least toys that get thrown into different bins stay separated. The following diagram shows a top view of an example toy box.
For this problem, you are asked to determine how many toys fall into each partition as John throws them into the toy box. 
![](http://poj.org/images/2318_1.jpg?_=3188484)
Input
The input file contains one or more problems. The first line of a problem consists of six integers, n m x1 y1 x2 y2. The number of cardboard partitions is n (0 < n <= 5000) and the number of toys is m (0 < m <= 5000). The coordinates of the upper-left corner and the lower-right corner of the box are (x1,y1) and (x2,y2), respectively. The following n lines contain two integers per line, Ui Li, indicating that the ends of the i-th cardboard partition is at the coordinates (Ui,y1) and (Li,y2). You may assume that the cardboard partitions do not intersect each other and that they are specified in sorted order from left to right. The next m lines contain two integers per line, Xj Yj specifying where the j-th toy has landed in the box. The order of the toy locations is random. You may assume that no toy will land exactly on a cardboard partition or outside the boundary of the box. The input is terminated by a line consisting of a single 0.
Output 
    The output for each problem will be one line for each separate bin in the toy box. For each bin, print its bin number, followed by a colon and one space, followed by the number of toys thrown into that bin. Bins are numbered from 0 (the leftmost bin) to n (the rightmost bin). Separate the output of different problems by a single blank line.  
Sample Input
5 6 0 10 60 0 
    3 1 
    4 3 
    6 8 
    10 10 
    15 30 
    1 5 
    2 1 
    2 8 
    5 5 
    40 10 
    7 9 
    4 10 0 10 100 0 
    20 20 
    40 40 
    60 60 
    80 80 
     5 10 
    15 10 
    25 10 
    35 10 
    45 10 
    55 10 
    65 10 
    75 10 
    85 10 
    95 10 
    0
Sample Output
0: 2 
    1: 1 
    2: 1 
    3: 1 
    4: 0 
    5: 1
0: 2 
    1: 2 
    2: 2 
    3: 2 
    4: 2
Hint 
    As the example illustrates, toys that fall on the boundary of the box are “in” the box. 
大意： 
    给了m个点，落在n+1个区域中，问各个区域有多少个点。
解法： 
    利用叉积去判断点在线段的哪一侧，可以二分去做，比较快。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
int a[5005];
double s[5005][2];
struct node{
    double x,y;
}k;
double Check(double a,double b,double x1,double y1,double x2,double y2)
{
    return (x1-a)*(y2-b)-(y1-b)*(x2-a);
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
        for(int i=0;i<n;i++)
          scanf("%lf %lf",&s[i][0],&s[i][1]);
        s[n][0]=s[n][1]=x2;
        while(m--)
        {
            scanf("%lf %lf",&k.x,&k.y);
            int l=0,r=n,cnt;
            while(l<=r)
            {
                int mid=(l+r)/2;
                double p=Check(k.x,k.y,s[mid][0],y1,s[mid][1],y2);
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
          printf("%d: %d\n",i,a[i]);
        printf("\n");
    }
    return 0;
}
```

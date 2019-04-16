# HDU1007   Quoit Design  分治+递归 - 紫芝的博客 - CSDN博客





2018年05月09日 10:48:28[紫芝](https://me.csdn.net/qq_40507857)阅读数：48










# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1007)

# Quoit Design
****Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 61473    Accepted Submission(s): 16298****


Problem Description

Have you ever played quoit in a playground? Quoit is a game in which flat rings are pitched at some toys, with all the toys encircled awarded.
In the field of Cyberground, the position of each toy is fixed, and the ring is carefully designed so it can only encircle one toy at a time. On the other hand, to make the game look more attractive, the ring is designed to have the largest radius. Given a configuration of the field, you are supposed to find the radius of such a ring.

Assume that all the toys are points on a plane. A point is encircled by the ring if the distance between the point and the center of the ring is strictly less than the radius of the ring. If two toys are placed at the same point, the radius of the ring is considered to be 0.





Input

The input consists of several test cases. For each case, the first line contains an integer N (2 <= N <= 100,000), the total number of toys in the field. Then N lines follow, each contains a pair of (x, y) which are the coordinates of a toy. The input is terminated by N = 0.





Output

For each test case, print in one line the radius of the ring required by the Cyberground manager, accurate up to 2 decimal places. 





Sample Input


20 01 121 11 13-1.5 00 00 1.50




Sample Output


0.71
0.00
0.75






Author

CHEN, Yue




Source

[ZJCPC2004](http://acm.hdu.edu.cn/search.php?field=problem&key=ZJCPC2004&source=1&searchmode=source)




Recommend

JGShining   |   We have carefully selected several similar problems for you:  [1006](http://acm.hdu.edu.cn/showproblem.php?pid=1006)[1009](http://acm.hdu.edu.cn/showproblem.php?pid=1009)[1005](http://acm.hdu.edu.cn/showproblem.php?pid=1005)[1008](http://acm.hdu.edu.cn/showproblem.php?pid=1008)[1004](http://acm.hdu.edu.cn/showproblem.php?pid=1004)



HDU1007分治+递归

## 题意：
给n个点的坐标，求距离最近的一对点之间距离的一半


## 思路：
其实就是求最近点对的距离，主要思想就是分治
先把n个点按x坐标排序，然后求左边n/2个和右边n/2个的最近距离
然后合并；重点也是合并：

首先，假设点是n个，编号为0到n-1。我们要分治求，则找一个中间的编号mid，
先求出1到mid点的最近距离设为d1，还有mid+1到n的最近距离设为d2。
这里的点需要按x坐标的顺序排好，并且假设这些点中，没有2点在同一个位置。
（若有，则直接最小距离为0了）。


然后，令ans为d1, d2中较小的那个点。
如果说最近点对中的两点都在1-mid集合中，或者mid+1到n集合中，则d就是最小距离了。
但是还有可能的是最近点对中的两点分属这两个集合，所以我们必须先检测一下这种情况是否会存在，
若存在，则把这个最近点对的距离记录下来，去更新ans。这样我们就可以得道最小的距离ans了。


关键是要去检测最近点对，理论上每个点都要和对面集合的点匹配一次，那效率还是不能满足我们的要求。
所以这里要优化。怎么优化呢？考虑一下，假如以我们所选的分割点mid为界，
如果某一点的横坐标到点mid的横坐标的绝对值超过d1并且超过d2，
那么这个点到mid点的距离必然超过d1和d2中的小者，所以这个点到对方集合的任意点的距离必然不是所有点中最小的。


所以我们先把在mid为界左右一个范围内的点全部筛选出来，放到一个集合里。
筛选好以后，当然可以把这些点两两求距离去更新d了，不过这样还是很慢，
万一满足条件的点很多呢。这里还得继续优化。首先把这些点按y坐标排序。
假设排序好以后有cnt个点，编号为0到cnt-1。那么我们用0号去和1到cnt-1号的点求一下距离，
然后1号和2到cnt-1号的点求一下距离。

如果某两个点y轴距离已经超过了ans，这次循环就可以直接break了，开始从下一个点查找了.

```cpp
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<cstring>
using namespace std;
const int maxn=1e5+10;
struct point{
double x,y;
}p[maxn];
int a[maxn];
bool cmpx(point a,point b){return a.x<b.x;};
bool cmpy(int a,int b){return p[a].y<p[b].y;};
double dis(point a,point b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double find_min(int l,int r)
{
    if(l+1==r)  return dis(p[l],p[r]);//只有两个点
    if(l+2==r)  return min(dis(p[l],p[r]),min(dis(p[l],p[l+1]),dis(p[l+1],p[r])));//三个点
    int mid=(l+r)>>1;
    double ans=min(find_min(l,mid),find_min(mid+1,r));

    //检测最近点对中的两点分属这两个集合
    int i,j,cnt=0;
    for(i=l;i<=r;i++){//记录最近点对
        if(p[i].x>=p[mid].x-ans&&p[i].x<=p[mid].x+ans)
            a[cnt++]=i;
    }
    sort(a,a+cnt,cmpy);
    for(i=0;i<cnt;i++)
    {
        for(j=i+1;j<cnt;j++)
        {
            //如果某两个点y轴距离已经超过了ans
            if(p[a[j]].y-p[a[i]].y>=ans)    break;
            ans=min(ans,dis(p[a[i]],p[a[j]]));
        }
    }
    return ans;
}
int main()
{
    int n;
    while(scanf("%d",&n)&&n){
        for(int i=0;i<n;i++){
            scanf("%lf%lf",&p[i].x,&p[i].y);
        }
        sort(p,p+n,cmpx);
        printf("%.2lf\n",find_min(0,n-1)/2);
    }
    return 0;
}
```







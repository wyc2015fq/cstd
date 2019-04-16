# poj1556 The Doors 计算几何+最短路Dijkstra算法 - 紫芝的博客 - CSDN博客





2018年04月30日 22:49:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：39








# [The Doors](http://poj.org/problem?id=1556)
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 10243||**Accepted:** 3803|

### Description

You are to find the length of the shortest path through a chamber containing obstructing walls. The chamber will always have sides at x = 0, x = 10, y = 0, and y = 10. The initial and final points of the path are always (0, 5) and (10, 5). There will also be from 0 to 18 vertical walls inside the chamber, each with two doorways. The figure below illustrates such a chamber and also shows the path of minimal length. 

![](http://poj.org/images/1556_1.jpg)

**Input**

The input data for the illustrated chamber would appear as follows. 


2 

4  2  7  8  9 

7  3  4.5  6  7 

The first line contains the number of interior walls. Then there is a line for each such wall, containing five real numbers. The first number is the x coordinate of the wall (0 < x < 10), and the remaining four are the y coordinates of the ends of the doorways in that wall. The x coordinates of the walls are in increasing order, and within each line the y coordinates are in increasing order. The input file will contain at least one such set of data. The end of the data comes when the number of walls is -1. 

**Output**

The output should contain one line of output for each chamber. The line should contain the minimal path length rounded to two decimal places past the decimal point, and always showing the two decimal places past the decimal point. The line should contain no blanks.

**Sample Input**

1
5 4 6 7 8
2
4 2 7 8 9
7 3 4.5 6 7
-1
**Sample Output**

10.00
10.06
Source

[Mid-Central USA 1996](http://poj.org/searchproblem?field=source&key=Mid-Central+USA+1996)

## 题意：

房间上有n堵墙，每面墙上有两扇门，

求从房间左端中点到右端中点的最短路径

## 题解：建图+最短路 

1.建图：

每个门上的两点与其他点可以直达就建边，边权就是他们的距离

2.最短路 

 Dijkstra算法

```cpp
#include<cstring>
#include<algorithm>
#include<cmath>
#include<cstdio>
#include<vector>
#include<queue>
#define N 1010
#define inf 999999999.0
using namespace std;
struct point{
	double x,y;
	int id;
};
struct line{
	point d,u;
	int th;
};
int n,pnum,pline;
vector<line> vec;
vector<point> p;
double mp[N][N];
void add_line(point a,point b)
{
	line it;
	it.d=a;
	it.u=b;
	it.th=pline++;
	vec.push_back(it);
}
//叉积 
double multi(point p0,point p1,point p2)
{
	return (p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
}
//判断两条线段是否相交
bool is_inter(point s1,point e1,point s2,point e2)
{
	return 
	(max(s1.x,e1.x)>=min(s2.x,e2.x))&&
	(max(s2.x,e2.x)>=min(s1.x,e1.x))&&
	(max(s1.y,e1.y)>=min(s2.y,e2.y))&&
	(max(s2.y,e2.y)>=min(s1.y,e1.y))&&
	(multi(s1,s2,e1)*multi(s1,e1,e2)>0)&&
	(multi(s2,s1,e2)*multi(s2,e2,e1)>0);
 }
double dist(point a,point b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
 } 
//建图
void init()
{
	for(int i=0;i<=pnum;i++){
		for(int j=0;j<=pnum;j++){
			if(i==j){
				mp[i][j]=0.0;
				continue;
			}
			bool flag=1;
			point s1=p[i],e1=p[j];
			int l=(i+1)/2,r=(j+1)/2;
			for(int k=l+1;k<r;k++){
				if(is_inter(s1,e1,vec[k].d,vec[k].u)){
					flag=0;
					break;
				}
			}
			if(flag) mp[i][j]=mp[i][j]=dist(s1,e1);
			else mp[i][j]=mp[i][j]=inf;
		}
	}
 } 
bool vis[N];
int pre[N];
double d[N];
void Dijkstra(int begin)
{
	for(int i=0;i<=pnum;i++){
		d[i]=inf;
		vis[i]=false;
	}
	d[begin]=0;
	
	for(int j=0;j<=pnum;j++){
		int k=-1,MIN=inf;
		for(int i=0;i<=pnum;i++)
		if(!vis[i]&&d[i]<MIN){
			MIN=d[i];
			k=i;
		}
		if(k==-1)	break;
		vis[k]=1;
		for(int i=0;i<=pnum;i++)
		if(!vis[i]&&d[i]<MIN){
			MIN=d[i];
			k=i;
		}
		if(k==-1)	break;
		vis[k]=1;
		for(int i=0;i<=pnum;i++)
		if(!vis[i]&&d[k]+mp[k][i]<d[i]){
			d[i]=d[k]+mp[k][i];
		}
	}
}
int main()
{
	while(scanf("%d",&n)!=EOF&&n!=-1){
		vec.clear();
		p.clear();
		
		double x,y1,y2,y3,y4;
		pnum=0;pline=0;
		point a,b;
		a.x=0,a.y=5.0,a.id=pnum++;
		p.push_back(a);
		
		for(int i=0;i<n;i++)
		{
			scanf("%lf%lf%lf%lf%lf",&x,&y1,&y2,&y3,&y4);
			a.x=x;b.x=x;
			
			a.y=0;b.y=y1;a.id=pnum++;b.id=pnum++;
			add_line(a,b);
			p.push_back(a);p.push_back(b);
			
			a.y=y2;b.y=y3;a.id=pnum++;b.id=pnum++;
			add_line(a,b);
			p.push_back(a);p.push_back(b);
			
			a.y=y4;b.y=10.0;a.id=pnum++;b.id=pnum++;
			add_line(a,b);
			p.push_back(a);p.push_back(b);
		}
		a.x=10.0;a.y=5.0;a.id=pnum;
		p.push_back(a);
		init();
		Dijkstra(0);
		printf("%.2f\n",d[pnum]);
	}
	return 0;
}
```






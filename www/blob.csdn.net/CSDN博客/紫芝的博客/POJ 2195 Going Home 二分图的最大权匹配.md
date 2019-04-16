# POJ 2195 Going Home 二分图的最大权匹配 - 紫芝的博客 - CSDN博客





2018年08月29日 10:13:34[紫芝](https://me.csdn.net/qq_40507857)阅读数：39








# [POJ2195 Going Home 最小费用最大流](https://blog.csdn.net/qq_40507857/article/details/82945486)

# [Going Home](https://vjudge.net/problem/POJ-2195)
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 25567||**Accepted:** 12838|

Description

On a grid map there are n little men and n houses. In each unit time, every little man can move one unit step, either horizontally, or vertically, to an adjacent point. For each little man, you need to pay a $1 travel fee for every step he moves, until he enters a house. The task is complicated with the restriction that each house can accommodate only one little man. 


Your task is to compute the minimum amount of money you need to pay in order to send these n little men into those n different houses. The input is a map of the scenario, a '.' means an empty space, an 'H' represents a house on that point, and am 'm' indicates there is a little man on that point. 

![](http://poj.org/images/2195_1.jpg)



You can think of each point on the grid map as a quite large square, so it can hold n little men at the same time; also, it is okay if a little man steps on a grid with a house without entering that house.

Input

There are one or more test cases in the input. Each case starts with a line giving two integers N and M, where N is the number of rows of the map, and M is the number of columns. The rest of the input will be N lines describing the map. You may assume both N and M are between 2 and 100, inclusive. There will be the same number of 'H's and 'm's on the map; and there will be at most 100 houses. Input will terminate with 0 0 for N and M.

Output

For each test case, output one line with the single integer, which is the minimum amount, in dollars, you need to pay.

Sample Input

```
2 2
.m
H.
5 5
HH..m
.....
.....
.....
mm..H
7 8
...H....
...H....
...H....
mmmHmmmm
...H....
...H....
...H....
0 0
```

Sample Output

```
2
10
28
```

Source

[Pacific Northwest 2004](http://poj.org/searchproblem?field=source&key=Pacific+Northwest+2004)

**题意：**

有k个人，k间房子，每人进入一个房子，求最小的总距离
**分析：**

对每条边取相反数，然后得到的结果再取相反数，就能得到最小权匹配

```cpp
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define INF 0x3f3f3f3f
#define MAXN 105

using namespace std;

int n;//二分图顶点集的大小
int a[MAXN][MAXN];//a[i][j]表示i到j的权值
int vx[MAXN],vy[MAXN];
int lx[MAXN],ly[MAXN];//同时调节两个数组，使得权值和最大
int pre[MAXN];//记录顶点集n2中点x在顶点集n1中所匹配的点x的编号
int slack[MAXN];//松弛操作
int N,M;//N行M列
int cnth,cntm;
struct point{
    int x,y;
}tmph[MAXN],tmpm[MAXN];
char st[MAXN][MAXN];

bool dfs(int index)
{
    vx[index]=1;
    for(int i=1;i<=n;i++)
    {
        if(!vy[i]&&lx[index]+ly[i]==a[index][i])
        {//这个条件下使用匈牙利算法
            vy[i]=1;
    //这个房子没人住或者可以让住这个房子的人找另外的房子
            if(!pre[i]||dfs(pre[i]))
            {
                pre[i]=index;
                return 1;
            }
        }
        else if(!vy[i]&&lx[index]+ly[i]>a[index][i])
            slack[i]=min(slack[i], lx[index]+ly[i]-a[index][i]);
    }
    return 0;
}

int KM()
{
    memset(ly,0,sizeof(ly));
    //首先把每个居民出钱最多的那个房子给他
    for(int i=1;i<=n;i++)
    {
        lx[i]=-INF;
        for(int j=1;j<=n;j++)
            lx[i]=max(lx[i],a[i][j]);
    }

    int ans=0,d;
    memset(pre,0,sizeof(pre));

    //给第i位居民分配房子
    for(int i=1;i<=n;i++)
    {
        memset(slack,INF,sizeof(slack));
        while(1)
        {
            memset(vx,0,sizeof(vx));
            memset(vy,0,sizeof(vy));
            if(dfs(i))
                break;

            d=INF;
            for(int i=1;i<=n;i++)
                if(!vy[i])
                d=min(d,slack[i]);//找到最小松弛量

            if(d==INF) return -1;           //no matching
            for(int j=1;j<=n;j++){
                if(vx[j]) lx[j]-=d;
                if(vy[j]) ly[j]+=d;
            }
        }
    }
    for(int i=1;i<=n;i++)
        ans+=a[pre[i]][i];
    return ans;
}

int main()
{
    while(~scanf("%d %d", &N, &M))
    {
        if(!N&&!M)
            break;
        cnth=cntm=0;
        getchar();
        for(int i=1;i<=N;i++)
        {
            for(int j=1;j<=M;j++)
            {
                scanf("%c",&st[i][j]);
                if(st[i][j]=='m')
                    cntm++,tmpm[cntm].x=i, tmpm[cntm].y=j;
                if(st[i][j]=='H')
                    cnth++,tmph[cnth].x=i, tmph[cnth].y=j;
            }
            getchar();
        }

        n=cntm;
        for(int i=1;i<=cntm;i++)
            for(int j=1;j<=cnth;j++)
                a[i][j]=-(abs(tmpm[i].x-tmph[j].x)+abs(tmpm[i].y-tmph[j].y));


        printf("%d\n",-KM());
    }


    return 0;
}
```

Java

```java
import java.math.*;
import java.util.*;

public class Main {
	static int inf=(int)1e8+7;
	static int maxn=110;
	static int n;//n个球，n个洞
	static int[][] w=new int[maxn][maxn];//w[i][j]表示i到j的权值
	static int[] lx=new int[maxn];
	static int[] ly=new int[maxn];
	static int[] link=new int[maxn];
	static int[] slack=new int[maxn];
	
	static boolean[] visx=new boolean[maxn];
	static boolean[] visy=new boolean[maxn];
	
	static boolean dfs(int x) {
		visx[x]=true;
		for(int y=1;y<=n;y++) {
			//if(visy[y])	continue;
			int t=lx[x]+ly[y]-w[x][y];
			if(!visy[y]&&t==0) {
				visy[y]=true;
				if(link[y]==0||dfs(link[y])) {
					link[y]=x;
					return true;
				}
			}
			else if(!visy[y]&&lx[x]+ly[y]>w[x][y])
				slack[y]=Math.min(slack[y], lx[x]+ly[y]-w[x][y]);
		}
		return false;
	}
	
	static int km() {
		for(int i=0;i<maxn;i++) {
			lx[i]=-inf;
			ly[i]=0;
			link[i]=0;
		}
		for(int i=1;i<=n;i++)
			for(int j=1;j<=n;j++)
					lx[i]=Math.max(lx[i], w[i][j]);
		
		for(int i=1;i<=n;i++) {
			
			for(int j=1;j<=n;j++) 
				slack[j]=inf;
				
				while(true) {
					
					for(int k=1;k<maxn;k++) {
						visx[k]=false;
						visy[k]=false;
					}
					
					if(dfs(i))	break;
					int d=inf;
					
					for(int k=1;k<=n;k++)
						if(!visy[k]&&d>slack[k])
							d=slack[k];
					if(d==-1)	return -1;// no matching
					for(int k=1;k<=n;k++) {
						if(visx[k])	lx[k]-=d;
						if(visy[k])	ly[k]+=d;
					}
				}
			}
		
		int ans=0;
		for(int i=1;i<=n;i++)
			ans+=w[link[i]][i];
		return ans;
	}
	
    public static void main (String[] args) {
    	Scanner cin=new Scanner(System.in);
        while(cin.hasNext()) {
        	int[] ball_x=new int[maxn];
        	int[] ball_y=new int[maxn];
        	int[] hole_x=new int[maxn];
        	int[] hole_y=new int[maxn];
        	
        	int bk=1,hk=1;
        	int M=cin.nextInt();
        	int N=cin.nextInt();
        	String str="";
        	
        	if(M==0&&N==0)	break;
        	
        	for(int i=0;i<M;i++) {
        		str=cin.next();
        		for(int j=0;j<N;j++) {
        			if(str.charAt(j)=='m') {
        				ball_x[bk]=i;
        				ball_y[bk]=j;
        				bk++;
        			}else if(str.charAt(j)=='H') {
        				hole_x[hk]=i;
        				hole_y[hk]=j;
        				hk++;
        			}
        		}
        	}
        	n=bk-1;
        	//System.out.println(n);
        	for(int i=1;i<=n;i++) {
        		for(int j=1;j<=n;j++) {
        			w[i][j]=-Math.abs(ball_x[i]-hole_x[j])-Math.abs(ball_y[i]-hole_y[j]);
        			//System.out.print(-w[i][j]+" ");
        		}
        		//System.out.println();
        	}
        	System.out.println(-km());
        }
        cin.close();
    }
}
```






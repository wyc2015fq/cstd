# Fire Net（HDU-1045） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 21:08:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Suppose that we have a square city with straight streets. A map of a city is a square board with n rows and n columns, each representing a street or a piece of wall. 

A blockhouse is a small castle that has four openings through which to shoot. The four openings are facing North, East, South, and West, respectively. There will be one machine gun shooting through each opening. 

Here we assume that a bullet is so powerful that it can run across any distance and destroy a blockhouse on its way. On the other hand, a wall is so strongly built that can stop the bullets. 

The goal is to place as many blockhouses in a city as possible so that no two can destroy each other. A configuration of blockhouses is legal provided that no two blockhouses are on the same horizontal row or vertical column in a map unless there is at least one wall separating them. In this problem we will consider small square cities (at most 4x4) that contain walls through which bullets cannot run through. 

The following image shows five pictures of the same board. The first picture is the empty board, the second and third pictures show legal configurations, and the fourth and fifth pictures show illegal configurations. For this board, the maximum number of blockhouses in a legal configuration is 5; the second picture shows one way to do it, but there are several other ways. 

![](https://img-blog.csdnimg.cn/20190313211107917.jpg)

Your task is to write a program that, given a description of a map, calculates the maximum number of blockhouses that can be placed in the city in a legal configuration. 

# Input

The input file contains one or more map descriptions, followed by a line containing the number 0 that signals the end of the file. Each map description begins with a line containing a positive integer n that is the size of the city; n will be at most 4. The next n lines each describe one row of the map, with a '.' indicating an open space and an uppercase 'X' indicating a wall. There are no spaces in the input file.

# Output

For each test case, output one line containing the maximum number of blockhouses that can be placed in the city in a legal configuration. 

# Sample Input

**4.X......XX......2XX.X3.X.X.X.X.3....XX.XX4................0**

# **Sample Output**

**51524**


题意：有一个 n*n 的图，. 代表空白区域，X 代表墙，现在要在空白区域放置结点，要求同一行同一列只能放一个，除非有墙阻隔，问最多能放多少个点

思路：

只有在墙的阻隔情况下，才会出现一行/列出现多个点的情况，那么可以考虑进行缩点，将同一行且没有墙体阻隔的区域缩成一个点，放到左点集中，将同一列且没有墙体阻隔的区域缩成一个点，放到右点集中，从而建成一个二分图

**![](https://img-blog.csdnimg.cn/20190313212207838.png)**

假设 i 为行编号，j 为列编号，若 i-j 之间存在一条边，就相当于在方格 (i,j) 上放了一个点，这个假设使得在没有墙体阻隔的情况下，i 行 j 列不能再放其他的点，那么在不考虑 不能同行同列 的情况下，将所有边连接起来，即**行列缩点后，对应方格编号连边 **

建好图后，在图上求最大匹配即可

![](https://img-blog.csdnimg.cn/20190313212220428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=1E9+7;
const int N=1000+5;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int n;//n行n列
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
int G[N][N];//存边
char str[N][N];
int x[N][N],cntX;//行点集
int y[N][N],cntY;//列点集
bool dfs(int x){
    for(int y=0;y<cntY;y++){//对x的每个邻接点
        if(G[x][y]==1&&!vis[y]){//不在交替路中
            vis[y]=true;//放入交替路
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;//不存在增广路，返回失败
}
int hungarian(){
    int ans=0;//记录最大匹配数
    memset(link,-1,sizeof(link));
    for(int i=0;i<cntX;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
int main(){

    while(scanf("%d",&n)!=EOF&&n){
        memset(x,0,sizeof(x));
        memset(y,0,sizeof(y));
        memset(G,false,sizeof(G));

        for(int i=0;i<n;i++)
            scanf("%s",str[i]);

        //对行缩点
        cntX=1;
        for(int i=0;i<n;i++){//第i行
            for(int j=0;j<n;j++){//第j列
                if(str[i][j]=='.')//同一区域
                    x[i][j]=cntX;
                if(str[i][j]=='X')//墙体阻隔
                    cntX++;
            }
            cntX++;//下一行
        }

        //对列缩点
        cntY=1;
        for(int j=0;j<n;j++){//第j列
            for(int i=0;i<n;i++){//第i行
                if(str[i][j]=='.')//同一区域
                    y[i][j]=cntY;
                if(str[i][j]=='X')//墙体阻隔
                    cntY++;
            }
            cntY++;//下一列
        }

        //连边
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(str[i][j]=='.')
                    G[x[i][j]][y[i][j]]=true;

        printf("%d\n",hungarian());
    }
    return 0;
}
```







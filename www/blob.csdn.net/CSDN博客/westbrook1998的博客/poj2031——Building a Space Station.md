# poj2031——Building a Space Station - westbrook1998的博客 - CSDN博客





2018年08月13日 13:25:13[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：20标签：[最小生成树																[简单几何](https://so.csdn.net/so/search/s.do?q=简单几何&t=blog)](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
You are a member of the space station engineering team, and are assigned a task in the construction process of the station. You are expected to write a computer program to complete the task.  

  The space station is made up with a number of units, called cells. All cells are sphere-shaped, but their sizes are not necessarily uniform. Each cell is fixed at its predetermined position shortly after the station is successfully put into its orbit. It is quite strange that two cells may be touching each other, or even may be overlapping. In an extreme case, a cell may be totally enclosing another one. I do not know how such arrangements are possible.  

  All the cells must be connected, since crew members should be able to walk from any cell to any other cell. They can walk from a cell A to another cell B, if, (1) A and B are touching each other or overlapping, (2) A and B are connected by a `corridor’, or (3) there is a cell C such that walking from A to C, and also from B to C are both possible. Note that the condition (3) should be interpreted transitively.  

  You are expected to design a configuration, namely, which pairs of cells are to be connected with corridors. There is some freedom in the corridor configuration. For example, if there are three cells A, B and C, not touching nor overlapping each other, at least three plans are possible in order to connect all three cells. The first is to build corridors A-B and A-C, the second B-C and B-A, the third C-A and C-B. The cost of building a corridor is proportional to its length. Therefore, you should choose a plan with the shortest total length of the corridors.  

  You can ignore the width of a corridor. A corridor is built between points on two cells’ surfaces. It can be made arbitrarily long, but of course the shortest one is chosen. Even if two corridors A-B and C-D intersect in space, they are not considered to form a connection path between (for example) A and C. In other words, you may consider that two corridors never intersect.  

  Input 

  The input consists of multiple data sets. Each data set is given in the following format.  

  n  

  x1 y1 z1 r1  

  x2 y2 z2 r2  

  …  

  xn yn zn rn  

  The first line of a data set contains an integer n, which is the number of cells. n is positive, and does not exceed 100.  

  The following n lines are descriptions of cells. Four values in a line are x-, y- and z-coordinates of the center, and radius (called r in the rest of the problem) of the sphere, in this order. Each value is given by a decimal fraction, with 3 digits after the decimal point. Values are separated by a space character.  

  Each of x, y, z and r is positive and is less than 100.0.  

  The end of the input is indicated by a line containing a zero.  

  Output 

  For each data set, the shortest total length of the corridors should be printed, each in a separate line. The printed values should have 3 digits after the decimal point. They may not have an error greater than 0.001.  

  Note that if no corridors are necessary, that is, if all the cells are connected without corridors, the shortest total length of the corridors is 0.000.  

  Sample Input 

  3 

  10.000 10.000 50.000 10.000 

  40.000 10.000 50.000 10.000 

  40.000 40.000 50.000 10.000 

  2 

  30.000 30.000 30.000 20.000 

  40.000 40.000 40.000 20.000 

  5 

  5.729 15.143 3.996 25.837 

  6.013 14.372 4.818 10.671 

  80.115 63.292 84.477 15.120 

  64.095 80.924 70.029 14.881 

  39.472 85.116 71.369 5.553 

  0 

  Sample Output 

  20.000 

  0.000 

  73.834
这题真的有毒 wa到自闭，最后百度查了一个博客发现是g++编译的话，printf输出double要用%f而不能用%lf…… 

你妹

然后其他就是判断两个球的位置关系了，如果球面距离小于0，就把边权置为0

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
const int INF=0x3f3f3f3f;
double x[105];
double y[105];
double z[105];
double r[105];
int n;
double g[105][105];
double lowcost[105];
bool vis[105];
void init(){
    memset(x,0.0,sizeof(x));
    memset(y,0.0,sizeof(y));
    memset(z,0.0,sizeof(z));
    memset(g,0.0,sizeof(g));
}
double point_dis(int i,int j){
    return sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])+(z[i]-z[j])*(z[i]-z[j]))-r[i]-r[j];
}
double prim(){
    memset(lowcost,0,sizeof(lowcost));
    memset(vis,false,sizeof(vis));
    double ans=0;
    double Min=INF;
    int pos=1;
    vis[pos]=true;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            lowcost[i]=g[pos][i];
        }
    }
    for(int i=1;i<n;i++){
        Min=INF;
        //找到相邻最小点
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]<Min){
                Min=lowcost[j];
                pos=j;
            }
        }
        ans+=Min;
        vis[pos]=true;
        //更新lowcost
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]>g[pos][j]){
                lowcost[j]=g[pos][j];
            }
        }
    }
    return ans;
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d",&n) && n){
        init();
        for(int i=1;i<=n;i++){
            scanf("%lf%lf%lf%lf",&x[i],&y[i],&z[i],&r[i]);
        }
        //建图
        for(int i=1;i<=n-1;i++){
            for(int j=i+1;j<=n;j++){
                double dis=point_dis(i,j);
                if(dis<=0){
                    g[i][j]=g[j][i]=0;
                }
                else{
                    g[i][j]=g[j][i]=dis;
                }
            }
        }
        double ans=prim();
        printf("%.3f\n",ans);
    }
    return 0;
}
```







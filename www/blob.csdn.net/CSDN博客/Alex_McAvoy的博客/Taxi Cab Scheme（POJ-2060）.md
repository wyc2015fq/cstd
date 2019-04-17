# Taxi Cab Scheme（POJ-2060） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 19:43:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：32








> 
# Problem Description

Running a taxi station is not all that simple. Apart from the obvious demand for a centralised coordination of the cabs in order to pick up the customers calling to get a cab as soon as possible, there is also a need to schedule all the taxi rides which have been booked in advance. Given a list of all booked taxi rides for the next day, you want to minimise the number of cabs needed to carry out all of the rides. 

For the sake of simplicity, we model a city as a rectangular grid. An address in the city is denoted by two integers: the street and avenue number. The time needed to get from the address a, b to c, d by taxi is |a - c| + |b - d| minutes. A cab may carry out a booked ride if it is its first ride of the day, or if it can get to the source address of the new ride from its latest, at least one minute before the new ride’s scheduled departure. Note that some rides may end after midnight. 

# **Input**

On the first line of the input is a single positive integer N, telling the number of test scenarios to follow. Each scenario begins with a line containing an integer M, 0 < M < 500, being the number of booked taxi rides. The following M lines contain the rides. Each ride is described by a departure time on the format hh:mm (ranging from 00:00 to 23:59), two integers a b that are the coordinates of the source address and two integers c d that are the coordinates of the destination address. All coordinates are at least 0 and strictly smaller than 200. The booked rides in each scenario are sorted in order of increasing departure time. 

# Output

For each scenario, output one line containing the minimum number of cabs required to carry out all the booked taxi rides. 

# Sample Input

**2208:00 10 11 9 1608:07 9 16 10 11208:00 10 11 9 1608:06 9 16 10 11**

# Sample Output

**12**


题意：t 组数据，每组给出 n 个订单的开始时间和开始坐标与目的坐标，如果一辆车在处理完当前订单后能在下一个订单开始前一分钟到达目的地，那么可以继续接下来的订单，问处理完所有订单最少要多少辆车

思路：

对于每个订单，计算其完成时间，如果**前一个订单的结束时间+前一订单与下一订单的曼哈顿距离<小于下一个订单的开始时间**，就建立一条边，那么题目就转变为二分图的最小路径覆盖问题，然后跑匈牙利算法，最后用订单数减去最大匹配数即是答案

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
int n;
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
int G[N][N];//存边
bool dfs(int x){
    for(int y=1;y<=n;y++){//对x的每个邻接点
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
    for(int i=1;i<=n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
struct Node{
    int sx,sy;//开始坐标
    int ex,ey;//结束坐标
    int st;//开始时间
    int et;//结束时间
}a[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            int hour,mins;
            scanf("%d:%d",&hour,&mins);
            a[i].st=hour*60+mins;//转换为分钟

            scanf("%d%d%d%d",&a[i].sx,&a[i].sy,&a[i].ex,&a[i].ey);
            a[i].et=a[i].st+abs(a[i].ex-a[i].sx)+abs(a[i].ey-a[i].sy);//结束时间=开始时间+两坐标的曼哈顿距离
        }
        memset(G,false,sizeof(G));

        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(i==j)
                    G[i][j]=false;
                else{
                    int temp=abs(a[j].sx-a[i].ex)+abs(a[j].sy-a[i].ey);//上一订单与下一订单的曼哈顿距离
                    if(a[i].et+temp<a[j].st)//上一订单的结束时间+上一订单与下一订单的曼哈顿距离<下一订单的开始时间
                        G[i][j]=true;
                }
            }
        }
        printf("%d\n",n-hungarian());
    }
    return 0;
}
```






# 图论 —— 网络流 —— Edmonds-Karp 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 21:50:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58








# 【概述】

EK 算法是求解最大流的一般增广路方法，其是在 Ford-Fulkerson 算法的基础上求解的算法，该算法是求解最大流的最基础的算法，其时间复杂度为 O(n*m*m)。

其核心思想是增广路定理：**网络达到最大流当且仅当残留网络中没有增广路**

程序的实现过程与增广路求最大流的过程基本一致，即每一次更新都进行一次找增广路然后更新路径上的流量的过程。

在传统的 FF 算法中，每次找增广路的过程十分繁琐，常常会走冤枉路，此时更新增广路的复杂度就会增加，而 EK 算法为了规避这个问题使用了 bfs 来寻找增广路，然后在寻找增广路的时候总是向离汇点越来越近的方向去寻找下一个结点。

# 【基本思想】

1）若存在增广路径，则找出一条增广路径（通过 BFS）

2）沿着找出的增广路径进行更新流量

3）当没有增广路时，网络达到最大流

# 【沿增广路径增广方法】

第一步：计算可增加流量

  设：某一增广路径结点为 a1,a2,...,an，可增加流的增加流量 dis=INF

    若 (u,v) 是正向边，则：dis=min(dis,c(ai,aj)-f(ai,aj))，其中：j=i+1,i=1,2,...,n-1

    若 (u,v) 是逆向边，则：dis=min(dis,f(ai,aj))，其中：j=i+1,i=1,2,...,n-1

第二步：更新流量

  若 (u,v) 是正向边，则：f(u,v)=f(u,v)+dis

  若 (u,v) 是负向边，则：f(u,v)=f(u,v)-dis（伴随着这部分流量的减去，必有另一部分的管道流量会增加）

![](https://img-blog.csdnimg.cn/20181226134404210.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181226134414760.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181226134428360.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181226134440202.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【实现】

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#define INF 0x3f3f3f3f
#define N 101
int n,m;
int cap[N][N];//容量
int flow[N][N];//流量

int EK(int s,int t){//沿着增广路增广
    int res=0;//最大流
    int dis[N];//a[i]表示从s到i的最小残量
    int p[N];//增广路中的上一节点
    queue<int> Q;
    while(true){
        memset(dis,0,sizeof(dis));
        dis[s]=INF;
        Q.push(s);
        //计算可增加流量
        while(!Q.empty()){
            int x=Q.front();
            Q.pop();
            for(int y=1;y<=n;y++){
                if(!dis[y] && cap[x][y]>flow[x][y]){
                    p[y]=x;
                    Q.push(y);
                    dis[y]=min(dis[x],cap[x][y]-flow[x][y]);
                }
            }
        }

       if(dis[t]==0)//当网络中没有增广路径
            break;
       //更新流量
       for(int x=t;x!=s;x=p[x]){
           flow[p[x]][x]+=dis[t];
           flow[x][p[x]]-=dis[t];
       }
       res+=dis[t];
   }
   return res;//返回最大流
}

int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    memset(cap,0,sizeof(cap));
    memset(flow,0,sizeof(flow));
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);//两点的容量
        cap[x][y]=+w;//可能有重边
    }
    printf("%d\n",EK(1,n));
    return 0;
}
```




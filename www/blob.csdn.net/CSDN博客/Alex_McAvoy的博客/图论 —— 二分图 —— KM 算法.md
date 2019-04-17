# 图论 —— 二分图 —— KM 算法 - Alex_McAvoy的博客 - CSDN博客





2019年02月20日 22:52:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)









# 【原理】

KM 算法是用于求带权二分图的最优匹配的算法，其时间复杂度为 O(N^3)。

1.首先选择顶点数较少的为 X 部（左点集），初始时对 X 部的每一个顶点设置顶标，顶标的值为该点关联的最大边的权值，Y 部（右点集）的顶点顶标为 0。

2.对于 X 部中的每个顶点，在相等子图中利用匈牙利算法找一条增广路径，如果没有找到，则修改顶标，扩大相等子图，继续找增广路径。

3.当 X 部的每个点都找到增广路径时，此时意味着每个点都在匹配中，即找到了该二分图的完全匹配。该完全匹配即为二分图的最优匹配。

# 【有关概念】

1）相等子图：由于每个顶点有一个顶标，如果选择边权等于两端点的顶标之和的边，它们组成的图称为相等子图。

2）顶标：每个点的顶标为该点关联的最大边的权值。

# 【顶标的修改】

如果从 X 部中的某个点 Xi 出发在相等子图中没有找到增广路径，则需要修改顶标。

如果没有找到增广路径，则一定找到了许多条从 Xi 出发并结束于 X 部的匹配边与未匹配边交替出现的路径，即交错路。

将交错路中 X 部的顶点顶标减去一个值 d，交错路中属于 Y 部的顶点顶标加上一个值 d，那么会发现：
- 两端都在交错路中的边（i,j），其顶标和没有变化，即：其原属于相等子图，现仍属于相等子图。
- 两端都不在交错路中的边（i,j），其顶标也没有变化，即：其原来属于（或不属于）相等子图，现仍属于（或不属于）相等子图。
- X 端不在交错路中，Y 端在交错路中的边（i,j），其顶标和会增大，即：其原来不属于相等子图，现仍不属于相等子图。
- X 端在交错路中，Y 端不在交错路中的边（i,j），其顶标和会减小，即：其原来不属于相等子图，现可能进入相等子图，从而使相等子图得到扩大。

修改顶标的目的就是要扩大相等子图，为保证至少有一条边进入相等子图，可以在交错路的边中寻找顶标和与边权之差最小的边，也即前述的 d 值。

将交错路中属于 X 部的顶点减去 d，交错路中属于 Y 部的顶点加上 d，则可以保证至少有一条边扩充进入相等子图。

# 【相等子图的性质】

1）任意时刻，相等子图的 **最大权匹配 ≤ 相等子图的顶标和**

2）任意时刻，相等子图的 **顶标和=所有顶点的顶标和**

3）扩充相等子图后，相等子图的顶标和会减小

4）相等子图的 最大匹配=原图的完全匹配 时，匹配边的权值和=所有顶点的顶标和，此匹配即为最优匹配

# 【实现】

## 1.最优匹配

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#define INF 0x3f3f3f3f
#define N 1001
int n,m;//x、y中结点个数，下标从1开始
int G[N][N];//边权值矩阵
int Lx[N],Ly[N];//x、y中每个点的期望值
bool visX[N],visY[N];//标记左右点集是否已被访问过
int linkX[N],linkY[N];//linkX[i]表示与X部中点i匹配的点,linkY[i]表示与Y部中点i匹配的点,-1时表示无匹配
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){//不在交替路中
                visY[y]=true;//放入交替路
                if(linkY[y]==-1 || dfs(linkY[y])){//如果是未匹配点,说明交替路是增广路
                    linkX[x]=y;//交换路径
                    linkY[y]=x;
                    return true;//返回成功
                }
            }
        }
    }
    return false;//不存在增广路
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++){//找出边权与顶标和的最小的差值
        if(visX[i]){
            for(int j=1;j<=m;j++){
                if(!visY[j]){
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);
                }
            }
        }
    }

    for(int j=1;j<=n;j++)//将交错路中X部的点的顶标减去minn
        if(visX[j])
            Lx[j]-=minn;
    for(int j=1;j<=m;j++)//将交错路中Y部的点的顶标加上minn
        if(visY[j])
            Ly[j]+=minn;
}
int KM(){//更新理想值,纳入更多的边
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));
    memset(Lx,0,sizeof(Lx));
    memset(Ly,0,sizeof(Ly));
    
    for(int i=1;i<=n;i++)//更新理想值
        for(int j=1;j<=m;j++)
            Lx[i]=max(Lx[i],G[i][j]);

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++)
        if(linkY[i]!=-1)//若存在边
            ans+=G[linkY[i]][i];//统计边权和

    return ans;
}

int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                scanf("%d",&G[i][j]);

        printf("%d\n",KM());
    }
    return 0;
}
```

## 2.**边权和最小的完全匹配**

使用 KM 算法只能求二分图的最优匹配，即边权和最大的完全匹配， 这里有一个技巧，就是将所有的边权取负，再进行 KM 算法，得到的解取负就是边权和最小的完全匹配

假设存在一个最优解 res，是所有解中花费最小的，那么 -res 自然是所有花费中最大的解，当将所有边权取负后，用 KM 算法得到的最优匹配必然是那个花费最大的解，取负后就是所需的最小边权值的解

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#define INF 0x3f3f3f3f
#define N 1001
int n,m;//x、y中结点个数，下标从1开始
int G[N][N];//边权值矩阵
int Lx[N],Ly[N];//x、y中每个点的期望值
bool visX[N],visY[N];//标记左右点集是否已被访问过
int linkX[N],linkY[N];//linkX[i]表示与X部中点i匹配的点,linkY[i]表示与Y部中点i匹配的点,-1时表示无匹配
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){//不在交替路中
                visY[y]=true;//放入交替路
                if(linkY[y]==-1 || dfs(linkY[y])){//如果是未匹配点,说明交替路是增广路
                    linkX[x]=y;//交换路径
                    linkY[y]=x;
                    return true;//返回成功
                }
            }
        }
    }
    return false;//不存在增广路
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++){//找出边权与顶标和的最小的差值
        if(visX[i]){
            for(int j=1;j<=m;j++){
                if(!visY[j]){
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);
                }
            }
        }
    }

    for(int j=1;j<=n;j++)//将交错路中X部的点的顶标减去minn
        if(visX[j])
            Lx[j]-=minn;
    for(int j=1;j<=m;j++)//将交错路中Y部的点的顶标加上minn
        if(visY[j])
            Ly[j]+=minn;
}
int KM(){//更新理想值,纳入更多的边
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));
    memset(Lx,0,sizeof(Lx));
    memset(Ly,0,sizeof(Ly));
    
    for(int i=1;i<=n;i++)//更新理想值
        for(int j=1;j<=m;j++)
            Lx[i]=max(Lx[i],G[i][j]);

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}

int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                scanf("%d",&G[i][j]);

        G[i][j]=-G[i][j];


        printf("%d\n",-KM());
    }
    return 0;
}
```

## 3.最小有向环覆盖权值和

若原图能由多个不相交的有向环覆盖，那么二分图一定存在完全匹配，比如：原图中有向环为 1-2-3-1，则二分图的完全匹配就是 1-2'，2-3'，3-1'

由于有向环覆盖对应一个二分图的完全匹配，该完全匹配的权值对应有向环覆盖的权值，因此原图权值最大的有向环匹配就是二分图最优匹配的值

要求最小权值和，因此将边的权值全部取负，再进行计算，最后结果再取负即可

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#define INF 0x3f3f3f3f
#define N 1001
int n,m;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=m;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);
 
    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;
 
    for(int i=1;i<=m;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));
 
    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=m;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }
 
    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));
 
            if(dfs(i))
                break;
            else
                update();
        }
    }
 
    int ans=0;
    for(int i=1;i<=m;i++){
        if(G[linkY[i]][i]==-INF){
            return 1;
        }
        ans+=G[linkY[i]][i];
    }
 
    return ans;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                G[i][j]=-INF;
 
        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            G[x][y]=max(G[x][y],-w);//处理重边
            G[y][x]=G[x][y];//无向图
        }

        int res=-KM();
        if(res==-1)
            printf("NO\n";
        else
            printf("%d\n",res);
    }
    return 0;
}
```

## 4.优先用原匹配边构建的最优匹配

最优匹配可以直接使用 KM 模版，但是要在原匹配边的基础上使得改变的边最少，可以进行如下的处理：

左边点集有 n 个点，且 n<=m，则最优匹配必有 n 条边，让原图中的每条边的权值都乘以 (n+1)，即扩大 n+1 倍，且若某边本来就是原匹配用的其中一条边，那么该边权值在扩大 n+1 倍后，再加1。

因此任意一条边的权值只能是 **n+1 的倍数** 或 **(n+1 的倍数)+1**，要在这种权值的边中选出 n 条来，最终得到的最优权值和 ans  除以 n+1，即为最优匹配解，因此就算是所有边均使用原先的匹配，也即在所有权值的基础上加了 n 个 1，此时除以 n+1，减去原匹配的值就是最优匹配比原匹配增长的值

如果在新二分图中求出的权值和为 n+1 的倍数，则说明最优匹配中一条老边都没有复用。

综上：**所有边权值*(n+1)，老边再 +1，最终 ans%(n+1) 就是复用旧边的条数，ans/(n+1)-oldVal 就是最优匹配比原匹配增长的值。**

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#define INF 0x3f3f3f3f
#define N 1001
int n,m;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=m;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=m;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));

    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=m;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=m;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                scanf("%d",&G[i][j]);
                G[i][j]=G[i][j]*(n+1);//每条边乘以(n+1)
            }
        }

        int oldVal=0;//记录原匹配权值和
        for(int i=1;i<=n;i++){
            int j;
            scanf("%d",&j);
            oldVal+=(G[i][j]/(n+1));//累计原匹配权值
            G[i][j]++;//老边+1
        }

        int ans=KM();
        int v1=ans/(n+1);//最优匹配的权值和
        int v2=v1-oldVal;//最优匹配比原匹配相比多的权值数
        int v3=ans%(n+1);//最优匹配使用的老边数
        int v4=n-v3;//最优匹配使用的新边数

        printf("%d\n",v1);
        printf("%d\n",v2);
        printf("%d\n",v3);
        printf("%d\n",v4);
    }
    return 0;
}
```






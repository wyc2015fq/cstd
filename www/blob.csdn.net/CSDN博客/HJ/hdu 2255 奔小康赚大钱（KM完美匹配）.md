# hdu 2255 奔小康赚大钱（KM完美匹配） - HJ - CSDN博客
2017年10月14日 14:20:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：213
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2255](http://acm.split.hdu.edu.cn/showproblem.php?pid=2255)
KM的裸题。
简单描述一下KM算法的思想：
```
1）首先选择顶点数较少的为X部，初始的时候对X部的每一个顶点设置顶标，顶标的值为该点关联的最大边的权值，Y部的顶点坐标为0.
2）对于X部中的每个顶点，在相等子图中利用匈牙利算法找一条增广路径，如果没有找到，则修改顶标，扩大相等子图，继续找增广路径。当每个点都找到增广路径时，此时意味着每个点都在匹配中，即找到了二分图的完备匹配。该完备匹配即为二分图的最佳匹配。
```
**相等子图**
因为每个顶点都有一个顶标，如果选择边权等于两端点的顶标之和的边，它们组成的图成为相等子图。 
相等子图的相关性质：
```
① 在任意时刻，相等子图上的最大权匹配一定小于相等子图的顶标和；
② 在任意时刻，相等子图的顶标和即为所有顶点的顶标和；
③ 扩充相等子图后，相等子图的顶标和将减小；
④ 当相等子图的最大匹配为原图的完备匹配时，匹配边的权值和等于所有顶点的顶标和，此匹配即为最佳匹配；
```
**如何修改顶标**
从许多条Xi出发并结束于X部的匹配边与未匹配边交替出现的路径（姑且称之为交错树）。我们将交错树种的X部的顶点顶标减去一个值d，交错树中属于Y部的顶点顶标加上一个值d，那么就会发现： 
    ①两端都在交错树的边（i,j），其顶标和没有变化。 
    ②两端都不在交错树的边（i,j），其顶标和也没有变化。 
    ③X端不在交错树中，Y端在交错树中的边（i,j），它的顶标和会增大。 
    ④X端在交错树中，Y端不在交错树中的边（i,j），它的顶标和会减小，它原来不属于相等子图，现在可能进入了相等子图。
**d 值**
我们修改顶标的目的就是扩大相等子图。为了保证至少有一条边进入相等子图，我们可以在交错树的边中寻找顶标和与边权之差最小的边，这就是前面说的d值。将交错树中属于X部的顶点减去d，交错树中属于Y部的顶点加上d，则可以保证至少有一条边扩充进入相等子图。
KM 算法模板
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int N=305;
int n,w[N][N],link[N],slack[N],lx[N],ly[N];
bool visx[N],visy[N];
bool dfs(int u)
{
    visx[u]=true;
    for(int i=1;i<=n;i++)
    {
        if(visy[i])
          continue;
        int tmp=lx[u]+ly[i]-w[u][i];
        if(tmp)      //若lx[u]+ly[i]>=w[u][i] 更新点i顶标和与点i相关联的边的权值之差最小的差值 
          slack[i]=min(slack[i],tmp);
        else        //若 lx[u]+ly[i]==w[u][i] 表示这是一条可行边  按照匈牙利算法继续寻找增广路 
        {
            visy[i]=true;
            if(!link[i]||dfs(link[i]))
            {
                link[i]=u;
                return true;
            }
        }
    }
    return false;
}
int KM()
{
    memset(link,0,sizeof(link));
    memset(ly,0,sizeof(ly));       //ly[i]初始值赋为0 
    for(int i=1;i<=n;i++)
      for(int j=1;j<=n;j++)
        lx[i]=max(lx[i],w[i][j]);   //lx[i]初始标号为与这个i点相关联的权值最大的边的权值 
    for(int i=1;i<=n;i++)
    {
        memset(slack,INF,sizeof(slack));  //slack[i]保存与点i相关联的权值与点i顶标差值最小的值 因此初始值赋为无穷大 
        while(1)
        {
            memset(visx,false,sizeof(visx));
            memset(visy,false,sizeof(visy));
            if(dfs(i))
              break;
            int d=INF;
            for(int j=1;j<=n;j++)  
              if(!visy[j])  
                d=min(d,slack[j]);   // 在交错树中寻找顶标和与边权之差最小的边 并且找出这个值 更新为d值 
            for(int j=1;j<=n;j++)
            {
                if(visx[j])   //交错树中属于X部的顶点减去d 
                  lx[j]-=d;
                if(visy[j])  //交错树中属于Y部的顶点加上d 
                  ly[j]+=d;
                else       //不在交错树中属于Y部的顶点减去d 
                  slack[j]-=d;
            }
        }
    }   
    int ans=0;
    for(int i=1;i<=n;i++)
      if(link[i])
        ans+=w[link[i]][i];
    return ans;
}
int main()
{
    while(~scanf("%d",&n))
    {
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            scanf("%d",&w[i][j]);
        printf("%d\n",KM());
    }
    return 0;
}
```

# hdu 1533 Going Home（KM 最小权值匹配） - HJ - CSDN博客
2017年10月14日 14:25:38[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：139
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1533](http://acm.split.hdu.edu.cn/showproblem.php?pid=1533)
题意： 
给一个n*m的地图，’m’表示人，’H’表示房子，求所有人都回到房子所走的距离之和的最小值（距离为曼哈顿距离）。
思路： 
二分图最大权匹配模型，将每个人向房子连一条边，边权为曼哈顿距离的相反数（由于是求最小，所以先取反后求最大，最后再取反回来即可），然后用KM算法跑一遍然后取反就是答案。
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int N=120;
char map[N][N];
int n,lx[N],ly[N],link[N],slack[N],w[N][N];
bool visx[N],visy[N];
bool dfs(int u)
{
    visx[u]=true;
    for(int i=0;i<n;i++)
    {
        if(visy[i])
          continue;
        int tmp=lx[u]+ly[i]-w[u][i];
        if(tmp)
          slack[i]=min(slack[i],tmp);
        else
        {
            visy[i]=true;
            if(link[i]==-1||dfs(link[i]))
            {
                link[i]=u;
                return true;
            }
        }
    }
    return false;
}
void KM()
{
    memset(link,-1,sizeof(link));
    memset(ly,0,sizeof(ly));
    for(int i=0;i<n;i++)
    {
        lx[i]=-INF;           //赋初始值为负无穷大 
        for(int j=0;j<n;j++)
          lx[i]=max(lx[i],w[i][j]);        
    }
    for(int i=0;i<n;i++)
    {
        memset(slack,INF,sizeof(slack));
        while(1)
        {
            memset(visx,false,sizeof(visx));
            memset(visy,false,sizeof(visy));
            if(dfs(i))
              break;
            int d=INF;
            for(int j=0;j<n;j++)
              if(!visy[j])
                d=min(d,slack[j]);
            for(int j=0;j<=n;j++)
            {
                if(visx[j])
                  lx[j]-=d;
                if(visy[j])
                  ly[j]+=d;
                else
                  slack[j]-=d;
            }
        }
    }
} 
int main()
{
    int ans,row,col,numi,numj; 
    while(cin>>row>>col&&(row||col))
    {
        ans=n=numi=numj=0;
        for(int i=0;i<row;i++)
        {
            cin>>map[i];
            for(int j=0;j<col;j++)
              if(map[i][j]=='m')
                n++;
        }
        for(int i=0;i<row;i++)
          for(int j=0;j<col;j++)
            if(map[i][j]=='m')
            {
                for(int p=0;p<row;p++)
                  for(int q=0;q<col;q++)
                    if(map[p][q]=='H')
                      w[numi][numj++]=-(abs(i-p)+abs(j-q));    //保存曼哈顿距离（注意是负数） 
                numi++;
                numj=0;
            }
        KM();
        for(int i=0;i<n;i++)
          if(link[i]!=-1)
            ans+=w[link[i]][i];
        cout<<-ans<<endl;
    } 
    return 0;
}
```

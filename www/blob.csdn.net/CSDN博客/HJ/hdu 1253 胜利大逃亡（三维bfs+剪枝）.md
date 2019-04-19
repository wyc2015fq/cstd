# hdu 1253 胜利大逃亡（三维bfs+剪枝） - HJ - CSDN博客
2017年07月02日 13:00:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：402
Problem Description 
Ignatius被魔王抓走了,有一天魔王出差去了,这可是Ignatius逃亡的好机会.
魔王住在一个城堡里,城堡是一个A*B*C的立方体,可以被表示成A个B*C的矩阵,刚开始Ignatius被关在(0,0,0)的位置,离开城堡的门在(A-1,B-1,C-1)的位置,现在知道魔王将在T分钟后回到城堡,Ignatius每分钟能从一个坐标走到相邻的六个坐标中的其中一个.现在给你城堡的地图,请你计算出Ignatius能否在魔王回来前离开城堡(只要走到出口就算离开城堡,如果走到出口的时候魔王刚好回来也算逃亡成功),如果可以请输出需要多少分钟才能离开,如果不能则输出-1.
![这里写图片描述](https://img-blog.csdn.net/20170702125923697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Input 
输入数据的第一行是一个正整数K,表明测试数据的数量.每组测试数据的第一行是四个正整数A,B,C和T(1<=A,B,C<=50,1<=T<=1000),它们分别代表城堡的大小和魔王回来的时间.然后是A块输入数据(先是第0块,然后是第1块,第2块……),每块输入数据有B行,每行有C个正整数,代表迷宫的布局,其中0代表路,1代表墙.(如果对输入描述不清楚,可以参考Sample Input中的迷宫描述,它表示的就是上图中的迷宫)
特别注意:本题的测试数据非常大,请使用scanf输入,我不能保证使用cin能不超时.在本OJ上请使用Visual C++提交.
Output 
对于每组测试数据,如果Ignatius能够在魔王回来前离开城堡,那么请输出他最少需要多少分钟,否则输出-1.
Sample Input
1 
3 3 4 20 
0 1 1 1 
0 0 1 1 
0 1 1 1 
1 1 1 1 
1 0 0 1 
0 1 1 1 
0 0 0 0 
0 1 1 0 
0 1 1 0
Sample Output
11
也是三维bfs题目。但关键点在于一个剪枝：当从左上角（0,0,0）到达右下角（a-1,b-1,c-1）最少步数都大于t的时候，就不必bfs了，即
```
if(a+b+c-3>t)  ans=-1;
 else  ans=bfs(0,0,0);
```
会发现，这一步剪枝时间快了很多！
![这里写图片描述](https://img-blog.csdn.net/20170702125900296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;
#define maxn 60
int s[maxn][maxn][maxn],vis[maxn][maxn][maxn];
int a,b,c,t,dx[]={0,0,0,0,1,-1},dy[]={0,0,1,-1,0,0},dz[]={1,-1,0,0,0,0};
struct node{
    int x,y,z;
    int step;
};
int check(int x,int y,int z)
{
    if(x<0||x>=a||y<0||y>=b||z<0||z>=c||s[x][y][z])
      return 0;
    return 1;
}
int bfs(int x,int y,int z)
{
    queue<node> q;
    node m,next;
    m.x=x;m.y=y;m.z=z;
    m.step=0;
    vis[x][y][z]=1;
    q.push(m);
    while(!q.empty())
    {
        m=q.front();
        q.pop();
        if(m.x==a-1&&m.y==b-1&&m.z==c-1&&m.step<=t)
          return m.step;
        for(int i=0;i<6;i++)
        {
            next=m;
            next.x+=dx[i];next.y+=dy[i];next.z+=dz[i];
            if(check(next.x,next.y,next.z)&&!vis[next.x][next.y][next.z])
            {
                vis[next.x][next.y][next.z]=1;
                next.step++;
                q.push(next);
            } 
        }
    }
    return -1;
}
int main()
{
    int tt;
    scanf("%d",&tt);
    while(tt--)
    {
        scanf("%d%d%d%d",&a,&b,&c,&t);
        memset(s,0,sizeof(s));
        memset(vis,0,sizeof(vis));
        for(int i=0;i<a;i++)
          for(int j=0;j<b;j++)
            for(int k=0;k<c;k++)
              scanf("%d",&s[i][j][k]);
        int ans=(a+b+c-3<=t)?bfs(0,0,0):-1;    //剪枝 
        printf("%d\n",ans);
    }
    return 0;
}
```

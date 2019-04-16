# hdu 1429 胜利大逃亡(续)  bfs+状态压缩 - 紫芝的博客 - CSDN博客





2018年05月01日 19:01:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：29
个人分类：[BFS](https://blog.csdn.net/qq_40507857/article/category/7566192)











# 胜利大逃亡(续)
**Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 10137    Accepted Submission(s): 3664**


Problem Description

Ignatius再次被魔王抓走了(搞不懂他咋这么讨魔王喜欢)……

这次魔王汲取了上次的教训，把Ignatius关在一个n*m的地牢里，并在地牢的某些地方安装了带锁的门，钥匙藏在地牢另外的某些地方。刚开始Ignatius被关在(sx,sy)的位置，离开地牢的门在(ex,ey)的位置。Ignatius每分钟只能从一个坐标走到相邻四个坐标中的其中一个。魔王每t分钟回地牢视察一次，若发现Ignatius不在原位置便把他拎回去。经过若干次的尝试，Ignatius已画出整个地牢的地图。现在请你帮他计算能否再次成功逃亡。只要在魔王下次视察之前走到出口就算离开地牢，如果魔王回来的时候刚好走到出口或还未到出口都算逃亡失败。





Input

每组测试数据的第一行有三个整数n,m,t(2<=n,m<=20,t>0)。接下来的n行m列为地牢的地图，其中包括:

.   代表路
*   代表墙
@   代表Ignatius的起始位置
^   代表地牢的出口
A-J 代表带锁的门,对应的钥匙分别为a-j
a-j 代表钥匙，对应的门分别为A-J

每组测试数据之间有一个空行。





Output

针对每组测试数据，如果可以成功逃亡，请输出需要多少分钟才能离开，如果不能则输出-1。





Sample Input


4 5 17
@A.B.
a*.*.
*..*^
c..b*

4 5 16
@A.B.
a*.*.
*..*^
c..b*





Sample Output


16
-1





Author

LL




Source

[ACM暑期集训队练习赛（三）](http://acm.hdu.edu.cn/search.php?field=problem&key=+ACM%CA%EE%C6%DA%BC%AF%D1%B5%B6%D3%C1%B7%CF%B0%C8%FC%A3%A8%C8%FD%A3%A9&source=1&searchmode=source)




Recommend

linle


思路：
1.因为有10种不同的钥匙，每种都有两种状态，结合二进制的特点，
刚好把这10把钥匙当成每一个位，要 1<<10 个位保存所有的状态


2.模拟捡起钥匙，捡起钥匙就是说明这个位上的数字变成1状态
所以想到位运算 |，改变这个点的状态


3.模拟碰到门的情况，就和这个位置上的位&一次，

结果是1代表有钥匙，可以开门 

```cpp
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<queue>
using namespace std;
const int maxn=20+10;
char map[maxn][maxn];
int n,m,t;
bool vis[maxn][maxn][(1<<10)+10];
//状态压缩，最多有10种钥匙 
int dx[]={-1,1,0,0};
int dy[]={0,0,-1,1};
struct Point
{
    int x,y,step;
    int key;
};
queue<Point>Q;
Point st;
bool check(int x,int y)
{
    if(x>=1&&x<=n&&y>=1&&y<=m&&map[x][y]!='*')
         return true;
    return false;
}
int bfs()
{
    while(!Q.empty())  Q.pop();
    memset(vis,false,sizeof(vis));
    vis[st.x][st.y][st.key]=true;
    st.key=st.step=0;
    Q.push(st);
    Point cur,nex;
    while(!Q.empty())
    {
        cur=Q.front();
        Q.pop();
        if(map[cur.x][cur.y]=='^')
            return cur.step;
        for(int i=0;i<4;i++)
        {
            nex.x=cur.x+dx[i];
            nex.y=cur.y+dy[i];
            nex.key=cur.key;
            if(check(nex.x,nex.y))
            {
               nex.step=cur.step+1;
               if(nex.step>=t)
                   continue;
               else if(map[nex.x][nex.y]>='A'&&map[nex.x][nex.y]<='Z')
               {
                  int temp=map[nex.x][nex.y]-'A';
                  //int nk=cur.key&(1<<temp);
                  int nk=cur.key&1<<temp;//先左移，后与运算 
                  if(nk&&!vis[nex.x][nex.y][nex.key])
                  {
                      vis[nex.x][nex.y][nex.key]=true;
                      Q.push(nex);
                  }
               }
               else if(map[nex.x][nex.y]>='a'&&map[nex.x][nex.y]<='z')
               {
                   int temp=map[nex.x][nex.y]-'a';
                   //nex.key=cur.key|(1<<temp);
                   nex.key=cur.key|1<<temp;//先左移，后或运算 
                   if(!vis[nex.x][nex.y][nex.key])
                   {
                       vis[nex.x][nex.y][nex.key]=true;
                       Q.push(nex);
                   }
               }
               else
               {
                   if(!vis[nex.x][nex.y][nex.key])
                   {
                       vis[nex.x][nex.y][nex.key]=true;
                       Q.push(nex);
                   }
               }//else
            }//if
         }//for
    }
    return -1;
}
inline void read_graph()
{
    char str[maxn];
    for(int i=1;i<=n;i++)
    {
        scanf("%s",str+1);
        for(int j=1;j<=m;j++)
        {
            if(str[j]=='@')
            {
                st.x=i;
                st.y=j;
                map[i][j]=str[j];
            }
            else  map[i][j]=str[j];
        }
}
} 
int main()
{
    while(~scanf("%d%d%d",&n,&m,&t))
    {
        read_graph();
        int ans=bfs();
        printf("%d\n",ans);
    }
    return 0;
}
```





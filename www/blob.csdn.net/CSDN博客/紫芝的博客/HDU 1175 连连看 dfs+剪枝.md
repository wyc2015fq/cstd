# HDU 1175 连连看 dfs+剪枝 - 紫芝的博客 - CSDN博客





2018年05月11日 13:01:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：49








# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1175)

# 连连看
****Time Limit: 20000/10000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 40337    Accepted Submission(s): 9971****


Problem Description

“连连看”相信很多人都玩过。没玩过也没关系，下面我给大家介绍一下游戏规则：在一个棋盘中，放了很多的棋子。如果某两个相同的棋子，可以通过一条线连起来（这条线不能经过其它棋子），而且线的转折次数不超过两次，那么这两个棋子就可以在棋盘上消去。不好意思，由于我以前没有玩过连连看，咨询了同学的意见，连线不能从外面绕过去的，但事实上这是错的。现在已经酿成大祸，就只能将错就错了，连线不能从外围绕过。
玩家鼠标先后点击两块棋子，试图将他们消去，然后游戏的后台判断这两个方格能不能消去。现在你的任务就是写这个后台程序。





Input

输入数据有多组。每组数据的第一行有两个正整数n,m(0<n<=1000,0<m<1000)，分别表示棋盘的行数与列数。在接下来的n行中，每行有m个非负整数描述棋盘的方格分布。0表示这个位置没有棋子，正整数表示棋子的类型。接下来的一行是一个正整数q(0<q<50)，表示下面有q次询问。在接下来的q行里，每行有四个正整数x1,y1,x2,y2,表示询问第x1行y1列的棋子与第x2行y2列的棋子能不能消去。n=0,m=0时，输入结束。
注意：询问之间无先后关系，都是针对当前状态的！





Output

每一组输入数据对应一行输出。如果能消去则输出"YES",不能则输出"NO"。





Sample Input


3 4
1 2 3 4
0 0 0 0
4 3 2 1
4
1 1 3 4
1 1 2 4
1 1 3 3
2 1 2 4
3 4
0 1 4 3
0 2 4 1
0 0 0 0
2
1 1 2 4
1 3 2 3
0 0





Sample Output


YES
NO
NO
NO
NO
YES






Author

lwg




Recommend

We have carefully selected several similar problems for you:  [1180](http://acm.hdu.edu.cn/showproblem.php?pid=1180)[1072](http://acm.hdu.edu.cn/showproblem.php?pid=1072)[1016](http://acm.hdu.edu.cn/showproblem.php?pid=1016)[1026](http://acm.hdu.edu.cn/showproblem.php?pid=1026)[1010](http://acm.hdu.edu.cn/showproblem.php?pid=1010)


## 思路：
定义函数时注意设计参数：dfs(x,y,dic,turns);
x,y为当前搜索起点
dic为当前前进方向
turns为当前转弯数
剪枝：第二次转弯后，判断与目标是否在同一条直线上


## 前进方向：
![](https://img-blog.csdn.net/20180511130004112)

AC代码：

```cpp

```


#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

int maze[1010][1010];
bool vis[1010][1010];
int sx,sy,ex,ey;
bool flag;
int n,m,q;
/*
int dicx[]={1,-1,0,0};
int dicy[]={0,0,1,-1};
*/
int to[4][2]={{0,-1},{-1,0},{0,1},{1,0}};
void dfs(int x,int y,int dic,int turns){
    if(turns>2||flag) return;//转弯次数大于2或者已经找到就终止
    if(turns==2&&(x-ex)!=0&&(y-ey)!=0) return;//剪枝：判断两次转弯后是否与目标在同一直线上
    if(x==ex&&y==ey&&turns<=2){//搜索终点
        flag=1;
        return;
    }
    /*
    for(int i=0;i<4;++i){//搜索四个方向
        int xx=x+dicx[i];
        int yy=y+dicy[i];
        if(xx<1||xx>n||yy<1||yy>m||vis[xx][yy]) continue;//边界情况
        if(maze[xx][yy]==0||(xx==ex&&yy==ey)){
            vis[xx][yy]=1;
            if(dic==-1||dic==i)//如果在起点或者同向的情况turns不变及不转向,并将当前方向记为i
                dfs(xx,yy,i,turns);
            else
                dfs(xx,yy,i,turns+1);//否则turns+1
            vis[xx][yy]=0;
        }
    }
    */
    for(int i=0;i<4;++i){//搜索四个方向
        int xx=x+to[i][0];
        int yy=y+to[i][1];
        if(xx<1||xx>n||yy<1||yy>m||vis[xx][yy]) continue;//边界情况
        if(maze[xx][yy]==0||(xx==ex&&yy==ey)){
            vis[xx][yy]=1;
            if(dic==-1||dic==i)//如果在起点或者同向的情况turns不变及不转向,并将当前方向记为i
                dfs(xx,yy,i,turns);
            else
                dfs(xx,yy,i,turns+1);//否则turns+1
            vis[xx][yy]=0;
        }
    }
    return;
}

int main(){
    while(~scanf("%d%d",&n,&m)){
        if(n==0&&m==0)
            break;
        memset(maze,0,sizeof(maze));

        for(int i=1;i<=n;++i)
            for(int j=1;j<=m;++j)
                scanf("%d",&maze[i][j]);

        scanf("%d",&q);
        for(int i=0;i<q;++i){
            scanf("%d%d%d%d",&sx,&sy,&ex,&ey);
            memset(vis,0,sizeof(vis));

            flag=0;//初始化
            if(maze[sx][sy]==maze[ex][ey]&&maze[sx][sy])
                dfs(sx,sy,-1,0);//将初始方向设为-1

            if(flag) printf("YES\n");
            else printf("NO\n");
        }
    }
    return 0;
}









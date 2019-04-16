# HDU 2612 Find a way bfs - 紫芝的博客 - CSDN博客





2018年05月30日 22:16:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：28
个人分类：[BFS](https://blog.csdn.net/qq_40507857/article/category/7566192)









# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=2612)

# Find a way
**Time Limit: 3000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 21857    Accepted Submission(s): 7120**


Problem Description

Pass a year learning in Hangzhou, yifenfei arrival hometown Ningbo at finally. Leave Ningbo one year, yifenfei have many people to meet. Especially a good friend Merceki.
Yifenfei’s home is at the countryside, but Merceki’s home is in the center of city. So yifenfei made arrangements with Merceki to meet at a KFC. There are many KFC in Ningbo, they want to choose one that let the total time to it be most smallest. 
Now give you a Ningbo map, Both yifenfei and Merceki can move up, down ,left, right to the adjacent road by cost 11 minutes.





Input

The input contains multiple test cases.
Each test case include, first two integers n, m. (2<=n,m<=200). 
Next n lines, each line included m character.
‘Y’    express  yifenfei initial position.
‘M’      express  Merceki initial position.
‘#’    forbid road;
‘.’     Road.
‘@’   KCF





Output

For each test case output the minimum total time that both yifenfei and Merceki to arrival one of KFC.You may sure there is always have a KFC that can let them meet.




Sample Input


4 4

Y.#@

....

.#..

@..M

4 4

Y.#@

....

.#..

@#.M

5 5

Y..@.

.#...

.#...

@..M.

#...#




Sample Output


66

88

66


Author

yifenfei




Source

[奋斗的年代](http://acm.hdu.edu.cn/search.php?field=problem&key=%B7%DC%B6%B7%B5%C4%C4%EA%B4%FA&source=1&searchmode=source)




Recommend

yifenfei   |   We have carefully selected several similar problems for you:  [2717](http://acm.hdu.edu.cn/showproblem.php?pid=2717)[1254](http://acm.hdu.edu.cn/showproblem.php?pid=1254)[1728](http://acm.hdu.edu.cn/showproblem.php?pid=1728)[2102](http://acm.hdu.edu.cn/showproblem.php?pid=2102)[1072](http://acm.hdu.edu.cn/showproblem.php?pid=1072)





## 题意：


给一张地图，Y代表第一个人所在的位置，M代表第二个人所在的位置，.代表路，#代表墙，@代表KFC，

两个人想在同一个kfc见面，问他们见面花费最小的时间；




## 分析：


遍历地图，对于每一KFC，分别求出Y到这个KFC的距离dy，M到这个KFC的距离dm，

如果距离为dy=-1或者dm=-1，则说明无法到达；
否则，跟新最短距离：ans=min(ans,dy+dm);
这个思路比较简单，但是当KFC的数量非常多的时候，就会超时

根据上面的思路，我们可以优化一下，减少重复计算，设置一个距离数组dis[N][N][2];初始化为INF

dis[i][j][0]代表Y到坐标为（i，j）的KFC的距离，dis[i][j][1]代表M到坐标为（i，j）的KFC的距离

然后，BFS求出Y到地图上所有点的最短距离，若这个点是KFC，则更新dis[i][j][0],

同理，BFS求出M到地图上所有点的最短距离，若这个点是KFC，则更新dis[i][j][1],

最后遍历地图，如果这个点是KFC，且当前最短距离 > Y到这个KFC的距离+M到这个KFC的距离，

则更新最短距离，最后不要忘记*11哦


超时代码


```cpp
#include<iostream>
#include<cstring>
#include<queue>
#include<cmath>
#include<cstdio>
using namespace std;
#define MAXV 209
char mp[MAXV][MAXV];//地图
bool vis[MAXV][MAXV];//判断点是否在队列
int n,m;
struct point
{
    int x,y,step;
};
point Y,M;//两人的位置
vector<point> KFC;//储存KFC的位置
int dic[4][2]={0,1,0,-1,1,0,-1,0};
bool check(int x,int y)//若这一点可以走，返回0
{
    if(x<0||x>=n||y<0||y>=m||vis[x][y]||mp[x][y]=='#')
        return 1;
    return 0;
}
int bfs(point s,point e)//求s点到e点的最短距离，无法到达距离为-1
{
    memset(vis,0,sizeof(vis));
    queue<point>q;
    q.push(s);//起点入队
    vis[s.x][s.y]=1;
    while(!q.empty())
    {
        point now=q.front();
        q.pop();
        if(now.x==e.x&&now.y==e.y)//已经到达终点
        return now.step;
        for(int i=0;i<4;i++)//上下左右，四个方向搜索
        {
            int x=now.x+dic[i][0];
            int y=now.y+dic[i][1];
            if(check(x,y))//不合法
                continue;
            point next;
            next.x=x;next.y=y;
            next.step=now.step+1;
            vis[x][y]=1;
            q.push(next);
        }
    }
    return -1;//无法到达
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
            KFC.clear();
        for(int i=0;i<n;i++)
        {
            scanf("%s",mp[i]);
            for(int j=0;j<m;j++)
            {
                if(mp[i][j]=='Y')
                {
                    Y.x=i;Y.y=j;Y.step=0;
                }
                if(mp[i][j]=='M')
                {
                    M.x=i;M.y=j;M.step=0;
                }
                if(mp[i][j]=='@')
                {
                    point temp;
                    temp.x=i;temp.y=j;temp.step=0;
                    KFC.push_back(temp);
                }
            }
        }
        int ans=1<<20;
        for(int i=0;i<KFC.size();i++)//遍历所有KFC
        {
          int dy=bfs(Y,KFC[i]),dm=bfs(M,KFC[i]);
          if(dy==-1||dm==-1)
            continue;
          ans=min(ans,dy+dm);
        }
        cout<<ans*11<<endl;
    }
    return 0;
}
```


AC代码


```cpp
#include<iostream>
#include<queue>
#include<cstdio>
#include<cstring>
using namespace std;
const int N = 210;
const int inf = 100000000;
int n, m, flag;//flag=0代表Y，flag=1代表M
int dis[N][N][2];
//dis[i][j][0]代表Y到坐标为（i，j）的KFC的距离
//dis[i][j][1]代表M到坐标为（i，j）的KFC的距离
int mark[N][N];//标记点是否在队列中
int dir[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}};//搜索方向
char s[N][N];//地图
struct node
{
    int x, y, step;
};
void bfs(int x, int y)
{
    queue<node>q;
    node temp, type;
    temp.x = x;
    temp.y = y;
    temp.step = 0;
    q.push(temp);//起点入队
    mark[x][y] = 1;//标记

    while(!q.empty())
    {
        temp = q.front();
        q.pop();//出队
        type.step = temp.step + 1;//步数加一
        for(int i = 0; i < 4; i++)
        {
            type.x = x = temp.x + dir[i][0];
            type.y = y = temp.y + dir[i][1];
            if(x >= 0 && x < n && y >= 0 && y < m && mark[x][y] == 0 && s[x][y]!='#')//可以走
            {
                mark[x][y] = 1;
                if(s[x][y] == '@')
                     dis[x][y][flag] = type.step;//距离
                q.push(type);
            }
        }
    }
}
int main()
{
    while(scanf("%d%d", &n, &m)!=EOF)
    {
        int min = inf;
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
        dis[i][j][0] = dis[i][j][1] = inf;//初始化距离为INF

        for(int i = 0; i < n; i++)
            scanf("%s", s[i]);
            for(int i = 0; i < n; i++)
                for(int j = 0; j < m; j++)
            {
                if(s[i][j] == 'Y')
                {
                    flag = 0;
                    memset(mark, 0, sizeof(mark));
                    bfs(i, j);
                }
                if(s[i][j] == 'M')
                {
                    flag = 1;
                    memset(mark, 0, sizeof(mark));
                    bfs(i, j);
                }
            }
            for(int i = 0; i < n; i++)
                for(int j = 0; j < m; j++)
                if(s[i][j] == '@' && min > dis[i][j][0] + dis[i][j][1])//松驰操作
            {
                min = dis[i][j][0] + dis[i][j][1];//更新最短距离
            }
            printf("%d\n", min*11);
    }
}
```







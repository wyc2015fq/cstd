# Rescue（HDU-1242） - Alex_McAvoy的博客 - CSDN博客





2018年03月18日 10:57:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：98
个人分类：[HDU																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

    Angel's friends want to save Angel. Their task is: approach Angel. We assume that approach Angel is to get to the position where Angel stays. When there's a guard in the grid, we must kill him (or her?) to move into the grid. We assume that we moving up, down, right, left takes us 1 unit time, and killing a guard takes 1 unit time, too. And we are strong enough to kill all the guards.

    You have to calculate the minimal time to approach Angel. (We can move only UP, DOWN, LEFT and RIGHT, to the neighbor grid within bound, of course.)

# Input

    First line contains two integers stand for N and M.

    Then N lines follows, every line has M characters. "." stands for road, "a" stands for Angel, and "r" stands for each of Angel's friend.

    Process to the end of the file.

# Output

     For each test case, your program should output a single integer, standing for the minimal time needed. If such a number does no exist, you should output a line containing "Poor ANGEL has to stay in the prison all his life." 

# Sample Input

**7 8    #.#####.    #.a#..r.    #..#x...    ..#..#.#    #...##..    .#......    ........**

# Sample Output

**   13**


思路：由于朋友r可能不止一个，因此从朋友找天使的角度来说较为繁琐，故以天使找朋友的角度来搜索即可。

# Source Program

```cpp
#include<iostream>
#include<queue>
#include<cstring>
using namespace std;

int n,m;
char maps[201][201];
int used[201][201];
int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};//方向数组

struct node
{
    int x;
    int y;
    int step;
}start,endd;

bool judge(int x,int y)//越界判断
{
    if(x<0||y<0||x>=n||y>=m||used[x][y]||maps[x][y]=='#')
        return false;
    else
        return true;
}
void bfs()
{
    int i;
    queue <node> q;
    used[start.x][start.y]=1;
    q.push(start);//初始数据入队

    while(!q.empty())
    {
        start=q.front();
        q.pop();
        if(maps[start.x][start.y]=='r')//营救到天使，结束搜索
        {
            cout<<start.step<<endl;
            return;
        }
        if(maps[start.x][start.y]=='x')//遇到守卫
        {
            maps[start.x][start.y]='.';//进行标记，打败守卫
            start.step=start.step+1;//多花1个单位时间
            used[start.x][start.y]=1;
            q.push(start);//元素入队
        }
        for(i=0;i<4;i++)
        {
            endd.x=start.x+dir[i][0];
            endd.y=start.y+dir[i][1];
            endd.step=start.step+1;

            if(judge(endd.x,endd.y))//子节点满足条件
            {
                used[endd.x][endd.y]=1;//标记
                q.push(endd);//元素入队
            }
        }
    }
    cout<<"Poor ANGEL has to stay in the prison all his life."<<endl;
}
int main()
{
    int i,j;
    while(cin>>n>>m)
    {
        memset(used,0,sizeof(used));//地图清零
        for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                cin>>maps[i][j];
                if(maps[i][j]=='a')//记录起始位置
                {
                    start.x=i;
                    start.y=j;
                    start.step=0;
                }
            }
        }
        bfs();
    }
    return 0;
}
```







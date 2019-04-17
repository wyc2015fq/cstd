# T1249 Lake Counting（#Ⅱ- 8） - Alex_McAvoy的博客 - CSDN博客





2018年05月26日 12:30:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：587
个人分类：[信息学奥赛一本通																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7429967)








> 
# 【题目描述】

有一块N×M的土地，雨后积起了水，有水标记为‘W’，干燥为‘.’。八连通的积水被认为是连接在一起的。请求出院子里共有多少水洼？

# 【输入】

第一行为N,M(1≤N,M≤110)。

下面为N*M的土地示意图。

# 【输出】

一行，共有的水洼数。

# 【输入样例】

**10 12W........WW..WWW.....WWW....WW...WW..........WW..........W....W......W...W.W.....WW.W.W.W.....W..W.W......W...W.......W.**

# 【输出样例】

**3**


# 【源程序】

```cpp
#include<cstdio>
int n,m;
char a[120][120];
int dir[8][2]={{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};
struct node
{
    int x;
    int y;
}q[100000];
void bfs(int x0,int y0)
{
    int head=1,tail=1;

    a[x0][y0]='.';
    q[tail].x=x0;
    q[tail].y=y0;
    tail++;

    while(head<tail)
    {
        int x=q[head].x;
        int y=q[head].y;
        for(int i=0;i<8;i++)
        {
            int nx=x+dir[i][0];
            int ny=y+dir[i][1];
            if(nx>=0&&nx<n&&ny>=0&&ny<m&&a[nx][ny]=='W')
            {
                a[nx][ny]='.';
                q[tail].x=nx;
                q[tail].y=ny;
                tail++;
            }
        }
        head++;
    }
}
int main()
{
    int sum=0;
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++)
        scanf("%s",a[i]);
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            if(a[i][j]=='W')
            {
                sum++;
                bfs(i,j);
            }
    printf("%d\n",sum);
    return 0;
}
```







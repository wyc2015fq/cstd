# HDU1045 Fire Net 递归回溯 - 紫芝的博客 - CSDN博客





2018年05月28日 21:31:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：38
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)









# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1045)

# Fire Net

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 14238    Accepted Submission(s): 8616****

Problem Description

Suppose that we have a square city with straight streets. A map of a city is a square board with n rows and n columns, each representing a street or a piece of wall.


A blockhouse is a small castle that has four openings through which to shoot. The four openings are facing North, East, South, and West, respectively. There will be one machine gun shooting through each opening.


Here we assume that a bullet is so powerful that it can run across any distance and destroy a blockhouse on its way. On the other hand, a wall is so strongly built that can stop the bullets.


The goal is to place as many blockhouses in a city as possible so that no two can destroy each other. A configuration of blockhouses is legal provided that no two blockhouses are on the same horizontal row or vertical column in a map unless there is at least one wall separating them. In this problem we will consider small square cities (at most 4x4) that contain walls through which bullets cannot run through.


The following image shows five pictures of the same board. The first picture is the empty board, the second and third pictures show legal configurations, and the fourth and fifth pictures show illegal configurations. For this board, the maximum number of blockhouses in a legal configuration is 5; the second picture shows one way to do it, but there are several other ways.

![](http://acm.hdu.edu.cn/data/images/1045-1.jpg)


Your task is to write a program that, given a description of a map, calculates the maximum number of blockhouses that can be placed in the city in a legal configuration.

### Input

The input file contains one or more map descriptions, followed by a line containing the number 0 that signals the end of the file. Each map description begins with a line containing a positive integer n that is the size of the city; n will be at most 4. The next n lines each describe one row of the map, with a '.' indicating an open space and an uppercase 'X' indicating a wall. There are no spaces in the input file.

### Output

For each test case, output one line containing the maximum number of blockhouses that can be placed in the city in a legal configuration.

## Sample Input

4

.X..

....

XX..

.... 2

XX .X

3

.X.

X.X

.X.

3

...

.XX

.XX

4

....

....

....

....

0

## Sample Output

5

1

5

2

4

Source

[Zhejiang University Local Contest 2001](http://acm.hdu.edu.cn/search.php?field=problem&key=Zhejiang+University+Local+Contest+2001&source=1&searchmode=source)

Recommend

We have carefully selected several similar problems for you:  [1016](http://acm.hdu.edu.cn/showproblem.php?pid=1016)[1051](http://acm.hdu.edu.cn/showproblem.php?pid=1051)[1050](http://acm.hdu.edu.cn/showproblem.php?pid=1050)[1312](http://acm.hdu.edu.cn/showproblem.php?pid=1312)[1067](http://acm.hdu.edu.cn/showproblem.php?pid=1067)+

### 题意：

给一个二维地图，“.”表示可以放置一个炮台，"X"表示墙壁，炮台不能放在同行同列，除非中间有墙壁

求最多可以放多少个炮台？

### 题解：

```cpp
#include<bits/stdc++.h>
using namespace std;
int n;
int mp[5][5];//-1表示墙，0表示可行，1表示炮台
struct point
{
    int x,y;
};
int dic[4][2]={{-1,0},{1,0},{0,1},{0,-1}};
point p[20];
int cnt,ans;
int judge(int x,int y)
{
    if(x<0||y<0||x>=n||y>=n)
        return -1;//墙壁
    return mp[x][y];
}

int check(int k)
{
    int flag=0;
    int x,y;
    for(int i=0;i<4;i++)
    {
        if(flag)    break;
        x=p[k].x+dic[i][0];
        y=p[k].y+dic[i][1];
        while(x>=0&&y>=0&&x<n&&y<n)
        {
            int t=judge(x,y);
            if(t==-1)   break;
            if(t==1){//找到此行或此列已经有炮台即退出
                flag=1; break;
            }
            x+=dic[i][0];
            y+=dic[i][1];
        }
    }
    return flag;
}
void dfs(int cur,int turn)
{
    if(turn>ans)    ans=turn;
    if(cur==cnt)    return;
    dfs(cur+1,turn);
    if(!check(cur))
    {
        mp[p[cur].x][p[cur].y]=1;
        dfs(cur+1,turn+1);
        mp[p[cur].x][p[cur].y]=0;//回溯
    }
}
int main()
{
    char s[10];
    while(scanf("%d",&n)&&n){
            cnt=0;ans=0;
        for(int i=0;i<n;i++){
             scanf("%s",s);

            for(int j=0;j<n;j++)
       {
           if(s[j]=='.'){
                mp[i][j]=0;//可以放置
            p[cnt].x=i;p[cnt].y=j;cnt++;
           }else       mp[i][j]=-1;
       }
    }
        dfs(0,0);
           printf("%d\n",ans);
    }
    return 0;
}
```






# Pusher（HDU-2821） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:28:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74
个人分类：[HDU																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

    PusherBoy is an online game http://www.hacker.org/push . There is an R * C grid, and there are piles of blocks on some positions. The goal is to clear the blocks by pushing into them. 

    You should choose an empty area as the initial position of the PusherBoy. Then you can choose which direction (U for up, D for down, L for left and R for right) to push. Once the direction is chosen, the PusherBoy will walk ahead until he met a pile of blocks (Walking outside the grid is invalid). Then he remove one block from the pile (so if the pile contains only one block, it will become empty), and push the remaining pile of blocks to the next area. (If there have been some blocks in the next area, the two piles will form a new big pile.)

    Please note if the pusher is right up against the block, he can't remove and push it. That is, there must be a gap between the pusher and the pile. As the following figure, the pusher can go up, but cannot go down. (The cycle indicates the pusher, and the squares indicate the blocks. The nested squares indicate a pile of two blocks.)

    And if a whole pile is pushed outside the grid, it will be considered as cleared.

![](https://img-blog.csdn.net/20180325091652780)



# Input

    There are several test cases in each input. The first two lines of each case contain two numbers C and R. (R,C <= 25) Then R lines follow, indicating the grid. '.' stands for an empty area, and a lowercase letter stands for a pile of blocks. ('a' for one block, 'b' for two blocks, 'c' for three, and so on.)

# Output

    Output three lines for each case. The first two lines contains two numbers x and y, indicating the initial position of the PusherBoy. (0 <= x < R, 0 <= y < C). The third line contains a moving sequence contains 'U', 'D', 'L' and 'R'. Any correct answer will be accepted.

# Sample Input

**3    7    ...    ...    .b.    ...    ...    .a.    ...**

# Sample Output

**4   1   UDU**

**![](https://img-blog.csdn.net/20180325091730864)**


————————————————————————————————————————————

题意：

    有一个R*C的方格，‘.’代表空地，‘a~z’分别代表该处有1~26个箱子，某人可以从距离箱子至少一个空格处推箱子，推一次此处少一个箱子，如果这个格还有其他箱子，则和它下一个格的箱子合并或到下一个格，朝着某个方向一直推到边界或者遇到箱子不能推为止才可以换方向，任意输出一种可以把箱子推完的方案，输出推箱子时起点的位置以及推箱子时的方向。

    P.S.游戏链接：http://www.hacker.org/push/

# Source Program

```cpp
#include<cstring>
#include<cstdio>
int C,R;
char maps[30][30];
int direction[4][2]={{1,0},{-1,0},{0,1},{0,-1}};//方向数组
char dir_block[4]={'D','U','R','L'};//推的方向与方向数组是对应的
int block[30][30];//记录每个点的箱子数量
char dir_point[1000];//记录当前点的推的方向

int sum;

bool judge(int x,int y)
{
    if(x>=0&&x<R&&y>=0&&y<C)
        return true;
    else
        return false;
}

bool dfs(int x0,int y0,int step)
{
    int x,y;
    int temp;
    int i;

    if(step==sum)//箱子清理完成
        return true;

    for(i=0;i<4;i++)
    {
        x=x0+direction[i][0];
        y=y0+direction[i][1];

        if(!judge(x,y)||block[x][y])//越界判断
            continue;

        while(judge(x,y)&&!block[x][y])//一直前行，直到越界或者遇到箱子
        {
            x+=direction[i][0];
            y+=direction[i][1];
        }

        if(judge(x,y)==0)//如果越界，说明行走方向不对
            continue;

        temp=block[x][y];//当前点的箱子数
        if(temp>1)//如果当前点不止一个石块
        {
            if(judge(x+direction[i][0],y+direction[i][1])==false)//如果再向前走一步就越界，说明方向不对
                continue;
            else
                block[x+direction[i][0]][y+direction[i][1]]+=temp-1;//箱子数减一
        }

        /*记录*/
        block[x][y]=0;
        dir_point[step]=dir_block[i];//记录当前每个点行走方向

        if(dfs(x,y,step+1))
            return true;

        /*还原*/
        block[x][y]=temp;
        block[x+direction[i][0]][y+direction[i][1]]-=(temp-1);
    }
    return false;
}
int main()
{
    int i,j;

    while(scanf("%d%d",&C,&R)!=EOF)
    {
        sum=0;
        
        memset(maps,0,sizeof(maps));
        memset(dir_point,0,sizeof(dir_point));
        memset(block,0,sizeof(block));
        
        for(i=0;i<R;i++)
        {
            getchar();
            scanf("%s",maps[i]);

            for(j=0;j<C;j++)//记录每个点的箱子数量
            {
                if(maps[i][j]=='.')
                    block[i][j]=0;
                else
                    block[i][j]=maps[i][j]-'a'+1;
                sum+=block[i][j];//记录箱子总数量
            }
        }

        for(i=0;i<R;i++)
            for(j=0;j<C;j++)
            {
                if(block[i][j]==0)//在没有箱子的地方开始搜索
                {
                    if(dfs(i,j,0))//搜索成功
                    {
                        printf("%d\n%d\n%s\n",i,j,dir_point);//输出
                        i=R;j=C;//控制终止搜索
                    }
                }
            }
    }
    return 0;
}
```







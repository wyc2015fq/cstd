# Another Eight Puzzle（HDU-2514） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:32:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：86
个人分类：[HDU																[搜索——更具技巧的 DFS](https://blog.csdn.net/u011815404/article/category/8228515)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Fill the following 8 circles with digits 1~8,with each number exactly once . Conntcted circles cannot be filled with two consecutive numbers.

    There are 17 pairs of connected cicles:

    A-B , A-C, A-D

    B-C, B-E, B-F

    C-D, C-E, C-F, C-G

    D-F, D-G

    E-F, E-H

    F-G, F-H

    G-H

    Filling G with 1 and D with 2 (or G with 2 and D with 1) is illegal since G and D are connected and 1 and 2 are consecutive .However ,filling A with 8 and B with 1 is legal since 8 and 1 are not consecutive .

    In this problems,some circles are already filled,your tast is to fill the remaining circles to obtain a solution (if possivle).

# Input

    There are several test cases in each input. The first two lines of each case contain two numbers C and R. (R,C <= 25) Then R lines follow, indicating the grid. '.' stands for an empty area, and a lowercase letter stands for a pile of blocks. ('a' for one block, 'b' for two blocks, 'c' for three, and so on.)

# Output

    Output three lines for each case. The first two lines contains two numbers x and y, indicating the initial position of the PusherBoy. (0 <= x < R, 0 <= y < C). The third line contains a moving sequence contains 'U', 'D', 'L' and 'R'. Any correct answer will be accepted.

# Sample Input

****3****    7 3 1 4 5 8 0 0****    7 0 0 0 0 0 0 0****

****    1 0 0 0 0 0 0 0****

# Sample Output

**Case 1: 7 3 1 4 5 8 6 2    Case 2: Not unique    Case 3: No answer**

————————————————————————————————————————————

题意：

    给出各位的点，以及所有关联位置的点，这些关连位置上的数都不能是连续的，已经给出八个点以及各点的连接情况，两个相连的点的值不能连续，输入部分点的值，让你填其他点的值，检查能否填出已经是否有唯一解。

思路：

    只有八个点，将数值填入点中，检查即可。

    首先把关连的点用vis表示，然后在输入时将不是零的点标记，再用dfs从搜索1~9。

    对于零的位置，用判断函数找出与此位置是关联的位置，只要这些位置的值都不是连续，就可以把为零的位置赋予此值。

# Source Program

```cpp
#include<iostream>
#include<cstring>
using namespace std;

int a[10];
int result[10];
int vis[10];
int flag;

bool judge_num(int num,int n)//数字关联判断
{
    if(n==1&&num==2)
        return true;
    if(n==8&&num==7)
        return true;
    if((n>1&&n<8)&&(num==(n-1)||num==(n+1)))
            return true;
    return false;
}

bool judge(int postion,int n)//对位置的判断
{
    int i;

    if(postion==1)//A-B,A-C,A-D的判断
    {
        if(judge_num(a[2],n))
            return false;
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[4],n))
            return false;
        return true;
    }
    if(postion==2)//B-A,B-C,B-E,B-F的判断
    {
        if(judge_num(a[1],n))
            return false;
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[5],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        return true;
    }
    if(postion==3)//C-A,C-B,C-D,C-E,C-F,C-G的判断
    {
        if(judge_num(a[1],n))
            return false;
        if(judge_num(a[2],n))
            return false;
        if(judge_num(a[4],n))
            return false;
        if(judge_num(a[5],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        if(judge_num(a[7],n))
            return false;
        return true;
    }
    if(postion==4)//D-A,D-C,D-F,D-G的判断
    {
        if(judge_num(a[1],n))
            return false;
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        if(judge_num(a[7],n))
            return false;
        return true;
    }
    if(postion==5)//E-B,E-C,E-F,E-H的判断
    {
        if(judge_num(a[2],n))
            return false;
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        if(judge_num(a[8],n))
            return false;
        return true;
    }
    if(postion==6)//F-B,F-C,F-D,F-E,F-G,F-H的判断
    {
        if(judge_num(a[2],n))
            return false;
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[4],n))
            return false;
        if(judge_num(a[5],n))
            return false;
        if(judge_num(a[7],n))
            return false;
        if(judge_num(a[8],n))
            return false;
        return true;
    }
    if(postion==7)//G-C,G-D,G-F,G-H的判断
    {
        if(judge_num(a[3],n))
            return false;
        if(judge_num(a[4],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        if(judge_num(a[8],n))
            return false;
        return true;
    }
    if(postion==8)//H-E,H-F的判断
    {
        if(judge_num(a[5],n))
            return false;
        if(judge_num(a[6],n))
            return false;
        return true;
    }

}

void dfs(int postion)
{
    int i;

    if(postion>8)
    {
        flag++;
        for(i=1;i<=8;i++)
            result[i]=a[i];
        return;
    }

    if(a[postion]==0)//对于是0的位置进行判断
    {
        for(i=1;i<=8;i++)//回溯搜索
        {
            if(!vis[i]&&judge(postion,i))//越界判断
            {
                vis[i]=1;
                a[postion]=i;

                dfs(postion+1);

                vis[i]=0;
                a[postion]=0;
            }
        }
    }
    else
        dfs(postion+1);
}
int main()
{
    int t,i,j;

    scanf("%d",&t);
    for(j=1;j<=t;j++)
    {
        memset(vis,0,sizeof(vis));
        for(i=1;i<=8;i++)
        {
            scanf("%d",&a[i]);
            vis[a[i]]=1;
        }
        flag=0;

        dfs(1);

        if(!flag)
        {
            printf("Case %d: ",j);
            printf("No answer\n");
        }
        else if(flag>1)
        {
            printf("Case %d: ",j);
            printf("Not unique\n");
        }
        else
        {
            printf("Case %d: ",j);
            printf("%d",result[1]);
            for(i=2;i<=8;i++)
                printf(" %d",result[i]);
            printf("\n");
        }
    }
    return 0;
}
```







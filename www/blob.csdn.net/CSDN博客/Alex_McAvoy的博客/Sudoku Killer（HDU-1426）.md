# Sudoku Killer（HDU-1426） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:38:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：101
个人分类：[HDU																[搜索——更具技巧的 DFS](https://blog.csdn.net/u011815404/article/category/8228515)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    自从2006年3月10日至11日的首届数独世界锦标赛以后，数独这项游戏越来越受到人们的喜爱和重视。

    据说，在2008北京奥运会上，会将数独列为一个单独的项目进行比赛，冠军将有可能获得的一份巨大的奖品———HDU免费七日游外加lcy亲笔签名以及同hdu acm team合影留念的机会。

    所以全球人民前仆后继，为了奖品日夜训练茶饭不思。当然也包括初学者linle，不过他太笨了又没有多少耐性，只能做做最最基本的数独题，不过他还是想得到那些奖品，你能帮帮他吗？你只要把答案告诉他就可以，不用教他是怎么做的。

    数独游戏的规则是这样的：在一个9x9的方格中，你需要把数字1-9填写到空格当中，并且使方格的每一行和每一列中都包含1-9这九个数字。同时还要保证，空格中用粗线划分成9个3x3的方格也同时包含1-9这九个数字。

![](https://img-blog.csdn.net/20180325162351848)![](https://img-blog.csdn.net/20180325162356776)

# Input

    本题包含多组测试，每组之间由一个空行隔开。每组测试会给你一个 9*9 的矩阵，同一行相邻的两个元素用一个空格分开。其中1-9代表该位置的已经填好的数，问号（?）表示需要你填的数。

# Output

    对于每组测试，请输出它的解，同一行相邻的两个数用一个空格分开。两组解之间要一个空行。<br>对于每组测试数据保证它有且只有一个解。

# Sample Input

**7 1 2 ? 6 ? 3 5 8**

**    ? 6 5 2 ? 7 1 ? 4    ? ? 8 5 1 3 6 7 2    9 2 4 ? 5 6 ? 3 7    5 ? 6 ? ? ? 2 4 1    1 ? 3 7 2 ? 9 ? 5    ? ? 1 9 7 5 4 8 6    6 ? 7 8 3 ? 5 1 9    8 5 9 ? 4 ? ? 2 3**

# Sample Output

**7 1 2 4 6 9 3 5 8    3 6 5 2 8 7 1 9 4    4 9 8 5 1 3 6 7 2    9 2 4 1 5 6 8 3 7    5 7 6 3 9 8 2 4 1    1 8 3 7 2 4 9 6 5    2 3 1 9 7 5 4 8 6    6 4 7 8 3 2 5 1 9    8 5 9 6 4 1 7 2 3**

————————————————————————————————————————————

思路：行列判断好解决，但区域判断不好处理，使用dfs，先判断行列，若行列成立再判断区域。

# Source Program

```cpp
#include<bits/stdc++.h>
using namespace std;
int a[10][10];
int ans,tot,flag;

struct pointer
{
    int x;
    int y;
    int step;
}save[1000];

int dfs(int x,int y)
{
    int i,j;

    if(flag==1)
        return 0;

    if(ans==(tot+1))
    {
        for(i=1;i<=9;i++)
        {
            for(j=1;j<=9;j++)
            {
                if(j<9)
                    printf("%d ",a[i][j]);
                else
                    printf("%d",a[i][j]);
            }
            cout<<endl;
        }

        flag=1;
        return 0;
    }

    int row[10],line[10];
    memset(row,0,sizeof(row));
    memset(line,0,sizeof(line));
    for(i=1;i<=9;i++)
    {
        if(a[x][i]!=0)//判断行
            row[a[x][i]]=1;
        if(a[i][y]!=0)//判断列
        line[a[i][y]]=1;
    }
    int x1=(x/4)*4,y1=(y/4)*4,flag1=0,k,t;//
    for(i=1;i<=9;i++)
    {
        if(row[i]&&line[i])
            continue;  //如果行列成立，才往下
        flag1=0;
        for(k=x1;k<x1+3;k++)//判断区域
        {
            for(t=y1;t<y1+3;t++)
                if(a[k][t]==i)
                {
                    flag1=1;
                    break;
                }
            if(flag==1)
                break;
        }
        if(flag1==0)
        {
            ans++;
            a[x][y]=i;
            dfs(save[ans].x,save[ans].y);
            a[x][y]=0;
            ans--;
        }
    }
    return 0;
}
int main()
{
    int i,j;
    int k;

    while(1)
    {
        k=0;
        flag=0;
        ans=1;//记录找到第几个？
        tot=0;//问号总数
        memset(save,0,sizeof(save));

        for(i=1;i<=9;i++)
            for(j=1;j<=9;j++)
            {
                scanf("%d",&a[i][j]);
                if(a[i][j]==0)//输入？，会变成0
                {
                    tot++;
                    k++;
                    /*记录问号*/
                    save[k].x=i;
                    save[k].y=j;
                }
                cin.get();
            }

        dfs(save[1].x,save[1].y);
        cout<<endl;
    }
}
```







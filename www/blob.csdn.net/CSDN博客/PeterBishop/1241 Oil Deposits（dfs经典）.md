# 1241 Oil Deposits（dfs经典） - PeterBishop - CSDN博客





2018年07月26日 22:53:40[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









```cpp
#include<stdio.h>
#include<string.h>
char map[110][110];
int move[8][2]={1,0,-1,0,0,1,0,-1,1,1,-1,-1,1,-1,-1,1};//两个坐标一组 分为8组
int h,w;
void dfs(int x,int y)//定义dfs函数，主函数找到了@，dfs启动，寻找主函数找到的@八面存在的@
{
    int next_x,next_y,i;
    map[x][y]='*';//把找到的@变为*，以免重复搜索
    for(i=0;i<8;i++)
    {
        next_x=x+move[i][0];//[0]表示两个坐标一组的第一个[i]表示两个坐标一组的第几组
        next_y=y+move[i][1];//[1]表示两个坐标一组的第二个[i]表示两个坐标一组的第几组
        if(next_x>=0&&next_x<h&&next_y>=0&&next_y<w)
        {
            if(map[next_x][next_y]=='@')
            {
                dfs(next_x,next_y);
            }
        }
    }
}
int main()//主函数开始，寻找第一个@
{
    int i,j,sum;
    while(scanf("%d%d",&h,&w)&&(h!=0||w!=0))
    {
        for(i=0;i<h;i++)
        scanf("%s",map[i]);
        sum=0;
        for(i=0;i<h;i++)
        {
            for(j=0;j<w;j++)
            {
                if(map[i][j]=='@')
                {
                    dfs(i,j);//转移到dfs函数，由dfs函数开始搜索主函数找到@的八面存在的@
                    sum++;
                }
            }
        }
        printf("%d\n",sum);
    }
    return 0;
}
```






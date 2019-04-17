# Catch him（HDU-2351） - Alex_McAvoy的博客 - CSDN博客





2018年03月16日 14:54:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：90
个人分类：[HDU																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

    在美式足球中，四分卫负责指挥整只球队的进攻战术和跑位，以及给接球员传球的任务。四分卫是一只球队进攻组最重要的球员，而且一般身体都相对比较弱小，所以通常球队会安排5-7名大汉来保护他，其中站在四分卫前方、排成一线的5名球员称为进攻锋线，他们通常都是135公斤左右的壮汉。

    对防守方来说，攻击对手的四分卫当然是最直接的限制对手进攻的方法。如果效果好，就可以在对方四分卫传球之前将其按翻在地，称之为擒杀。擒杀是最好的鼓舞防守队士气的方法，因为对方连传球的机会都没有，进攻就结束了，还必须倒退一些距离开球。凶狠的擒杀甚至能够将对方的四分卫弄伤，从而迫使对方更换这个进攻核心。

    在本题中，输入给出准备擒杀四分卫的防守球员的位置、对方每个进攻锋线球员的位置以及对方四分卫的位置，你的任务是求出这名准备擒杀的防守球员至少要移动多少步，才能够擒杀对方四分卫。

    假设对方进攻锋线和四分卫在这个过程中都不会移动。只有1名防守球员，防守球员只要碰到对方四分卫就算擒杀。

    所有的球员都是一块连续的、不中空的2维区域。防守球员不可以从进攻锋线的身体上穿过，也不可以从界外穿过(只能走空地)。

    防守队员不可以转动身体，只能平移。防守队员的身体所有部分向同一个方向(上、下、左、右)移动1格的过程叫做1步。

# Input

    输入包含多组数据。每组数据第一行都是两个整数H，W(0<H,W<=100)，表示整个区域的高度和宽度，H=W=0表示输入结束。接下来有H行，每行W个字符。每个字符如果是’.’，表示这里是空地，如果是’O’，表示是进攻锋线队员的身体，如果是’D’，表示是准备擒杀的防守球员的身体，如果是’Q’，表示是四分卫的身体。

    输入保证符合上面的条件。防守球员的身体总共不超过20格。

# Output

     对每组数据，输出包含擒杀所需最少步数的一行。如果不能擒杀，输出带’Impossible’的一行。

# Sample Input

**6 6**

**    .Q....    QQ..OO    .OO..O    ...O.O    OO.O..    ....DD    7 7    .Q.....    QQ.OOO.    ...O...    O......    OO..OO.    .O.....    .....DD    0 0**

# Sample Output

**    Impossible    9**


思路：

    与一般的bfs题不同，本题是以“块”为单位（即：身体体积），来进行移动和判断操作。

    所以开的结构体是“块”（数组），用每一个点的集合来表示。

# Source Program

```cpp
#include<cstring>
#include<cstdio>
#include<queue>
using namespace std;

char mapp[105][105];
int vis[105][105];
int h,w;
int direction[4][2]={{-1,0},{1,0},{0,-1},{0,1}};

struct body{
int x[25];
int y[25];
int k;
int step;
}start,now,nextt;

void bfs()
{
    queue<body> q;
    int i,j;
    int x,y;
    bool flag;

    vis[start.x[0]][start.y[0]]=1;
    memset(vis,0,sizeof(vis));

    q.push(start);//元素入队

    while(!q.empty())
    {
        now=q.front();
        q.pop();//出队

        for(i=0;i<now.k;i++)
        {
            if(mapp[now.x[i]][now.y[i]]=='Q')//成功扑杀，输出最小步数
            {
                printf("%d\n",now.step);
                return;
            }
        }

        for(i=0;i<4;i++)
        {
            flag=true;
            for(j=0;j<now.k;j++)
            {
                x=nextt.x[j]=now.x[j]+direction[i][0];
                y=nextt.y[j]=now.y[j]+direction[i][1];

                nextt.step=now.step+1;//统计步数
                nextt.k=now.k;//记录位置

                if(vis[nextt.x[0]][nextt.y[0]])
                    {flag=false;break;}

                if(nextt.x[j]<0||nextt.x[j]>=h||nextt.y[j]<0||nextt.y[j]>=w||mapp[nextt.x[j]][nextt.y[j]]=='O')
                {flag=false;break;} //越界判断
            }
            if(flag)//子节点满足条件
            {
                q.push(nextt);//入列
                vis[nextt.x[0]][nextt.y[0]]=1;
            }
        }
    }
    printf("Impossible\n");
}

int main()
{
    int i,j;
    int k;
    while(scanf("%d %d",&h,&w)!=EOF)
    {
        if(h==0&&w==0)  break;
        k=0;
        for(i=0;i<h;i++)
        {
            scanf("%s",mapp[i]);//以字符串的形式输入
            for(j=0;j<w;j++)
            {

                if(mapp[i][j]=='D') //记录擒杀队员位置
                {
                    start.x[k]=i;
                    start.y[k]=j;
                    k++;
                }
            }
        }

        start.k=k;//记录初始位置
        start.step=0;//记录初始步数
        bfs();
    }
    return 0;
}
```







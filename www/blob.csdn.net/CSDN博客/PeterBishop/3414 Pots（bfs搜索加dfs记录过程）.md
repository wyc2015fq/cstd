# 3414 Pots（bfs搜索加dfs记录过程） - PeterBishop - CSDN博客





2018年07月26日 22:20:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20








```cpp
# include<stdio.h>
# include <queue>
# include <string.h>
using namespace std;

# define max 110
int A,B,C;
char num[12][12] = {"FILL(1)","FILL(2)", "DROP(1)","DROP(2)","POUR(1,2)","POUR(2,1)"};
struct node
{
    int x,y,step,op;//OP是用来最后作为num数组的下标指引路径 
}v[max][max];

struct point
{
    int x, y;
};

void DFS(int x, int y)//用递归来返回路径 
{
    if(x == 0 && y == 0)
    return;
    DFS(v[x][y].x, v[x][y].y);
    printf("%s\n",num[ v[x][y].op ]);
}

int BFS( point t )
{   
    queue<point>Q;
    v[0][0].step = 1;
    Q.push(t);  

    while(Q.size())
    {
        point s = Q.front();
        Q.pop();

        if(s.x == C || s.y == C)
        {
            printf("%d\n", v[s.x][s.y].step-1);
            DFS(s.x, s.y);
            return 0;   
        }

        for(int i = 0; i<6; i++)
        {
            point q = s;    
            if(i == 0)
            {
                q.x = A;
            }
            else if(i == 1)
            {
                q.y = B;
            }
            else if(i == 2)
            {

                q.x = 0;
            }
            else if( i== 3)
            {
                q.y = 0;
            }
         else if(i == 4)
            {
                if(q.x+q.y <= B)
                    q.y += q.x, q.x = 0;//把A里面的水全倒B里面
                else
                    q.x -= (B-q.y), q.y = B;//把B倒满
            }
            else
            {
                if(q.x+q.y <= A)
                    q.x += q.y, q.y = 0;
                else
                    q.y -= (A-q.x), q.x = A;
            }
            if( v[q.x][q.y].step == 0)//用当下当成下标，将以前当成值 
            {
                v[q.x][q.y].step = v[s.x][s.y].step +1;
                v[q.x][q.y].x = s.x;
                v[q.x][q.y].y = s.y;
                v[q.x][q.y].op = i;//路径下标指引所以上面0-5的操作要和路径数组里要求的一致 
                Q.push(q);
            }           
        }       
    }
    return -1;  
}
int main(void)
{
    while(~scanf("%d%d%d",&A,&B,&C))
    {
        memset(v, 0, sizeof(v));
        point s;
        s.x = 0;
        s.y = 0;
        int ans = BFS(s);
        if(ans == -1)
          printf("impossible\n");               
    }   
    return 0;
}
```






# 3126 PrimePath - PeterBishop - CSDN博客





2018年07月26日 21:52:59[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：34
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<queue>
#include<algorithm>
using namespace std;
 
int n, m;
const int N = 1e4 + 100;
int vis[N];
struct node
{
    int x, step;
};
queue<node> Q;
 
bool judge_prime(int x) //判断素数
{
    if(x == 0 || x == 1)
        return false;
    else if(x == 2 || x == 3)
        return true;
    else
    {
        for(int i = 2; i <= (int)sqrt(x); i++)
            if(x % i == 0)
                return false;
        return true;
    }
}
 
void BFS()//对位数分解然后再去变化加入队列就行了
{
    int X, STEP, i;
    while(!Q.empty())
    {
        node tmp;
        tmp = Q.front();
        Q.pop();
        X = tmp.x;
        STEP = tmp.step;
        if(X == m)
        {
            printf("%d\n",STEP);
            return ;
        }
        for(i = 1; i <= 9; i += 2) //个位
        {
            int s = X / 10 * 10 + i;
            if(s != X && !vis[s] && judge_prime(s))
            {
                vis[s] = 1;
                node temp;
                temp.x = s;
                temp.step = STEP + 1;
                Q.push(temp);
            }
        }
        for(i = 0; i <= 9; i++) //十位
        {
            int s = X / 100 * 100 + i * 10 + X % 10;
            if(s != X && !vis[s] && judge_prime(s))
            {
                vis[s] = 1;
                node temp;
                temp.x = s;
                temp.step = STEP + 1;
                Q.push(temp);
            }
        }
        for(i = 0; i <= 9; i++) //百位
        {
            int s = X / 1000 * 1000 + i * 100 + X % 100;
            if(s != X && !vis[s] && judge_prime(s))
            {
                vis[s] = 1;
                node temp;
                temp.x = s;
                temp.step = STEP + 1;
                Q.push(temp);
            }
        }
        for(i = 1; i <= 9; i++) //千位
        {
            int s = i * 1000 + X % 1000;
            if(s != X && !vis[s] && judge_prime(s))
            {
                vis[s] = 1;
                node temp;
                temp.x = s;
                temp.step = STEP + 1;
                Q.push(temp);
            }
        }
    }
    printf("Impossible\n");
    return ;
}
 
int main()
{
    int t, i;
    scanf("%d",&t);
    while(t--)
    {
        while(!Q.empty()) Q.pop();
        scanf("%d%d",&n,&m);
        memset(vis,0,sizeof(vis));
        vis[n] = 1;
        node tmp;
        tmp.x = n;
        tmp.step = 0;
        Q.push(tmp);
        BFS();
    }
    return 0;
}
```






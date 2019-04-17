# A strange lift（HDU-1548）(dfs实现) - Alex_McAvoy的博客 - CSDN博客





2018年03月18日 11:06:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：103
个人分类：[HDU																[搜索——深度优先搜索 DFS](https://blog.csdn.net/u011815404/article/category/8115770)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    There is a strange lift.The lift can stop can at every floor as you want, and there is a number Ki(0 <= Ki <= N) on every floor.The lift have just two buttons: up and down.When you at floor i,if you press the button "UP" , you will go up Ki floor,i.e,you will go to the i+Ki th floor,as the same, if you press the button "DOWN" , you will go down Ki floor,i.e,you will go to the i-Ki th floor. Of course, the lift can't go up high than N,and can't go down lower than 1. For example, there is a buliding with 5 floors, and k1 = 3, k2 = 3,k3 = 1,k4 = 2, k5 = 5.Begining from the 1 st floor,you can press the button "UP", and you'll go up to the 4 th floor,and if you press the button "DOWN", the lift can't do it, because it can't go down to the -2 th floor,as you know ,the -2 th floor isn't exist.

    Here comes the problem: when you are on floor A,and you want to go to floor B,how many times at least he has to press the button "UP" or "DOWN"?

# Input

    The input consists of several test cases.,Each test case contains two lines.

    The first line contains three integers N ,A,B( 1 <= N,A,B <= 200) which describe above,The second line consist N integers k1,k2,....kn.

    A single 0 indicate the end of the input.

# Output

     For each case of the input output a interger, the least times you have to press the button when you on floor A,and you want to go to floor B.If you can't reach floor B,printf "-1".

# Sample Input

**5 1 5    3 3 1 2 5    0**

# Sample Output

**   3**

————————————————————————————————————————————

思路：本题若以深搜的方式实现，需要进行剪枝

# Source Program

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

int n,A,B;
int mapp[201],step;
int ans=10000;
bool vis[201];

void dfs(int x)
{
    if(x==B)//用ans记录到达时的步数
    {
        ans=min(ans,step);
        return;
    }
    vis[x]=true;//状态标记，表明已走过
    step++;//步数+1
    if(x+mapp[x]<=n&&vis[x+mapp[x]]!=true)//越界判断
        dfs(x+mapp[x]);//向上搜索
    if(x-mapp[x]>=1&&vis[x-mapp[x]]!=true)//越界判断
        dfs(x-mapp[x]);//向下搜索
    step--;//步数回归
    vis[x]=false;//状态回归
}
int main()
{
    int i;

    while(scanf("%d%d%d",&n,&A,&B))
    {
        if(n==0)    break;

        for(i=1;i<=n;i++)
            scanf("%d",&mapp[i]);
        dfs(A);
        if(ans==0)
            printf("-1");
        else
            printf("%d",ans);
    }
    return 0;
}
```







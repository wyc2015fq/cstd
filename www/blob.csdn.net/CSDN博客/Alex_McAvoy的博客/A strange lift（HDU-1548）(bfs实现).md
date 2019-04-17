# A strange lift（HDU-1548）(bfs实现) - Alex_McAvoy的博客 - CSDN博客





2018年03月18日 11:03:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95
个人分类：[HDU																[搜索——广度优先搜索 BFS](https://blog.csdn.net/u011815404/article/category/8115771)](https://blog.csdn.net/u011815404/article/category/7923257)








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

思路：简单的广搜

# Source Program

```cpp
#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;

int n,A,B;
int mapp[201],vis[201];

struct node
{
    int x;
    int step;
}start,endd;

int bfs()
{
    queue <node> q;
    int i;

    memset(vis,0,sizeof(vis));//标记数组清零
    vis[A] = 1;//标记初始状态

    start.x = A;
    start.step = 0;
    q.push(start);//初始状态入列

    while(!q.empty())
    {
        start=q.front();
        q.pop();

        if(start.x==B)//到达层数B
        	return start.step;//输出步数

        for(i=-1;i<=1;i+=2)//向上下两个方向搜索
        {
            endd=start;
            endd.x +=i*mapp[endd.x];

            if(endd.x>n||endd.x<=0||vis[endd.x])//越界判断
                continue;

            vis[endd.x]=1;//进行标记
            endd.step++;//步数+1
            q.push(endd);//元素入队
        }
    }
    return -1;
}

int main()
{
    int i,j;
    int res;

    while(scanf("%d%d%d",&n,&A,&B)!=0)
    {
        if(n==0)    break;

        for(i = 1;i<=n;i++)
        	scanf("%d",&mapp[i]);

        res=bfs();
        printf("%d\n",res);
    }

    return 0;
}
```







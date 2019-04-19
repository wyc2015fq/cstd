# POJ  3278  Catch That Cow（BFS） - HJ - CSDN博客
2017年01月14日 20:32:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：189
个人分类：[====ACM====																[-----------搜索-----------																[---------OJ---------																[POJ																[BFS](https://blog.csdn.net/feizaoSYUACM/article/category/7238039)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
Farmer John has been informed of the location of a fugitive cow and wants to catch her immediately. He starts at a point N (0 ≤ N ≤ 100,000) on a number line and the cow is at a point K (0 ≤ K ≤ 100,000) on the same number line. Farmer John has two modes of transportation: walking and teleporting.
- Walking: FJ can move from any point X to the points X - 1 or X + 1 in a single minute
- Teleporting: FJ can move from any point X to the point 2 × X in a single minute.
If the cow, unaware of its pursuit, does not move at all, how long does it take for Farmer John to retrieve it?
Input 
Line 1: Two space-separated integers: N and K
Output 
Line 1: The least amount of time, in minutes, it takes for Farmer John to catch the fugitive cow.
Sample Input
5 17
Sample Output
4
Hint 
The fastest way for Farmer John to reach the fugitive cow is to move along the following path: 5-10-9-18-17, which takes 4 minutes.
Source 
USACO 2007 Open Silver
题目大意： 
    FJ要抓奶牛。开始输入N（FJ的位置）K（奶牛的位置）。  
    FJ有三种移动方法： 
    向前走一步，耗时一分钟。 
    向后走一步，耗时一分钟； 
    向前移动到当前位置的两倍N*2，耗时一分钟。 
    问FJ抓到奶牛的最少时间。PS：奶牛是不会动的。
解法： 
   BFS+单调队列
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
#define Max 100005
int n,k,step[Max],head,next;
bool visit[Max];
queue<int> q;
int bfs()
{
    q.push(n);
    step[n]=0;
    visit[n]=1;
    while(!q.empty())
    {
        head=q.front();
        q.pop();
        for(int i=0;i<3;i++)                      //三种选择：加1，减1，或者乘2 
        {
            if(i==0)  next=head-1;
            else if(i==1)  next=head+1;
            else  next=head*2; 
            if(next>Max||next<0)                 //边界
              continue;
            if(!visit[next])
            {
                q.push(next);
                step[next]=step[head]+1;
                visit[next]=1;
            }
            if(next==k)                         //搜索跳出边界 
              return step[next];
        }   
    }
    return -1;
}
int main()
{   
    while(cin>>n>>k)
    {   
        memset(visit,0,sizeof(visit));
        if(n>=k)
          cout<<n-k<<endl;
        else
          cout<<bfs()<<endl;
    }
    return 0;
}
```

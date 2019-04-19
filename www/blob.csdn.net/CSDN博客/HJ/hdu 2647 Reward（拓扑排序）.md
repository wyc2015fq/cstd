# hdu  2647  Reward（拓扑排序） - HJ - CSDN博客
2017年04月27日 14:46:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：425
Problem Description 
Dandelion’s uncle is a boss of a factory. As the spring festival is coming , he wants to distribute rewards to his workers. Now he has a trouble about how to distribute the rewards. 
The workers will compare their rewards ,and some one may have demands of the distributing of rewards ,just like a’s reward should more than b’s.Dandelion’s unclue wants to fulfill all the demands, of course ,he wants to use the least money.Every work’s reward will be at least 888 , because it’s a lucky number.
Input 
One line with two integers n and m ,stands for the number of works and the number of demands .(n<=10000,m<=20000) 
then m lines ,each line contains two integers a and b ,stands for a’s reward should be more than b’s.
Output 
For every case ,print the least money dandelion ‘s uncle needs to distribute .If it’s impossible to fulfill all the works’ demands ,print -1.
Sample Input
2 1 
1 2 
2 2 
1 2 
2 1
Sample Output
1777 
-1
题目大意： 
    老板打算给员工们发奖励，但是那些员工会去比较各自的奖励，并且会存在一些要求，例如，a员工的奖励要比b员工的奖励多。然而老板想要知道自己最少需要多少钱俩奖励员工。那么，老板决定每个员工的最低奖励为888元。
解题思路： 
注意逆向建图， 然后拓扑排序分层次（ 第一次的工资为888（最低）， 第二层的工资 + 1）即可。
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#include<vector>
using namespace std;
#define maxn 10000
queue<int> q;
vector<int>t[10005];
int in[10005],sum[10005],cnt,u,v,ans;
void Init()           //初始化 
{
    for(int i=0;i<maxn;i++)
      t[i].clear();
    while(!q.empty())
      q.pop();
    memset(in,0,sizeof(in));
    ans=0;
}
int main()
{
    int n,m;
    while(cin>>n>>m)
    {
        Init();
        for(int i=0;i<m;i++)     //逆向构图 
        {
            cin>>u>>v;
            u--;v--;
            in[u]++;
            t[v].push_back(u);
        }
        for(int i=0;i<n;i++)
        {
            if(!in[i])
            {
                q.push(i);
                sum[i]=888;
            }
        }
        cnt=0;     
        while(!q.empty())
        {
            cnt++;            //统计纳入拓扑排序队列中点的个数 
            int p=q.front();
            q.pop();
            for(int i=0;i<t[p].size();i++)     //扫描当前点的邻接表 
            {
                in[t[p][i]]--;    //对于找到的入度为零的点，要让其对应的弧头入度减1 
                if(in[t[p][i]]==0)           //记录下当前找到的入度为零的点 
                {
                    q.push(t[p][i]);        //压入队列 
                    sum[t[p][i]]=sum[p]+1;     //记录下应该发的工资 
                }
            }
        }
        for(int i=0;i<n;i++)
          ans+=sum[i];
        if(cnt!=n)
          ans=-1;
        cout<<ans<<endl;
    }
    return 0;
}
```

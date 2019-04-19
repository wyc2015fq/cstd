# hdu   1520   Anniversary party（树形dp） - HJ - CSDN博客
2016年08月12日 21:14:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：245
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[树形DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238032)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
There is going to be a party to celebrate the 80-th Anniversary of the Ural State University. The University has a hierarchical structure of employees. It means that the supervisor relation forms a tree rooted at the rector V. E. Tretyakov. In order to make the party funny for every one, the rector does not want both an employee and his or her immediate supervisor to be present. The personnel office has evaluated conviviality of each employee, so everyone has some number (rating) attached to him or her. Your task is to make a list of guests with the maximal possible sum of guests’ conviviality ratings.
Input 
Employees are numbered from 1 to N. A first line of input contains a number N. 1 <= N <= 6 000. Each of the subsequent N lines contains the conviviality rating of the corresponding employee. Conviviality rating is an integer number in a range from -128 to 127. After that go T lines that describe a supervisor relation tree. Each line of the tree specification has the form:  
L K  
It means that the K-th employee is an immediate supervisor of the L-th employee. Input is ended with the line  
0 0
Output 
Output should contain the maximal sum of guests’ ratings.
Sample Input 
7 
1 
1 
1 
1 
1 
1 
1 
1 3 
2 3 
6 4 
7 4 
4 5 
3 5 
0 0
Sample Output 
5
解析：题目大意是一次老总要开聚会，想要尽量多的氛围，每个员工都有一定的活跃度，活跃度越大，氛围越好，但是每一位员工都不愿意跟自己的直接上司同事出席，因为公司里面的员工是以树的形式分配的，每位员工的直接上司可能一样，也可能不一样，现在要求这个聚会能达到的最大活跃度。
思路：很经典的树形dp，状态
```
d[i][1]+=d[j][0]    //如果上司来，下属就不会来，那么就要计算所有孙子结点的个数
```
另外一种情况的状态转移方程为
```
d[i][0]+=max(d[j][1],d[j][0]   //如果上司不来，下属可以选择来，也可以选择不来
```
代码如下：
```cpp
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
using namespace std;
vector<int>aa[6005];
int indeg[6005],val[6005],f[6005][2],vis[6005],a,b;
void dfs(int rr)
{
    vis[rr]=1;       //标记数组 
    f[rr][0]=0;
    f[rr][1]=val[rr];
    for(int i=0;i<aa[rr].size();i++)
    {
        int v=aa[rr][i];       //寻找rr的孩子结点，v为下属 
        if(!vis[v])
        {
            dfs(v);     //递归调用孩子结点 
            f[rr][0]+=max(f[v][1],f[v][0]);      //上司不来，下属来或者不来 
            f[rr][1]+=f[v][0];                 //上司来，下属不来，计算所有孙子结点的个数 
        } 
    }
}
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          aa[i].clear();           //每次调用注意清空
        for(int i=1;i<=n;i++)
          cin>>val[i];
        memset(indeg,0,sizeof(indeg));
        memset(f,0,sizeof(f));
        while(cin>>a>>b)
        {
            if(a==0&&b==0)
              break;
            aa[b].push_back(a);       //标记孩子结点和父节点 
            indeg[a]++;           //标记除了根结点之外的结点 
        }
        for(int i=1;i<=n;i++)
        {
            if(!indeg[i])
            {
                memset(vis,0,sizeof(vis));         //递归搜索前注意清空标记数组 
                dfs(i);
                cout<<max(f[i][0],f[i][1])<<endl;
                break;
            }
        }
    }
    return 0;
}
```

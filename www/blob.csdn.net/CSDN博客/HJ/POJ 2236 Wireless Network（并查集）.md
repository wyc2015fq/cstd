# POJ 2236  Wireless Network（并查集） - HJ - CSDN博客
2016年08月18日 15:21:07[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：217
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------数据结构--------																[并查集](https://blog.csdn.net/feizaoSYUACM/article/category/6307988)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
An earthquake takes place in Southeast Asia. The ACM (Asia Cooperated Medical team) have set up a wireless network with the lap computers, but an unexpected aftershock attacked, all computers in the network were all broken. The computers are repaired one by one, and the network gradually began to work again. Because of the hardware restricts, each computer can only directly communicate with the computers that are not farther than d meters from it. But every computer can be regarded as the intermediary of the communication between two other computers, that is to say computer A and computer B can communicate if computer A and computer B can communicate directly or there is a computer C that can communicate with both A and B.
In the process of repairing the network, workers can take two kinds of operations at every moment, repairing a computer, or testing if two computers can communicate. Your job is to answer all the testing operations.
Input 
The first line contains two integers N and d (1 <= N <= 1001, 0 <= d <= 20000). Here N is the number of computers, which are numbered from 1 to N, and D is the maximum distance two computers can communicate directly. In the next N lines, each contains two integers xi, yi (0 <= xi, yi <= 10000), which is the coordinate of N computers. From the (N+1)-th line to the end of input, there are operations, which are carried out one by one. Each line contains an operation in one of following two formats: 
1. “O p” (1 <= p <= N), which means repairing computer p. 
2. “S p q” (1 <= p, q <= N), which means testing whether computer p and q can communicate.
The input will not exceed 300000 lines.
Output 
For each Testing operation, print “SUCCESS” if the two computers can communicate, or “FAIL” if not.
Sample Input
4 1 
0 1 
0 2 
0 3 
0 4 
O 1 
O 2 
O 4 
S 1 4 
O 3 
S 1 4
Sample Output
FAIL 
SUCCESS
解析：理解一下即可，由于战争电脑连接系统出现障碍，现在连接两台电脑需要在一定的距离范围之内。另外，技术人员也一边正在抢修，看数据，实际上是判断输入两台电脑的根节点是否相同，代码如下：
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int x[1020],y[1020],parent[1020],vis[1020],n,d;
int find(int x)           //递归寻找根节点
{
    return parent[x]==x?x:find(parent[x]);
}
void Union(int a,int b)        //合并
{
    int x=find(a);
    int y=find(b);
    if(x!=y)
      parent[x]=y;
}
bool OK(int a,int b)      //判断两台电脑之间的距离是否在规定距离之内
{
    int t1=x[a]-x[b];
    int t2=y[a]-y[b];
    if(t1*t1+t2*t2<=d*d)   
      return 1;
    else 
      return 0;
}
int main()
{
        cin>>n>>d;
        for(int i=1;i<=n;i++)
          cin>>x[i]>>y[i];
        for(int i=1;i<=n;i++)
        {
            parent[i]=i;
            vis[i]=0;
        }
        char c;
        int t;
        while(cin>>c)
        {
            if(c=='O')
            {
                cin>>t;
                vis[t]=1;
                for(int i=1;i<=n;i++)
                  if(i!=t&&vis[i]&&OK(i,t))   //如果两台电脑都未被连接并且距离在要求之内
                    Union(i,t);        //可以恢复这两台电脑的连接情况
            } 
            if(c=='S')
            {
                int a,b;
                cin>>a>>b;
                int t1=find(a);
                int t2=find(b);
                if(t1!=t2)        //如果根节点不相同
                  cout<<"FAIL"<<endl;
                else                //如果相同
                  cout<<"SUCCESS"<<endl;
            }
        }
    return 0;
}
```

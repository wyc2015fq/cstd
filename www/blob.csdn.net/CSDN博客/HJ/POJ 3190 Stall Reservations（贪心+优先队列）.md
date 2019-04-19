# POJ  3190 Stall Reservations（贪心+优先队列） - HJ - CSDN博客
2017年01月31日 02:58:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：318
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------数据结构--------																[STL](https://blog.csdn.net/feizaoSYUACM/article/category/6308005)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
Oh those picky N (1 <= N <= 50,000) cows! They are so picky that each one will only be milked over some precise time interval A..B (1 <= A <= B <= 1,000,000), which includes both times A and B. Obviously, FJ must create a reservation system to determine which stall each cow can be assigned for her milking time. Of course, no cow will share such a private moment with other cows.
Help FJ by determining:
```
The minimum number of stalls required in the barn so that each cow can have her private milking period
An assignment of cows to these stalls over time
```
Many answers are correct for each test dataset; a program will grade your answer.
Input 
Line 1: A single integer, N
Lines 2..N+1: Line i+1 describes cow i’s milking interval with two space-separated integers.
Output 
Line 1: The minimum number of stalls the barn must have.
Lines 2..N+1: Line i+1 describes the stall to which cow i will be assigned for her milking period.
Sample Input
5 
1 10 
2 4 
3 6 
5 8 
4 7
Sample Output
4 
1 
2 
3 
2 
4
Hint 
Explanation of the sample:
Here’s a graphical schedule for this output:
Time     1  2  3  4  5  6  7  8  9 10
Stall 1 c1>>>>>>>>>>>>>>>>>>>>>>>>>>>
Stall 2 .. c2>>>>>> c4>>>>>>>>> .. ..
Stall 3 .. .. c3>>>>>>>>> .. .. .. ..
Stall 4 .. .. .. c5>>>>>>>>> .. .. ..
Other outputs using the same number of stalls are possible.
Source 
USACO 2006 February Silver
题目大意： 
  给出n个开始时间和结束时间，要求输出最少要开多少个空间容纳，并在后面输出序号
解题思路： 
  直接暴力会超时，典型的贪心，先排序（按照开始时间和结束时间从小到大排序） 
然后用优先队列维护空间个数，每次只要跟队头元素相比较即可
注意优先队列的排序规则！
```cpp
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;
int vis[1000005];
struct node{
    int a,b,num;
    bool operator < (const node &x)const{    //优先队列元素二级排序 
        if(b==x.b)
          return a>x.a;
        return b>x.b;
    }
}s[1000005];
int cmp(node x,node y)    //结构体数组二级排序 
{
    if(x.a==y.a)
      return x.b<y.b;
    return x.a<y.a;
}
int main()
{
    int n;
    while(cin>>n)
    {
        int cnt=1;
        for(int i=0;i<n;i++)
        {
            cin>>s[i].a>>s[i].b;
            s[i].num=i;
        }
        sort(s,s+n,cmp);
        priority_queue<node>q;
        q.push(s[0]);          //先把第一个元素压入队列 
        vis[s[0].num]=1;
        for(int i=1;i<n;i++)
        {
            if(!q.empty()&&q.top().b<s[i].a)    //如果队列非空并且当前节点的开始时间大于队头元素的结束时间 
            {
                vis[s[i].num]=vis[q.top().num];    //把当前节点和队头两个元素的序号归一化 
                q.pop();                 //把队头元素出队列然后下面会将当前节点入队 
            }
            else
            {
                cnt++;
                vis[s[i].num]=cnt;    //否则另外开一个序号 
            }
            q.push(s[i]);    //入队当前节点，自动排序，方便进行下一轮比较 
        }
        cout<<cnt<<endl;
        for(int i=0;i<n;i++)
          cout<<vis[i]<<endl;
        while(!q.empty())
          q.pop();
    }
    return 0;
}
```

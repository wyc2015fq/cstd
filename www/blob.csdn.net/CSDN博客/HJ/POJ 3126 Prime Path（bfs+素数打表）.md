# POJ 3126 Prime Path（bfs+素数打表） - HJ - CSDN博客
2017年07月02日 17:15:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：362
个人分类：[====ACM====																[-----------搜索-----------																[---------OJ---------																[POJ																[BFS](https://blog.csdn.net/feizaoSYUACM/article/category/7238039)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
The ministers of the cabinet were quite upset by the message from the Chief of Security stating that they would all have to change the four-digit room numbers on their offices. 
— It is a matter of security to change such things every now and then, to keep the enemy in the dark. 
— But look, I have chosen my number 1033 for good reasons. I am the Prime minister, you know! 
— I know, so therefore your new number 8179 is also a prime. You will just have to paste four new digits over the four old ones on your office door. 
— No, it’s not that simple. Suppose that I change the first digit to an 8, then the number will read 8033 which is not a prime! 
— I see, being the prime minister you cannot stand having a non-prime number on your door even for a few seconds. 
— Correct! So I must invent a scheme for going from 1033 to 8179 by a path of prime numbers where only one digit is changed from one prime to the next prime.
Now, the minister of finance, who had been eavesdropping, intervened. 
— No unnecessary expenditure, please! I happen to know that the price of a digit is one pound. 
— Hmm, in that case I need a computer program to minimize the cost. You don’t know some very cheap software gurus, do you? 
— In fact, I do. You see, there is this programming contest going on… Help the prime minister to find the cheapest prime path between any two given four-digit primes! The first digit must be nonzero, of course. Here is a solution in the case above.
```
1033
1733
3733
3739
3779
8779
8179
```
The cost of this solution is 6 pounds. Note that the digit 1 which got pasted over in step 2 can not be reused in the last step – a new 1 must be purchased.
Input 
One line with a positive number: the number of test cases (at most 100). Then for each test case, one line with two numbers separated by a blank. Both numbers are four-digit primes (without leading zeros).
Output 
One line for each case, either with a number stating the minimal cost or containing the word Impossible.
Sample Input
3 
1033 8179 
1373 8017 
1033 1033
Sample Output
6 
7 
0
题目大意：给出两个四位数a,b，且都是素数。要求从a变化到b最少需要多少步，每一步可以改动a的任意位上面的数字，但必须保证改变之后的a仍然要为一个素数。
解题思路：首先把1~10000素数表打出来，然后用bfs搜一遍，记录下步骤即可。
可能唯一有技巧性地就是从队列中取出一个数如何遍历改变，请看代码：
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
#define maxn 10000
int s[10005],vis[10005],ans;
struct node{
    int x,step;
};
void Init()
{
    memset(s,0,sizeof(s));
    int p=0;
    for(int i=2;i*i<=maxn;i++)
      if(s[i]==0)
        for(int j=i*i;j<=maxn;j+=i)
          s[j]=1;         
}
int bfs(int x)
{
    if(x==ans)
      return 0;
    queue<node> q;
    memset(vis,0,sizeof(vis));
    q.push((node){x,0});        //将x对应的结构体压入队列 
    vis[x]=1;
    while(!q.empty())
    {
        node a=q.front();
        q.pop();
        for(int i=1;i<=1000;i*=10)
        {
            for(int j=0;j<10;j++)
            {
                if(i==1000&&j==0)            //首位不能改成0 
                  continue;
                int t=a.x%i+j*i+a.x/(i*10)*(i*10);        //改变把a.x上面第i位数改成j 
                if(t==ans)
                  return a.step+1;
                if(s[t]==0)             //改动之后的t要为素数 
                {
                    if(vis[t])          //并且未被访问 
                      continue;
                    vis[t]=1;
                    q.push((node){t,a.step+1});      //压入队列 
                }
            }
        }
    } 
    return -1;
}
int main()
{
    Init();
    int t,x;
    cin>>t;
    while(t--)
    {
        cin>>x>>ans;
        cout<<bfs(x)<<endl; 
    }
    return 0;
}
```

# hdu  2955 Robberies（背包问题） - HJ - CSDN博客
2017年02月02日 02:01:22[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：191
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[背包问题](https://blog.csdn.net/feizaoSYUACM/article/category/7238225)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
The aspiring Roy the Robber has seen a lot of American movies, and knows that the bad guys usually gets caught in the end, often because they become too greedy. He has decided to work in the lucrative business of bank robbery only for a short while, before retiring to a comfortable job at a university.
For a few months now, Roy has been assessing the security of various banks and the amount of cash they hold. He wants to make a calculated risk, and grab as much money as possible.
His mother, Ola, has decided upon a tolerable probability of getting caught. She feels that he is safe enough if the banks he robs together give a probability less than this.
Input 
The first line of input gives T, the number of cases. For each scenario, the first line of input gives a floating point number P, the probability Roy needs to be below, and an integer N, the number of banks he has plans for. Then follow N lines, where line j gives an integer Mj and a floating point number Pj . 
Bank j contains Mj millions, and the probability of getting caught from robbing it is Pj .
Output 
For each test case, output a line with the maximum number of millions he can expect to get while the probability of getting caught is less than the limit set.
Notes and Constraints 
0 < T <= 100 
0.0 <= P <= 1.0 
0 < N <= 100 
0 < Mj <= 100 
0.0 <= Pj <= 1.0 
A bank goes bankrupt if it is robbed, and you may assume that all probabilities are independent as the police have very low funds.
Sample Input
3 
0.04 3 
1 0.02 
2 0.03 
3 0.05 
0.06 3 
2 0.03 
2 0.03 
3 0.05 
0.10 3 
1 0.03 
2 0.02 
3 0.05
Sample Output
2 
4 
6
题目大意： 
 有一个小偷要偷银行的钱，可是他偷没家银行总是有一定的概率被抓，现在给了你一个概率P，只要他被抓的概率乘积不大与P，他就是安全的。问你在他安全的情况下，他最多可以偷多少钱。
解题思路： 
dp，转换成背包问题
d[j]表示抢到j钱不被抓到的期望值（一维背包一定要注意逆序）
```cpp
#include<iostream>
#include<cstring>
using namespace std;
double d[1000005],k[105];
int w[105];
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        double b;
        int ans=0,sum=0;
        cin>>b>>n;
        for(int i=0;i<n;i++)
        {
            cin>>w[i]>>k[i];
            k[i]=1-k[i];       //计算不被抓到的概率 
            sum+=w[i];         //统计银行金钱总数 
        }
        memset(d,0,sizeof(d));
        d[0]=1;
        for(int i=0;i<n;i++)
          for(int j=sum;j>=w[i];j--)
            d[j]=max(d[j],d[j-w[i]]*k[i]);     //把概率看做价值，把金钱看做体积，因此可以观察到这是个典型的01背包问题 
        for(int i=sum;i>=1;i--)                //逆序遍历，找到第一个大于等于其期望的背包，把金钱赋值给ans
          if(d[i]>=1-b)                       //易知第一个赋值的ans体积最大（也就是金钱最大） 
          {
              ans=i;
              break;
          }
        cout<<ans<<endl;
    }
    return 0;
}
```

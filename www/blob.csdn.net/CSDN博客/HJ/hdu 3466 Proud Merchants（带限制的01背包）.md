# hdu  3466 Proud Merchants（带限制的01背包） - HJ - CSDN博客
2017年08月02日 13:10:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：198
Problem Description 
Recently, iSea went to an ancient country. For such a long time, it was the most wealthy and powerful kingdom in the world. As a result, the people in this country are still very proud even if their nation hasn’t been so wealthy any more. 
The merchants were the most typical, each of them only sold exactly one item, the price was Pi, but they would refuse to make a trade with you if your money were less than Qi, and iSea evaluated every item a value Vi. 
If he had M units of money, what’s the maximum value iSea could get?
Input 
There are several test cases in the input.
Each test case begin with two integers N, M (1 ≤ N ≤ 500, 1 ≤ M ≤ 5000), indicating the items’ number and the initial money. 
Then N lines follow, each line contains three numbers Pi, Qi and Vi (1 ≤ Pi ≤ Qi ≤ 100, 1 ≤ Vi ≤ 1000), their meaning is in the description.
The input terminates by end of file marker.
Output 
For each test case, output one integer, indicating maximum value iSea could get.
Sample Input
2 10 
10 15 10 
5 10 5 
3 10 
5 10 5 
3 5 6 
2 7 3
Sample Output
5 
11
题目大意： 
给你m数量的钱 ，让你去买东西， 共有 n 件物品，每件物品有价格 P、价值 V、  属性 Q, 属性Q 是指你如过想买这件物品你的手中的钱的数量不低于Q 。但是你实际上买这样东西只需要花费价格P。问给你钱M ，列出N件物品，最多能获得多少价值的东西？
解题思路： 
首先考虑，如果在价格一样，价值一样但属性不一样的情况下，先选哪一种？
一定是先考虑属性Q大的物品！！！ 
而价格一样，属性大，那么Q-P的值就大。
因此，在进行01背包选择之前，需要对物品按照Q-P从小到大排序（因为01背包物品选择需要逆序）
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
struct node{
    int p,q,v,c;
}s[5005];
int dp[5005];
int cmp(node a,node b)
{
    return a.q-a.p<b.q-b.p;
}
int main()
{
    int n,m;
    while(cin>>n>>m)
    {
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
          cin>>s[i].p>>s[i].q>>s[i].v;  
        sort(s,s+n,cmp);
        for(int i=0;i<n;i++)
          for(int j=m;j>=s[i].q;j--)
            dp[j]=max(dp[j],dp[j-s[i].p]+s[i].v); 
        cout<<dp[m]<<endl;
    }
    return 0;
}
```

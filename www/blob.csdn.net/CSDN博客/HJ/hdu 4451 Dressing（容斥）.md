# hdu  4451  Dressing（容斥） - HJ - CSDN博客
2017年07月17日 17:34:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：259
Problem Description 
Wangpeng has N clothes, M pants and K shoes so theoretically he can have N×M×K different combinations of dressing. 
One day he wears his pants Nike, shoes Adiwang to go to school happily. When he opens the door, his mom asks him to come back and switch the dressing. Mom thinks that pants-shoes pair is disharmonious because Adiwang is much better than Nike. After being asked to switch again and again Wangpeng figure out all the pairs mom thinks disharmonious. They can be only clothes-pants pairs or pants-shoes pairs. 
Please calculate the number of different combinations of dressing under mom’s restriction.
Input 
There are multiple test cases. 
For each case, the first line contains 3 integers N,M,K(1≤N,M,K≤1000) indicating the number of clothes, pants and shoes. 
Second line contains only one integer P(0≤P≤2000000) indicating the number of pairs which mom thinks disharmonious. 
Next P lines each line will be one of the two forms“clothes x pants y” or “pants y shoes z”. 
The first form indicates pair of x-th clothes and y-th pants is disharmonious(1≤x≤N,1 ≤y≤M), and second form indicates pair of y-th pants and z-th shoes is disharmonious(1≤y≤M,1≤z≤K). 
Input ends with “0 0 0”. 
It is guaranteed that all the pairs are different.
Output 
For each case, output the answer in one line.
Sample Input
2 2 2 
0 
2 2 2 
1 
clothes 1 pants 1 
2 2 2 
2 
clothes 1 pants 1 
pants 1 shoes 1 
0 0 0
Sample Output
8 
6 
5
题目大意： 
有n件衣服，m条裤子，k双鞋子，有p个约定，每个约定内容是哪件衣服不能跟哪条裤子同时穿或者是哪条裤子不能跟哪双鞋子同时穿。问有多少种搭配方法。
解题思路： 
因为数据比较小，可以用数组下标来表示裤子的标号。
```
d[i][0] 表示与标号为i的裤子不能同时穿的上衣件数 
d[i][1] 表示与标号为i的裤子不能同时穿的鞋子数量
```
最后枚举裤子条数，每次累加搭配方法。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 1005
int d[maxn][3],a,b;         
/*
  d[i][0] 表示与标号为i的裤子不能同时穿的上衣件数  
  d[i][1] 表示与标号为i的裤子不能同时穿的鞋子数量 
*/ 
char s1[10],s2[10]; 
void check(char s,int x)      
{
    if(s=='p')               //如果为裤子，那么把标号记下来 
      a=x;
    else                     //否则判断是上衣还是鞋子 
      b=s=='s'?1:0;
    return ;
}
int main()
{
    int n,m,k,p,x,y;
    while(cin>>n>>m>>k)
    {
        if(n==0&&m==0&&k==0)
          break;
        cin>>p;
        memset(d,0,sizeof(d));        //刚开始全部赋初值为0 
        long long ans=0;
        for(int i=0;i<p;i++)           
        {
            cin>>s1>>x>>s2>>y;
            check(s1[0],x);
            check(s2[0],y);
            d[a][b]++;               
        } 
        for(int i=1;i<=m;i++)        //枚举裤子的数量 
          ans+=(n-d[i][0])*(k-d[i][1]);          //累加能够同时穿的上衣数目 与 能够同时穿的鞋子数目 的 乘积 
        cout<<ans<<endl; 
    }
    return 0;
}
```

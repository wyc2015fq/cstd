# Codeforeces  703B  Mishka and trip（路径权值的计算） - HJ - CSDN博客
2016年08月20日 10:09:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：227
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Little Mishka is a great traveller and she visited many countries. After thinking about where to travel this time, she chose XXX — beautiful, but little-known northern country.
Here are some interesting facts about XXX:
```
XXX consists of n cities, k of whose (just imagine!) are capital cities.
All of cities in the country are beautiful, but each is beautiful in its own way. Beauty value of i-th city equals to ci.
All the cities are consecutively connected by the roads, including 1-st and n-th city, forming a cyclic route 1 — 2 — ... — n — 1. Formally, for every 1 ≤ i < n there is a road between i-th and i + 1-th city, and another one between 1-st and n-th city.
Each capital city is connected with each other city directly by the roads. Formally, if city x is a capital city, then for every 1 ≤ i ≤ n,  i ≠ x, there is a road between cities x and i.
There is at most one road between any two cities.
Price of passing a road directly depends on beauty values of cities it connects. Thus if there is a road between cities i and j, price of passing it equals ci·cj.
```
Mishka started to gather her things for a trip, but didn’t still decide which route to follow and thus she asked you to help her determine summary price of passing each of the roads in XXX. Formally, for every pair of cities a and b (a < b), such that there is a road between a and b you are to find sum of products ca·cb. Will you help her? 
Input
The first line of the input contains two integers n and k (3 ≤ n ≤ 100 000, 1 ≤ k ≤ n) — the number of cities in XXX and the number of capital cities among them.
The second line of the input contains n integers c1, c2, …, cn (1 ≤ ci ≤ 10 000) — beauty values of the cities.
The third line of the input contains k distinct integers id1, id2, …, idk (1 ≤ idi ≤ n) — indices of capital cities. Indices are given in ascending order. 
Output
Print the only integer — summary price of passing each of the roads in XXX.
Examples
Input 
4 1 
2 3 1 2 
3
Output 
17
Input 
5 2 
3 5 2 2 4 
1 4
Output 
71
题目链接：[http://codeforces.com/problemset/problem/703/B](http://codeforces.com/problemset/problem/703/B)
解析：因为一个一个来寻找中心城市计算权值和不仅麻烦，而且很容易重复计算。因此，可以考虑转换一种思想，先计算每个点跟其他任何点的权值和，然后设置一个标记变量，控制计算次数，这样既不麻烦也不会重复计算，代码如下：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int n,k;
    while(cin>>n>>k)
    {
        int a[100005],b[100005];
        long long sum=0,res=0;
        for(int i=1;i<=n;i++)
        {
            cin>>a[i];
            sum+=a[i];    //先计算好所有权值
        }
        for(int j=1;j<=k;j++)
        {
            scanf("%d",&b[j]);
            res+=(sum-a[b[j]])*a[b[j]];   //然后累加变量计算权值和出除去本身与本身的乘积
            sum-=a[b[j]];        //权值和减去本身
            a[b[j]]=0;       //标记这个值已经被计算过一次，赋值为0
        }
        for(int i=2;i<=n;i++)
          res+=a[i]*a[i-1];       //依次累加每个点跟其他点相连接的权值的和
        res+=a[1]*a[n];        //最后要考虑加上第一个点跟最后一个点的连接情况
        cout<<res<<endl;
    }
    return 0;
}
```

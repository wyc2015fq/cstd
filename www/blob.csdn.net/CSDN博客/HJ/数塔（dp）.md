# 数塔（dp） - HJ - CSDN博客
2016年05月11日 20:43:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：448
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
POJ     The Triangle （1163）
Description 
7 
3   8 
8   1   0 
2   7   4   4 
4   5   2   6   5
(Figure 1) 
Figure 1 shows a number triangle. Write a program that calculates the highest sum of numbers passed on a route that starts at the top and ends somewhere on the base. Each step can go either diagonally down to the left or diagonally down to the right. 
Input 
Your program is to read from standard input. The first line contains one integer N: the number of rows in the triangle. The following N lines describe the data of the triangle. The number of rows in the triangle is > 1 but <= 100. The numbers in the triangle, all integers, are between 0 and 99.
Output 
Your program is to write to standard output. The highest sum is written as an integer.
Sample Input 
5 
7 
3 8 
8 1 0  
2 7 4 4 
4 5 2 6 5
Sample Output 
30
先来理解题目意思： 
给你一个数n，以及n层的数塔，要你输出从这个数塔的最上面一层走到最下面一层的数字累加之和最大的值，并且特殊要求每到一个数字的位置只能往正下方选择或者往右下方的一个数字选择。
题目给的案例很简单，只有五层，蛮算的话一两分钟也就出来了，用暴力的话循环次数也不会很大。不过，若是n的数值比较大该怎么办捏，还是用暴力么，不，坚决阻止暴力，这个时候就提倡一种新的算法：
```
动态规划（dynamic programming）简称dp
```
详细的介绍在后文会陆续讲解，请恕我今天在这里节省篇赋。针对这个题目，分析起来，最平常的思路，也是最简单就能想到的办法就是一个一个去算，从上往下，一条路一条路地去走。
但是在动态规划里面针对于路径的选择是十分需要掌握方法的。动态规划与其它不同在于对每个问题的子问题都保存其中的解，当其重复出现的时候即可避免重复求解，大大减少了时间的开销。
那么什么是子问题呢？
譬如说你从最上面任意选一条路走，当你走到最底层不能再走的时候求出来的解就是该子问题的解，这个解会存在一个被叫做备忘录的空间里面，当你求其它子问题的时候出现同样的解就可以直接pass掉，省去时间开销。
你从最上面往下走总会有两个选择，而且走到第二步的时候可能会重复选择在同一个点上面，这样这个点就会重复计算多次，为了避免这种冗杂计算的情况发生。这里特地采用了一种自底向上的方法。
也就是说从最底层开始，每一个数都只有一个选择，保留每一个数这条路径最大值，存放在该点位置上，遍历循环后从子问题里面选择最优解。
```
状态d[i][j]   表示在第i行第j列从最上层一直到当前节点位置所求路径中最大的数值
```
每次都选择上一层较大的一个数，这样层层递进，最后输出最顶层的值，也就是最佳路径，求出的数值也是最大数值
详情看代码：
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int main()
{
    int m;
    cin>>m;
    while(m--)
    {
        int n,i,j,p,q;
        cin>>n;
        int d[105][105],a[105][105];
        for(q=1;q<=n;q++)
            for(p=1;p<=q;p++)
              cin>>a[q][p];
        for(j=1;j<=n;j++)
          d[n][j]=a[n][j];        //自下而上，先讲最底层数据记录下来
        for(i=n-1;i>=1;i--)
          for(j=1;j<=i;j++)
            d[i][j]=a[i][j]+max(d[i+1][j],d[i+1][j+1]);  //优先选择最大值，层层递进
        cout<<d[1][1]<<endl;  //最后输出最顶层的数值，也就是最大值
    } 
    return 0;
}
```

# codeforces 166-E. Tetrahedron（预处理+dp） - HJ - CSDN博客
2017年07月11日 11:20:05[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：432
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
You are given a tetrahedron. Let’s mark its vertices with letters A, B, C and D correspondingly.
![这里写图片描述](http://codeforces.com/predownloaded/c3/1e/c31ef91a9c3bc31bc8934878dfeedbd74f3cc335.png)
An ant is standing in the vertex D of the tetrahedron. The ant is quite active and he wouldn’t stay idle. At each moment of time he makes a step from one vertex to another one along some edge of the tetrahedron. The ant just can’t stand on one place.
You do not have to do much to solve the problem: your task is to count the number of ways in which the ant can go from the initial vertex D to itself in exactly n steps. In other words, you are asked to find out the number of different cyclic paths with the length of n from vertex D to itself. As the number can be quite large, you should print it modulo 1000000007 (10^9 + 7). 
Input
The first line contains the only integer n (1 ≤ n ≤ 10^7) — the required length of the cyclic path. 
Output
Print the only integer — the required number of ways modulo 1000000007 (10^9 + 7). 
Examples 
Input
2
Output
3
Input
4
Output
21
Note
The required paths in the first sample are:
```
D - A - D
D - B - D
D - C - D
```
题目大意： 
一个蚂蚁在正四面体的顶角，每秒都要移动一下去另外一个点，问n秒后又回到起点的方案数。
解题思路： 
分别用0,1,2,3,表示ABCD四个点 
设d[i][j]表示移动i秒待在第j个点的方案数。最后输出d[n][3]即可。
```cpp
#include<iostream>
using namespace std;
#define mod 1000000007 
int d[10000010][10]={0};
int main()
{       
    d[1][0]=d[1][1]=d[1][2]=1;
    for(int i=1;i<=10000000;i++)
      for(int j=0;j<4;j++)
        for(int k=0;k<4;k++)
          if(j!=k)
            d[i][j]=(d[i][j]+d[i-1][k])%mod;
    int n;
    while(cin>>n)
      cout<<d[n][3]<<endl;
    return 0;
}
```

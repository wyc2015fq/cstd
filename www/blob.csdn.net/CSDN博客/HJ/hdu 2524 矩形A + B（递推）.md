# hdu  2524  矩形A + B（递推） - HJ - CSDN博客
2017年04月08日 18:29:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：232
Problem Description 
给你一个高为n ，宽为m列的网格，计算出这个网格中有多少个矩形，下图为高为2，宽为4的网格.
![这里写图片描述](http://acm.hdu.edu.cn/data/images/c153-1005.JPG)
Input 
第一行输入一个t, 表示有t组数据，然后每行输入n,m,分别表示网格的高和宽 ( n < 100 , m < 100).
Output 
每行输出网格中有多少个矩形.
Sample Input
2 
1 2 
2 4
Sample Output
3 
30
解题思路：
把一二行打表看一看，应该就知道规律了。
```cpp
#include<iostream>
using namespace std;
long long a[105][105];
int main()
{
    a[0][0]=0;
    for(int i=1;i<=100;i++)
      a[i][1]=a[1][i]=a[1][i-1]+i;
    for(int i=2;i<=100;i++)
      for(int j=2;j<=100;j++)
        a[i][j]=a[1][j]*a[i][1];
    int n,m,t;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        cout<<a[n][m]<<endl;
    }
    return 0;
}
```

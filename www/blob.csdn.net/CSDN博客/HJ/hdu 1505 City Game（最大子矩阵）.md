# hdu  1505 City Game（最大子矩阵） - HJ - CSDN博客
2017年02月02日 19:28:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
Problem Description 
Bob is a strategy game programming specialist. In his new city building game the gaming environment is as follows: a city is built up by areas, in which there are streets, trees,factories and buildings. There is still some space in the area that is unoccupied. The strategic task of his game is to win as much rent money from these free spaces. To win rent money you must erect buildings, that can only be rectangular, as long and wide as you can. Bob is trying to find a way to build the biggest possible building in each area. But he comes across some problems – he is not allowed to destroy already existing buildings, trees, factories and streets in the area he is building in.
Each area has its width and length. The area is divided into a grid of equal square units.The rent paid for each unit on which you’re building stands is 3$.
Your task is to help Bob solve this problem. The whole city is divided into K areas. Each one of the areas is rectangular and has a different grid size with its own length M and width N.The existing occupied units are marked with the symbol R. The unoccupied units are marked with the symbol F.
Input 
The first line of the input contains an integer K – determining the number of datasets. Next lines contain the area descriptions. One description is defined in the following way: The first line contains two integers-area length M<=1000 and width N<=1000, separated by a blank space. The next M lines contain N symbols that mark the reserved or free grid units,separated by a blank space. The symbols used are:
R – reserved unit
F – free unit
In the end of each area description there is a separating line.
Output 
For each data set in the input print on a separate line, on the standard output, the integer that represents the profit obtained by erecting the largest building in the area encoded by the data set.
Sample Input
2 
5 6 
R F F F F F 
F F F F F F 
R R R F F F 
F F F F F F 
F F F F F F
5 5 
R R R R R 
R R R R R 
R R R R R 
R R R R R 
R R R R R
Sample Output
45 
0
题目大意： 
求最大子矩阵，是1506的升级版
解题思路：
注意一行一行地来，先赋值后遍历就好了 
传送 
[http://blog.csdn.net/feizaosyuacm/article/details/54837186](http://blog.csdn.net/feizaosyuacm/article/details/54837186)
代码如下：
```cpp
#include<bits/stdc++.h>
using namespace std;
int a[1005][1005],l[1005],r[1005];
char s[1005][1005];
int main()
{
    int tt,t,n,m;
    cin>>tt;
    while(tt--)
    {
        cin>>n>>m;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=m;j++)
            cin>>s[i][j];
        for(int i=1;i<=m;i++)    //为了方便起见，虚拟a[0][]的存在 
          a[0][i]=0;
        for(int j=1;j<=m;j++)
          for(int i=1;i<=n;i++)
            if(s[i][j]=='F')
              a[i][j]=a[i-1][j]+1;   //给每一行的每一个点都赋上值 
            else
              a[i][j]=0;
        int ans=0;
        for(int i=1;i<=n;i++)
        {
            l[1]=1,r[m]=m;
            for(int j=2;j<=m;j++)
            {
                if(a[i][j]==0)
                  continue;
                t=j;
                while(t>1&&a[i][j]<=a[i][t-1])     //往右寻找连续的比a[i][j]大的最右边下标 
                  t=l[t-1];
                l[j]=t;
            }
            for(int j=m-1;j>=1;j--)
            {
                if(a[i][j]==0)
                  continue;
                t=j;
                while(t<m&&a[i][j]<=a[i][t+1])    //同上，向左寻找连续的比a[i][j]大的最左边下标 
                  t=r[t+1];
                r[j]=t;
            }
            for(int j=1;j<=m;j++)
              if(ans<(r[j]-l[j]+1)*a[i][j])   //寻找每一行中的最大子矩阵  
                ans=(r[j]-l[j]+1)*a[i][j];
        }
        cout<<ans*3<<endl;    //最后求出整个平面内最大子矩阵，注意结果乘以3 
    }
    return 0;
}
```

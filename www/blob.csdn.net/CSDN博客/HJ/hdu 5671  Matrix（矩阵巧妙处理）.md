# hdu  5671   Matrix（矩阵巧妙处理） - HJ - CSDN博客
2016年08月20日 10:01:55[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：327
Problem Description 
There is a matrix M that has n rows and m columns (1≤n≤1000,1≤m≤1000).Then we perform q(1≤q≤100,000) operations:
1 x y: Swap row x and row y (1≤x,y≤n);
2 x y: Swap column x and column y (1≤x,y≤m);
3 x y: Add y to all elements in row x (1≤x≤n,1≤y≤10,000);
4 x y: Add y to all elements in column x (1≤x≤m,1≤y≤10,000);
Input 
There are multiple test cases. The first line of input contains an integer T(1≤T≤20) indicating the number of test cases. For each test case:
The first line contains three integers n, m and q. 
The following n lines describe the matrix M.(1≤Mi,j≤10,000) for all (1≤i≤n,1≤j≤m). 
The following q lines contains three integers a(1≤a≤4), x and y.
Output 
For each test case, output the matrix M after all q operations.
Sample Input
2 
3 4 2 
1 2 3 4 
2 3 4 5 
3 4 5 6 
1 1 2 
3 1 10 
2 2 2 
1 10 
10 1 
1 1 2 
2 1 2
Sample Output
12 13 14 15 
1 2 3 4 
3 4 5 6 
1 10 
10 1
Hint 
 Recommand to use scanf and printf 
解析：本题很容易超时，需要注意，要用数组来记录数据代替循环，注意理解，代码如下：
```
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int s[1005][1005];
int p[1005],q[1005];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int g[1005]={0},h[1005]={0};
        int a,b,n;
        scanf("%d %d %d",&a,&b,&n);
        for(int i=1; i<=a; i++)
            p[i]=i;
        for(int j=1; j<=b; j++)
            q[j]=j;
        for(int i=1;i<=a;i++)
          for(int j=1;j<=b;j++) 
            scanf("%d",&s[i][j]);
        int k,x,y;
        while(n--)
        {
            scanf("%d %d %d",&k,&x,&y);
            if(k==1)
              swap(p[x],p[y]);    //数组保存的数据对换，代替循环
            else if(k==2)
              swap(q[x],q[y]);
            else  if(k==3)
              g[p[x]]+=y;         //利用数组下标的数值加y代替循环，处理方法很巧妙
            else
              h[q[x]]+=y;
        } 
        for(int i=1;i<=a;i++)
        {
            for(int j=1;j<b;j++)
               printf("%d ",s[p[i]][q[j]]+g[p[i]]+h[q[j]]);
            printf("%d\n",s[p[i]][q[b]]+g[p[i]]+h[q[b]]);
        } 
    }
    return 0;
}
```

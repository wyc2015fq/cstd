# HDU1016 Prime Ring Problem dfs+回溯 - 紫芝的博客 - CSDN博客





2018年05月29日 12:58:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：35
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)









# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1016)

# Prime Ring Problem

****Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 62693    Accepted Submission(s): 26947****



Problem Description

A ring is compose of n circles as shown in diagram. Put natural number 1, 2, ..., n into each circle separately, and the sum of numbers in two adjacent circles should be a prime.


Note: the number of first circle should always be 1.

![](http://acm.hdu.edu.cn/data/images/1016-1.gif)

Input

n (0 < n < 20).

Output

The output format is shown as sample below. Each row represents a series of circle numbers in the ring beginning from 1 clockwisely and anticlockwisely. The order of numbers must satisfy the above requirements. Print solutions in lexicographical order.

You are to write a program that completes above process.

Print a blank line after each case.

Sample Input

6

8

Sample Output

Case 1:

1 4 3 2 5 6

1 6 5 2 3 4

Case 2:

1 2 3 8 5 6 7 4

1 2 5 8 3 4 7 6

1 4 7 6 5 8 3 2

1 6 7 4 3 8 5 2

Source

[Asia 1996, Shanghai (Mainland China)](http://acm.hdu.edu.cn/search.php?field=problem&key=Asia+1996%2C+Shanghai+%28Mainland+China%29&source=1&searchmode=source)

Recommend

JGShining   |   We have carefully selected several similar problems for you:  [1312](http://acm.hdu.edu.cn/showproblem.php?pid=1312)[1072](http://acm.hdu.edu.cn/showproblem.php?pid=1072)[1242](http://acm.hdu.edu.cn/showproblem.php?pid=1242)[1253](http://acm.hdu.edu.cn/showproblem.php?pid=1253)[1181](http://acm.hdu.edu.cn/showproblem.php?pid=1181)



题意：

输入一个数n，把1到n的自然数放到一个环里，相邻两个数之和为素数，第一个数必须是1

思路：

素数打表+dfs回溯

因为第一个数是1，所以从2开始搜索

如果这个数没用过，并且这个数和上一个放到环里的数之和是素数，则把这个数字放入环中

```cpp
#include<bits/stdc++.h>
#include<iostream>
using namespace std;
int a[20],vis[20],isprime[45]={0},n;
void get_prime()
{
  int i,j;

  for(i=2;i<8;i++)
    if(!isprime[i])
     for(j=i*i;j<45;j+=i)
       isprime[j]=1;
}//把40以内的素数打出来.0代表是素数
int dfs(int step)
{
    int i;
    if(step==n+1&&!isprime[a[n]+a[1]])//深搜到结束条件
    {
      for(i=1;i<n;i++)
      printf("%d ",a[i]);
      printf("%d\n",a[n]);
      return 0;
    }
    for(i=2;i<=n;i++)
    {
      if(!vis[i]&&!isprime[i+a[step-1]])//如果这个数没用过，并且这个数和上一个放到环里的数之和是素数
      {
        a[step]=i;
        vis[i]=1;
        dfs(step+1);
        vis[i]=0;//回溯
      }
    }
}
int main()
{
    int k=1;
    a[1]=1;
    get_prime();
    while(scanf("%d",&n)!=EOF)
    {
      memset(vis,0,sizeof(vis));
      printf("Case %d:\n",k++);
      dfs(2);
      printf("\n");
    }
}
```






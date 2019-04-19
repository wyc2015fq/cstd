# hdu  1506  Largest Rectangle in a Histogram（最大子矩阵） - HJ - CSDN博客
2017年02月02日 18:42:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：241
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
A histogram is a polygon composed of a sequence of rectangles aligned at a common base line. The rectangles have equal widths but may have different heights. For example, the figure on the left shows the histogram that consists of rectangles with the heights 2, 1, 4, 5, 1, 3, 3, measured in units where 1 is the width of the rectangles:
Usually, histograms are used to represent discrete distributions, e.g., the frequencies of characters in texts. Note that the order of the rectangles, i.e., their heights, is important. Calculate the area of the largest rectangle in a histogram that is aligned at the common base line, too. The figure on the right shows the largest aligned rectangle for the depicted histogram.
Input 
The input contains several test cases. Each test case describes a histogram and starts with an integer n, denoting the number of rectangles it is composed of. You may assume that 1 <= n <= 100000. Then follow n integers h1, …, hn, where 0 <= hi <= 1000000000. These numbers denote the heights of the rectangles of the histogram in left-to-right order. The width of each rectangle is 1. A zero follows the input for the last test case.
Output 
For each test case output on a single line the area of the largest rectangle in the specified histogram. Remember that this rectangle must be aligned at the common base line.
Sample Input
7 2 1 4 5 1 3 3 
4 1000 1000 1000 1000 
0
Sample Output
8 
4000
题目大意： 
求最大的子矩阵（见图）
解题思路：
我们先求出连续比它高的最左边的下标假设为l
然后求出比它高的最右边的下标假设为r
然后矩阵的面积就是(r-l+1)*1；
我们从左到右扫一遍，求出每个点的l保存在l[]数组里，然后从右到左扫一遍，求出每个点的r保存在r[]数组里，最后可以求出最大的矩阵了。
```cpp
#include<bits/stdc++.h>
using namespace std;
long long a[100005],l[100005],r[100005];
int main()
{
    long long n,t;
    while(cin>>n&&n)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i];
        l[1]=1,r[n]=n;
        for(int i=2;i<=n;i++)
        {
            t=i;
            while(t>1&&a[i]<=a[t-1])     //往右寻找连续的比a[i]大的最右边下标 
              t=l[t-1];
            l[i]=t;
        }
        for(int i=n-1;i>=1;i--)
        {
            t=i;
            while(t<n&&a[i]<=a[t+1])    //同上，向左寻找连续的比a[i]大的最左边下标 
              t=r[t+1];
            r[i]=t;
        }
        long long ans=0;
        for(int i=1;i<=n;i++)
          ans=max(ans,(r[i]-l[i]+1)*a[i]);     //寻找最大子矩阵 
        cout<<ans<<endl;
    }
    return 0;
}
```

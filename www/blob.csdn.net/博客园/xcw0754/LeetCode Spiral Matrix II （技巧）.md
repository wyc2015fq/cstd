# LeetCode  Spiral Matrix II （技巧） - xcw0754 - 博客园
# [LeetCode  Spiral Matrix II （技巧）](https://www.cnblogs.com/xcw0754/p/4937053.html)
题意：　　
　　从1开始产生连续的n2个数字，以螺旋的方式填满一个n*n的数组。
思路：
　　由于是填满一个矩阵，那么只需要每次都填一圈即可。应该注意特殊情况。
　　迭代：
```
1 class Solution {
 2 public:
 3     vector<vector<int> > generateMatrix(int n)
 4     {
 5         vector<vector<int> > ans(n,vector<int>(n));
 6         int cnt=0, i=0;
 7         while(1)
 8         {
 9             if(cnt==n*n)    break;
10             for(int j=i; j<n-i; j++)    ans[i][j]=++cnt;
11             for(int j=i+1; j<n-i; j++)    ans[j][n-i-1]=++cnt;
12             for(int j=n-i-2; j>=i; j--)ans[n-i-1][j]=++cnt;
13             for(int j=n-i-2; j>i; j--)    ans[j][i]=++cnt;
14             i++;
15         }
16         return ans;
17     }
18 };
AC代码
```
　　递归：　　
```
1 class Solution {
 2 public:
 3     vector<vector<int> > ans;
 4     int n;
 5     void generate(int i,int cnt)
 6     {
 7         if(cnt==n*n)    return ;
 8         for(int j=i; j<n-i; j++)    ans[i][j]=++cnt;
 9         for(int j=i+1; j<n-i; j++)    ans[j][n-i-1]=++cnt;
10         for(int j=n-i-2; j>=i; j--)ans[n-i-1][j]=++cnt;
11         for(int j=n-i-2; j>i; j--)    ans[j][i]=++cnt;
12         generate(i+1,cnt);
13     }
14     
15     
16     vector<vector<int> > generateMatrix(int n)
17     {
18         ans=vector<vector<int> >(n,vector<int>(n));
19         this->n=n;
20         generate(0, 0);
21         return ans;
22     }
23 };
AC代码
```


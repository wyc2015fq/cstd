# LeetCode  Combinations （DFS） - xcw0754 - 博客园
# [LeetCode  Combinations （DFS）](https://www.cnblogs.com/xcw0754/p/4940998.html)
题意：
　　产生从1～n的k个数的所有组合，按升序排列并返回。
思路：
　　DFS一遍即可解决。注意升序。
```
1 class Solution {
 2 public:
 3     vector<vector<int>> ans;
 4     int nn, kk;
 5     void DFS(vector<int>& seq,int n,int cnt)
 6     {
 7         if(cnt==kk)    ans.push_back(seq);
 8         else if(n>nn)    return ;
 9         else
10         {
11             for(int i=n; i<=nn; i++)
12             {
13                 seq[cnt]=i;
14                 DFS(seq,i+1,cnt+1);
15             }        
16         }
17     }
18     vector<vector<int>> combine(int n, int k) {
19         nn=n;kk=k;
20         vector<int> tmp(k,0);
21         DFS(tmp,1,0);
22         return ans;
23     }
24 };
AC代码
```


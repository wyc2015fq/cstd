# LeetCode  Combination Sum III （DFS） - xcw0754 - 博客园
# [LeetCode  Combination Sum III （DFS）](https://www.cnblogs.com/xcw0754/p/4970087.html)
题意：
　　在1～9这9个数字中选择k个出来，若他们的和为n，则加入答案序列，注意升序。
思路：
　　用DFS的方式，每次决定一个数字，共决策k次。假设上个决策是第i位为5，那么i+1位的范围就是6～9。
```
1 class Solution {
 2 public:
 3     vector<vector<int>> combinationSum3(int k, int n) {
 4         vector<vector<int>> ans;
 5         vector<int> num(k,0);
 6         DFS(ans,num,k,0,0,0,n);
 7         return ans;
 8     }
 9     void DFS(vector<vector<int>>& ans,vector<int>& num,int k,int j,int i,int sum,int n)
10     {
11         if(i==k)    
12         {
13             if(sum==n)    ans.push_back(num);
14             return ;
15         }
16         else    
17         {
18             for(j++; j<10; j++)
19             {
20                 num[i]=j;
21                 DFS(ans,num,k,j,i+1,sum+j,n);
22             }
23         }
24     }
25 };
AC代码
```


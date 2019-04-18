# LeetCode Subsets II （DFS） - xcw0754 - 博客园
# [LeetCode Subsets II （DFS）](https://www.cnblogs.com/xcw0754/p/4976184.html)
题意：
　　给一个集合，有n个可能相同的元素，求出所有的子集（包括空集，但是不能重复）。
思路：
　　看这个就差不多了。[LEETCODE SUBSETS （DFS）](http://www.cnblogs.com/xcw0754/p/4975981.html)
```
1 class Solution {
 2 public:
 3     vector<vector<int>> subsets(vector<int>& nums) {
 4         sort(nums.begin(),nums.end());
 5         DFS(0,nums,tmp);
 6         ans.push_back(vector<int>());
 7         return ans;        
 8     }
 9 
10     void DFS(int pos,vector<int>& nums,vector<int>& seq)
11     {
12         if(pos>=nums.size())
13         {
14             if(!seq.empty())    ans.push_back(seq);
15             return ;
16         }
17         for( ; pos<nums.size(); pos++)    
18         {
19             seq.push_back(nums[pos]);
20             DFS(pos+1,nums,seq);    //放
21             seq.pop_back();
22 
23             while(pos+1<nums.size() && nums[pos]==nums[pos+1])    pos++;//主要在这
24         }
25         DFS(pos+1,nums,seq);
26     }
27 private:
28     vector<vector<int>> ans;
29     vector<int> tmp;
30 };
AC代码
```


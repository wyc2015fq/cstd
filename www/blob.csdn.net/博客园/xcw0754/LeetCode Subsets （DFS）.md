# LeetCode  Subsets （DFS） - xcw0754 - 博客园
# [LeetCode  Subsets （DFS）](https://www.cnblogs.com/xcw0754/p/4975981.html)
题意：
　　给一个集合，有n个互不相同的元素，求出所有的子集（包括空集，但是不能重复）。
思路：
　　DFS方法：由于集合中的元素是不可能出现相同的，所以不用解决相同的元素而导致重复统计。
```
1 class Solution {
 2 public:
 3     vector<vector<int>> subsets(vector<int>& nums) {
 4         sort(nums.begin(),nums.end());
 5         DFS(0,nums,tmp);
 6         return ans;        
 7     }
 8 
 9     void DFS(int pos,vector<int>& nums,vector<int>& seq)
10     {
11         ans.push_back(seq);
12         for( ; pos<nums.size(); pos++)    
13         {
14             seq.push_back(nums[pos]);
15             DFS(pos+1,nums,seq);    //放
16             seq.pop_back();
17         }
18     }
19 private:
20     vector<vector<int>> ans;
21     vector<int> tmp;
22 };
AC代码
```
　　迭代解决：由于集合中的元素是不可能出现相同的，所以子集的个数必定是2n个，即每个数字有可取可不取这两种选择。
```
1 class Solution {
 2 public:
 3     vector<vector<int>> subsets(vector<int>& nums) {
 4         sort(nums.begin(),nums.end());
 5         int n=nums.size();
 6         for(int i=0; i<(1<<n); i++)
 7         {
 8             vector<int> tmp;
 9             for(int j=0; j<n; j++)
10             {
11                 if((i>>j)&1)
12                     tmp.push_back(nums[j]);
13             }
14             ans.push_back(tmp);
15         }
16         return ans;        
17     }
18 private:
19     vector<vector<int>> ans;
20 };
AC代码
```
　　如果集合中有相同的元素的话，解法同[LEETCODE COMBINATION SUM II （DFS）](http://www.cnblogs.com/xcw0754/p/4970397.html)，主要在于去重，而去重的技术一样。
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


# LeetCode Summary Ranges (统计有序数列范围) - xcw0754 - 博客园
# [LeetCode Summary Ranges (统计有序数列范围)](https://www.cnblogs.com/xcw0754/p/4628929.html)
题意：给出个有序不重复数列（可能负数），用缩写法记录这个数列。
思路：找每个范围的起始和结束即可。
```
1 class Solution {
 2 public:
 3     vector<string> summaryRanges(vector<int>& nums) {
 4         if(nums.empty())    return vector<string>();
 5         vector<string> vect;
 6         for(int i=0; i<nums.size(); i++)
 7         {
 8             int s=nums[i], e;
 9             while(i+1<nums.size()&&nums[i]+1==nums[i+1])    i++;
10             e=nums[i];
11             if(s<e) vect.push_back(to_string(s)+"->"+to_string(e));
12             else    vect.push_back(to_string(s));
13         }
14         return vect;
15     }
16 };
AC代码
```


# LeetCode Contains Duplicate  （判断重复元素） - xcw0754 - 博客园
# [LeetCode Contains Duplicate  （判断重复元素）](https://www.cnblogs.com/xcw0754/p/4621502.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/042349006325812.png)
题意：
　　如果所给序列的元素不是唯一的，则返回true，否则false。
思路：
　　哈希map解决。
```
1 class Solution {
 2 public:
 3     bool containsDuplicate(vector<int>& nums) {
 4         unordered_map<int,int> mapp;
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             if(mapp[nums[i]])   return true;
 8             else    mapp[nums[i]]=1;
 9         }
10         return false;
11     }
12 };
AC代码
```
python3
直接排序，再比对相邻元素
```
1 class Solution(object):
 2     def containsDuplicate(self, nums):
 3         """
 4         :type nums: List[int]
 5         :rtype: bool
 6         """
 7         if nums==[]:    return False
 8         nums=sorted(nums)
 9         i=1
10         while i<len(nums):
11             if nums[i-1]==nums[i]:
12                 return True
13             i+=1
14         return False
AC代码
```
用set辅助
```
1 class Solution(object):
2     def containsDuplicate(self, nums):
3         """
4         :type nums: List[int]
5         :rtype: bool
6         """
7         return len(set(nums))<len(nums)
AC代码
```


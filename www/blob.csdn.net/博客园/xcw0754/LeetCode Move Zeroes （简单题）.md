# LeetCode Move Zeroes （简单题） - xcw0754 - 博客园
# [LeetCode Move Zeroes （简单题）](https://www.cnblogs.com/xcw0754/p/4916801.html)
题意：
　　给定一个整型数组nums，要求将其中所有的0移动到末尾，并维护所有非0整数的相对位置不变。
思路：
　　扫一遍，两个指针维护0与非0的交界，将非0的数向前赋值就行了。
C++ 
```
1 class Solution {
 2 public:
 3     void moveZeroes(vector<int>& nums) {
 4         int idx=0;
 5         for(int i=0; i<nums.size(); i++)
 6             if(nums[i]!=0)
 7                 nums[idx++]=nums[i];
 8         while(idx<nums.size())
 9             nums[idx++]=0;
10     }
11 };
AC代码
```
python3
```
1 class Solution(object):
2     def moveZeroes(self, nums):
3         """
4         :type nums: List[int]
5         :rtype: void Do not return anything, modify nums in-place instead.
6         """
7         nums.sort(key=lambda x:0 if x==0 else -1 )
AC代码
```
```
1 class Solution(object):
 2     def moveZeroes(self, nums):
 3         """
 4         :type nums: List[int]
 5         :rtype: void Do not return anything, modify nums in-place instead.
 6         """
 7         cnt=nums.count(0)
 8         for i in range(cnt):
 9             nums.remove(0)
10             nums.append(0)
AC代码
```


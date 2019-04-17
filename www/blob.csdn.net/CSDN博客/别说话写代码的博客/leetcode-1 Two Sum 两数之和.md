# leetcode-1:Two Sum 两数之和 - 别说话写代码的博客 - CSDN博客





2018年12月01日 13:26:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：30标签：[Two Sum																[两数之和																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=两数之和&t=blog)




**题目**：

Given an array of integers, return **indices** of the two numbers such that they add up to a specific target.

You may assume that each input would have ***exactly*** one solution, and you may not use the *same* element twice.

**Example:**

```
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```

给定一个整数数组 `nums` 和一个目标值 `target`，请你在该数组中找出和为目标值的 **两个** 整数。

你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。

**示例:**

给定 nums = [2, 7, 11, 15], target = 9

因为 nums[**0**] + nums[**1**] = 2 + 7 = 9
所以返回 [**0, 1**]
**思路**：（1）可以暴力循环，遍历两个数之和，O(N^2)   （2）空间换时间，可以用哈希表unordered_map将所有元素存下来， 接下来找target-nums[i]是否存在。注意要判断查找到的数字不是第一个数字，比如target是4，遍历到了一个2，那么另外一个2不能是之前那个2

使用哈希表方法的代码

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> m;
        vector<int> ret;
        for(int i=0;i<nums.size();++i)
            m[nums[i]] = i;
        for(int i=0;i<nums.size();++i)
        {
            int t = target - nums[i];
            if(m.count(t) && m[t]!=i)
            {
                ret.push_back(i);
                ret.push_back(m[t]);
                break;
            }
        }return ret;
    }
};
```

还可以只遍历一遍

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> m;
        for(int i=0;i<nums.size();++i)
        {
            if(m.count(target-nums[i]))
               return {m[target-nums[i]],i};
            m[nums[i]] = i;
        }return {};
    }
};
```](https://so.csdn.net/so/search/s.do?q=Two Sum&t=blog)





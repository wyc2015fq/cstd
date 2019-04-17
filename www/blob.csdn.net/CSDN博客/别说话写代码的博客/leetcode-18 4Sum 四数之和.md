# leetcode-18.4Sum 四数之和 - 别说话写代码的博客 - CSDN博客





2018年12月28日 18:33:32[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22标签：[四数之和																[4Sum																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=4Sum&t=blog)




题目：
|Given an array `nums` of *n* integers and an integer `target`, are there elements *a*, *b*, *c*, and *d* in `nums` such that *a* + *b* + *c* + *d* = `target`? Find all unique quadruplets in the array which gives the sum of `target`.**Note:**The solution set must not contain duplicate quadruplets.**Example:**Given array nums = [1, 0, -1, 0, -2, 2], and target = 0.A solution set is:[  [-1,  0, 0, 1],  [-2, -1, 1, 2],  [-2,  0, 0, 2]]|给定一个包含 *n* 个整数的数组 `nums` 和一个目标值 `target`，判断 `nums` 中是否存在四个元素 *a，**b，c* 和 *d* ，使得 *a* + *b* + *c* + *d* 的值与 `target` 相等？找出所有满足条件且不重复的四元组。**注意：**答案中不可以包含重复的四元组。**示例：**给定数组 nums = [1, 0, -1, 0, -2, 2]，和 target = 0。满足要求的四元组集合为：[  [-1,  0, 0, 1],  [-2, -1, 1, 2],  [-2,  0, 0, 2]]|
|----|----|

思路：与[2Sum](https://blog.csdn.net/qq_21997625/article/details/84672868), [3Sum](https://blog.csdn.net/qq_21997625/article/details/85062164), [3Sum Clost](https://blog.csdn.net/qq_21997625/article/details/85112227)这几个 题目类似。这道题首先对原数组排序，然后固定 第一个参数i，然后固定第二个参数j=i+1，然后使用left从j+1开始往后和right从len-1开始往前找四数之和是否为target,如果 是加入输出数组，遍历所有 情况， 时间复杂度为O(N^2).这里注意要判断不重复，我们可以借用set来去除重复。

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        set<vector<int>> res;
        sort(nums.begin(),nums.end());
        int left,right,j,sum=0,len=nums.size();
        
        for(int i=0;i<len-3;++i)
        {
            for(j=i+1;j<len-2;++j)
            {
                if(j>i+1 && nums[j]==nums[j-1]) continue; //判断元素是否相等
                left=j+1; right = len-1;
                while(left<right)
                {
                    sum=nums[i]+nums[j]+nums[left]+nums[right];
                    if(sum==target)
                    {
                        vector<int> output{nums[i],nums[j],nums[left],nums[right]};
                        res.insert(output);
                        left++;
                    }else if(sum<target) left++;
                    else right--;
                }
            }
        }return vector<vector<int>> {res.begin(),res.end()};
    }
};
```](https://so.csdn.net/so/search/s.do?q=四数之和&t=blog)





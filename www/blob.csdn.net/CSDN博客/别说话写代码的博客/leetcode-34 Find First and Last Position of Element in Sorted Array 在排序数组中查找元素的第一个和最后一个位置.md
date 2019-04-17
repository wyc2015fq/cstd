# leetcode-34. Find First and Last Position of Element in Sorted Array 在排序数组中查找元素的第一个和最后一个位置 - 别说话写代码的博客 - CSDN博客





2019年01月07日 14:42:26[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[在排序数组中查找元素的第一个和最后一个位置																[leetcode 34																[leetcode																[Find First and Last Position o](https://so.csdn.net/so/search/s.do?q=Find First and Last Position o&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




题目：
|Given an array of integers `nums` sorted in ascending order, find the starting and ending position of a given `target` value.Your algorithm's runtime complexity must be in the order of *O*(log *n*).If the target is not found in the array, return `[-1, -1]`.**Example 1:****Input:** nums = [`5,7,7,8,8,10]`, target = 8**Output:** [3,4]**Example 2:****Input:** nums = [`5,7,7,8,8,10]`, target = 6**Output:** [-1,-1]|给定一个按照升序排列的整数数组 `nums`，和一个目标值 `target`。找出给定目标值在数组中的开始位置和结束位置。你的算法时间复杂度必须是 *O*(log *n*) 级别。如果数组中不存在目标值，返回 `[-1, -1]`。**示例 1:****输入:** nums = [`5,7,7,8,8,10]`, target = 8**输出:** [3,4]**示例 2:****输入:** nums = [`5,7,7,8,8,10]`, target = 6**输出:** [-1,-1]|
|----|----|

思路：题目限定时间复杂度O(logn),所以想到先二分 查找，查找到某个元素后分块查找，即从查找到的位置 向左向右找相同的值

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int n=nums.size();
        if(n==0) return {-1,-1};
        int left=0,right=n,mid,i,j;
        while(left<=right && right>=0 && left<n)
        {
            mid=(left+right)/2;
            if(nums[mid]<target) left=mid+1;
            else if (nums[mid]>target) right=mid-1;
            else
            {
                for(j=mid;j<n;++j)
                    if(nums[j]!=target) break;
                for(i=mid;i>=0;--i)
                    if(nums[i]!=target) break;           
                return {i+1,j-1};
            }
        }
        return {-1,-1};
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode 34&t=blog)](https://so.csdn.net/so/search/s.do?q=在排序数组中查找元素的第一个和最后一个位置&t=blog)





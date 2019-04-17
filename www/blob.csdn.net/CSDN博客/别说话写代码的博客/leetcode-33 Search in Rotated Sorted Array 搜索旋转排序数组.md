# leetcode-33.Search in Rotated Sorted Array 搜索旋转排序数组 - 别说话写代码的博客 - CSDN博客





2018年12月28日 19:53:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11标签：[搜索旋转排序数组																[leetcode																[Search in Rotated Sorted Array](https://so.csdn.net/so/search/s.do?q=Search in Rotated Sorted Array&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




题目：
|Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.(i.e., `[0,1,2,4,5,6,7]` might become `[4,5,6,7,0,1,2]`).You are given a target value to search. If found in the array return its index, otherwise return `-1`.You may assume no duplicate exists in the array.Your algorithm's runtime complexity must be in the order of *O*(log *n*).**Example 1:****Input:** nums = [`4,5,6,7,0,1,2]`, target = 0**Output:** 4**Example 2:****Input:** nums = [`4,5,6,7,0,1,2]`, target = 3**Output:** -1|假设按照升序排序的数组在预先未知的某个点上进行了旋转。( 例如，数组 `[0,1,2,4,5,6,7]` 可能变为 `[4,5,6,7,0,1,2]` )。搜索一个给定的目标值，如果数组中存在这个目标值，则返回它的索引，否则返回 `-1` 。你可以假设数组中不存在重复的元素。你的算法时间复杂度必须是 *O*(log *n*) 级别。**示例 1:****输入:** nums = [`4,5,6,7,0,1,2]`, target = 0**输出:** 4**示例 2:****输入:** nums = [`4,5,6,7,0,1,2]`, target = 3**输出:** -1|
|----|----|

思路：排序数组首先考虑使用二分查找。两种划分，第一种nums[mid]<nums[j]，说明后面是有序的，看target是否在这一区间，如果在i=mid+1,不在j=mid-1就在左边找。第二种，nums[mid]>num[j],说明mid前面是有序，看target是否在这一区间，如果在j=mid-1,不在i=mid+1

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int i=0,j=nums.size()-1,mid;
        while(i<=j)
        {
            mid = (i+j)/2;
            if(nums[mid]==target) return mid;
            else if(nums[mid]<nums[j])
            {
                if(nums[mid]<target && target<=nums[j]) i=mid+1;
                else j=mid-1;
            }else
            {
                if(nums[i]<=target && target<nums[mid]) j=mid-1;
                else i=mid+1;
            }
        }return -1;
    }
};
```](https://so.csdn.net/so/search/s.do?q=搜索旋转排序数组&t=blog)





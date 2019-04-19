# leetcode 35. Search Insert Position （二分） - HJ - CSDN博客
2018年03月04日 12:27:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：78
个人分类：[leetcode](https://blog.csdn.net/feizaoSYUACM/article/category/7480844)
题目链接： [https://segmentfault.com/a/1190000007287526](https://segmentfault.com/a/1190000007287526)
解题方法：
二分查找nums数组中第一个不小于target值的数所对应的nums数组下标
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0,right = nums.size();
        while(left < right) {
            int mid = (left + right) / 2;
            if(nums[mid] >= target) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }
        return left;
    }
};
```

# [CareerCup] 11.3 Search in Rotated Sorted Array 在旋转有序矩阵中搜索 - Grandyang - 博客园







# [[CareerCup] 11.3 Search in Rotated Sorted Array 在旋转有序矩阵中搜索](https://www.cnblogs.com/grandyang/p/4884594.html)







11.3 Given a sorted array of n integers that has been rotated an unknown number of times, write code to find an element in the array. You may assume that the array was originally sorted in increasing order.
 EXAMPLE
 Input: find 5 in {15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14}
 Output: 8 (the index of 5 in the array)



LeetCode上的原题，请参见我之前的博客[Search in Rotated Sorted Array 在旋转有序数组中搜索](http://www.cnblogs.com/grandyang/p/4325648.html)和[Search in Rotated Sorted Array II 在旋转有序数组中搜索之二](http://www.cnblogs.com/grandyang/p/4325840.html)。



```
class Solution {
public:
    int search(vector<int> nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {
            int mid = left + (right - left ) / 2;
            if (nums[mid] == target) return mid;
            else if (nums[mid] < nums[right]) {
                if (nums[mid] < target && nums[right] >= target) left = mid + 1;
                else right = mid - 1;
            } else if (nums[mid] > nums[right]) {
                if (nums[left] <= target && nums[mid] > target) right = mid - 1;
                else left = mid + 1;
            } else --right;
        }
        return -1;
    }
};
```















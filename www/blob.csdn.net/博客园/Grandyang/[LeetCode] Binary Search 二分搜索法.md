# [LeetCode] Binary Search 二分搜索法 - Grandyang - 博客园







# [[LeetCode] Binary Search 二分搜索法](https://www.cnblogs.com/grandyang/p/9937844.html)







Given a sorted (in ascending order) integer array `nums` of `n` elements and a `target` value, write a function to search `target` in `nums`. If `target` exists, then return its index, otherwise return `-1`.


Example 1:
Input: `nums` = [-1,0,3,5,9,12], `target` = 9
Output: 4
Explanation: 9 exists in `nums` and its index is 4


Example 2:
Input: `nums` = [-1,0,3,5,9,12], `target` = 2
Output: -1
Explanation: 2 does not exist in `nums` so return -1



Note:
- You may assume that all elements in `nums` are unique.
- `n` will be in the range `[1, 10000]`.
- The value of each element in `nums` will be in the range `[-9999, 9999]`.



这道题就是最基本的二分搜索法了，这是博主之前总结的[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)的四种之中的第一类，也是最简单的一类，写法什么很模版啊，注意right的初始化值，还有while的循环条件，以及right的更新值，这三点不同的人可能会有不同的写法，选一种自己最习惯的就好啦，参见代码如下：



```
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size();
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;
            else if (nums[mid] < target) left = mid + 1;
            else right = mid;
        }
        return -1;
    }
};
```



类似题目：

[Search in a Sorted Array of Unknown Size](https://www.cnblogs.com/grandyang/p/9937770.html)



参考资料：

[https://leetcode.com/problems/binary-search](https://leetcode.com/problems/binary-search)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













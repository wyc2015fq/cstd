# [LeetCode] Find Minimum in Rotated Sorted Array II 寻找旋转有序数组的最小值之二 - Grandyang - 博客园







# [[LeetCode] Find Minimum in Rotated Sorted Array II 寻找旋转有序数组的最小值之二](https://www.cnblogs.com/grandyang/p/4040438.html)





> 
*Follow up* for "Find Minimum in Rotated Sorted Array":
What if *duplicates* are allowed?

Would this affect the run-time complexity? How and why?


Suppose a sorted array is rotated at some pivot unknown to you beforehand.

(i.e., `0 1 2 4 5 6 7` might become `4 5 6 7 0 1 2`).

Find the minimum element.

The array may contain duplicates.



寻找旋转有序重复数组的最小值是对之前问题的延伸([http://www.cnblogs.com/grandyang/p/4032934.html](http://www.cnblogs.com/grandyang/p/4032934.html))，当数组中存在大量的重复数字时，就会破坏二分查找法的机制，我们无法取得O(lgn)的时间复杂度，又将会回到简单粗暴的O(n)，比如如下两种情况：

{2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 2} 和 {2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2}， 我们发现，当第一个数字和最后一个数字，还有中间那个数字全部相等的时候，二分查找法就崩溃了，因为它无法判断到底该去左半边还是右半边。这种情况下，我们将左指针右移一位，略过一个相同数字，这对结果不会产生影响，因为我们只是去掉了一个相同的，然后对剩余的部分继续用二分查找法，在最坏的情况下，比如数组所有元素都相同，时间复杂度会升到O(n)，参见代码如下：



```
class Solution {
public:
    int findMin(vector<int> &nums) {
        if (nums.empty()) return 0;
        int left = 0, right = nums.size() - 1, res = nums[0];
        while (left < right - 1) {
            int mid = left + (right - left) / 2;
            if (nums[left] < nums[mid]) {
                res = min(res, nums[left]);
                left = mid + 1;
            } else if (nums[left] > nums[mid]) {
                res = min(res, nums[right]);
                right = mid;
            } else ++left;
        }
        res = min(res, nums[left]);
        res = min(res, nums[right]);
        return res;
    }
};
```



参考资料：

[http://blog.csdn.net/linhuanmars/article/details/40449299](http://blog.csdn.net/linhuanmars/article/details/40449299)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













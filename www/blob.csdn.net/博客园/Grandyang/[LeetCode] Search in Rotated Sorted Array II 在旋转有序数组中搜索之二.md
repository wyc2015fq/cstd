# [LeetCode] Search in Rotated Sorted Array II 在旋转有序数组中搜索之二 - Grandyang - 博客园







# [[LeetCode] Search in Rotated Sorted Array II 在旋转有序数组中搜索之二](https://www.cnblogs.com/grandyang/p/4325840.html)







Follow up for "Search in Rotated Sorted Array":
What if *duplicates* are allowed?

Would this affect the run-time complexity? How and why?

Write a function to determine if a given target is in the array.



这道是之前那道[Search in Rotated Sorted Array 在旋转有序数组中搜索](http://www.cnblogs.com/grandyang/p/4325648.html) 的延伸，现在数组中允许出现重复数字，这个也会影响我们选择哪半边继续搜索，由于之前那道题不存在相同值，我们在比较中间值和最右值时就完全符合之前所说的规律：**如果中间的数小于最右边的数，则右半段是有序的，若中间数大于最右边数，则左半段是有序的**。而如果可以有重复值，就会出现来面两种情况，[3 1 1] 和 [1 1 3 1]，对于这两种情况中间值等于最右值时，目标值3既可以在左边又可以在右边，那怎么办么，对于这种情况其实处理非常简单，只要把最右值向左一位即可继续循环，如果还相同则继续移，直到移到不同值为止，然后其他部分还采用[Search in Rotated Sorted Array 在旋转有序数组中搜索](http://www.cnblogs.com/grandyang/p/4325648.html) 中的方法，可以得到代码如下：



```
class Solution {
public:
    bool search(int A[], int n, int target) {
        if (n == 0) return false;
        int left = 0, right = n - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (A[mid] == target) return true;
            else if (A[mid] < A[right]) {
                if (A[mid] < target && A[right] >= target) left = mid + 1;
                else right = mid - 1;
            } else if (A[mid] > A[right]){
                if (A[left] <= target && A[mid] > target) right = mid - 1;
                else left = mid + 1;
            } else --right;
        }
        return false;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













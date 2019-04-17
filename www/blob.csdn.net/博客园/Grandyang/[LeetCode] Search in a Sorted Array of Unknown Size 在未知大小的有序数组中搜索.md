# [LeetCode] Search in a Sorted Array of Unknown Size 在未知大小的有序数组中搜索 - Grandyang - 博客园







# [[LeetCode] Search in a Sorted Array of Unknown Size 在未知大小的有序数组中搜索](https://www.cnblogs.com/grandyang/p/9937770.html)







Given an integer array sorted in ascending order, write a function to search `target` in `nums`.  If `target` exists, then return its index, otherwise return `-1`. However, the array size is unknown to you. You may only access the array using an `ArrayReader` interface, where `ArrayReader.get(k)` returns the element of the array at index `k` (0-indexed).

You may assume all integers in the array are less than `10000`, and if you access the array out of bounds, `ArrayReader.get` will return `2147483647`.



Example 1:
Input: `array` = [-1,0,3,5,9,12], `target` = 9
Output: 4
Explanation: 9 exists in `nums` and its index is 4

Example 2:
Input: `array` = [-1,0,3,5,9,12], `target` = 2
Output: -1
Explanation: 2 does not exist in `nums` so return -1


Note:
- You may assume that all elements in the array are unique.
- The value of each element in the array will be in the range `[-9999, 9999]`.



这道题给了我们一个未知大小的数组，让我们在其中搜索数字。给了我们一个ArrayReader的类，我们可以通过get函数来获得数组中的数字，如果越界了的话，会返回整型数最大值。既然是有序数组，又要搜索，那么二分搜索法肯定是不二之选，问题是需要知道数组的首尾两端的位置，才能进行二分搜索，而这道题刚好就是大小未知的数组。所以博主的第一个想法就是先用二分搜索法来求出数组的大小，然后再用一个二分搜索来查找数字，这种方法是可以通过OJ的。但其实我们是不用先来确定数组的大小的，而是可以直接进行搜索数字，我们实际上是假设数组就有整型最大值个数字，在多余的位置上相当于都填上了整型最大值，那么这也是一个有序的数组，我们可以直接用一个二分搜索法进行查找即可，参见代码如下：



```
// Forward declaration of ArrayReader class.
class ArrayReader;

class Solution {
public:
    int search(const ArrayReader& reader, int target) {
        int left = 0, right = INT_MAX;
        while (left < right) {
            int mid = left + (right - left) / 2, x = reader.get(mid);
            if (x == target) return mid;
            else if (x < target) left = mid + 1;
            else right = mid;
        }
        return -1;
    }
};
```



类似题目：

[Binary Search](https://www.cnblogs.com/grandyang/p/9937844.html)



类似题目：

[https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/](https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/)

[https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/discuss/171669/Straight-forward-binary-search.](https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/discuss/171669/Straight-forward-binary-search.)

[https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/discuss/151685/Shortest-and-cleanest-Java-solution-so-far...](https://leetcode.com/problems/search-in-a-sorted-array-of-unknown-size/discuss/151685/Shortest-and-cleanest-Java-solution-so-far...)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













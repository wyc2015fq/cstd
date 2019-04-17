# [LeetCode] Wiggle Sort II 摆动排序之二 - Grandyang - 博客园







# [[LeetCode] Wiggle Sort II 摆动排序之二](https://www.cnblogs.com/grandyang/p/5139057.html)







Given an unsorted array `nums`, reorder it such that `nums[0] < nums[1] > nums[2] < nums[3]...`.

Example 1:
Input: `nums = [1, 5, 1, 1, 6, 4]`
Output: One possible answer is `[1, 4, 1, 5, 1, 6]`.
Example 2:
Input: `nums = [1, 3, 2, 2, 3, 1]`
Output: One possible answer is `[2, 3, 1, 3, 1, 2]`.
Note:
You may assume all input has valid answer.

Follow Up:
Can you do it in O(n) time and/or in-place with O(1) extra space?



这道题给了我们一个无序数组，让我们排序成摆动数组，满足nums[0] < nums[1] > nums[2] < nums[3]...，并给了我们例子。我们可以先给数组排序，然后在做调整。调整的方法是找到数组的中间的数，相当于把有序数组从中间分成两部分，然后从前半段的末尾取一个，在从后半的末尾去一个，这样保证了第一个数小于第二个数，然后从前半段取倒数第二个，从后半段取倒数第二个，这保证了第二个数大于第三个数，且第三个数小于第四个数，以此类推直至都取完，参见代码如下：



解法一：

```
// O(n) space
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        vector<int> tmp = nums;
        int n = nums.size(), k = (n + 1) / 2, j = n; 
        sort(tmp.begin(), tmp.end());
        for (int i = 0; i < n; ++i) {
            nums[i] = i & 1 ? tmp[--j] : tmp[--k];
        }
    }
};
```



这道题的Follow up让我们用O(n)的时间复杂度和O(1)的空间复杂度，这个真的比较难，参见[网友的解答](https://leetcode.com/problems/wiggle-sort-ii/discuss/77677/O(n)%2BO(1)-after-median-Virtual-Indexing)，(未完待续。。)



解法二：

```
// O(1) space
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        #define A(i) nums[(1 + 2 * i) % (n | 1)]
        int n = nums.size(), i = 0, j = 0, k = n - 1;
        auto midptr = nums.begin() + n / 2;
        nth_element(nums.begin(), midptr, nums.end());
        int mid = *midptr;
        while (j <= k) {
            if (A(j) > mid) swap(A(i++), A(j++));
            else if (A(j) < mid) swap(A(j), A(k--));
            else ++j;
        }
    }
};
```



类似题目：

[Sort Colors](http://www.cnblogs.com/grandyang/p/4341243.html)

[Kth Largest Element in an Array](http://www.cnblogs.com/grandyang/p/4539757.html)

[Wiggle Sort](http://www.cnblogs.com/grandyang/p/5177285.html)



参考资料：

[https://leetcode.com/problemset/algorithms/](https://leetcode.com/problemset/algorithms/)

[https://leetcode.com/problems/wiggle-sort-ii/discuss/77706/Short-simple-C%2B%2B](https://leetcode.com/problems/wiggle-sort-ii/discuss/77706/Short-simple-C%2B%2B)

[https://leetcode.com/problems/wiggle-sort-ii/discuss/77677/O(n)%2BO(1)-after-median-Virtual-Indexing](https://leetcode.com/problems/wiggle-sort-ii/discuss/77677/O(n)%2BO(1)-after-median-Virtual-Indexing)

[https://leetcode.com/problems/wiggle-sort-ii/discuss/77682/Step-by-step-explanation-of-index-mapping-in-Java](https://leetcode.com/problems/wiggle-sort-ii/discuss/77682/Step-by-step-explanation-of-index-mapping-in-Java)

[https://leetcode.com/problems/wiggle-sort-ii/discuss/77681/O(n)-time-O(1)-space-solution-with-detail-explanations](https://leetcode.com/problems/wiggle-sort-ii/discuss/77681/O(n)-time-O(1)-space-solution-with-detail-explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













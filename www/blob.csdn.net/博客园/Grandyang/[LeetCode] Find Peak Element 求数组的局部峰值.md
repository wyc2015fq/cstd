# [LeetCode] Find Peak Element 求数组的局部峰值 - Grandyang - 博客园







# [[LeetCode] Find Peak Element 求数组的局部峰值](https://www.cnblogs.com/grandyang/p/4217175.html)







A peak element is an element that is greater than its neighbors.

Given an input array `nums`, where `nums[i] ≠ nums[i+1]`, find a peak element and return its index.

The array may contain multiple peaks, in that case return the index to any one of the peaks is fine.

You may imagine that `nums[-1] = nums[n] = -∞`.

Example 1:
Input: nums = `[1,2,3,1]`
Output: 2
Explanation: 3 is a peak element and your function should return the index number 2.
Example 2:
Input: nums = `[`1,2,1,3,5,6,4]
Output: 1 or 5 
Explanation: Your function can return either index number 1 where the peak element is 2, 
             or index number 5 where the peak element is 6.

Note:

Your solution should be in logarithmic complexity.



这道题是求数组的一个峰值，如果这里用遍历整个数组找最大值肯定会出现Time Limit Exceeded，但题目中说了这个峰值可以是局部的最大值，所以我们只需要找到第一个局部峰值就可以了。所谓峰值就是比周围两个数字都大的数字，那么只需要跟周围两个数字比较就可以了。既然要跟左右的数字比较，就得考虑越界的问题，题目中给了nums[-1] = nums[n] = -∞，那么我们其实可以把这两个整型最小值直接加入到数组中，然后从第二个数字遍历到倒数第二个数字，这样就不会存在越界的可能了。由于题目中说了峰值一定存在，那么有一个很重要的corner case我们要注意，就是当原数组中只有一个数字，且是整型最小值的时候，我们如果还要首尾垫数字，就会形成一条水平线，从而没有峰值了，所以我们对于数组中只有一个数字的情况在开头直接判断一下即可，参见代码如下：



C++ 解法一：

```
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        if (nums.size() == 1) return 0;
        nums.insert(nums.begin(), INT_MIN);
        nums.push_back(INT_MIN);
        for (int i = 1; i < (int)nums.size() - 1; ++i) {
            if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) return i - 1;
        }
        return -1;
    }
};
```



Java 解法一：

```
class Solution {
    public int findPeakElement(int[] nums) {
        if (nums.length == 1) return 0;
        int[] newNums = new int[nums.length + 2];
        System.arraycopy(nums, 0, newNums, 1, nums.length);
        newNums[0] = Integer.MIN_VALUE;
        newNums[newNums.length - 1] = Integer.MIN_VALUE;
        for (int i = 1; i < newNums.length - 1; ++i) {
            if (newNums[i] > newNums[i - 1] && newNums[i] > newNums[i + 1]) return i - 1;
        }
        return -1;
    }
}
```



我们可以对上面的线性扫描的方法进行一些优化，可以省去首尾垫值的步骤。由于题目中说明了局部峰值一定存在，那么实际上可以从第二个数字开始往后遍历，如果第二个数字比第一个数字小，说明此时第一个数字就是一个局部峰值；否则就往后继续遍历，现在是个递增趋势，如果此时某个数字小于前面那个数字，说明前面数字就是一个局部峰值，返回位置即可。如果循环结束了，说明原数组是个递增数组，返回最后一个位置即可，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] < nums[i - 1]) return i - 1;
        }
        return nums.size() - 1;
    }
};
```



Java 解法二：

```
public class Solution {
    public int findPeakElement(int[] nums) {
        for (int i = 1; i < nums.length; ++i) {
            if (nums[i] < nums[i - 1]) return i - 1;
        }
        return nums.length - 1;
    }
}
```



由于题目中提示了要用对数级的时间复杂度，那么我们就要考虑使用类似于二分查找法来缩短时间，由于只是需要找到任意一个峰值，那么我们在确定二分查找折半后中间那个元素后，和紧跟的那个元素比较下大小，如果大于，则说明峰值在前面，如果小于则在后面。这样就可以找到一个峰值了，代码如下：



C++ 解法三：

```
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[mid + 1]) left = mid + 1;
            else right = mid;
        }
        return right;
    }
};
```



Java 解法三：

```
public class Solution {
    public int findPeakElement(int[] nums) {
        int left = 0, right = nums.length - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[mid + 1]) left = mid + 1;
            else right = mid;
        }
        return right;
    }
}
```



类似题目：

Peak Index in a Mountain Array



参考资料：

[https://leetcode.com/problems/find-peak-element](https://leetcode.com/problems/find-peak-element)

[https://leetcode.com/problems/find-peak-element/discuss/50232/find-the-maximum-by-binary-search-recursion-and-iteration](https://leetcode.com/problems/find-peak-element/discuss/50232/find-the-maximum-by-binary-search-recursion-and-iteration)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













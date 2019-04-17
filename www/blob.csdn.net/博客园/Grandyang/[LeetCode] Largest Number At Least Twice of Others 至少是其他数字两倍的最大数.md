# [LeetCode] Largest Number At Least Twice of Others 至少是其他数字两倍的最大数 - Grandyang - 博客园







# [[LeetCode] Largest Number At Least Twice of Others 至少是其他数字两倍的最大数](https://www.cnblogs.com/grandyang/p/8387593.html)







In a given integer array `nums`, there is always exactly one largest element.

Find whether the largest element in the array is at least twice as much as every other number in the array.

If it is, return the index of the largest element, otherwise return -1.

Example 1:
Input: nums = [3, 6, 1, 0]
Output: 1
Explanation: 6 is the largest integer, and for every other number in the array x,
6 is more than twice as big as x.  The index of value 6 is 1, so we return 1.



Example 2:
Input: nums = [1, 2, 3, 4]
Output: -1
Explanation: 4 isn't at least as big as twice the value of 3, so we return -1.



Note:
- `nums` will have a length in the range `[1, 50]`.
- Every `nums[i]` will be an integer in the range `[0, 99]`.



这道题让我们找一个至少是其他数字两倍的最大数字，那么我们想，首先明确的一点是这个要求的数字一定是数组中的最大数字，因为其是其他所有的数字的至少两倍。然后就是，如果该数字是数组中第二大的数字至少两倍的话，那么它一定是其他所有数字的至少两倍，所以我们可以遍历一次数组分别求出最大数字和第二大数字，然后判断一下最大数字是否是第二大数字的两倍即可，注意这里我们判断两倍的方法并不是直接相除，为了避免除以零的情况，我们采用减法，参见代码如下：



解法一：

```
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int mx = INT_MIN, secondMx = INT_MIN, mxId = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] > mx) {
                secondMx = mx;
                mx = nums[i];
                mxId = i;
            } else if (nums[i] > secondMx) {
                secondMx = nums[i];
            }
        }
        return (mx - secondMx >= secondMx) ? mxId : -1;
    }
};
```



当然我们也可以使用更straightforward的方法，首先遍历一遍数组找出最大数字，然后再遍历一遍数组，验证这个数字是否是其他数字的至少两倍，参见代码如下：



解法二：

```
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int mx = INT_MIN, mxId = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (mx < nums[i]) {
                mx = nums[i];
                mxId = i;
            }
        }
        for (int num : nums) {
            if (mx != num && mx - num < num) return -1;
        }
        return mxId;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













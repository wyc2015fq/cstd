# [LeetCode] Set Mismatch 设置不匹配 - Grandyang - 博客园







# [[LeetCode] Set Mismatch 设置不匹配](https://www.cnblogs.com/grandyang/p/7324242.html)







The set `S` originally contains numbers from 1 to `n`. But unfortunately, due to the data error, one of the numbers in the set got duplicated to another number in the set, which results in repetition of one number and loss of another number.

Given an array `nums` representing the data status of this set after the error. Your task is to firstly find the number occurs twice and then find the number that is missing. Return them in the form of an array.

Example 1:
Input: nums = [1,2,2,4]
Output: [2,3]



Note:
- The given array size will in the range [2, 10000].
- The given array's numbers won't have any order.



这道题给了我们一个长度为n的数组，说里面的数字是从1到n，但是有一个数字重复出现了一次，从而造成了另一个数字的缺失，让我们找出重复的数字和缺失的数字。那么最直接的一种解法就是统计每个数字出现的次数了，然后再遍历次数数组，如果某个数字出现了两次就是重复数，如果出现了0次，就是缺失数，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res(2, 0), cnt(nums.size(), 0);
        for (int num : nums) ++cnt[num - 1];
        for (int i = 0; i < cnt.size(); ++i) {
            if (res[0] != 0 && res[1] != 0) return res;
            if (cnt[i] == 2) res[0] = i + 1;
            else if (cnt[i] == 0) res[1] = i + 1;
        }
        return res;
    }
};
```



我们来看一种更省空间的解法，这种解法思路相当巧妙，遍历每个数字，然后将其应该出现的位置上的数字变为其相反数，这样如果我们再变为其相反数之前已经成负数了，说明该数字是重复数，将其将入结果res中，然后再遍历原数组，如果某个位置上的数字为正数，说明该位置对应的数字没有出现过，加入res中即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res(2, -1);
        for (int i : nums) {
            if (nums[abs(i) - 1] < 0) res[0] = abs(i);
            else nums[abs(i) - 1] *= -1;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] > 0) res[1] = i + 1;
        }
        return res;
    }
};
```



下面这种方法也很赞，首先我们把乱序的数字放到其正确的位置上，用while循环来不停的放，直到该数字在正确的位置上，那么一旦数组有序了，我们只要从头遍历就能直接找到重复的数字，然后缺失的数字同样也就知道了，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        for (int i = 0; i < nums.size(); ++i) {
            while (nums[i] != nums[nums[i] - 1]) swap(nums[i], nums[nums[i] - 1]);
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != i + 1) return {nums[i], i + 1};
        }
    }
};
```



类似题目：

[Find the Duplicate Number](http://www.cnblogs.com/grandyang/p/4843654.html)



参考资料：

[https://discuss.leetcode.com/topic/96808/java-o-n-time-o-1-space](https://discuss.leetcode.com/topic/96808/java-o-n-time-o-1-space)

[https://discuss.leetcode.com/topic/97091/c-6-lines-solution-with-explanation](https://discuss.leetcode.com/topic/97091/c-6-lines-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













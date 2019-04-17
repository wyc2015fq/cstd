# [LeetCode] 3Sum Smaller 三数之和较小值 - Grandyang - 博客园







# [[LeetCode] 3Sum Smaller 三数之和较小值](https://www.cnblogs.com/grandyang/p/5235086.html)







Given an array of *n* integers *nums* and a *target*, find the number of index triplets `i, j, k` with `0 <= i < j < k < n` that satisfy the condition `nums[i] + nums[j] + nums[k] < target`.






For example, given *nums* = `[-2, 0, 1, 3]`, and *target* = 2.

Return 2. Because there are two triplets which sums are less than 2:
[-2, 0, 1]
[-2, 0, 3]

Follow up:
Could you solve it in *O*(*n*2) runtime?








这道题是3Sum问题的一个变形，让我们求三数之和小于一个目标值，那么最简单的方法就是穷举法，将所有的可能的三个数字的组合都遍历一遍，比较三数之和跟目标值之间的大小，小于的话则结果自增1，参见代码如下:



解法一：


```
// O(n^3)
class Solution {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        int res = 0;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < int(nums.size() - 2); ++i) {
            int left = i + 1, right = nums.size() - 1, sum = target - nums[i];
            for (int j = left; j <= right; ++j) {
                for (int k = j + 1; k <= right; ++k) {
                    if (nums[j] + nums[k] < sum) ++res;
                }
            }
        }
        return res;
    }
};
```



题目中的Follow up让我们在O(n^2)的时间复杂度内实现，那么我们借鉴之前那两道题[3Sum Closest](http://www.cnblogs.com/grandyang/p/4510984.html)和[3Sum](http://www.cnblogs.com/grandyang/p/4481576.html)中的方法，采用双指针来做，这里面有个trick就是当判断三个数之和小于目标值时，此时结果应该加上right-left，以为数组排序了以后，如果加上num[right]小于目标值的话，那么加上一个更小的数必定也会小于目标值，然后我们将左指针右移一位，否则我们将右指针左移一位，参见代码如下：



解法二：

```
// O(n^2)
class Solution {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        if (nums.size() < 3) return 0;
        int res = 0, n = nums.size();
        sort(nums.begin(), nums.end());
        for (int i = 0; i < n - 2; ++i) {
            int left = i + 1, right = n - 1;
            while (left < right) {
                if (nums[i] + nums[left] + nums[right] < target) {
                    res += right - left;
                    ++left;
                } else {
                    --right;
                }
            }
        }
        return res;
    }
};
```



类似题目：

[3Sum Closest](http://www.cnblogs.com/grandyang/p/4510984.html)

[3Sum](http://www.cnblogs.com/grandyang/p/4481576.html)




参考资料：


[https://leetcode.com/discuss/85994/13-lines-concise-and-easy-understand-c-solution](https://leetcode.com/discuss/85994/13-lines-concise-and-easy-understand-c-solution)

[https://leetcode.com/discuss/56164/simple-and-easy-understanding-o-n-2-java-solution](https://leetcode.com/discuss/56164/simple-and-easy-understanding-o-n-2-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














# [LeetCode] Patching Array 补丁数组 - Grandyang - 博客园







# [[LeetCode] Patching Array 补丁数组](https://www.cnblogs.com/grandyang/p/5165821.html)







Given a sorted positive integer array *nums* and an integer *n*, add/patch elements to the array such that any number in range `[1, n]` inclusive can be formed by the sum of some elements in the array. Return the minimum number of patches required.

Example 1:
*nums* = `[1, 3]`, *n* = `6`
Return `1`.

Combinations of *nums* are `[1], [3], [1,3]`, which form possible sums of: `1, 3, 4`.
Now if we add/patch `2` to *nums*, the combinations are: `[1], [2], [3], [1,3], [2,3], [1,2,3]`.
Possible sums are `1, 2, 3, 4, 5, 6`, which now covers the range `[1, 6]`.
So we only need `1` patch.

Example 2:
*nums* = `[1, 5, 10]`, *n* = `20`
Return `2`.
The two patches can be `[2, 4]`.

Example 3:
*nums* = `[1, 2, 2]`, *n* = `5`
Return `0`.

Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给我们一个有序的正数数组nums，又给了我们一个正整数n，问我们最少需要给nums加几个数字，使其能组成[1,n]之间的所有数字，注意数组中的元素不能重复使用，否则的话只有要有1，就能组成所有的数字了。这道题我又不会了，上网看到了史蒂芬大神的解法，膜拜啊，这里就全部按他的解法来讲吧。我们定义一个变量miss，用来表示[0,n]之间最小的不能表示的值，那么初始化为1，为啥不为0呢，因为n=0没啥意义，直接返回0了。那么此时我们能表示的范围是[0, miss)，表示此时我们能表示0到miss-1的数，如果此时的num <= miss，那么我们可以把我们能表示数的范围扩大到[0, miss+num)，如果num>miss，那么此时我们需要添加一个数，为了能最大限度的增加表示数范围，我们加上miss它本身，以此类推直至遍历完整个数组，我们可以得到结果。下面我们来举个例子说明：

给定nums = [1, 2, 4, 11, 30], n = 50，我们需要让[0, 50]之间所有的数字都能被nums中的数字之和表示出来。

首先使用1, 2, 4可能表示出0到7之间的所有数，表示范围为[0, 8)，但我们不能表示8，因为下一个数字11太大了，所以我们要在数组里加上一个8，此时能表示的范围是[0, 16)，那么我们需要插入16吗，答案是不需要，因为我们数组有1和4，可以组成5，而下一个数字11，加一起能组成16，所以有了数组中的11，我们此时能表示的范围扩大到[0, 27)，但我们没法表示27，因为30太大了，所以此时我们给数组中加入一个27，那么现在能表示的范围是[0, 54)，已经满足要求了，我们总共添加了两个数8和27，所以返回2即可。



解法一：

```
class Solution {
public:
    int minPatches(vector<int>& nums, int n) {
        long miss = 1, res = 0, i = 0;
        while (miss <= n) {
            if (i < nums.size() && nums[i] <= miss) {
                miss += nums[i++];
            } else {
                miss += miss;
                ++res;
            }
        }
        return res;
    }
};
```



下面这种方法跟上面那种方法原理都一样，稍有不同之处在于真正的patch了nums数组，把需要插入的数字真正的加入了数组中，那么最后用新数组的长度减去原始长度就知道我们加入了几个数字了。



解法二：

```
class Solution {
public:
    int minPatches(vector<int>& nums, int n) {
        long miss = 1, k = nums.size(), i = 0;
        while (miss <= n) {
            if (i >= nums.size() || nums[i] > miss) {
                nums.insert(nums.begin() + i, miss);
            }
            miss += nums[i++];
        }
        return nums.size() - k;
    }
};
```



参考资料：

[https://leetcode.com/discuss/82895/actually-patching](https://leetcode.com/discuss/82895/actually-patching)

[https://leetcode.com/discuss/82822/solution-explanation](https://leetcode.com/discuss/82822/solution-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)















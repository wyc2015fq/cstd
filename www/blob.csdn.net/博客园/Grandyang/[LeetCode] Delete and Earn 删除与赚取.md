# [LeetCode] Delete and Earn 删除与赚取 - Grandyang - 博客园







# [[LeetCode] Delete and Earn 删除与赚取](https://www.cnblogs.com/grandyang/p/8176933.html)







Given an array `nums` of integers, you can perform operations on the array.

In each operation, you pick any `nums[i]` and delete it to earn `nums[i]` points. After, you must delete everyelement equal to `nums[i] - 1` or `nums[i] + 1`.

You start with 0 points. Return the maximum number of points you can earn by applying such operations.

Example 1:
Input: nums = [3, 4, 2]
Output: 6
Explanation: 
Delete 4 to earn 4 points, consequently 3 is also deleted.
Then, delete 2 to earn 2 points. 6 total points are earned.



Example 2:
Input: nums = [2, 2, 3, 3, 3, 4]
Output: 9
Explanation: 
Delete 3 to earn 3 points, deleting both 2's and the 4.
Then, delete 3 again to earn 3 points, and 3 again to earn 3 points.
9 total points are earned.



Note:
- The length of `nums` is at most `20000`.
- Each element `nums[i]` is an integer in the range `[1, 10000]`.



博主浪了整整一个圣诞假期，现在也该收收心了，2018了，今年对于博主是很关键的一年，有太多的事情要去做，各种小目标需要完成，还有梦想去追逐，又要开始努力啦～在博主停更的这一周半的时间内，收到了网友们的私信和留言催更，请大家放心，2018年博主会继续坚持下去，继续追赶进度，虽然一直都没有完全追上-.-|||，照LeetCode这出题速度，今年题号有望突破一千大关啊，感觉碉堡了有木有，一起为了幸福而奋斗吧～

好了，来做题吧。这道题给了我们一个数组，每次让我们删除一个数字，删除的数字本身变为了积分累积，并且要同时移除之前数的加1和减1的数，但此时移除的数字不累计积分，让我们求最多能获得多少积分。博主最开始尝试的方法是积分大小来排列，先删除大的数字，但是不对。于是乎，博主发现相同的数字可以同时删除，于是就是建立了每个数字和其出现次数之间的映射，然后放到优先队列里，重写排序方式comparator为数字乘以其出现次数，先移除能产生最大积分的数字，可是还是不对。其实这道题跟之前那道[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)的本质是一样的，那道题小偷不能偷相邻的房子，这道题相邻的数字不能累加积分，是不是一个道理？那么对于每一个数字，我们都有两个选择，拿或者不拿。如果我们拿了当前的数字，我们就不能拿之前的数字（如果我们从小往大遍历就不需要考虑后面的数字），那么当前的积分就是不拿前面的数字的积分加上当前数字之和。如果我们不拿当前的数字，那么对于前面的数字我们既可以拿也可以不拿，于是当前的积分就是拿前面的数字的积分和不拿前面数字的积分中的较大值。这里我们用take和skip分别表示拿与不拿上一个数字，takei和skipi分别表示拿与不拿当前数字，每次更新完当前的takei和skipi时，也要更新take和skip，为下一个数字做准备，最后只要返回take和skip中的较大值即可，参见代码如下：



解法一：

```
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        vector<int> sums(10001, 0);
        int take = 0, skip = 0;
        for (int num : nums) sums[num] += num;
        for (int i = 0; i < 10001; ++i) {
            int takei = skip + sums[i];
            int skipi = max(skip, take);
            take = takei; skip = skipi;
        }
        return max(skip, take); 
    }
};
```



下面这种解法直接使用sums数组来更新，而没有使用额外的变量。上面解法中没有讲解这个sums数组，这里的sums实际上相当于建立了数字和其总积分的映射，这里的总积分的计算方法是由数字乘以其出现次数得来的。由于题目中说了每个数字不会超过10000，所以sums的长度可以初始化为10001，然后遍历原数组，将遇到的数字都累加到该数字在数组中的位置上。然后从sums数组的第三个数字开始遍历，更新方法跟上面解法的思路很类似，当前的sums[i]值就等于前一个值sums[i-1]和前两个值sums[i-2]加上当前的sums[i]值中的较大值，其实思想就是在不拿当前数的积分，跟不拿前一个数的积分加上当前的积分之和，取二者中的较大值更新当前值sums[i]，参见代码如下：



解法二：

```
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        vector<int> sums(10001, 0);
        for (int num : nums) sums[num] += num;
        for (int i = 2; i < 10001; ++i) {
            sums[i] = max(sums[i - 1], sums[i - 2] + sums[i]);
        }
        return sums[10000];
    }
};
```



类似题目：

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)



参考资料：

[https://discuss.leetcode.com/topic/112807/java-c-clean-code-with-explanation](https://discuss.leetcode.com/topic/112807/java-c-clean-code-with-explanation)

[https://discuss.leetcode.com/topic/112916/sharing-my-simple-straight-forward-java-o-n-solution-explanation-included](https://discuss.leetcode.com/topic/112916/sharing-my-simple-straight-forward-java-o-n-solution-explanation-included)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













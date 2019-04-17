# [LeetCode] Candy 分糖果问题 - Grandyang - 博客园







# [[LeetCode] Candy 分糖果问题](https://www.cnblogs.com/grandyang/p/4575026.html)







There are *N* children standing in a line. Each child is assigned a rating value.

You are giving candies to these children subjected to the following requirements:
- Each child must have at least one candy.
- Children with a higher rating get more candies than their neighbors.

What is the minimum candies you must give?



这道题看起来很难，其实解法并没有那么复杂，当然我也是看了别人的解法才做出来的，先来看看两遍遍历的解法，首先初始化每个人一个糖果，然后这个算法需要遍历两遍，第一遍从左向右遍历，如果右边的小盆友的等级高，等加一个糖果，这样保证了一个方向上高等级的糖果多。然后再从右向左遍历一遍，如果相邻两个左边的等级高，而左边的糖果又少的话，则左边糖果数为右边糖果数加一。最后再把所有小盆友的糖果数都加起来返回即可。代码如下：



解法一：

```
class Solution {
public:
    int candy(vector<int>& ratings) {
        int res = 0, n = ratings.size();
        vector<int> nums(n, 1);
        for (int i = 0; i < n - 1; ++i) {
            if (ratings[i + 1] > ratings[i]) nums[i + 1] = nums[i] + 1;
        }
        for (int i = n - 1; i > 0; --i) {
            if (ratings[i - 1] > ratings[i]) nums[i - 1] = max(nums[i - 1], nums[i] + 1);
        }
        for (int num : nums) res += num;
        return res;
    }
};
```



下面来看一次遍历的方法，相比于遍历两次的思路简单明了，这种只遍历一次的解法就稍有些复杂了。首先我们给第一个同学一个糖果，那么对于接下来的一个同学就有三种情况：

1. 接下来的同学的rating等于前一个同学，那么给接下来的同学一个糖果就行。

2. 接下来的同学的rating大于前一个同学，那么给接下来的同学的糖果数要比前一个同学糖果数加1。

3.接下来的同学的rating小于前一个同学，那么我们此时不知道应该给这个同学多少个糖果，需要看后面的情况。

对于第三种情况，我们不确定要给几个，因为要是只给1个的话，那么有可能接下来还有rating更小的同学，总不能一个都不给吧。也不能直接给前一个同学的糖果数减1，有可能给多了，因为如果后面再没人了的话，其实只要给一个就行了。还有就是，如果后面好几个rating越来越小的同学，那么前一个同学的糖果数可能还得追加，以保证最后面的同学至少能有1个糖果。来一个例子吧，四个同学，他们的rating如下：

1 3 2 1

先给第一个rating为1的同学一个糖果，然后从第二个同学开始遍历，第二个同学rating为3，比1大，所以多给一个糖果，第二个同学得到两个糖果。下面第三个同学，他的rating为2，比前一个同学的rating小，如果我们此时给1个糖果的话，那么rating更小的第四个同学就得不到糖果了，所以我们要给第四个同学1个糖果，而给第三个同学2个糖果，此时要给第二个同学追加1个糖果，使其能够比第三个同学的糖果数多至少一个。那么我们就需要统计出多有个连着的同学的rating变小，用变量cnt来记录，找出了最后一个减小的同学，那么就可以往前推，每往前一个加一个糖果，这就是个等差数列，我们可以直接利用求和公式算出这些rating减小的同学的糖果之和。然后我们还要看第一个开始减小的同学的前一个同学需不需要追加糖果，只要比较cnt和pre的大小，pre是之前同学得到的最大糖果数，二者做差加1就是需要追加的糖果数，加到结果res中即可，参见代码如下：



解法二：

```
class Solution {
public:
    int candy(vector<int>& ratings) {
        if (ratings.empty()) return 0;
        int res = 1, pre = 1, cnt = 0;
        for (int i = 1; i < ratings.size(); ++i) {
            if (ratings[i] >= ratings[i - 1]) {
                if (cnt > 0) {
                    res += cnt * (cnt + 1) / 2;
                    if (cnt >= pre) res += cnt - pre + 1;
                    cnt = 0;
                    pre = 1;
                }
                pre = (ratings[i] == ratings[i - 1]) ? 1 : pre + 1;
                res += pre;
            } else {
                ++cnt;
            }
        }     
        if (cnt > 0) {
            res += cnt * (cnt + 1) / 2;
            if (cnt >= pre) res += cnt - pre + 1;
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/5243/a-simple-solution](https://discuss.leetcode.com/topic/5243/a-simple-solution)

[https://discuss.leetcode.com/topic/8208/one-pass-constant-space-java-solution](https://discuss.leetcode.com/topic/8208/one-pass-constant-space-java-solution)

[https://discuss.leetcode.com/topic/17722/two-c-solutions-given-with-explanation-both-with-o-n-time-one-with-o-1-space-the-other-with-o-n-space](https://discuss.leetcode.com/topic/17722/two-c-solutions-given-with-explanation-both-with-o-n-time-one-with-o-1-space-the-other-with-o-n-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Split Array with Equal Sum 分割数组成和相同的子数组 - Grandyang - 博客园







# [[LeetCode] Split Array with Equal Sum 分割数组成和相同的子数组](https://www.cnblogs.com/grandyang/p/6854492.html)







Given an array with n integers, you need to find if there are triplets (i, j, k) which satisfies following conditions:
- 0 < i, i + 1 < j, j + 1 < k < n - 1
- Sum of subarrays (0, i - 1), (i + 1, j - 1), (j + 1, k - 1) and (k + 1, n - 1) should be equal.

where we define that subarray (L, R) represents a slice of the original array starting from the element indexed L to the element indexed R.

**Example:**

**Input:** [1,2,1,2,1,2,1]
**Output:** True
**Explanation:**
i = 1, j = 3, k = 5. 
sum(0, i - 1) = sum(0, 0) = 1
sum(i + 1, j - 1) = sum(2, 2) = 1
sum(j + 1, k - 1) = sum(4, 4) = 1
sum(k + 1, n - 1) = sum(6, 6) = 1

**Note:**
- 1 <= n <= 2000.
- Elements in the given array will be in range [-1,000,000, 1,000,000].



这道题给了我们一个数组，让我们找出三个位置，使得数组被分为四段，使得每段之和相等，问存不存在这样的三个位置，注意三个位置上的数字不属于任何一段。刚开始博主觉得这题貌似跟之前那道 [Partition Equal Subset Sum](http://www.cnblogs.com/grandyang/p/5951422.html) 很像，所以在想能不能用DP来做，可是想了半天不知道DP该如何定义，更别说推导状态转移方程了。于是就尝试了建立累加和数组，并搜索所有的可能组合，进行暴力破解，结果却TLE了。说明OJ不接受时间复杂度为三次方的解法，那么就要想办法来优化了，博主只好上网学习大神们的解法，发现大神们的解法果然巧妙，只是改变了一个查找顺序，就轻易的将时间复杂度降到了平方级，碉堡了有木有。思路是这样的，因为我们需要找三个位置i，j，k，如果我们按正常的顺序来暴力搜索，那么就会遍历所有的情况，其实大部分的情况都是不符合题意的，会有大量的无用的运算。而如果我们换一个角度，先搜索j的位置，那么i和k的位置就可以固定在一个小的范围内了，而且可以在j的循环里面同时进行，这样就少嵌套了一个循环，所以时间复杂度会降一维度。确定j的范围应该左右各留3个数字，因为四段均不能为空，而且分割位上的数字不能算入四段。再确定了j的位置后，i和k的位置就能分别确定了，我们要做的是先遍历i的所有可能位置，然后遍历所有的拆分情况，如果拆出的两段和相等，则把这个相等的值加入一个集合中，然后再遍历k的所有情况，同样遍历所有的拆分情况，如果拆出两段和相等，再看这个相等的和是否在集合中，如果存在，说明拆出的四段和都可以相同，那么返回true即可，否则当遍历结束了，返回false。唉，为啥自己就想不到呢，估计这就是和大神的差距吧，泪目中。。



解法一：


```
class Solution {
public:
    bool splitArray(vector<int>& nums) {
        if (nums.size() < 7) return false;
        int n = nums.size();
        vector<int> sums = nums;
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        for (int j = 3; j < n - 3; ++j) {
            unordered_set<int> s;
            for (int i = 1; i < j - 1; ++i) {
                if (sums[i - 1] == (sums[j - 1] - sums[i])) {
                    s.insert(sums[i - 1]);
                }
            }
            for (int k = j + 1; k < n - 1; ++k) {
                int s3 = sums[k - 1] - sums[j], s4 = sums[n - 1] - sums[k];
                if (s3 == s4 && s.count(s3)) return true;
            }
        }
        return false;
    }
};
```



下面这种解法是递归的暴力破解写法，刚开始博主还纳闷了，为啥博主之前写的迭代形式的暴力破解过不了OJ，而这个递归版本的确能通过呢，仔细研究了一下，发现这种解法有两个地方做了优化。第一个优化是在for循环里面，如果i不等于1，且当前数字和之前数字均为0，那么跳过这个位置，因为加上0也不会对target有任何影响，那为什么要加上i不等于1的判断呢，因为输入数组如果是七个0，那么实际上应该返回true的，而如果没有i != 1这个条件限制，后面的代码均不会得到执行，那么就直接返回false了，是不对的。第二个优化的地方是在递归函数里面，只有当curSum等于target了，才进一步调用递归函数，这样就相当于做了剪枝处理，减少了大量的不必要的运算，这可能就是其可以通过OJ的原因吧。

再来说下子函数for循环的终止条件 i < n - 5 + 2*cnt 是怎么得来的，是的，这块的确是个优化，因为i的位置是题目中三个分割点i，j，k的位置，所以其分别有自己可以取值的范围，由于只有三个分割点，所以cnt的取值可以是0，1，2。
-> 当cnt=0时，说明是第一个分割点，那么i < n - 5，表示后面必须最少要留5个数字，因为分割点本身的数字不记入子数组之和，那么所留的五个数字为：数字，第二个分割点，数字，第三个分割点，数字。
-> 当cnt=1时，说明是第二个分割点，那么i < n - 3，表示后面必须最少要留3个数字，因为分割点本身的数字不记入子数组之和，那么所留的三个数字为：数字，第三个分割点，数字。
-> 当cnt=2时，说明是第三个分割点，那么i < n - 1，表示后面必须最少要留1个数字，因为分割点本身的数字不记入子数组之和，那么所留的一个数字为：数字。



解法二：

```
class Solution {
public:
    bool splitArray(vector<int>& nums) {
        if (nums.size() < 7) return false;
        int n = nums.size(), target = 0;
        int sum = accumulate(nums.begin(), nums.end(), 0);
        for (int i = 1; i < n - 5; ++i) {
            if (i != 1 && nums[i] == 0 && nums[i - 1] == 0) continue;
            target += nums[i - 1];
            if (helper(nums, target, sum - target - nums[i], i + 1, 1)) {
                return true;
            }
        }
        return false;
    }
    bool helper(vector<int>& nums, int target, int sum, int start, int cnt) {
        if (cnt == 3) return sum == target;
        int curSum = 0, n = nums.size();
        for (int i = start + 1; i < n - 5 + 2 * cnt; ++i) {
            curSum += nums[i - 1];
            if (curSum == target && helper(nums, target, sum - curSum - nums[i], i + 1, cnt + 1)) {
                return true;
            }
        }
        return false;
    }
};
```



基于上面递归的优化方法的启发，博主将两个优化方法加到了之前写的迭代的暴力破解解法上，就能通过OJ了，perfect!



解法三：

```
class Solution {
public:
    bool splitArray(vector<int>& nums) {
        int n = nums.size();
        vector<int> sums = nums;
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        for (int i = 1; i <= n - 5; ++i) {
            if (i != 1 && nums[i] == 0 && nums[i - 1] == 0) continue;
            for (int j = i + 2; j <= n - 3; ++j) {
                if (sums[i - 1] != (sums[j - 1] - sums[i])) continue;
                for (int k = j + 2; k <= n - 1; ++k) {
                    int sum3 = sums[k - 1] - sums[j];
                    int sum4 = sums[n - 1] - sums[k];
                    if (sum3 == sum4 && sum3 == sums[i - 1]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/problems/split-array-with-equal-sum/](https://leetcode.com/problems/split-array-with-equal-sum/)

[https://leetcode.com/problems/split-array-with-equal-sum/discuss/101484/java-solution-dfs](https://leetcode.com/problems/split-array-with-equal-sum/discuss/101484/java-solution-dfs)

[https://leetcode.com/problems/split-array-with-equal-sum/discuss/101481/simple-java-solution-on2](https://leetcode.com/problems/split-array-with-equal-sum/discuss/101481/simple-java-solution-on2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














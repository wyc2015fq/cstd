# [LeetCode] Maximum Average Subarray II 子数组的最大平均值之二 - Grandyang - 博客园







# [[LeetCode] Maximum Average Subarray II 子数组的最大平均值之二](https://www.cnblogs.com/grandyang/p/8021421.html)







Given an array consisting of `n` integers, find the contiguous subarray whose length is greater than or equal to `k` that has the maximum average value. And you need to output the maximum average value.

Example 1:
Input: [1,12,-5,-6,50,3], k = 4
Output: 12.75
Explanation:
when length is 5, maximum average value is 10.8,
when length is 6, maximum average value is 9.16667.
Thus return 12.75.



Note:
- 1 <= `k` <= `n` <= 10,000.
- Elements of the given array will be in range [-10,000, 10,000].
- The answer with the calculation error less than 10-5 will be accepted.



这道题是之前那道[Maximum Average Subarray I](http://www.cnblogs.com/grandyang/p/7294585.html)的拓展，那道题说是要找长度为k的子数组的最大平均值，而这道题要找长度大于等于k的子数组的最大平均值。加了个大于k的条件，那么情况就复杂很多了，之前只要遍历所有长度为k的子数组就行了，现在还要包括所有长度大于k的子数组。我们首先来看brute force的方法，就是遍历所有的长度大于等于k的子数组，并计算平均值并更新结果res。那么我们先建立累加和数组sums，结果res初始化为前k个数字的平均值，然后让i从k+1个数字开始遍历，那么此时的sums[i]就是前k+1个数组组成的子数组之和，我们用其平均数来更新结果res，然后要做的就是从开头开始去掉数字，直到子数组剩余k个数字为止，然后用其平均值来更新解结果res，通过这种方法，我们就遍历了所有长度大于等于k的子数组。这里需要注意的一点是，更新结果res的步骤不能写成res = min(res, t / (i + 1)) 这种形式，会TLE，必须要在if中判断 t > res * (i + 1) 才能accept，写成t / (i + 1) > res 也不行，必须要用乘法，这也说明了计算机不喜欢算除法吧，参见代码如下：



解法一：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> sums = nums;
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        double res = (double)sums[k - 1] / k;
        for (int i = k; i < n; ++i) {
            double t = sums[i];
            if (t > res * (i + 1)) res = t / (i + 1);
            for (int j = i - k; j >= 0; --j) {
                t = sums[i] -  sums[j];
                if (t > res * (i - j)) res = t / (i - j);
            }
        }
        return res;
    }
};
```



我们再来看一种O(n2)时间复杂度的方法，这里对上面的解法进行了空间上的优化，并没有长度为n数组，而是使用了preSum和sum两个变量来代替，preSum初始化为前k个数字之和，sum初始化为preSum，结果res初始化为前k个数字的平均值，然后从第k+1个数字开始遍历，首先preSum加上这个数字，sum更新为preSum，然后此时用当前k+1个数字的平均值来更新结果res。和上面的方法一样，我们还是要从开头开始去掉数字，直到子数组剩余k个数字为止，然后用其平均值来更新解结果res，那么每次就用sum减去nums[j]，就可以不断的缩小子数组的长度了，用当前平均值更新结果res，注意还是要用乘法来判断大小，参见代码如下：



解法二：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double preSum = accumulate(nums.begin(), nums.begin() + k, 0);
        double sum = preSum, res = preSum / k;
        for (int i = k; i < nums.size(); ++i) {
            preSum += nums[i];
            sum = preSum;
            if (sum > res * (i + 1)) res = sum / (i + 1);
            for (int j = 0; j <= i - k; ++j) {
                sum -= nums[j];
                if (sum > res * (i - j)) res = sum / (i - j);
            }
        }
        return res;
    }
};
```



下面来看一种优化时间复杂度到O(nlg(max - min))的解法，其中max和min分别是数组中的最大值和最小值，是利用了二分搜索法，博主之前写了一篇[LeetCode Binary Search Summary 二分搜索法小结](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwjtjZeBuYDYAhXikOAKHYHPAaAQFggqMAA&url=http%3A%2F%2Fwww.cnblogs.com%2Fgrandyang%2Fp%2F6854825.html&usg=AOvVaw1vzLoPulVw-9V5ExEOqnas)的博客，这里的二分法应该是小结的第四类，也是最难的那一类，因为判断折半的方向是一个子函数，这里我们没有用子函数，而是写到了一起，可以抽出来成为一个子函数，这一类的特点就是不再是简单的大小比较，而是需要一些复杂的操作来确定折半方向。这里主要借鉴了[蔡文森特大神的帖子](https://discuss.leetcode.com/topic/96228/c-clean-binary-search-solution-with-explanation)，所求的最大平均值一定是介于原数组的最大值和最小值之间，所以我们的目标是用二分法来快速的在这个范围内找到我们要求的最大平均值，初始化left为原数组的最小值，right为原数组的最大值，然后mid就是left和right的中间值，难点就在于如何得到mid和要求的最大平均值之间的大小关系，从而判断折半方向。我们想，如果我们已经算出来了这个最大平均值maxAvg，那么对于任意一个长度大于等于k的数组，如果让每个数字都减去maxAvg，那么得到的累加差值一定是小于等于0的，这个不难理解，比如下面这个数组：

[1, 2, 3, 4]   k = 2

我们一眼就可以看出来最大平均值maxAvg = 3.5，所以任何一个长度大于等于2的子数组每个数字都减去maxAvg的差值累加起来都小于等于0，只有产生这个最大平均值的子数组[3, 4]，算出来才正好等于0，其他都是小于0的。那么我们可以根据这个特点来确定折半方向，我们通过left和right值算出来的mid，可以看作是maxAvg的一个candidate，所以我们就让数组中的每一个数字都减去mid，然后算差值的累加和，一旦发现累加和大于0了，那么说明我们mid比maxAvg小，这样就可以判断方向了。

我们建立一个累加和数组sums，然后求出原数组中最小值赋给left，最大值赋给right，题目中说了误差是1e-5，所以我们的循环条件就是right比left大1e-5，然后我们算出来mid，定义一个minSum初始化为0，布尔型变量check，初始化为false。然后开始遍历数组，先更新累加和数组sums，注意这个累加和数组不是原始数字的累加，而是它们和mid相减的差值累加。我们的目标是找长度大于等于k的子数组的平均值大于mid，由于我们每个数组都减去了mid，那么就转换为找长度大于等于k的子数组的差累积值大于0。我们建立差值累加数组的意义就在于通过sums[i] - sums[j]来快速算出j和i位置中间数字之和，那么我们只要j和i中间正好差k个数字即可，然后minSum就是用来保存j位置之前的子数组差累积的最小值，所以当i >= k时，我们用sums[i - k]来更新minSum，这里的i - k就是j的位置，然后判断如果sums[i] - minSum > 0了，说明我们找到了一段长度大于等k的子数组平均值大于mid了，就可以更新left为mid了，我们标记check为true，并退出循环。在for循环外面，当check为true的时候，left更新为mid，否则right更新为mid，参见代码如下：



解法三：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        vector<double> sums(n + 1, 0);
        double left = *min_element(nums.begin(), nums.end());
        double right = *max_element(nums.begin(), nums.end());
        while (right - left > 1e-5) {
            double minSum = 0, mid = left + (right - left) / 2;
            bool check = false;
            for (int i = 1; i <= n; ++i) {
                sums[i] = sums[i - 1] + nums[i - 1] - mid;
                if (i >= k) {
                    minSum = min(minSum, sums[i - k]);
                }
                if (i >= k && sums[i] > minSum) {check = true; break;} 
            }
            if (check) left = mid;
            else right = mid;
        }
        return left;
    }
};
```



下面这种解法对上面的方法优化了空间复杂度 ，使用preSum和sum来代替数组，思路和上面完全一样，可以参加上面的讲解，注意这里我们的第二个if中是判断i >= k - 1，而上面的方法是判断i >= k，这是因为上面的sums数组初始化了n + 1个元素，注意坐标的转换，而第一个if中i >= k不变是因为j和i之间就差了k个，所以不需要考虑坐标的转换，参见代码如下：



解法四：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double left = *min_element(nums.begin(), nums.end());
        double right = *max_element(nums.begin(), nums.end());
        while (right - left > 1e-5) {
            double minSum = 0, sum = 0, preSum = 0, mid = left + (right - left) / 2;
            bool check = false;
            for (int i = 0; i < nums.size(); ++i) {
                sum += nums[i] - mid;
                if (i >= k) {
                    preSum += nums[i - k] - mid;
                    minSum = min(minSum, preSum);
                }
                if (i >= k - 1 && sum > minSum) {check = true; break;}
            }
            if (check) left = mid;
            else right = mid;
        }
        return left;
    }
};
```



类似题目：

[Maximum Average Subarray I](http://www.cnblogs.com/grandyang/p/7294585.html)



参考资料：

[https://discuss.leetcode.com/topic/96095/c-solution-o-n-2](https://discuss.leetcode.com/topic/96095/c-solution-o-n-2/2)

[https://discuss.leetcode.com/topic/96141/c-binary-search-130ms](https://discuss.leetcode.com/topic/96141/c-binary-search-130ms)

[https://discuss.leetcode.com/topic/96199/10-line-c-ac-barely-solution-o-n-2](https://discuss.leetcode.com/topic/96199/10-line-c-ac-barely-solution-o-n-2)

[https://discuss.leetcode.com/topic/96228/c-clean-binary-search-solution-with-explanation](https://discuss.leetcode.com/topic/96228/c-clean-binary-search-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













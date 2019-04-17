# [LeetCode] K-th Smallest Prime Fraction 第K小的质分数 - Grandyang - 博客园







# [[LeetCode] K-th Smallest Prime Fraction 第K小的质分数](https://www.cnblogs.com/grandyang/p/9135156.html)









A sorted list `A` contains 1, plus some number of primes.  Then, for every p < q in the list, we consider the fraction p/q.

What is the `K`-th smallest fraction considered?  Return your answer as an array of ints, where `answer[0] = p`and `answer[1] = q`.
Examples:
Input: A = [1, 2, 3, 5], K = 3
Output: [2, 5]
Explanation:
The fractions to be considered in sorted order are:
1/5, 1/3, 2/5, 1/2, 3/5, 2/3.
The third fraction is 2/5.

Input: A = [1, 7], K = 1
Output: [1, 7]

Note:
- `A` will have length between `2` and `2000`.
- Each `A[i]` will be between `1` and `30000`.
- `K` will be between `1` and `A.length * (A.length - 1) / 2`.





这道题给了我们一个有序数组，里面是1和一些质数，说是对于任意两个数，都可以组成一个 [0, 1] 之间分数，让我们求第K小的分数是什么，题目中给的例子也很好的说明了题意。那么最直接暴力的解法就是遍历出所有的分数，然后再进行排序，返回第K小的即可。但是这种无脑暴力搜索的方法OJ是不答应的，无奈，只能想其他的解法。我们想，由于数组是有序的，所以最小的分数肯定是由第一个数字和最后一个数字组成的，而接下来第二小的分数我们就不确定是由第二个数字和最后一个数字组成的，还是由第一个数字跟倒数第二个数字组成的。我们的想法是用一个最小堆来存分数，那么每次取的时候就可以将最小的分数取出来，由于前面说了，我们不能遍历所有的分数都存入最小堆，那么我们怎么办呢，我们可以先存n个，哪n个呢？其实就是数组中的每个数字都和最后一个数字组成的分数。由于我们需要取出第K小的分数，那么我们在最小堆中取K个分数就可以了，第一个取出的分数就是那个由第一个数字和最后一个数字组成的最小的分数，然后就是精髓所在了，我们此时将分母所在的位置前移一位，还是和当前的分子组成新的分数，这里即为第一个数字和倒数第二个数字组成的分数，存入最小堆中，那么由于之前我们已经将第二个数字和倒数第一个数字组成的分数存入了最小堆，所以不用担心第二小的分数不在堆中，这样每取出一个分数，我们都新加一个稍稍比取出的大一点的分数，这样我们取出了第K个分数即为所求，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& A, int K) {
        priority_queue<pair<double, pair<int, int>>> q;
        for (int i = 0; i < A.size(); ++i) {
            q.push({-1.0 * A[i] / A.back(), {i, A.size() - 1}});
        }
        while (--K) {
            auto t = q.top().second; q.pop();
            --t.second;
            q.push({-1.0 * A[t.first] / A[t.second], {t.first, t.second}});
        }
        return {A[q.top().second.first], A[q.top().second.second]};
    }
};
```



其实这道题比较经典的解法是用二分搜索法Binary Search，这道题使用的二分搜索法是博主归纳总结帖[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)中的第四种，即二分法的判定条件不是简单的大小关系，而是可以抽离出子函数的情况，下面我们来看具体怎么弄。这种高级的二分搜索法在求第K小的数的时候经常使用，比如 [Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)，[Kth Smallest Number in Multiplication Table](http://www.cnblogs.com/grandyang/p/8367505.html)，和 [Find K-th Smallest Pair Distance](http://www.cnblogs.com/grandyang/p/8627783.html) 等。思路都是用mid当作candidate，然后统计小于mid的个数cnt，和K进行比较，从而确定折半的方向。这道题也是如此，mid为候选的分数值，刚开始时是0.5，然后我们需要统计出不大于mid的分数都个数cnt，同时也需要找出最接近mid的分数，当作返回的候选值，因为一旦cnt等于K了，直接将这个候选值返回即可，否则如果cnt小于K，说明我们应该增大一些mid，将left赋值为mid，反之如果cnt大于K，我们需要减小mid，将right赋值为mid，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& A, int K) {
        double left = 0, right = 1;
        int p = 0, q = 1, cnt = 0, n = A.size();
        while (true) {
            double mid = left + (right - left) / 2.0;
            cnt = 0; p = 0;
            for (int i = 0, j = 0; i < n; ++i) {
                while (j < n && A[i] > mid * A[j]) ++j;
                cnt += n - j;
                if (j < n && p * A[j] < q * A[i]) {
                    p = A[i];
                    q = A[j];
                }
            }
            if (cnt == K) return {p, q};
            else if (cnt < K) left = mid;
            else right = mid;
        }
    }
};
```



类似题目：

[Find K Pairs with Smallest Sums](http://www.cnblogs.com/grandyang/p/5653127.html)

[Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)

[Kth Smallest Number in Multiplication Table](http://www.cnblogs.com/grandyang/p/8367505.html)

[Find K-th Smallest Pair Distance](http://www.cnblogs.com/grandyang/p/8627783.html)



参考资料：

[https://leetcode.com/problems/k-th-smallest-prime-fraction/solution/](https://leetcode.com/problems/k-th-smallest-prime-fraction/solution/)

[https://leetcode.com/problems/k-th-smallest-prime-fraction/discuss/115531/C++-9lines-priority-queue](https://leetcode.com/problems/k-th-smallest-prime-fraction/discuss/115531/C++-9lines-priority-queue)

[https://leetcode.com/problems/k-th-smallest-prime-fraction/discuss/115819/Summary-of-solutions-for-problems-%22reducible%22-to-LeetCode-378](https://leetcode.com/problems/k-th-smallest-prime-fraction/discuss/115819/Summary-of-solutions-for-problems-%22reducible%22-to-LeetCode-378)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













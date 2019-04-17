# [LeetCode] Number of Subarrays with Bounded Maximum 有界限最大值的子数组数量 - Grandyang - 博客园







# [[LeetCode] Number of Subarrays with Bounded Maximum 有界限最大值的子数组数量](https://www.cnblogs.com/grandyang/p/9237967.html)







We are given an array `A` of positive integers, and two positive integers `L` and `R` (`L <= R`).

Return the number of (contiguous, non-empty) subarrays such that the value of the maximum array element in that subarray is at least `L` and at most `R`.
Example :
Input: 
A = [2, 1, 4, 3]
L = 2
R = 3
Output: 3
Explanation: There are three subarrays that meet the requirements: [2], [2, 1], [3].

Note:
- L, R  and `A[i]` will be an integer in the range `[0, 10^9]`.
- The length of `A` will be in the range of `[1, 50000]`.



这道题给了我们一个数组，又给了我们两个数字L和R，表示一个区间范围，让我们求有多少个这样的子数组，使得其最大值在[L, R]区间的范围内。既然是求子数组的问题，那么最直接，最暴力的方法就是遍历所有的子数组，然后维护一个当前的最大值，只要这个最大值在[L, R]区间的范围内，结果res自增1即可。但是这种最原始，最粗犷的暴力搜索法，OJ不答应。但是其实我们略作优化，就可以通过了。优化的方法是，首先，如果当前数字大于R了，那么其实后面就不用再遍历了，不管当前这个数字是不是最大值，它都已经大于R了，那么最大值可能会更大，所以没有必要再继续遍历下去了。同样的剪枝也要加在内层循环中加，当curMax大于R时，直接break掉内层循环即可，参见代码如下：



解法一：

```
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
        int res = 0, n = A.size();
        for (int i = 0; i < n; ++i) {
            if (A[i] > R) continue;
            int curMax = INT_MIN;
            for (int j = i; j < n; ++j) {
                curMax = max(curMax, A[j]);
                if (curMax > R) break;
                if (curMax >= L) ++res;
            }
        }
        return res;
    }
};
```



虽然上面的方法做了剪枝后能通过OJ，但是我们能不能在线性的时间复杂度内完成呢。答案是肯定的，我们先来看一种[官方解答贴](https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/solution/)中的方法，这种方法是用一个子函数来算出最大值在[-∞, x]范围内的子数组的个数，而这种区间只需要一个循环就够了，为啥呢？我们来看数组[2, 1, 4, 3]的最大值在[-∞, 4]范围内的子数组的个数。当遍历到2时，只有一个子数组[2]，遍历到1时，有三个子数组，[2], [1], [2,1]。当遍历到4时，有六个子数组，[2], [1], [4], [2,1], [1,4], [2,1,4]。当遍历到3时，有十个子数组。其实如果长度为n的数组的最大值在范围[-∞, x]内的话，其所有子数组都是符合题意的，而长度为n的数组共有n(n+1)/2个子数组，刚好是等差数列的求和公式。所以我们在遍历数组的时候，如果当前数组小于等于x，则cur自增1，然后将cur加到结果res中；如果大于x，则cur重置为0。这样我们可以正确求出最大值在[-∞, x]范围内的子数组的个数。而要求最大值在[L, R]范围内的子数组的个数，只需要用最大值在[-∞, R]范围内的子数组的个数，减去最大值在[-∞, L-1]范围内的子数组的个数即可，参见代码如下：



解法二：

```
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
        return count(A, R) - count(A, L - 1);
    }
    int count(vector<int>& A, int bound) {
        int res = 0, cur = 0;
        for (int x : A) {
            cur = (x <= bound) ? cur + 1 : 0;
            res += cur;
        }
        return res;
    }
};
```



下面这种解法也是线性时间复杂度的，跟上面解法的原理很类似，只不过没有写子函数而已。我们使用left和right来分别标记子数组的左右边界，使得其最大值在范围[L,R]内。那么当遍历到的数字大于等于L时，right赋值为当前位置i，那么每次res加上right - left，随着right的不停自增1，每次加上的right - left，实际上也是一个等差数列，跟上面解法中的子函数本质时一样的。当A[i]大于R的时候，left = i，那么此时A[i]肯定也大于等于L，于是rihgt=i，那么right - left为0，相当于上面的cur重置为0的操作，发现本质联系了吧，参见代码如下：



解法三：

```
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
        int res = 0, left = -1, right = -1;
        for (int i = 0; i < A.size(); ++i) {
            if (A[i] > R) left = i;
            if (A[i] >= L) right = i;
            res += right - left;
        }
        return res;
    }
};
```



我们可以对上面的解法稍稍做下优化，在A[i] > R的时候，left和right都赋值为i，然后continue，这样省去了后面的用0来更新结果res的步骤，能提高一些运行效率，参见代码如下：



解法四：

```
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
        int res = 0, left = -1, right = -1;
        for (int i = 0; i < A.size(); ++i) {
            if (A[i] > R) {
                left = right = i;
                continue;
            }
            if (A[i] >= L) right = i;
            res += right - left;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/solution/](https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/solution/)

[https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/117585/Java-9-liner](https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/117585/Java-9-liner)

[https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/117616/C++-O(n)-less10-lines](https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/117616/C++-O(n)-less10-lines)

[https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/119162/Clean-and-simple-O(n)-Java](https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/discuss/119162/Clean-and-simple-O(n)-Java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













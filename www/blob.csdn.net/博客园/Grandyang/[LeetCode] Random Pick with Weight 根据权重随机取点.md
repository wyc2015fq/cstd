# [LeetCode] Random Pick with Weight 根据权重随机取点 - Grandyang - 博客园







# [[LeetCode] Random Pick with Weight 根据权重随机取点](https://www.cnblogs.com/grandyang/p/9784690.html)







Given an array `w` of positive integers, where `w[i]` describes the weight of index `i`, write a function `pickIndex` which randomly picks an index in proportion to its weight.

Note:
- `1 <= w.length <= 10000`
- `1 <= w[i] <= 10^5`
- `pickIndex` will be called at most `10000` times.

Example 1:
Input: 
["Solution","pickIndex"]
[[[1]],[]]
Output: [null,0]


Example 2:
Input: 
["Solution","pickIndex","pickIndex","pickIndex","pickIndex","pickIndex"]
[[[1,3]],[],[],[],[],[]]
Output: [null,0,1,1,1,0]

Explanation of Input Syntax:

The input is two lists: the subroutines called and their arguments. `Solution`'s constructor has one argument, the array `w`. `pickIndex` has no arguments. Arguments are always wrapped with a list, even if there aren't any.



这道题给了一个权重数组，让我们根据权重来随机取点，现在的点就不是随机等概率的选取了，而是要根据权重的不同来区别选取。比如题目中例子2，权重为 [1, 3]，表示有两个点，权重分别为1和3，那么就是说一个点的出现概率是四分之一，另一个出现的概率是四分之三。由于我们的rand()函数是等概率的随机，那么我们如何才能有权重的随机呢，我们可以使用一个trick，由于权重是1和3，相加为4，那么我们现在假设有4个点，然后随机等概率取一个点，随机到第一个点后就表示原来的第一个点，随机到后三个点就表示原来的第二个点，这样就可以保证有权重的随机啦。那么我们就可以建立权重数组的累加和数组，比如若权重数组为 [1, 3, 2] 的话，那么累加和数组为 [1, 4, 6]，整个的权重和为6，我们 rand() % 6，可以随机出范围 [0, 5] 内的数，随机到 0 则为第一个点，随机到 1，2，3 则为第二个点，随机到 4，5 则为第三个点，所以我们随机出一个数字x后，然后再累加和数组中查找第一个大于随机数x的数字，使用二分查找法可以找到第一个大于随机数x的数字的坐标，即为所求，参见代码如下：



解法一：

```
class Solution {
public:
    Solution(vector<int> w) {
        sum = w;
        for (int i = 1; i < w.size(); ++i) {
            sum[i] = sum[i - 1] + w[i];
        }
    }
    
    int pickIndex() {
        int x = rand() % sum.back(), left = 0, right = sum.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (sum[mid] <= x) left = mid + 1;
            else right = mid;
        }
        return right;
    }
    
private:
    vector<int> sum;
};
```



我们也可以把二分查找法换为STL内置的upper_bound函数，根据上面的分析，我们随机出一个数字x后，然后再累加和数组中查找第一个大于随机数x的数字，使用二分查找法可以找到第一个大于随机数x的数字的坐标。而upper_bound() 函数刚好就是查找第一个大于目标值的数，lower_bound() 函数是找到第一个不小于目标值的数，用在这里就不太合适了，关于二分搜索发的分类可以参见博主之前的博客[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)，参见代码如下：



解法二：

```
class Solution {
public:
    Solution(vector<int> w) {
        sum = w;
        for (int i = 1; i < w.size(); ++i) {
            sum[i] = sum[i - 1] + w[i];
        }
    }
    
    int pickIndex() {
        int x = rand() % sum.back();
        return upper_bound(sum.begin(), sum.end(), x) - sum.begin();
    }
    
private:
    vector<int> sum;
};
```



讨论：这题有个很好的follow up，就是说当weights数组经常变换怎么办，那么这就有涉及到求变化的区域和问题了，在博主之前的一道 [Range Sum Query - Mutable](http://www.cnblogs.com/grandyang/p/4985506.html) 中有各种方法详细的讲解。只要把两道题的解法的精髓融合到一起就行了，可以参见论坛上的[这个帖子](https://leetcode.com/problems/random-pick-with-weight/discuss/182620/Follow-up%3A-what-if-we-can-change-weights-array)。



类似题目：

[Random Pick Index](http://www.cnblogs.com/grandyang/p/5875509.html)

[Random Pick with Blacklist](https://www.cnblogs.com/grandyang/p/10029772.html)

[Random Point in Non-overlapping Rectangles](https://www.cnblogs.com/grandyang/p/9752145.html)



参考资料：

[https://leetcode.com/problems/random-pick-with-weight/discuss/154024/JAVA-8-lines-TreeMap](https://leetcode.com/problems/random-pick-with-weight/discuss/154024/JAVA-8-lines-TreeMap)

[https://leetcode.com/problems/random-pick-with-weight/discuss/154772/C%2B%2B-concise-binary-search-solution](https://leetcode.com/problems/random-pick-with-weight/discuss/154772/C%2B%2B-concise-binary-search-solution)

[https://leetcode.com/problems/random-pick-with-weight/discuss/154044/Java-accumulated-freq-sum-and-binary-search](https://leetcode.com/problems/random-pick-with-weight/discuss/154044/Java-accumulated-freq-sum-and-binary-search)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Set Intersection Size At Least Two 设置交集大小至少为2 - Grandyang - 博客园







# [[LeetCode] Set Intersection Size At Least Two 设置交集大小至少为2](https://www.cnblogs.com/grandyang/p/8503476.html)







An integer interval `[a, b]` (for integers `a < b`) is a set of all consecutive integers from `a` to `b`, including `a` and `b`.

Find the minimum size of a set S such that for every integer interval A in `intervals`, the intersection of S with A has size at least 2.

Example 1:
Input: intervals = [[1, 3], [1, 4], [2, 5], [3, 5]]
Output: 3
Explanation:
Consider the set S = {2, 3, 4}.  For each interval, there are at least 2 elements from S in the interval.
Also, there isn't a smaller size set that fulfills the above condition.
Thus, we output the size of this set, which is 3.



Example 2:
Input: intervals = [[1, 2], [2, 3], [2, 4], [4, 5]]
Output: 5
Explanation:
An example of a minimum sized set is {1, 2, 3, 4, 5}.



Note:
- `intervals` will have length in range `[1, 3000]`.
- `intervals[i]` will have length `2`, representing some integer interval.
- `intervals[i][j]` will be an integer in `[0, 10^8]`.



这道题给了我们一些区间，让我们求一个集合S，使得S和每个区间的交集至少为2，即至少有两个相同的数字。博主最开始分析题目中的例子的时候，以为要求的集合S也必须是一个连续的区间，其实不需要的，离散的数字就可以了。比如如果区间是[1,3], [5,6]的话，那么返回的集合长度是4，而不是5。这道题可以是用贪婪算法来解，一般来说Hard的题目能用贪婪算法而不是DP解的是少之又少，这道题为我大Greedy算法正名了～！为了使得集合S中的数字尽可能的小，我们希望处理区间的时候从小区间开始，如果区间b完全覆盖了区间a，那么和区间a有两个相同数字的集合，一定和区间b也有两个相同数字。同样，我们不希望一会处理一个前面的区间，一会又处理一个很后面的区间，我们希望区间是有序的。那么如何给区间排序呢，是按起始位置排，还是按结束位置排，这里我们按结束位置从小往大排，当两个结束位置相同时，起始位置大的排前面先处理，这也符合我们先处理小区间的原则。那么遍历区间的时候，当前区间就和我们维护的集合S有三种情况：

1. 二者完全没有交集，这时候我们就需要从当前区间中取出两个数字加入集合S，取哪两个数呢？为了尽可能少使用数字，我们取当前区间中的最大两个数字，因为我们区间位置不断变大，所以取大的数字有更高的概率能和后面的区间有交集。

2. 二者有一个数字的交集，那么这个交集数字一定是区间的起始位置，那么我们需要从当前区间中再取一个数字加入集合S，根据上面的分析，我们取最大的那个数，即区间的结束位置。

3. 二者有两个及两个以上数字的交集，那么不用做任何处理。

好，分析到这里，代码也就不难写出来了，我们用个数组v来表示集合S，初始化放两个-1进去，因为题目中说明了区间都是大于0的，所以我们放这两个数组进去是为了防止越界的，不会有任何影响，最后统计长度的时候减去这个两个数字就可以了。先给区间排序，然后遍历每个区间，如果区间的起始位置小于等于数组的倒数第二个数字，说明此时已经有两个相同的数字了，直接跳过当前区间。否则如果区间的起始位置大于数组的最后一个位置，说明二者没有任何交集，我们此时先把区间的倒数第二小的数字加入数组v中。然后统一再把区间的结束位置加入数组v中，因为不管区间和数组有一个交集还是没有任何交集，结束位置都要加入数组中，所以不用放在if..else..中。最后循环结束，返回数组的大小减2，参见代码如下：



解法一：

```
class Solution {
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        vector<int> v{-1, -1};
        sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b){
            return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]);
        });
        for (auto &interval : intervals) {
            int len = v.size();
            if (interval[0] <= v[len - 2]) continue;
            if (interval[0] > v.back()) v.push_back(interval[1] - 1);
            v.push_back(interval[1]);
        }
        return v.size() - 2;
    }
};
```



我们可以对空间复杂度进行优化，我们不用保存整个集合S，因为结果只让我们返回长度即可，所以我们用两个变量p1和p2，其中p1表示集合S中倒数第二大的数字，p2为集合S中最大的数字。我们的整个逻辑跟上面的解法是相同的。遍历区间的时候，如果区间的起始位置小于等于p1，则跳过当前区间。否则如果起始位置大于p2，说明没有交集，需要加上两个数字，结果res自增2，然后p2赋值为当前区间的结束位置，p1赋值为第二大的数字。否则说明只有一个交集，结果res自增1，然后p1赋值为p2，p2赋值为当前区间的结束位置即可，参见代码如下：



解法二：

```
class Solution {
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        int res = 0, p1 = -1, p2 = -1;
        sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b){
            return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]);
        });
        for (auto &interval : intervals) {
            if (interval[0] <= p1) continue;
            if (interval[0] > p2) {
                res += 2;
                p2 = interval[1];
                p1 = p2 - 1;
            } else {
                ++res;
                p1 = p2;
                p2 = interval[1];
            }
        }
        return res;
    }
};
```



讨论：这道题的一个拓展就是一般化，改为交集大小至少为k，那么我们该怎么做呢？其实也不是很难，我们可以在解法一的基础上进行修改，我们还是用数组v来表示集合S，只不过我们初始化加入k个-1。然后还是要给区间排序，之后进行遍历，如果起始位置小于等于倒数第k个数，跳过当前区间。然后就是重点部分了，我们还是用起始位置跟数组v的最后面的数字比较，总共比到倒数第k-1个数就行了，因为小于等于倒数第k个数已经跳过了。如果大于最后一个数，则将区间后k个数加入数组；否则如果大于倒数第二个数，则将区间后k-1个数加入数组；否则如果大于倒数第三个数，则将数组后k-2个数加入数组，以此类推，直到比完倒数第k-1个数就行了，最后返回的是数组v的长度减去k。



参考资料：

[https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/117545/C++-sort-and-greedy-10-lines](https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/117545/C++-sort-and-greedy-10-lines)

[https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/113089/C++-concise-solution-O(nlogn)-greedy-39-ms](https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/113089/C++-concise-solution-O(nlogn)-greedy-39-ms)

[https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/113085/Ever-wonder-why-the-greedy-algorithm-works-Here-is-the-explanation!](https://leetcode.com/problems/set-intersection-size-at-least-two/discuss/113085/Ever-wonder-why-the-greedy-algorithm-works-Here-is-the-explanation!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













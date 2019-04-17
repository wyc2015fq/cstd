# [LeetCode] Smallest Range 最小的范围 - Grandyang - 博客园







# [[LeetCode] Smallest Range 最小的范围](https://www.cnblogs.com/grandyang/p/7200016.html)







You have `k` lists of sorted integers in ascending order. Find the smallest range that includes at least one number from each of the `k` lists.

We define the range [a,b] is smaller than range [c,d] if `b-a < d-c` or `a < c` if `b-a == d-c`.

Example 1:
Input:[[4,10,15,24,26], [0,9,12,20], [5,18,22,30]]
Output: [20,24]
Explanation: 
List 1: [4, 10, 15, 24,26], 24 is in range [20,24].
List 2: [0, 9, 12, 20], 20 is in range [20,24].
List 3: [5, 18, 22, 30], 22 is in range [20,24].



Note:
- The given list may contain duplicates, so ascending order means >= here.
- 1 <= `k` <= 3500
- -105 <= `value of elements` <= 105.
- For Java users, please note that the input type has been changed to List<List<Integer>>. And after you reset the code template, you'll see this point.



这道题给了我们一些数组，都是排好序的，让我们求一个最小的范围，使得这个范围内至少会包括每个数组中的一个数字。虽然每个数组都是有序的，但是考虑到他们之间的数字差距可能很大，所以我们最好还是合并成一个数组统一处理比较好，但是合并成一个大数组还需要保留其原属数组的序号，所以我们大数组中存pair对，同时保存数字和原数组的序号。然后我们重新按照数字大小进行排序，这样我们的问题实际上就转换成了求一个最小窗口，使其能够同时包括所有数组中的至少一个数字。这不就变成了那道[Minimum Window Substring](http://www.cnblogs.com/grandyang/p/4340948.html)。所以说啊，这些题目都是换汤不换药的，总能变成我们见过的类型。我们用两个指针left和right来确定滑动窗口的范围，我们还要用一个哈希表来建立每个数组与其数组中数字出现的个数之间的映射，变量cnt表示当前窗口中的数字覆盖了几个数组，diff为窗口的大小，我们让right向右滑动，然后判断如果right指向的数字所在数组没有被覆盖到，cnt自增1，然后哈希表中对应的数组出现次数自增1，然后我们循环判断如果cnt此时为k(数组的个数)且left不大于right，那么我们用当前窗口的范围来更新结果，然后此时我们想缩小窗口，通过将left向右移，移动之前需要减小哈希表中的映射值，因为我们去除了数字，如果此时映射值为0了，说明我们有个数组无法覆盖到了，cnt就要自减1。这样遍历后我们就能得到最小的范围了，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        vector<int> res;
        vector<pair<int, int>> v;
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            for (int num : nums[i]) {
                v.push_back({num, i});
            }
        }
        sort(v.begin(), v.end());
        int left = 0, n = v.size(), k = nums.size(), cnt = 0, diff = INT_MAX;
        for (int right = 0; right < n; ++right) {
            if (m[v[right].second] == 0) ++cnt;
            ++m[v[right].second];
            while (cnt == k && left <= right) {
                if (diff > v[right].first - v[left].first) {
                    diff = v[right].first - v[left].first;
                    res = {v[left].first, v[right].first};
                } 
                if (--m[v[left].second] == 0) --cnt;
                ++left;
            }
        }
        return res;
    }
};
```



这道题还有一种使用priority_queue来做的，优先队列默认情况是最大堆，但是这道题我们需要使用最小堆，我们可以重新写一下comparator就行了。解题的主要思路很上面的解法很相似，只是具体的数据结构的使用上略有不同，这curMax表示当前遇到的最大数字，用一个idx数组表示每个list中遍历到的位置，然后就是我们的优先队列了，里面放一个pair，是数字和其所属list组成的对儿。然后我们遍历所有的list，将每个list的首元素和该list序号组成pair放入队列中，然后idx数组中每个位置都赋值为1，因为0的位置已经放入队列了，所以指针向后移一个位置，还要更新当前最大值curMax。此时我们的queue中是每个list各有一个数字，由于是最小堆，所以最小的数字就在队首，再加上最大值curMax，就可以初始化结果res了。然后我们进行循环，注意这里循环的条件不是队列不为空，而是当某个list的数字遍历完了就结束循环，因为我们的范围要cover每个list至少一个数字。所以我们的while循环条件即是队首数字所在的list的遍历位置小于该list的总个数，在循环中，取出队首数字所在的list序号t，然后将该list中下一个位置的数字和该list序号t组成pair，加入队列中，然后用这个数字更新curMax，同时idx中t对应的位置也自增1。现在来更新结果res，如果结果res中两数之差大于curMax和队首数字之差，则我们更新结果res，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int curMax = INT_MIN, n = nums.size();
        vector<int> idx(n, 0);
        auto cmp = [](pair<int, int>& a, pair<int, int>& b) {return a.first > b.first;};
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp) > q(cmp);
        for (int i = 0; i < n; ++i) {
            q.push({nums[i][0], i});
            idx[i] = 1;
            curMax = max(curMax, nums[i][0]);
        }
        vector<int> res{q.top().first, curMax};
        while (idx[q.top().second] < nums[q.top().second].size()) {
            int t = q.top().second; q.pop();
            q.push({nums[t][idx[t]], t});
            curMax = max(curMax, nums[t][idx[t]]);
            ++idx[t];
            if (res[1] - res[0] > curMax - q.top().first) {
                res = {q.top().first, curMax};
            }
        }
        return res;
    }
};
```



类似题目：

[Minimum Window Substring](http://www.cnblogs.com/grandyang/p/4340948.html)



参考资料：

[https://discuss.leetcode.com/topic/94448/c-solution-with-priority_queue](https://discuss.leetcode.com/topic/94448/c-solution-with-priority_queue)

[https://discuss.leetcode.com/topic/96509/c-45ms-o-n-space-o-n-time-sol-without-priority-queue-iterators-just-vectors](https://discuss.leetcode.com/topic/96509/c-45ms-o-n-space-o-n-time-sol-without-priority-queue-iterators-just-vectors)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













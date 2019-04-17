# [LeetCode] Degree of an Array 数组的度 - Grandyang - 博客园







# [[LeetCode] Degree of an Array 数组的度](https://www.cnblogs.com/grandyang/p/7722949.html)







Given a non-empty array of non-negative integers `nums`, the degree of this array is defined as the maximum frequency of any one of its elements.

Your task is to find the smallest possible length of a (contiguous) subarray of `nums`, that has the same degree as `nums`.

Example 1:
Input: [1, 2, 2, 3, 1]
Output: 2
Explanation: 
The input array has a degree of 2 because both elements 1 and 2 appear twice.
Of the subarrays that have the same degree:
[1, 2, 2, 3, 1], [1, 2, 2, 3], [2, 2, 3, 1], [1, 2, 2], [2, 2, 3], [2, 2]
The shortest length is 2. So return 2.



Example 2:
Input: [1,2,2,3,1,4,2]
Output: 6



Note:
- `nums.length` will be between 1 and 50,000.
- `nums[i]` will be an integer between 0 and 49,999.



这道题给了我们一个数组，定义数组的度为某个或某些数字出现最多的次数，要我们找最短的子数组使其和原数组拥有相同的度。那么我们肯定需要统计每个数字出现的次数，就要用哈希表来建立每个数字和其出现次数之间的映射。由于我们要求包含原度的最小长度的子数组，那么最好的情况就是子数组的首位数字都是统计度的数字，即出现最多的数字。那么我们肯定要知道该数字的第一次出现的位置和最后一次出现的位置，由于我们开始不知道哪些数字会出现最多次，所以我们统计所有数字的首尾出现位置，那么我们再用一个哈希表，建立每个数字和其首尾出现的位置。我们用变量degree来表示数组的度。好，现在我们遍历原数组，累加当前数字出现的次数，当某个数字是第一次出现，那么我们用当前位置的来更新该数字出现的首尾位置，否则只更新尾位置。每遍历一个数，我们都更新一下degree。当遍历完成后，我们已经有了数组的度，还有每个数字首尾出现的位置，下面就来找出现次数为degree的数组，然后计算其首尾位置差加1就是candidate数组的长度，由于出现次数为degree的数字不一定只有一个，我们遍历所有的，找出其中最小的即可，参见代码如下：



解法一：

```
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        int n = nums.size(), res = INT_MAX, degree = 0;
        unordered_map<int, int> m;
        unordered_map<int, pair<int, int>> pos;
        for (int i = 0; i < nums.size(); ++i) {
            if (++m[nums[i]] == 1) {
                pos[nums[i]] = {i, i};
            } else {
                pos[nums[i]].second = i;
            }
            degree = max(degree, m[nums[i]]);
        }
        for (auto a : m) {
            if (degree == a.second) {
                res = min(res, pos[a.first].second - pos[a.first].first + 1);
            }
        }
        return res;
    }
};
```



下面这种方法只用了一次遍历，思路跟上面的解法很相似，还是要建立数字出现次数的哈希表，还有就是建立每个数字和其第一次出现位置之间的映射，那么我们当前遍历的位置其实可以看作是尾位置，还是可以计算子数组的长度的。我们遍历数组，累加当前数字出现的次数，如果某个数字是第一次出现，建立该数字和当前位置的映射，如果当前数字的出现次数等于degree时，当前位置为尾位置，首位置在startIdx中取的，二者做差加1来更新结果res；如果当前数字的出现次数大于degree，说明之前的结果代表的数字不是出现最多的，直接将结果res更新为当前数字的首尾差加1的长度，然后degree也更新为当前数字出现的次数。参见代码如下：



解法二：

```
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        int n = nums.size(), res = INT_MAX, degree = 0;
        unordered_map<int, int> m, startIdx;
        for (int i = 0; i < n; ++i) {
            ++m[nums[i]];
            if (!startIdx.count(nums[i])) startIdx[nums[i]] = i;
            if (m[nums[i]] == degree) {
                res = min(res, i - startIdx[nums[i]] + 1);
            } else if (m[nums[i]] > degree) {
                res = i - startIdx[nums[i]] + 1;
                degree = m[nums[i]];
            }
        }
        return res;
    }
};
```



类似题目：

[Maximum Subarray](http://www.cnblogs.com/grandyang/p/4377150.html)



参考资料：

[https://discuss.leetcode.com/topic/107097/java-o-n-time-o-n-space](https://discuss.leetcode.com/topic/107097/java-o-n-time-o-n-space)

[https://discuss.leetcode.com/topic/107216/concise-c-solution-using-hash-map-o-n-time](https://discuss.leetcode.com/topic/107216/concise-c-solution-using-hash-map-o-n-time)



[http://www.cnblogs.com/grandyang/p/5265628.html](http://www.cnblogs.com/grandyang/p/5265628.html)













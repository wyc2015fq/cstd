# [LeetCode] Longest Increasing Subsequence 最长递增子序列 - Grandyang - 博客园







# [[LeetCode] Longest Increasing Subsequence 最长递增子序列](https://www.cnblogs.com/grandyang/p/4938187.html)







Given an unsorted array of integers, find the length of longest increasing subsequence.

Example:
Input: 
```
[10,9,2,5,3,7,101,18]
```
Output: 4 
Explanation: The longest increasing subsequence is `[2,3,7,101]`, therefore the length is `4`. 
Note:
- There may be more than one LIS combination, it is only necessary for you to return the length.
- Your algorithm should run in O(*n2*) complexity.

Follow up: Could you improve it to O(*n* log *n*) time complexity?



这道题让我们求最长递增子串Longest Increasing Subsequence的长度，简称LIS的长度。我最早接触到这道题是在LintCode上，可参见我之前的博客 [Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4891500.html)，那道题写的解法略微复杂，下面我们来看其他的一些解法。首先来看一种动态规划Dynamic Programming的解法，这种解法的时间复杂度为O(n2)，类似brute force的解法，我们维护一个一维dp数组，其中dp[i]表示以nums[i]为结尾的最长递增子串的长度，对于每一个nums[i]，我们从第一个数再搜索到i，如果发现某个数小于nums[i]，我们更新dp[i]，更新方法为dp[i] = max(dp[i], dp[j] + 1)，即比较当前dp[i]的值和那个小于num[i]的数的dp值加1的大小，我们就这样不断的更新dp数组，到最后dp数组中最大的值就是我们要返回的LIS的长度，参见代码如下：



解法一：


```
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> dp(nums.size(), 1);
        int res = 0;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] > nums[j]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            res = max(res, dp[i]);
        }
        return res;
    }
};
```



下面我们来看一种优化时间复杂度到O(nlgn)的解法，这里用到了二分查找法，所以才能加快运行时间哇。思路是，我们先建立一个数组ends，把首元素放进去，然后比较之后的元素，如果遍历到的新元素比ends数组中的首元素小的话，替换首元素为此新元素，如果遍历到的新元素比ends数组中的末尾元素还大的话，将此新元素添加到ends数组末尾(注意不覆盖原末尾元素)。如果遍历到的新元素比ends数组首元素大，比尾元素小时，此时用二分查找法找到第一个不小于此新元素的位置，覆盖掉位置的原来的数字，以此类推直至遍历完整个nums数组，此时ends数组的长度就是我们要求的LIS的长度，特别注意的是ends数组的值可能不是一个真实的LIS，比如若输入数组nums为{4, 2， 4， 5， 3， 7}，那么算完后的ends数组为{2， 3， 5， 7}，可以发现它不是一个原数组的LIS，只是长度相等而已，千万要注意这点。参见代码如下：



解法二：

```
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;
        vector<int> ends{nums[0]};
        for (auto a : nums) {
            if (a < ends[0]) ends[0] = a;
            else if (a > ends.back()) ends.push_back(a);
            else {
                int left = 0, right = ends.size();
                while (left < right) {
                    int mid = left + (right - left) / 2;
                    if (ends[mid] < a) left = mid + 1;
                    else right = mid;
                }
                ends[right] = a;
            }
        }
        return ends.size();
    }
};
```



我们来看一种思路更清晰的二分查找法，跟上面那种方法很类似，思路是先建立一个空的dp数组，然后开始遍历原数组，对于每一个遍历到的数字，我们用二分查找法在dp数组找第一个不小于它的数字，如果这个数字不存在，那么直接在dp数组后面加上遍历到的数字，如果存在，则将这个数字更新为当前遍历到的数字，最后返回dp数字的长度即可，注意的是，跟上面的方法一样，特别注意的是dp数组的值可能不是一个真实的LIS。参见代码如下：



解法三：

```
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> dp;
        for (int i = 0; i < nums.size(); ++i) {
            int left = 0, right = dp.size();
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (dp[mid] < nums[i]) left = mid + 1;
                else right = mid;
            }
            if (right >= dp.size()) dp.push_back(nums[i]);
            else dp[right] = nums[i];
        }
        return dp.size();
    }
};
```



下面我们来看两种比较tricky的解法，利用到了C++中STL的lower_bound函数，lower_bound返回数组中第一个不小于指定值的元素，跟上面的算法类似，我们还需要一个一维数组v，然后对于遍历到的nums中每一个元素，找其lower_bound，如果没有lower_bound，说明新元素比一维数组的尾元素还要大，直接添加到数组v中，跟解法二的思路相同了。如果有lower_bound，说明新元素不是最大的，将其lower_bound替换为新元素，这个过程跟算法二的二分查找法的部分实现相同功能，最后也是返回数组v的长度，注意数组v也不一定是真实的LIS，参见代码如下：



解法四：

```
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> v;
        for (auto a : nums) {
            auto it = lower_bound(v.begin(), v.end(), a);
            if (it == v.end()) v.push_back(a);
            else *it = a;
        }
　　　　　 　return v.size();
    }
};
```



既然能用lower_bound，那么upper_bound就耐不住寂寞了，也要出来解个题。upper_bound是返回数组中第一个大于指定值的元素，和lower_bound的区别时，它不能返回和指定值相等的元素，那么当新进来的数和数组中尾元素一样大时，upper_bound无法返回这个元素，那么按算法三的处理方法是加到数组中，这样就不是严格的递增子串了，所以我们要做个处理，在处理每个新进来的元素时，先判断数组v中有无此元素，有的话直接跳过，这样就避免了相同数字的情况，参见代码如下：



解法五：

```
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> v;
        for (auto a : nums) {
            if (find(v.begin(), v.end(), a) != v.end()) continue;
            auto it = upper_bound(v.begin(), v.end(), a);
            if (it == v.end()) v.push_back(a);
            else *it = a;
        }
        return v.size();
    }
};
```



还有一种稍微复杂点的方法，参见我的另一篇博客 [Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4891500.html)，那是LintCode上的题，但是有点不同的是，那道题让求的LIS不是严格的递增的，允许相同元素存在。



类似题目：

[Increasing Triplet Subsequence](http://www.cnblogs.com/grandyang/p/5194599.html)

[Russian Doll Envelopes](http://www.cnblogs.com/grandyang/p/5568818.html)

[Maximum Length of Pair Chain](http://www.cnblogs.com/grandyang/p/7381633.html)

[Number of Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/7603903.html)

[Minimum ASCII Delete Sum for Two Strings](http://www.cnblogs.com/grandyang/p/7752002.html)



参考资料：

[https://leetcode.com/problems/longest-increasing-subsequence/](https://leetcode.com/problems/longest-increasing-subsequence/)

[https://leetcode.com/problems/longest-increasing-subsequence/discuss/74825/Short-Java-solution-using-DP-O(n-log-n)](https://leetcode.com/problems/longest-increasing-subsequence/discuss/74825/Short-Java-solution-using-DP-O(n-log-n))

[https://leetcode.com/problems/longest-increasing-subsequence/discuss/74848/9-lines-C%2B%2B-code-with-O(NlogN)-complexity](https://leetcode.com/problems/longest-increasing-subsequence/discuss/74848/9-lines-C%2B%2B-code-with-O(NlogN)-complexity)

[https://leetcode.com/problems/longest-increasing-subsequence/discuss/74824/JavaPython-Binary-search-O(nlogn)-time-with-explanation](https://leetcode.com/problems/longest-increasing-subsequence/discuss/74824/JavaPython-Binary-search-O(nlogn)-time-with-explanation)

[https://leetcode.com/problems/longest-increasing-subsequence/discuss/74989/C%2B%2B-Typical-DP-N2-solution-and-NLogN-solution-from-GeekForGeek](https://leetcode.com/problems/longest-increasing-subsequence/discuss/74989/C%2B%2B-Typical-DP-N2-solution-and-NLogN-solution-from-GeekForGeek)

[](http://www.cnblogs.com/lishiblog/p/4190936.html)

[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














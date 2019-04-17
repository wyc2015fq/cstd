# [LeetCode] Sliding Window Median 滑动窗口中位数 - Grandyang - 博客园







# [[LeetCode] Sliding Window Median 滑动窗口中位数](https://www.cnblogs.com/grandyang/p/6620334.html)







Median is the middle value in an ordered integer list. If the size of the list is even, there is no middle value. So the median is the mean of the two middle value.

Examples: 

`[2,3,4]` , the median is `3`

`[2,3]`, the median is `(2 + 3) / 2 = 2.5`

Given an array *nums*, there is a sliding window of size *k* which is moving from the very left of the array to the very right. You can only see the *k*
 numbers in the window. Each time the sliding window moves right by one 
position. Your job is to output the median array for each window in the 
original array.

For example,

Given *nums* = `[1,3,-1,-3,5,3,6,7]`, and *k* = 3.
Window position                Median
---------------               -----
[1  3  -1] -3  5  3  6  7       1
 1 [3  -1  -3] 5  3  6  7       -1
 1  3 [-1  -3  5] 3  6  7       -1
 1  3  -1 [-3  5  3] 6  7       3
 1  3  -1  -3 [5  3  6] 7       5
 1  3  -1  -3  5 [3  6  7]      6

Therefore, return the median sliding window as `[1,-1,-1,3,5,6]`.

**Note: **

You may assume *k* is always valid, ie: 1 ≤ k ≤ input array's size for non-empty array.



这道题给了我们一个数组，还是滑动窗口的大小，让我们求滑动窗口的中位数。我想起来之前也有一道滑动窗口的题[Sliding Window Maximum](http://www.cnblogs.com/grandyang/p/4656517.html)，于是想套用那道题的方法，可以用deque怎么也做不出，因为求中位数并不是像求最大值那样只操作deque的首尾元素。后来看到了史蒂芬大神的方法，原来是要用一个multiset集合，和一个指向最中间元素的iterator。我们首先将数组的前k个数组加入集合中，由于multiset自带排序功能，所以我们通过k/2能快速的找到指向最中间的数字的迭代器mid，如果k为奇数，那么mid指向的数字就是中位数；如果k为偶数，那么mid指向的数跟前面那个数求平均值就是中位数。当我们添加新的数字到集合中，multiset会根据新数字的大小加到正确的位置，然后我们看如果这个新加入的数字比之前的mid指向的数小，那么中位数肯定被拉低了，所以mid往前移动一个，再看如果要删掉的数小于等于mid指向的数(注意这里加等号是因为要删的数可能就是mid指向的数)，则mid向后移动一个。然后我们将滑动窗口最左边的数删掉，我们不能直接根据值来用erase来删数字，因为这样有可能删掉多个相同的数字，而是应该用lower_bound来找到第一个不小于目标值的数，通过iterator来删掉确定的一个数字，参见代码如下：



解法一：

```
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        vector<double> res;
        multiset<double> ms(nums.begin(), nums.begin() + k);
        auto mid = next(ms.begin(), k /  2);
        for (int i = k; ; ++i) {
            res.push_back((*mid + *prev(mid,  1 - k % 2)) / 2);        
            if (i == nums.size()) return res;
            ms.insert(nums[i]);
            if (nums[i] < *mid) --mid;
            if (nums[i - k] <= *mid) ++mid;
            ms.erase(ms.lower_bound(nums[i - k]));
        }
    }
};
```



上面的方法用到了很多STL内置的函数，比如next，lower_bound啥的，下面我们来看一种不使用这些函数的解法。这种解法跟[Find Median from Data Stream](http://www.cnblogs.com/grandyang/p/4896673.html)那题的解法很类似，都是维护了small和large两个堆，分别保存有序数组的左半段和右半段的数字，保持small的长度大于等于large的长度。我们开始遍历数组nums，如果i>=k，说明此时滑动窗口已经满k个了，再滑动就要删掉最左值了，我们分别在small和large中查找最左值，有的话就删掉。然后处理增加数字的情况（分两种情况：1.如果small的长度小于large的长度，再看如果large是空或者新加的数小于等于large的首元素，我们把此数加入small中。否则就把large的首元素移出并加入small中，然后把新数字加入large。2.如果small的长度大于large，再看如果新数字大于small的尾元素，那么新数字加入large中，否则就把small的尾元素移出并加入large中，把新数字加入small中）。最后我们再计算中位数并加入结果res中，根据k的奇偶性来分别处理，参见代码如下：



解法二：

```
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        vector<double> res;
        multiset<int> small, large;
        for (int i = 0; i < nums.size(); ++i) {
            if (i >= k) {
                if (small.count(nums[i - k])) small.erase(small.find(nums[i - k]));
                else if (large.count(nums[i - k])) large.erase(large.find(nums[i - k]));
            }
            if (small.size() <= large.size()) {
                if (large.empty() || nums[i] <= *large.begin()) small.insert(nums[i]);
                else {
                    small.insert(*large.begin());
                    large.erase(large.begin());
                    large.insert(nums[i]);
                }
            } else {
                if (nums[i] >= *small.rbegin()) large.insert(nums[i]);
                else {
                    large.insert(*small.rbegin());
                    small.erase(--small.end());
                    small.insert(nums[i]);
                }
            }
            if (i >= (k - 1)) {
                if (k % 2) res.push_back(*small.rbegin());
                else res.push_back(((double)*small.rbegin() + *large.begin()) / 2);
            }
        }
        return res;
    }
};
```



类似题目：

[Find Median from Data Stream](http://www.cnblogs.com/grandyang/p/4896673.html)

[Sliding Window Maximum](http://www.cnblogs.com/grandyang/p/4656517.html)



参考资料：

[https://discuss.leetcode.com/topic/74905/c-o-n-logk-using-two-std-set](https://discuss.leetcode.com/topic/74905/c-o-n-logk-using-two-std-set)

[https://discuss.leetcode.com/topic/75160/easy-to-understand-clean-java-code](https://discuss.leetcode.com/topic/75160/easy-to-understand-clean-java-code)

[https://discuss.leetcode.com/topic/74963/o-n-log-k-c-using-multiset-and-updating-middle-iterator](https://discuss.leetcode.com/topic/74963/o-n-log-k-c-using-multiset-and-updating-middle-iterator)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Create Maximum Number 创建最大数 - Grandyang - 博客园







# [[LeetCode] Create Maximum Number 创建最大数](https://www.cnblogs.com/grandyang/p/5136749.html)







Given two arrays of length `m` and `n` with digits `0-9` representing two numbers. Create the maximum number of length `k <= m + n` from digits of the two. The relative order of the digits from the same array must be preserved. Return an array of the `k` digits.

Note: You should try to optimize your time and space complexity.

Example 1:
Input:
nums1 = `[3, 4, 6, 5]`
nums2 = `[9, 1, 2, 5, 8, 3]`
k = `5`
Output:
`[9, 8, 6, 5, 3]`
Example 2:
Input:
nums1 = `[6, 7]`
nums2 = `[6, 0, 4]`
k = `5`
Output:
`[6, 7, 6, 0, 4]`
Example 3:
Input:
nums1 = `[3, 9]`
nums2 = `[8, 9]`
k = `3`
Output:
`[9, 8, 9]`
Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给了我们两个数组，里面数字是无序的，又给我们一个k值为k <= n1 + n2，然后我们从两个数组中共挑出k个数，数字之间的相对顺序不变，求能组成的最大的数。这道题的难度是Hard，博主木有想出解法，参考网上大神们的解法来做的。由于k的大小不定，所以有三种可能：

第一种是当k为0时，两个数组中都不取数。

第二种是当k不大于其中任意一个数组的长度时，这种情况下，有可能只从一个数组中取数，或者两个都取一些。

第三种情况是k大于其中任意一个数组的长度，则需要从两个数组中分别取数，至于每个数组中取几个，每种情况都要考虑到，然后每次更结果即可。

为了同时能处理这三种情况，我们假设从数组nums1中取i个数，那么就需要从nums2中取 k-i 个数。那么i的范围如何确定呢？我们从情况二中知道，假如k不大于任意一个数组的长度，那么有可能只从其中一个数组中取k个，就是说可以不从nums1中取数，所以 i 最小可以取到0。那么如果是第三种情况，假设k大于nums2的长度，就是说就算把nums2中所有的数字都取出来，都无法凑个k个，所以多余的 k-n2 个数字要从nums1中来取。所以我们只要比较 0 和 k-n2 的大小，取较大的为i的起始范围。那么i最大能到多大呢，还是要看 k 和 n1 的大小，如果 k 小于等于 n1，那么只需要取k个就行了，如果k大于n1，那么只能在nums1中取n1个，多余的要到nums2中取。

好，现在知道了分别要从两个数组中取数字的情况，那么我们肯定希望从nums1中取出的i个数字是最大的组合，同理，从nums2中取出的 k-i 个也是最大的数字组合。如何才能取出最大的组合呢？比如当前数组长度为n，需要取出k个数字，我们定义一个变量drop = n - k，表示需要丢弃的数字的个数，我们遍历数组中的数字，进行下列循环，如果此时drop为整数，且结果数组长度不为0，结果数组的尾元素小于当前遍历的元素，则去掉结果数组的尾元素，此时drop自减1，重复循环直至上述任意条件不满足为止，然后把当前元素加入结果数组中，最后我们返回结果数组中的前k个元素。

现在我们分别从nums1中取出了i个最大组合的数字，从nums2中取出了 k-i 个最大组合的数字，最后一步就是需要将两个取出的数组混合排序成一个数组，小数组中各自的数字之间的相对顺序不变。对于两个数组的混合，我们只要从两个数组开头每次取两个，把大的加入结果数组，然后删掉这个大的，然后继续取一对比较，直到两个数组都为空停止。参见代码如下：



```
class Solution {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int n1 = nums1.size(), n2 = nums2.size();
        vector<int> res;
        for (int i = max(0, k - n2); i <= min(k, n1); ++i) {
            res = max(res, mergeVector(maxVector(nums1, i), maxVector(nums2, k - i)));
        }
        return res;
    }
    vector<int> maxVector(vector<int>& nums, int k) {
        int drop = (int)nums.size() - k;
        vector<int> res;
        for (int num : nums) {
            while (drop > 0 && !res.empty() && res.back() < num) {
                res.pop_back();
                --drop;
            }
            res.push_back(num);
        }
        res.resize(k);
        return res;
    }
    vector<int> mergeVector(vector<int> nums1, vector<int> nums2) {
        vector<int> res;
        while (!nums1.empty() || !nums2.empty()) {
            vector<int> &tmp = (nums1 > nums2) ? nums1 : nums2;
            res.push_back(tmp[0]);
            tmp.erase(tmp.begin());
        }
        return res;
    }
};
```



类似题目：

[Maximum Swap](http://www.cnblogs.com/grandyang/p/7583875.html)

[Remove K Digits](http://www.cnblogs.com/grandyang/p/5883736.html)

[Remove Duplicate Letters](http://www.cnblogs.com/grandyang/p/5085379.html)



参考资料：

[https://leetcode.com/problems/create-maximum-number/](https://leetcode.com/problems/create-maximum-number/)

[https://leetcode.com/problems/create-maximum-number/discuss/77285/Share-my-greedy-solution](https://leetcode.com/problems/create-maximum-number/discuss/77285/Share-my-greedy-solution)

[https://leetcode.com/problems/create-maximum-number/discuss/77286/Short-Python-Ruby-C%2B%2B](https://leetcode.com/problems/create-maximum-number/discuss/77286/Short-Python-Ruby-C%2B%2B)

[https://leetcode.com/problems/create-maximum-number/discuss/77287/C%2B%2B-16ms-FASTEST-beats-97.](https://leetcode.com/problems/create-maximum-number/discuss/77287/C%2B%2B-16ms-FASTEST-beats-97.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













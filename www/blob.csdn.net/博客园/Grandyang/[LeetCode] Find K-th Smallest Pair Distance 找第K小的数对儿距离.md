# [LeetCode] Find K-th Smallest Pair Distance 找第K小的数对儿距离 - Grandyang - 博客园







# [[LeetCode] Find K-th Smallest Pair Distance 找第K小的数对儿距离](https://www.cnblogs.com/grandyang/p/8627783.html)







Given an integer array, return the k-th smallest distance among all the pairs. The distance of a pair (A, B) is defined as the absolute difference between A and B.

Example 1:
Input:
nums = [1,3,1]
k = 1
Output: 0 
Explanation:
Here are all the pairs:
(1,3) -> 2
(1,1) -> 0
(3,1) -> 2
Then the 1st smallest distance pair is (1,1), and its distance is 0.



Note:
- `2 <= len(nums) <= 10000`.
- `0 <= nums[i] < 1000000`.
- `1 <= k <= len(nums) * (len(nums) - 1) / 2`.



这道题给了我们一个数组，让我们找第k小的数对儿距离，数对儿距离就是任意两个数字之间的绝对值差。那么我们先来考虑最暴力的解法，是不是就是遍历任意两个数字，算出其绝对值差，然后将所有距离排序，取第k小的就行了。But，OJ摇着头说图样图森破。但是我们可以在纯暴力搜索的基础上做些优化，从而让OJ说YES。那么下面这种利用了桶排序的解法就是一种很好的优化，题目中给了数字的大小范围，不会超过一百万，所以我们就建立一百万个桶，然后还是遍历任意两个数字，将计算出的距离放到对应的桶中，这里桶不是存的具体距离，而是该距离出现的次数，桶本身的位置就是距离，所以我们才建立了一百万个桶。然后我们就可以从0开始遍历到一百万了，这样保证了我们先处理小距离，如果某个距离的出现次数大于等于k了，那么我们返回这个距离，否则就用k减去这个距离的出现次数，参见代码如下：



解法一：

```
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        int n = nums.size(), N = 1000000;
        vector<int> cnt(N, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                ++cnt[abs(nums[i] - nums[j])];
            }
        }
        for (int i = 0; i < N; ++i) {
            if (cnt[i] >= k) return i;
            k -= cnt[i];
        }
        return -1;
    }
};
```



上面的解法虽然逃脱了OJ的魔掌，但也仅仅是险过，并不高效。我们来看一种基于二分搜索的解法。这道题使用的二分搜索法是博主归纳总结帖[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)中的第四种，即二分法的判定条件不是简单的大小关系，而是可以抽离出子函数的情况，下面我们来看具体怎么弄。我们的目标是快速定位出第k小的距离，那么很适合用二分法来快速的缩小查找范围，然而最大的难点就是如何找到判定依据来折半查找，即如果确定搜索目标是在左半边还是右半边。做过[Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)和[Kth Smallest Number in Multiplication Table](http://www.cnblogs.com/grandyang/p/8367505.html)这两道题的同学应该对这种搜索方式并不陌生。核心思想是二分确定一个中间数，然后找到所有小于等于这个中间数的距离个数，用其跟k比较来确定折半的方向。具体的操作是，我们首先要给数组排序，二分搜索的起始left为0，结束位置right为最大距离，即排序后的数字最后一个元素减去首元素。然后进入while循环，算出中间值mid，此外我们还需要两个变量cnt和start，其中cnt是记录小于等于mid的距离个数，start是较小数字的位置，均初始化为0，然后我们遍历整个数组，先进行while循环，如果start未越界，并且当前数字减去start指向的数组之差大于mid，说明此时距离太大了，我们增加减数大小，通过将start右移一个，那么while循环退出后，就有i - start个距离小于等于mid，将其加入cnt中，举个栗子来说：

   1    2    3    3    5

start              i

mid = 2

如果start在位置0，i在位置3，那么以nums[i]为较大数可以产生三个（i - start）小于等于mid的距离，[1 3], [2 3], [3 3]，这样当i遍历完所有的数字后，所有小于等于mid的距离的个数就求出来了，即cnt。然后我们跟k比较，如果其小于k，那么left赋值为mid+1，反之，则right赋值为mid。最终返回right或left均可，参见代码如下：



解法二：

```
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), left = 0, right = nums.back() - nums[0];
        while (left < right) {
            int mid = left + (right - left) / 2, cnt = 0, start = 0;
            for (int i = 0; i < n; ++i) {
                while (start < n && nums[i] - nums[start] > mid) ++start;
                cnt += i - start;
            }
            if (cnt < k) left = mid + 1;
            else right = mid;
        }
        return right;
    }
};
```



类似题目：

[Find K Pairs with Smallest Sums](http://www.cnblogs.com/grandyang/p/5653127.html)

[Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)

[Find K Closest Elements](http://www.cnblogs.com/grandyang/p/7519466.html)

[Kth Smallest Number in Multiplication Table](http://www.cnblogs.com/grandyang/p/8367505.html)

K-th Smallest Prime Fraction



参考资料：

[https://leetcode.com/problems/find-k-th-smallest-pair-distance/solution/](https://leetcode.com/problems/find-k-th-smallest-pair-distance/solution/)

[https://leetcode.com/problems/find-k-th-smallest-pair-distance/discuss/109077/C++-counting-sort-O(n2)-and-binary-search-O(nlogn)](https://leetcode.com/problems/find-k-th-smallest-pair-distance/discuss/109077/C++-counting-sort-O(n2)-and-binary-search-O(nlogn))

[https://leetcode.com/problems/find-k-th-smallest-pair-distance/discuss/109082/Approach-the-problem-using-the-%22trial-and-error%22-algorithm](https://leetcode.com/problems/find-k-th-smallest-pair-distance/discuss/109082/Approach-the-problem-using-the-%22trial-and-error%22-algorithm)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













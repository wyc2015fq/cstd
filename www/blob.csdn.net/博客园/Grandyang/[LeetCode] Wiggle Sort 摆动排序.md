# [LeetCode] Wiggle Sort 摆动排序 - Grandyang - 博客园







# [[LeetCode] Wiggle Sort 摆动排序](https://www.cnblogs.com/grandyang/p/5177285.html)







Given an unsorted array nums, reorder it in-place such that nums[0] <= nums[1] >= nums[2] <= nums[3]....

For example, given nums = [3, 5, 2, 1, 6, 4], one possible answer is [1, 6, 2, 5, 3, 4].



这道题让我们求摆动排序，跟[Wiggle Sort II](http://www.cnblogs.com/grandyang/p/5139057.html)相比起来，这道题的条件宽松很多，只因为多了一个等号。由于等号的存在，当数组中有重复数字存在的情况时，也很容易满足题目的要求。这道题我们先来看一种时间复杂度为O(nlgn)的方法，思路是先给数组排个序，然后我们只要每次把第三个数和第二个数调换个位置，第五个数和第四个数调换个位置，以此类推直至数组末尾，这样我们就能完成摆动排序了，参见代码如下：



解法一：

```
// Time Complexity O(nlgn)
class Solution {
public:
    void wiggleSort(vector<int> &nums) {
        sort(nums.begin(), nums.end());
        if (nums.size() <= 2) return;
        for (int i = 2; i < nums.size(); i += 2) {
            swap(nums[i], nums[i - 1]);
        }
    }
};
```



这道题还有一种O(n)的解法，根据题目要求的nums[0] <= nums[1] >= nums[2] <= nums[3]....，我们可以总结出如下规律：

当i为奇数时，nums[i] >= nums[i - 1]

当i为偶数时，nums[i] <= nums[i - 1]

那么我们只要对每个数字，根据其奇偶性，跟其对应的条件比较，如果不符合就和前面的数交换位置即可，参见代码如下：



解法二：

```
// Time Complexity O(n)
class Solution {
public:
    void wiggleSort(vector<int> &nums) {
        if (nums.size() <= 1) return;
        for (int i = 1; i < nums.size(); ++i) {
            if ((i % 2 == 1 && nums[i] < nums[i - 1]) || (i % 2 == 0 && nums[i] > nums[i - 1])) {
                swap(nums[i], nums[i - 1]);
            }
        }
    }
};
```



类似题目：

[Wiggle Sort II](http://www.cnblogs.com/grandyang/p/5139057.html)



参考资料：

[https://segmentfault.com/a/1190000003783283](https://segmentfault.com/a/1190000003783283)

[http://www.cnblogs.com/jcliBlogger/p/4797531.html](http://www.cnblogs.com/jcliBlogger/p/4797531.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)















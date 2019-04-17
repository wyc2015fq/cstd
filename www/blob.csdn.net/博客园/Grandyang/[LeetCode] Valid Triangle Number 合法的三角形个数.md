# [LeetCode] Valid Triangle Number 合法的三角形个数 - Grandyang - 博客园







# [[LeetCode] Valid Triangle Number 合法的三角形个数](https://www.cnblogs.com/grandyang/p/7053730.html)







Given an array consists of non-negative integers, your task is to count the number of triplets chosen from the array that can make triangles if we take them as side lengths of a triangle.

Example 1:
Input: [2,2,3,4]
Output: 3
Explanation:
Valid combinations are: 
2,3,4 (using the first 2)
2,3,4 (using the second 2)
2,2,3



Note:
- The length of the given array won't exceed 1000.
- The integers in the given array are in the range of [0, 1000].



这道题给了我们一堆数字，问我们能组成多少个正确的三角形，我们初中就知道三角形的性质，任意两条边之和要大于第三边。那么问题其实就变成了找出所有这样的三个数字，使得任意两个数字之和都大于第三个数字。那么可以转变一下，三个数字中如果较小的两个数字之和大于第三个数字，那么任意两个数字之和都大于第三个数字，这很好证明，因为第三个数字是最大的，所以它加上任意一个数肯定大于另一个数。这样，我们就先要给数组排序，博主最先尝试了暴力破解法，结果TLE了(不要吐槽博主哈，博主就是喜欢霸王硬上弓～)，后来优化的方法是先确定前两个数，将这两个数之和sum作为目标值，然后用二分查找法来快速确定第一个小于目标值的数，这种情况属于博主之前的博客[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)中总结的第二类的变形，我们找到这个临界值，那么这之前一直到j的位置之间的数都满足题意，直接加起来即可，参见代码如下：



解法一：

```
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        int res = 0, n = nums.size();
        sort(nums.begin(), nums.end());
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int sum = nums[i] + nums[j], left = j + 1, right = n;
                while (left < right) {
                    int mid = left + (right - left) / 2;
                    if (nums[mid] < sum) left = mid + 1;
                    else right = mid;
                }
                res += right - 1 - j;
            }
        }
        return res;
    }
};
```



其实还有更进一步优化的方法，用的是博主之前那篇[3Sum Smaller](http://www.cnblogs.com/grandyang/p/5235086.html)里面的解法二，明明博主以前都总结过，换个题目情景就又没想到，看来博主的举一反三能力还是有所欠缺啊。没办法，只能继续刻意练习了。这种方法能将时间复杂度优化到O(n2), 感觉很叼了。思路是排序之后，从数字末尾开始往前遍历，将left指向首数字，将right之前遍历到的数字的前面一个数字，然后如果left小于right就进行循环，循环里面判断如果left指向的数加上right指向的数大于当前的数字的话，那么right到left之间的数字都可以组成三角形，这是为啥呢，相当于此时确定了i和right的位置，可以将left向右移到right的位置，中间经过的数都大于left指向的数，所以都能组成三角形，就说这思路叼不叼！加完之后，right自减一，即向左移动一位。如果left和right指向的数字之和不大于nums[i]，那么left自增1，即向右移动一位，参见代码如下：



解法二：

```
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        int res = 0, n = nums.size();
        sort(nums.begin(), nums.end());
        for (int i = n - 1; i >= 2; --i) {
            int left = 0, right = i - 1;
            while (left < right) {
                if (nums[left] + nums[right] > nums[i]) {
                    res += right - left;
                    --right;
                } else {
                    ++left;
                }
            }
        }
        return res;
    }
};
```



类似题目：

[3Sum Smaller](http://www.cnblogs.com/grandyang/p/5235086.html)



参考资料：

[https://discuss.leetcode.com/topic/92099/java-o-n-2-time-o-1-space](https://discuss.leetcode.com/topic/92099/java-o-n-2-time-o-1-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













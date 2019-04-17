# leetcode-16:3sum closest最近的三数之和 - 别说话写代码的博客 - CSDN博客





2018年12月20日 09:57:31[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19标签：[leetcode																[最近的三数之和																[3sum closest](https://so.csdn.net/so/search/s.do?q=3sum closest&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=最近的三数之和&t=blog)




题目：
|Given an array `nums` of *n* integers and an integer `target`, find three integers in `nums` such that the sum is closest to `target`. Return the sum of the three integers. You may assume that each input would have exactly one solution.**Example:**Given array nums = [-1, 2, 1, -4], and target = 1.The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).|给定一个包括 *n* 个整数的数组 `nums`和 一个目标值 `target`。找出 `nums`中的三个整数，使得它们的和与 `target` 最接近。返回这三个数的和。假定每组输入只存在唯一答案。例如，给定数组 nums = [-1，2，1，-4], 和 target = 1.与 target 最接近的三个数的和为 2. (-1 + 2 + 1 = 2).|
|----|----|

思路：和上一题类似，要保证和target的差 最小，那么定义 一个变量diff保存他们之间的差。首先排序，然后固定一个位置，剩下的两个数left从左往右，right从又往左，每确定两个数，我们求出此三数之和，然后算和给定值的差的绝对值存在newDiff中，然后和diff比较并更新diff和结果closest即可

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int closest = nums[0] + nums[1] + nums[2];
        int diff = abs(closest - target);
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size() - 2; ++i) {
            int left = i + 1, right = nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                int newDiff = abs(sum - target);
                if (diff > newDiff) {
                    diff = newDiff;
                    closest = sum;
                }
                if (sum < target) ++left;
                else --right;
            }
        }
        return closest;
    }
};
```

参考：[http://www.cnblogs.com/grandyang/p/4510984.html](http://www.cnblogs.com/grandyang/p/4510984.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)





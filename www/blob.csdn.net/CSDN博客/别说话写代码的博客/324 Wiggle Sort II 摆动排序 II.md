# 324. Wiggle Sort II 摆动排序 II - 别说话写代码的博客 - CSDN博客





2019年02月21日 17:57:20[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19标签：[Wiggle Sort II																[摆动排序 II																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=摆动排序 II&t=blog)



|Given an unsorted array `nums`, reorder it such that `nums[0] < nums[1] > nums[2] < nums[3]...`.**Example 1:****Input: **`nums = [1, 5, 1, 1, 6, 4]`**Output: **One possible answer is `[1, 4, 1, 5, 1, 6]`.**Example 2:****Input: **`nums = [1, 3, 2, 2, 3, 1]`**Output:** One possible answer is `[2, 3, 1, 3, 1, 2]`.**Note:**			You may assume all input has valid answer.**Follow Up:**			Can you do it in O(n) time and/or in-place with O(1) extra space?|给定一个无序的数组 `nums`，将它重新排列成 `nums[0] < nums[1] > nums[2] < nums[3]...` 的顺序。**示例 1:****输入: **`nums = [1, 5, 1, 1, 6, 4]`**输出: **一个可能的答案是 `[1, 4, 1, 5, 1, 6]`**示例 2:****输入: **`nums = [1, 3, 2, 2, 3, 1]`**输出:** 一个可能的答案是 `[2, 3, 1, 3, 1, 2]`**说明:**			你可以假设所有输入都会得到有效的结果。**进阶:**			你能用 O(n) 时间复杂度和 / 或原地 O(1) 额外空间来实现吗？|
|----|----|

思路：是O(nlogn)，先排序，然后l指向中间位置，r指向数组 最后位置。然后将第一个元素 置为l中间元素，然后l减一，然后放最后一个元素r，然后r-1，依次类推。就刚好满足题目 条件。比如，1,5,1,1,6,4， 排序后是1，1,1,4，5,6。l 指向第3个1，r指向6.。然后将数组逐个按规则往前排，为1,6,1,5,1,4

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        vector<int> tmp = nums;
        int n=nums.size(), l=(n+1)/2 , r=n;
        sort(tmp.begin(),tmp.end());
        for(int i=0;i<n;++i)
            nums[i] = i&1 ? tmp[--r]:tmp[--l];
    }
};
```](https://so.csdn.net/so/search/s.do?q=Wiggle Sort II&t=blog)





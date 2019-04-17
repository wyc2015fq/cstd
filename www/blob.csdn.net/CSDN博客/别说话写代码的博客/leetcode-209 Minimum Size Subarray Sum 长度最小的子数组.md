# leetcode-209. Minimum Size Subarray Sum 长度最小的子数组 - 别说话写代码的博客 - CSDN博客





2019年04月10日 22:51:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：6标签：[长度最小的子数组																[leetcode 209																[Minimum Size Subarray Sum](https://so.csdn.net/so/search/s.do?q=Minimum Size Subarray Sum&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode 209&t=blog)



|Given an array of **n** positive integers and a positive integer **s**, find the minimal length of a **contiguous** subarray of which the sum ≥ **s**. If there isn't one, return 0 instead.**Example: ****Input:**`s = 7, nums = [2,3,1,2,4,3]`**Output:** 2**Explanation: **the subarray `[4,3]` has the minimal length under the problem constraint.**Follow up:**If you have figured out the *O*(*n*) solution, try coding another solution of which the time complexity is *O*(*n* log *n*).|给定一个含有 **n **个正整数的数组和一个正整数 **s ，**找出该数组中满足其和** ≥ s **的长度最小的连续子数组**。**如果不存在符合条件的连续子数组，返回 0。**示例: ****输入:**`s = 7, nums = [2,3,1,2,4,3]`**输出:** 2**解释: **子数组 `[4,3]` 是该条件下的长度最小的连续子数组。**进阶:**如果你已经完成了*O*(*n*) 时间复杂度的解法, 请尝试 *O*(*n* log *n*) 时间复杂度的解法。|
|----|----|

思路：用两个指针，left指向左边，right向后遍历，直到right==nums.size()或者sum>s时停止，接着left向右遍历，直到找到最小的right-left为止。注意要处理一个特殊情况，就是所有元素和加起来都小于s，return 0.时间复杂度为O（n）

```cpp
class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
        if(nums.empty()) return 0;
        int left=0,right=0,sum=0,ret=nums.size();
        while(right<nums.size())
        {
            while(sum<s && right<nums.size())
                sum+=nums[right++];
            while(sum>=s)
            {
                ret=min(ret,right-left);
                sum-=nums[left++];
            }
        }
        if(left==0 && right==nums.size() && sum<s) return 0;
        return ret;
    }
};
```](https://so.csdn.net/so/search/s.do?q=长度最小的子数组&t=blog)





# leetcode-213. House Robber II 打家劫舍 II - 别说话写代码的博客 - CSDN博客





2019年01月27日 14:18:53[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[打家劫舍 II																[House Robber II																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=House Robber II&t=blog)



|You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed. All houses at this place are **arranged in a circle.** That means the first house is the neighbor of the last one. Meanwhile, adjacent houses have security system connected and **it will automatically contact the police if two adjacent houses were broken into on the same night**.Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight **without alerting the police**.**Example 1:****Input:** [2,3,2]**Output:** 3**Explanation:** You cannot rob house 1 (money = 2) and then rob house 3 (money = 2),             because they are adjacent houses.**Example 2:****Input:** [1,2,3,1]**Output:** 4**Explanation:** Rob house 1 (money = 1) and then rob house 3 (money = 3).             Total amount you can rob = 1 + 3 = 4.|你是一个专业的小偷，计划偷窃沿街的房屋，每间房内都藏有一定的现金。这个地方所有的房屋都**围成一圈，**这意味着第一个房屋和最后一个房屋是紧挨着的。同时，相邻的房屋装有相互连通的防盗系统，**如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警**。给定一个代表每个房屋存放金额的非负整数数组，计算你**在不触动警报装置的情况下，**能够偷窃到的最高金额。**示例 1:****输入:** [2,3,2]**输出:** 3**解释:** 你不能先偷窃 1 号房屋（金额 = 2），然后偷窃 3 号房屋（金额 = 2）, 因为他们是相邻的。**示例 2:****输入:** [1,2,3,1]**输出:** 4**解释:** 你可以先偷窃 1 号房屋（金额 = 1），然后偷窃 3 号房屋（金额 = 3）。     偷窃到的最高金额 = 1 + 3 = 4 。|
|----|----|

思路：组成一个 环，环的收尾相邻，也就是说收尾元素只能有一个被选中。我们 可以将元素分为0~nums.size()-1和1~nums.size()两部分， 就是要么选中 头要么选中尾。最终的结果就是这两种可能的最大值。

对于内层遍历，我们用两个节点代替dp数组， 分别保存偶数节点和奇数 节点的最大值，依次往后遍历就可以了

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.size()<2) return nums.empty()? 0 : nums[0];
        return max(rob(nums, 0, nums.size()-1),rob(nums, 1, nums.size()));
    }
    int rob(vector<int> &nums, int left, int right) {
        int robEven = 0, robOdd = 0;
        for (int i = left; i < right; ++i) 
        {
            if (i % 2 == 0) robEven = max(robEven + nums[i], robOdd);
            else robOdd = max(robEven, robOdd + nums[i]);
        }
        return max(robEven, robOdd);
    }
};
```](https://so.csdn.net/so/search/s.do?q=打家劫舍 II&t=blog)





# leetcode(22): House Rober（打家劫舍） - 刘炫320的博客 - CSDN博客
2018年09月03日 10:52:28[刘炫320](https://me.csdn.net/qq_35082030)阅读数：96
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
# 0.写在前面
这次我们讲一下房子盗窃者的问题，说白了就是动态规划。
# 1.House Robber(198)
## 1.1 题目描述
> 
You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night.
Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight without alerting the police.
Example
> 
Input: [1,2,3,1]
Output: 4
Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
Total amount you can rob = 1 + 3 = 4.
Example2
> 
Input: [2,7,9,3,1]
Output: 12
Explanation: Rob house 1 (money = 2), rob house 3 (money = 9) and rob house 5 (money = 1).
Total amount you can rob = 2 + 9 + 1 = 12.
## 1.2思路
其实这道题的题目就是，给定一个数组，求至少有一个间隔的最大和。
那么这就是一个动态规划的基本问题。我们首先去判断当前取和不取的最大值，然后再不断的迭代。
## 1.3代码
```
public int rob(int[] nums) {
		int exclude = 0;
	    int include = 0;
	    for (int i=0;i<nums.length;i++) {
	        int temp = exclude;
	        exclude = Math.max(exclude, include);
	        include = nums[i] + temp;
	    }
	    return Math.max(exclude, include);
    }
```
## 1.4更近一步
如果使用动态规划的表述，应该更容易理解。
```
public int rob(int[] nums) {
		int[][] dp = new int[nums.length + 1][2];
	    for (int i = 1; i <= nums.length; i++) {
	        dp[i][0] = Math.max(dp[i - 1][0], dp[i - 1][1]);
	        //如果不带约束的话
	        //dp[i][1]=Math.max(nums[i - 1] + dp[i - 1][0], nums[i - 1] + dp[i - 1][1]);
	        //带了约束条件了
	        dp[i][1] = nums[i - 1] + dp[i - 1][0];
	    }
	    return Math.max(dp[nums.length][0], dp[nums.length][1]);
    }
```
# 2. House Robber II(213)
## 2.1题目描述
> 
You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed. All houses at this place are arranged in a circle. That means the first house is the neighbor of the last one. Meanwhile, adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night.
Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight without alerting the police.
Example
> 
Input: [2,3,2]
Output: 3
Explanation: You cannot rob house 1 (money = 2) and then rob house 3 (money = 2),
because they are adjacent houses.
Example2
> 
Input: [1,2,3,1]
Output: 4
Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
Total amount you can rob = 1 + 3 = 4.
## 2.2思路
这一题就是做一个先行约束条件，即头和尾不能同时被选中，即要么从0-n-2,要么从1-n-1即可。
## 2.3 代码
```
public int rob(int[] nums) {
		 if (nums.length == 1) return nums[0];
		 return Math.max(rob(nums, 0, nums.length - 2), rob(nums, 1, nums.length - 1));
    }
	private int rob(int[] nums, int lo, int hi) {
	    int include = 0, exclude = 0;
	    for (int j = lo; j <= hi; j++) {
	    	int temp = exclude;
	        exclude = Math.max(exclude, include);
	        include = nums[j] + temp;
	    }
	    return Math.max(include, exclude);
	}
```
# 3.House Robber III (337)
## 3.1 题目描述
> 
The thief has found himself a new place for his thievery again. There is only one entrance to this area, called the “root.” Besides the root, each house has one and only one parent house. After a tour, the smart thief realized that “all houses in this place forms a binary tree”. It will automatically contact the police if two directly-linked houses were broken into on the same night.
Determine the maximum amount of money the thief can rob tonight without alerting the police.
Example
> 
Input: [3,2,3,null,3,null,1]
3
/ 
2   3
\   \
3   1
Output: 7
Explanation: Maximum amount of money the thief can rob = 3 + 3 + 1 = 7.
Example2
> 
Input: [3,4,5,1,3,null,1]
3
/ 
4   5
/ \   \
1   3   1
Output: 9
Explanation: Maximum amount of money the thief can rob = 4 + 5 = 9.
## 3.2 思路
使用DFS的话，是比较容易做出来的。尤其是这是二叉树。
## 3.3 代码
```
public int rob(TreeNode root) {
        int[] num = dfs(root);
        return Math.max(num[0], num[1]);
    }
    private int[] dfs(TreeNode x) {
        // if it's leaf return 0,0
        if (x == null) return new int[2];
        //get left and right maxvalue
        int[] left = dfs(x.left);
        int[] right = dfs(x.right);
        int[] res = new int[2];
        //case 0: Select the current node value;
        res[0] = left[1] + right[1] + x.val;
        //case 1: Don't select the current node value;
        res[1] = Math.max(left[0], left[1]) + Math.max(right[0], right[1]);
        return res;
    }
```
# 4.总结
好久没练，都生疏了很多。我们还是下期见。

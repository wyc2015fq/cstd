# leetcode_740. Delete and Earn ? 待解决 - wjheha的博客 - CSDN博客
2017年12月27日 19:33:48[wjheha](https://me.csdn.net/wjheha)阅读数：63标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given an array nums of integers, you can perform operations on the array.
In each operation, you pick any nums[I] and delete it to earn nums[I] points. After, you must delete every element equal to nums[I] - 1 or nums[I] + 1.
You start with 0 points. Return the maximum number of points you can earn by applying such operations.
Example 1: 
Input: nums = [3, 4, 2] 
Output: 6 
Explanation:  
Delete 4 to earn 4 points, consequently 3 is also deleted. 
Then, delete 2 to earn 2 points. 6 total points are earned. 
Example 2: 
Input: nums = [2, 2, 3, 3, 3, 4] 
Output: 9 
Explanation:  
Delete 3 to earn 3 points, deleting both 2’s and the 4. 
Then, delete 3 again to earn 3 points, and 3 again to earn 3 points. 
9 total points are earned.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/delete-and-earn/description/](https://leetcode.com/problems/delete-and-earn/description/)
***思路***：

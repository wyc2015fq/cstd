# leetcode_330. Patching Array ? 待解决 - wjheha的博客 - CSDN博客
2017年12月27日 19:38:12[wjheha](https://me.csdn.net/wjheha)阅读数：54
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given a sorted positive integer array nums and an integer n, add/patch elements to the array such that any number in range [1, n] inclusive can be formed by the sum of some elements in the array. Return the minimum number of patches required.
Example 1: 
nums = [1, 3], n = 6 
Return 1.
Combinations of nums are [1], [3], [1,3], which form possible sums of: 1, 3, 4. 
Now if we add/patch 2 to nums, the combinations are: [1], [2], [3], [1,3], [2,3], [1,2,3]. 
Possible sums are 1, 2, 3, 4, 5, 6, which now covers the range [1, 6]. 
So we only need 1 patch.
Example 2: 
nums = [1, 5, 10], n = 20 
Return 2. 
The two patches can be [2, 4].
Example 3: 
nums = [1, 2, 2], n = 5 
Return 0.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/patching-array/description/](https://leetcode.com/problems/patching-array/description/)
***思路***：

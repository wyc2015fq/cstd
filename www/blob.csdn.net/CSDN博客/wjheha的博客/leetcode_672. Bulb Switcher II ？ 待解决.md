# leetcode_672. Bulb Switcher II ？ 待解决 - wjheha的博客 - CSDN博客
2017年10月16日 11:38:09[wjheha](https://me.csdn.net/wjheha)阅读数：67
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
There is a room with n lights which are turned on initially and 4 buttons on the wall. After performing exactly m unknown operations towards buttons, you need to return how many different kinds of status of the n lights could be.
Suppose n lights are labeled as number [1, 2, 3 …, n], function of these 4 buttons are given below:
Flip all the lights. 
Flip lights with even numbers. 
Flip lights with odd numbers. 
Flip lights with (3k + 1) numbers, k = 0, 1, 2, … 
Example 1: 
Input: n = 1, m = 1. 
Output: 2 
Explanation: Status can be: [on], [off] 
Example 2: 
Input: n = 2, m = 1. 
Output: 3 
Explanation: Status can be: [on, off], [off, on], [off, off] 
Example 3: 
Input: n = 3, m = 1. 
Output: 4 
Explanation: Status can be: [off, on, off], [on, off, on], [off, off, off], [off, on, on]. 
Note: n and m both fit in range [0, 1000].
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/bulb-switcher-ii/description/](https://leetcode.com/problems/bulb-switcher-ii/description/)
***思路***：

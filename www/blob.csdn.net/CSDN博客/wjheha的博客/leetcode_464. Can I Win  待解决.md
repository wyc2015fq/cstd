# leetcode_464. Can I Win ? 待解决 - wjheha的博客 - CSDN博客
2017年11月10日 17:38:46[wjheha](https://me.csdn.net/wjheha)阅读数：93标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
In the “100 game,” two players take turns adding, to a running total, any integer from 1..10. The player who first causes the running total to reach or exceed 100 wins.
What if we change the game so that players cannot re-use integers?
For example, two players might take turns drawing from a common pool of numbers of 1..15 without replacement until they reach a total >= 100.
Given an integer maxChoosableInteger and another integer desiredTotal, determine if the first player to move can force a win, assuming both players play optimally.
You can always assume that maxChoosableInteger will not be larger than 20 and desiredTotal will not be larger than 300.
Example
Input: 
maxChoosableInteger = 10 
desiredTotal = 11
Output: 
false
Explanation: 
No matter which integer the first player choose, the first player will lose. 
The first player can choose an integer from 1 up to 10. 
If the first player choose 1, the second player can only choose integers from 2 up to 10. 
The second player will win by choosing 10 and get a total = 11, which is >= desiredTotal. 
Same with other integers chosen by the first player, the second player will always win.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/can-i-win/description/](https://leetcode.com/problems/can-i-win/description/)
***思路***：

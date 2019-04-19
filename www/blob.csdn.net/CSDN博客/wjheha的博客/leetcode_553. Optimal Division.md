# leetcode_553. Optimal Division - wjheha的博客 - CSDN博客
2018年01月25日 22:07:51[wjheha](https://me.csdn.net/wjheha)阅读数：42
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given a list of positive integers, the adjacent integers will perform the float division. For example, [2,3,4] -> 2 / 3 / 4.
However, you can add any number of parenthesis at any position to change the priority of operations. You should find out how to add parenthesis to get the maximum result, and return the corresponding expression in string format. Your expression should NOT contain redundant parenthesis.
Example: 
Input: [1000,100,10,2] 
Output: “1000/(100/10/2)” 
Explanation: 
1000/(100/10/2) = 1000/((100/10)/2) = 200 
However, the bold parenthesis in “1000/((100/10)/2)” are redundant,  
since they don’t influence the operation priority. So you should return “1000/(100/10/2)”. 
Other cases: 
1000/(100/10)/2 = 50 
1000/(100/(10/2)) = 50 
1000/100/10/2 = 0.5 
1000/100/(10/2) = 2 
Note:
The length of the input array is [1, 10]. 
Elements in the given array will be in range [2, 1000]. 
There is only one optimal division for each test case.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/optimal-division/description/](https://leetcode.com/problems/optimal-division/description/)
***思路***：

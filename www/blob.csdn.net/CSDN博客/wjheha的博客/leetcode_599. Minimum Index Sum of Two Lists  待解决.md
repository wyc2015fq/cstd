# leetcode_599. Minimum Index Sum of Two Lists ? 待解决 - wjheha的博客 - CSDN博客
2017年11月24日 09:49:24[wjheha](https://me.csdn.net/wjheha)阅读数：98标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Suppose Andy and Doris want to choose a restaurant for dinner, and they both have a list of favorite restaurants represented by strings.
You need to help them find out their common interest with the least list index sum. If there is a choice tie between answers, output all of them with no order requirement. You could assume there always exists an answer.
Example 1: 
Input: 
[“Shogun”, “Tapioca Express”, “Burger King”, “KFC”] 
[“Piatti”, “The Grill at Torrey Pines”, “Hungry Hunter Steakhouse”, “Shogun”] 
Output: [“Shogun”] 
Explanation: The only restaurant they both like is “Shogun”.
Example 2: 
Input: 
[“Shogun”, “Tapioca Express”, “Burger King”, “KFC”] 
[“KFC”, “Shogun”, “Burger King”] 
Output: [“Shogun”] 
Explanation: The restaurant they both like and have the least index sum is “Shogun” with index sum 1 (0+1). 
Note: 
The length of both lists will be in the range of [1, 1000]. 
The length of strings in both lists will be in the range of [1, 30]. 
The index is starting from 0 to the list length minus 1. 
No duplicates in both lists.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/minimum-index-sum-of-two-lists/description/](https://leetcode.com/problems/minimum-index-sum-of-two-lists/description/)
***思路***：

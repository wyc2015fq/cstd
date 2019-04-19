# leetcode_648. Replace Words ？ 待解决 - wjheha的博客 - CSDN博客
2017年12月05日 17:41:19[wjheha](https://me.csdn.net/wjheha)阅读数：64标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
In English, we have a concept called root, which can be followed by some other words to form another longer word - let’s call this word successor. For example, the root an, followed by other, which can form another word another.
Now, given a dictionary consisting of many roots and a sentence. You need to replace all the successor in the sentence with the root forming it. If a successor has many roots can form it, replace it with the root with the shortest length.
You need to output the sentence after the replacement.
Example 1: 
Input: dict = [“cat”, “bat”, “rat”] 
sentence = “the cattle was rattled by the battery” 
Output: “the cat was rat by the bat” 
Note: 
The input will only have lower-case letters. 
1 <= dict words number <= 1000 
1 <= sentence words number <= 1000 
1 <= root length <= 100 
1 <= sentence words length <= 1000
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/replace-words/description/](https://leetcode.com/problems/replace-words/description/)
***思路***：

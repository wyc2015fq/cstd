# leetcode_617. Merge Two Binary Trees ? 待解决 - wjheha的博客 - CSDN博客
2017年10月29日 15:04:41[wjheha](https://me.csdn.net/wjheha)阅读数：92标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Merge Two Binary Trees
Given two binary trees and imagine that when you put one of them to cover the other, some nodes of the two trees are overlapped while the others are not.
You need to merge them into a new binary tree. The merge rule is that if two nodes overlap, then sum node values up as the new value of the merged node. Otherwise, the NOT null node will be used as the node of new tree.
Example 1: 
Input:  
    Tree 1                     Tree 2 
          1                         2 
         / \                       / \ 
        3   2                     1   3 
       /                           \   \ 
      5                             4   7 
Output:  
Merged tree: 
         3 
        / \ 
       4   5 
      / \   \  
     5   4   7 
Note: The merging process must start from the root nodes of both trees.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/merge-two-binary-trees/description/](https://leetcode.com/problems/merge-two-binary-trees/description/)
***思路***：

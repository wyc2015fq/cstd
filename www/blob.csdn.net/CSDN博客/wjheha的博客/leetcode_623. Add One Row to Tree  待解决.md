# leetcode_623. Add One Row to Tree ? 待解决 - wjheha的博客 - CSDN博客
2017年11月24日 10:03:18[wjheha](https://me.csdn.net/wjheha)阅读数：75标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given the root of a binary tree, then value v and depth d, you need to add a row of nodes with value v at the given depth d. The root node is at depth 1.
The adding rule is: given a positive integer depth d, for each NOT null tree nodes N in depth d-1, create two tree nodes with value v as N’s left subtree root and right subtree root. And N’s original left subtree should be the left subtree of the new left subtree root, its original right subtree should be the right subtree of the new right subtree root. If depth d is 1 that means there is no depth d-1 at all, then create a tree node with value v as the new root of the whole original tree, and the original tree is the new root’s left subtree.
Example 1: 
Input:  
A binary tree as following: 
       4 
     /   \ 
    2     6 
   / \   /  
  3   1 5   
v = 1
d = 2
Output:  
       4 
      / \ 
     1   1 
    /     \ 
   2       6 
  / \     /  
 3   1   5   
Example 2: 
Input:  
A binary tree as following: 
      4 
     / 
    2 
   / \ 
  3   1    
v = 1
d = 3
Output:  
      4 
     / 
    2 
   / \ 
  1   1 
 /     \ 
3       1 
Note: 
The given d is in range [1, maximum depth of the given tree + 1]. 
The given binary tree has at least one tree node.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/add-one-row-to-tree/description/](https://leetcode.com/problems/add-one-row-to-tree/description/)
***思路***：

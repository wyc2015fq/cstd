# leetcode_606. Construct String from Binary Tree ？ 待解决 - wjheha的博客 - CSDN博客
2017年10月16日 11:39:27[wjheha](https://me.csdn.net/wjheha)阅读数：59标签：[leetcode																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
You need to construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way.
The null node needs to be represented by empty parenthesis pair “()”. And you need to omit all the empty parenthesis pairs that don’t affect the one-to-one mapping relationship between the string and the original binary tree.
Example 1: 
Input: Binary tree: [1,2,3,4] 
       1 
     /   \ 
    2     3 
   / 
  4     
Output: “1(2(4))(3)”
Explanation: Originallay it needs to be “1(2(4)())(3()())”,  
but you need to omit all the unnecessary empty parenthesis pairs.  
And it will be “1(2(4))(3)”. 
Example 2: 
Input: Binary tree: [1,2,3,null,4] 
       1 
     /   \ 
    2     3 
     \ 
      4 
Output: “1(2()(4))(3)”
Explanation: Almost the same as the first example,  
except we can’t omit the first parenthesis pair to break the one-to-one mapping relationship between the input and the output.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/construct-string-from-binary-tree/description/](https://leetcode.com/problems/construct-string-from-binary-tree/description/)
***思路***：


# 606. Construct String from Binary Tree - OraYang的博客 - CSDN博客

2017年08月29日 11:02:29[OraYang](https://me.csdn.net/u010665216)阅读数：214标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You need to construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way.
The null node needs to be represented by empty parenthesis pair "()". And you need to omit all the empty parenthesis pairs that don't affect the one-to-one mapping relationship between the string and the original binary tree.
Example 1:
Input:Binary tree: [1,2,3,4]
       1
     /   \
    2     3
   /    
  4Output:"1(2(4))(3)"
Explanation:Originallay it needs to be "1(2(4)())(3()())",
but you need to omit all the unnecessary empty parenthesis pairs.
And it will be "1(2(4))(3)".
Example 2:
Input:Binary tree: [1,2,3,null,4]
       1
     /   \
    2     3
     \  
      4Output:"1(2()(4))(3)"
Explanation:Almost the same as the first example,
except we can't omit the first parenthesis pair to break the one-to-one mapping relationship between the input and the output思路：本题，采用递归思路，每发生一次递归调用前后都要有一个括号，然后注意括号的优化，分析题意得当右结点为空时需要优化
代码：

```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    string tree2str(TreeNode* t) {
        return PreOrder(t);
        
    }
private:
    string PreOrder(TreeNode* T){
        if(T==NULL)
            return "";
        if(T->left==NULL&&T->right==NULL)
            return to_string(T->val)+"";
        
        if(T->right==NULL)
            return to_string(T->val)+"("+PreOrder(T->left)+")";
        return to_string(T->val)+"("+PreOrder(T->left)+")("+PreOrder(T->right)+")";
}  
};
```



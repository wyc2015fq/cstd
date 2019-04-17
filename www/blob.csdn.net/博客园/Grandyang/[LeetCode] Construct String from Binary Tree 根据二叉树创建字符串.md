# [LeetCode] Construct String from Binary Tree 根据二叉树创建字符串 - Grandyang - 博客园







# [[LeetCode] Construct String from Binary Tree 根据二叉树创建字符串](https://www.cnblogs.com/grandyang/p/7000040.html)









You need to construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way.

The null node needs to be represented by empty parenthesis pair "()". And you need to omit all the empty parenthesis pairs that don't affect the one-to-one mapping relationship between the string and the original binary tree.

Example 1:
Input: Binary tree: [1,2,3,4]
       1
     /   \
    2     3
   /    
  4     

Output: "1(2(4))(3)"

Explanation: Originallay it needs to be "1(2(4)())(3()())", 
but you need to omit all the unnecessary empty parenthesis pairs. 
And it will be "1(2(4))(3)".



Example 2:
Input: Binary tree: [1,2,3,null,4]
       1
     /   \
    2     3
     \  
      4 

Output: "1(2()(4))(3)"

Explanation: Almost the same as the first example, 
except we can't omit the first parenthesis pair to break the one-to-one mapping relationship between the input and the output.



这道题给我们了一个二叉树，让我们创建对应的字符串，之前有一道正好反过来的题[Construct Binary Tree from String](http://www.cnblogs.com/grandyang/p/6793904.html)。对于二叉树的处理，递归肯定是王道啊。想想如何来实现递归函数，我们观察到题目中的例子，发现如果左子结点为空，右子结点不为空时，需要在父结点后加上个空括号，而右子结点如果不存在，或者左右子结点都不存在就不需要这么做。那我们在递归函数中，如果当前结点不存在，直接返回，然后要在当前结点值前面加上左括号，然后判断，如果左子结点不存在，而右子结点存在的话，要在结果res后加上个空括号，然后分别对左右子结点调用递归函数，调用完之后要加上右括号，形成封闭的括号。由于最外面一层的括号不需要，所以我们再返回最终结果之前要去掉首尾的括号，参见代码如下：





解法一：


```
class Solution {
public:
    string tree2str(TreeNode* t) {
        if (!t) return "";
        string res = "";
        helper(t, res);
        return string(res.begin() + 1, res.end() - 1);
    }
    void helper(TreeNode* t, string& res) {
        if (!t) return;
        res += "(" + to_string(t->val);
        if (!t->left && t->right) res += "()";
        helper(t->left, res);
        helper(t->right, res);
        res += ")";
    }
};
```



下面来看一种不用额外函数的递归写法，这种做法是一开始调用递归函数求出左右子结点的返回字符串，如果左右结果串均为空，则直接返回当前结点值；如果左子结果串为空，那么返回当前结果res，加上一个空括号，再加上放在括号中的右子结果串；如果右子结果串为空，那么发返回当前结果res，加上放在括号中的左子结果串；如果左右子结果串都存在，那么返回当前结果，加上分别放在括号中的左右子结果串，参见代码如下：



解法二：

```
class Solution {
public:
    string tree2str(TreeNode* t) {
        if (!t) return "";
        string res = to_string(t->val);
        string left = tree2str(t->left), right = tree2str(t->right);
        if (left == "" && right == "") return res;
        if (left == "") return res + "()" + "(" + right + ")";
        if (right == "") return res + "(" + left + ")";
        return res + "(" + left + ")" + "(" + right + ")";
    }
};
```



下面这种解法更加简洁，由热心网友[edyyy](http://home.cnblogs.com/u/1090659/)提供，思路和上面解法相同，参见代码如下：



解法三：

```
class Solution {
public:
    string tree2str(TreeNode* t) {
        if (!t) return "";
        string res = to_string(t->val);
        if (!t->left && !t->right) return res;
        res += "(" + tree2str(t->left) + ")";
        if (t->right) res += "(" + tree2str(t->right) + ")";
        return res;
    }
};
```



类似题目：

[Construct Binary Tree from String](http://www.cnblogs.com/grandyang/p/6793904.html)



参考资料：

[https://discuss.leetcode.com/topic/91308/java-solution-tree-traversal](https://discuss.leetcode.com/topic/91308/java-solution-tree-traversal)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# leetcode（9）：tree - 刘炫320的博客 - CSDN博客
2018年03月17日 09:44:05[刘炫320](https://me.csdn.net/qq_35082030)阅读数：87标签：[镜像二叉树																[包含树																[二叉树合并																[检索二叉树](https://so.csdn.net/so/search/s.do?q=检索二叉树&t=blog)](https://so.csdn.net/so/search/s.do?q=二叉树合并&t=blog)](https://so.csdn.net/so/search/s.do?q=包含树&t=blog)](https://so.csdn.net/so/search/s.do?q=镜像二叉树&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
这里我们介绍几个关于树的问题。
# 1.leetcode#226. Invert Binary Tree
## 1.1问题描述
> 
Invert a binary tree. 
       4 
     /   \ 
    2     7 
   / \   / \ 
  1   3 6   9 
  to 
       4 
     /   \ 
    7     2 
   / \   / \ 
  9   6 3   1
## 1.2思路
这其实就是反转二叉树，使用递归即可，其中的关键一步在于让左子树等于右子树的递归，右子树等于左子树的递归即可。 
但是非常重要的一点就是确定好递归的顺序，其实递归顺序应当是先把两个子树交换以后，再进行递归。
## 1.3代码
```java
public TreeNode invertTree(TreeNode root) {
        if(root!=null){
            //记录临时的左子树的节点，因为在接下来反转的过程中左子树会发生改变。
            TreeNode tempright=root.left;
            TreeNode templeft=root.right;
            root.left=invertTree(templeft);
            root.right=invertTree(tempright);
            return root;
        }
        else{
            return null;
        }
    }
```
# 2.leetcode#538. Convert BST to Greater Tree
## 2.1问题描述
> 
Given a Binary Search Tree (BST), convert it to a Greater Tree such that every key of the original BST is changed to the original key plus sum of all keys greater than the original key in BST.
Example:
> 
Input: The root of a Binary Search Tree like this: 
                5 
              /   \ 
             2     13
Output: The root of a Greater Tree like this: 
             18 
            /   \ 
          20     13
## 2.2 思路
这道题的意思是说，要把所有的大于该数的数都加到这个节点上，比如上述例子中的18=13+5.20=2+5+13等。使用逆中序遍历即可解决。
## 2.3 代码
```java
int sum = 0;
    public TreeNode convertBST(TreeNode root) {
        if (root == null) return null;
        //先递归右子树    
        convertBST(root.right);
        //当前节点加上目前和    
        root.val += sum;
        //更新当前和    
        sum = root.val;
        //再递归左子树    
        convertBST(root.left);
        return root;
    }
```
# 3.leetcode#543. Diameter of Binary Tree
## 3.1问题描述
> 
Given a binary tree, you need to compute the length of the diameter of the tree. The diameter of a binary tree is the length of the longest path between any two nodes in a tree. This path may or may not pass through the root.
Example:
> 
Given a binary tree  
            1 
           / \ 
          2   3 
         / \ 
        4   5 
  Return 3, which is the length of the path [4,2,1,3] or [5,2,1,3]. 
  Note: The length of path between two nodes is represented by the number of edges between them.
## 3.2思路
这道题就是求最长的路径，也就是说，最长的路径为该节点的左子树最长+右子树最长。然后记录一下当前最长的值即可。
## 3.3代码
```java
public int diameterOfBinaryTree(TreeNode root) {
        if(root==null){
            return 0;
        }
        int nowlength=LongestLenth(root.left)+LongestLenth(root.right);
        return Math.max(nowlength,Math.max(diameterOfBinaryTree(root.left),diameterOfBinaryTree(root.right)));
    }
    public int LongestLenth(TreeNode root){
        if(root==null){
            return 0;
        }
        else{
            return Math.max(LongestLenth(root.left), LongestLenth(root.right))+1; 
        }
    }
```
这么做的好处是，不需要额外的全局变量，逻辑易于理解。但这样的坏处就是，每次都需要计算一次最长路径，浪费。
## 3.4更近一步
如果使用全局变量，再把比较放到最长路径计算里面，速度会提升不少。
```java
int longest=0;
    public int diameterOfBinaryTree(TreeNode root) {
        if(root==null){
            return 0;
        }
        LongestLenth(root);
        return longest;
    }
    public int LongestLenth(TreeNode root){
        if(root==null){
            return 0;
        }
        else{
            int left=LongestLenth(root.left);
            int right=LongestLenth(root.right);
            longest=Math.max(longest,left+right);
            return Math.max(LongestLenth(root.left), LongestLenth(root.right))+1; 
        }
    }
```
# 4.leetcode#572. Subtree of Another Tree
## 4.1问题描述
Given two non-empty binary trees s and t, check whether tree t has exactly the same structure and node values with a subtree of s. A subtree of s is a tree consists of a node in s and all of this node’s descendants. The tree s could also be considered as a subtree of itself.
Example
> 
Example 1: 
  Given tree s: 
       3 
      / \ 
     4   5 
    / \ 
   1   2 
  Given tree t: 
     4  
    / \ 
   1   2 
  Return true, because t has the same structure and node values with a subtree of s. 
  Example 2: 
  Given tree s: 
       3 
      / \ 
     4   5 
    / \ 
   1   2 
      / 
     0 
  Given tree t: 
     4 
    / \ 
   1   2 
  Return false.
## 4.2思路
这道题就是比较是否是子树，那么就要这样比较，是不是当前节点与目标树一致，或者是当前节点的左子树是目标树，或者是当前节点的右子树是目标树。
## 4.3代码
```java
public boolean isSubtree(TreeNode s, TreeNode t) {
        if (s == null) return false;
        //如果当前树是子树的话，返回相同
        if (isSame(s, t)) return true;
        //否则返回在左子树或者在右子树上
        return isSubtree(s.left, t) || isSubtree(s.right, t);
    }
    private boolean isSame(TreeNode s, TreeNode t) {
        //如果两个都为空，则相同
        if (s == null && t == null) return true;
        //如果两个有一个为空，则不同
        if (s == null || t == null) return false;
        //如果两个数目不对，则不同
        if (s.val != t.val) return false;
        //如果左子树相同，右子树也相同的话。
        return isSame(s.left, t.left) && isSame(s.right, t.right);
    }
```
# 5.leetcode#617. Merge Two Binary Trees
## 5.1问题描述
> 
Given two binary trees and imagine that when you put one of them to cover the other, some nodes of the two trees are overlapped while the others are not. 
  You need to merge them into a new binary tree. The merge rule is that if two nodes overlap, then sum node values up as the new value of the merged node. Otherwise, the NOT null node will be used as the node of new tree.
Example
> 
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
## 5.2思路
这道题思路很简单，那就是先合并当前节点，然后再合并左子树，合并右子树即可。
## 5.3代码
```java
public TreeNode mergeTrees(TreeNode t1, TreeNode t2) {
        if(t1==null){
            return t2;
        }
        else if(t2==null){
            return t1;
        }
        else{
            TreeNode newnode=new TreeNode(t1.val+t2.val);
            newnode.left=mergeTrees(t1.left, t2.left);
            newnode.right=mergeTrees(t1.right, t2.right);
            return newnode;
        }
    }
```
# 6.小结
这次我们主要针对树的简单题目进行了一个总结，主要包括树的转换、树的操作、树的合并、树的包含，其实思路是一样的，先判断当前树是否满足条件，然后递归判断左子树和右子树即可。

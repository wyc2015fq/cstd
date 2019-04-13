
# 剑指offer——二叉树的镜像 - manduner的博客 - CSDN博客


2019年03月19日 18:22:07[Manduner_TJU](https://me.csdn.net/manduner)阅读数：11标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
操作给定的二叉树，将其变换为源二叉树的镜像。
# 2，解题思路
这道题的思路倒是不难，就是利用递归的思路，如下：
先将根节点的左右子树调换；
再递归处理根节点的左右子树。
**但是这个题有两种特殊的二叉树需要考虑到，一种是只有左子树，一种是只有右子树。**
下面给出了自己写的有些冗余的代码，以及优化后的代码。
# 3，源码
自己写的代码（代码有些冗余）
```python
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    public void Mirror(TreeNode root) {
        if(root == null) return;
        if(root.left == null && root.right == null) return;
        
        if(root.left == null){
            root.left = root.right;
            root.right = null;
            Mirror(root.left);
        }else if(root.right == null){
            root.right = root.left;
            root.left = null;
            Mirror(root.right);
        }else if(root.left != null && root.right != null){
            TreeNode temp = root.left;
            root.left = root.right;
            root.right = temp;
            Mirror(root.left);
            Mirror(root.right);
        }
    }
}
```
优化后的代码
```python
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    public void Mirror(TreeNode root) {
        if(root == null) return;
        if(root.left == null && root.right == null) return;
        
        TreeNode temp = root.left;
        root.left = root.right;
        root.right = temp;
        
        if(root.left != null){
            Mirror(root.left);
        }
        if(root.right != null){
            Mirror(root.right);
        }
    }
}
```



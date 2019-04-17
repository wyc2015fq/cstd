# Lintcode——97. 二叉树的最大深度 - westbrook1998的博客 - CSDN博客





2018年04月03日 22:26:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
给定一个二叉树，找出其最大深度。 

  二叉树的深度为根节点到最远叶子节点的距离。


```java
public class Solution {
    /**
     * @param root: The root of binary tree.
     * @return: An integer
     */
    public int maxDepth(TreeNode root) {
        // write your code here
        if(root==null){
            return 0;
        }
        if(root.left==null && root.right!=null){
            return 1+maxDepth(root.right);
        }
        else if(root.left!=null && root.right==null){
            return 1+maxDepth(root.left);
        }
        else if(root.left!=null && root.right!=null){
            return 1+Math.max(maxDepth(root.left),maxDepth(root.right));
        }
        else{
            return 1;
        }
    }
}
```

好久没有写过这么一气呵成的题了，很爽不过这题应该算很简单的一个二叉树的遍历，采用递归的方法，完成




# Lintcode632——二叉树的最大节点 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:22:36[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：103








## 题目：

> 
在二叉树中寻找值最大的节点并返回。

您在真实的面试中是否遇到过这个题？ Yes 

  样例 

  给出如下一棵二叉树：
```
1
   /   \
 -5     2
 / \   /  \
0   3 -4  -5
```

> 
返回值为 3 的节点。


## 题解：

```
public class Solution {
    /*
     * @param root: the root of tree
     * @return: the max node
     */
    public TreeNode maxNode(TreeNode root) {
        // write your code here
        if(root==null){
            return null;
        }
        TreeNode max=root;
        if(root.left!=null){
            TreeNode max_left=maxNode(root.left);
            max= max.val > max_left.val ? max:max_left;
        }
        if(root.right!=null){
            TreeNode max_right=maxNode(root.right);
            max= (max.val > max_right.val) ? max:max_right;
        }
        return max;
    }
}
```

使用了递归的方法，最后回到根节点和左右子节点的比较






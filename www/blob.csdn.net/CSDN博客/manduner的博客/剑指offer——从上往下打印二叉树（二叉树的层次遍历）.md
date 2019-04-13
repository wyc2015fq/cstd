
# 剑指offer——从上往下打印二叉树（二叉树的层次遍历） - manduner的博客 - CSDN博客


2019年03月29日 14:56:34[Manduner_TJU](https://me.csdn.net/manduner)阅读数：7


# 1，问题描述
从上往下打印出二叉树的每个节点，同层节点从左至右打印。
# 2，解题思路
首先构建一个辅助队列存放二叉树节点，那么接下来该如何层次遍历呢？步骤如下：
将根节点添加到队列中
循环判断队列是否为空，若不为空则执行以下循环体语句：
将队列头节点取出，再将该节点不为空的左右子树的根节点依次添加至队列中，然后把该节点的值添加到列表中
循环至队列为空，返回列表
# 3，源码
```python
import java.util.ArrayList;
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
    public ArrayList<Integer> PrintFromTopToBottom(TreeNode root) {
        ArrayList<Integer> list = new ArrayList<Integer>();
        ArrayList<TreeNode> queue = new ArrayList<TreeNode>();
        if(root==null) return list;
        queue.add(root);
        while(!queue.isEmpty()) {
            TreeNode temp = queue.remove(0);
            if(temp.left!=null) queue.add(temp.left);
            if(temp.right!=null) queue.add(temp.right);
            list.add(temp.val);
        }
        return list;
    }
}
```



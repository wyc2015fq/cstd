
# 剑指offer——重建二叉树 - manduner的博客 - CSDN博客


2018年12月20日 21:04:14[Manduner_TJU](https://me.csdn.net/manduner)阅读数：38标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
# 2，解题思路
## 解题思路（一）：
（1）前序遍历序列中的第一元素是二叉树的根节点，题目中根节点的值是1。
（2）从中序遍历序列中找到根节点值1的下标（记为index=3）。那么中序遍历序列中index的左边就是左子树的中序遍历序列**in[0,index]（包左不包右），即[4,7,2]**；右边为右子树的中序遍历序列**in[index+1, in.length]（包左不包右）,即[5,3,8,6]**。
（3）根据（2）中的下标值从前序遍历序列中找到根节点的左子树前序遍历序列**pre[1,index+1]，即[2,4,7]**。那么右子树的前序遍历序列为**pre[index+1,pre.length]，即[3,5,6,8]**。
至此找到了该根节点左子树的前序遍历序列**pre[1,index+1]、中序遍历序列in[0,index]**，以及右子树的前序遍历序列**pre[index+1,pre.length]、中序遍历序列in[index+1, in.length]**。
注：java的Arrays.copyOfRange()方法，是包左不包右的。
（4）将左右子树看做是待构建的二叉树，再递归处理（1）（2）（3）步骤，递归出口是pre或者in任何一个为空就返回null；
## 解题思路（二）：（牛客网大牛的思路，但是觉得不如上面的思路好理解哇，我是费了吃奶的劲才想明白的。。。我是菜鸟，请忽略）
（1）首先找到中序的根节点对应的i，然后i的左边就是左子树（startIn，i-1），右边就是右子树（i+1，endIn）
（2）然后前序的左子树的长度就可以用中序的左子树的长度（i-startIn）得到，那么左子树就是（startPre+1，startPre+1+（i-startIn-1）），右子树的起点就是左子树的终点+1即（startPre+1+(i-startIn-1)+1，endPre）。
（3）递归前两步，递归出口是，startPre>endPre || starIn>endIn，返回null。
# 3，源码
解题思路一：
```python
import java.util.Arrays;
/**
 * Definition for binary tree
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
public class Solution {
    public TreeNode reConstructBinaryTree(int [] pre,int [] in) {
       if(pre.length == 0||in.length == 0) return null;
        TreeNode head = new TreeNode(pre[0]);
        for(int i = 0; i < in.length; i++){
            if(pre[0] == in[i]){
                head.left = reConstructBinaryTree(Arrays.copyOfRange(pre, 1, i+1), 
                                                  Arrays.copyOfRange(in, 0, i));
                head.right = reConstructBinaryTree(Arrays.copyOfRange(pre, i+1, pre.length),
                                                   Arrays.copyOfRange(in, i+1,in.length));
                break;
            }
        }
        return head;
    }
}
```
解题思路二：
```python
/**
 * Definition for binary tree
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
public class Solution {
    public TreeNode reConstructBinaryTree(int [] pre,int [] in) {
        TreeNode root=reConstructBinaryTree(pre,0,pre.length-1,in,0,in.length-1);
        return root;
    }
    //前序遍历{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}
    private TreeNode reConstructBinaryTree(int [] pre,int startPre,int endPre,int [] in,int startIn,int endIn) {
         
        if(startPre>endPre||startIn>endIn)
            return null;
        TreeNode root=new TreeNode(pre[startPre]);
         
        for(int i=startIn;i<=endIn;i++)
            if(in[i]==pre[startPre]){
                root.left=reConstructBinaryTree(pre,startPre+1,startPre+i-startIn,in,startIn,i-1);
                root.right=reConstructBinaryTree(pre,i-startIn+startPre+1,endPre,in,i+1,endIn);
                      break;
            }
                 
        return root;
    }
}
```



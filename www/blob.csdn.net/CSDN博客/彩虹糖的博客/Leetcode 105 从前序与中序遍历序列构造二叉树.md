# Leetcode 105. 从前序与中序遍历序列构造二叉树 - 彩虹糖的博客 - CSDN博客





2019年03月09日 17:21:20[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：16








# 题目

根据一棵树的前序遍历与中序遍历构造二叉树。

**注意:**

你可以假设树中没有重复的元素。

例如，给出

前序遍历 preorder = [3,9,20,15,7]
中序遍历 inorder = [9,3,15,20,7]
返回如下的二叉树：

    3
   / \
  9  20
    /  \
   15   7
# 题目分析

根据一棵树的前序遍历和中序遍历情况画出该树，是离散数学和数据结构课程中都讲过的内容。前序遍历的顺序是：根->左子树->右子树。而中序遍历的顺序是：左子树->根->右子树。因此，我们先看中序遍历的情况，比如第一个元素是3，这就是根，而它的左子树和右子树的情况可以看中序遍历的情况，以3为分界，左侧的值即为左子树，右侧的值即为右子树。那么怎么确认子树的根节点是哪一个呢？这个也很简单，比如我们看3的右子树，在前序遍历中，{15,20,7}中最先出现的是20，因此20就是右子树的根节点，即root.right为20。掌握了这个方法，我们就可以通过递归的思路解决这个问题。

# 代码

```java
/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {

    public TreeNode root;

    public int findIndex(int[] array, int target) {
        for (int i = 0; i < array.length; i++) {
            if (target == array[i])
                return i;
        }
        return -1;
    }

    public boolean contains(int[] array, int target) {
        for (int i = 0; i < array.length; i++) {
            if (target == array[i])
                return true;
        }
        return false;
    }

    public void buildTree(TreeNode root, int[] curArray, int[] preorder) {
        int index = findIndex(curArray, root.val);
        int[] leftArray = Arrays.copyOfRange(curArray, 0, index);
        int[] rightArray = Arrays.copyOfRange(curArray, index + 1, curArray.length);
        TreeNode leftNode = null;
        TreeNode rightNode = null;
        if (leftArray.length > 0) {
            for (int i : preorder) {
                if (contains(leftArray, i)) {
                    leftNode = new TreeNode(i);
                    break;
                }
            }
            root.left = leftNode;
            buildTree(leftNode, leftArray, preorder);
        }
        if (rightArray.length > 0) {
            for (int i : preorder) {
                if (contains(rightArray, i)) {
                    rightNode = new TreeNode(i);
                    break;
                }
            }
            root.right = rightNode;
            buildTree(rightNode, rightArray, preorder);
        }
    }

    public TreeNode buildTree(int[] preorder, int[] inorder) {
        if(preorder.length==0)
            return null;
        root = new TreeNode(preorder[0]);
        buildTree(root, inorder, preorder);
        return root;

    }
}
```

这个代码跑了2000多ms，效率极低，我们对其做一个小小的优化。在上述代码中，寻找某一棵子树最先出现的节点的时候，始终使用的是原先的前序遍历的数组。事实上，一旦确定了某个根节点，这个根节点之后是绝对不会再使用了，因此我们可以使用arraylist存储，不断删除这些已经确定的根节点，从而提高效率。

```java
/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {

    public TreeNode root;

    public int findIndex(int[] array, int target) {
        for (int i = 0; i < array.length; i++) {
            if (target == array[i])
                return i;
        }
        return -1;
    }

    public boolean contains(int[] array, int target) {
        for (int i = 0; i < array.length; i++) {
            if (target == array[i])
                return true;
        }
        return false;
    }

    public void buildTree(TreeNode root, int[] curArray, ArrayList<Integer> preorder) {
        int index = findIndex(curArray, root.val);
        int[] leftArray = Arrays.copyOfRange(curArray, 0, index);
        int[] rightArray = Arrays.copyOfRange(curArray, index + 1, curArray.length);
        TreeNode leftNode = null;
        TreeNode rightNode = null;
        if (leftArray.length > 0) {
            for (int i : preorder) {
                if (contains(leftArray, i)) {
                    leftNode = new TreeNode(i);
                    break;
                }
            }
            root.left = leftNode;
            preorder.remove(preorder.indexOf(leftNode.val));
            buildTree(leftNode, leftArray, preorder);
        }
        if (rightArray.length > 0) {
            for (int i : preorder) {
                if (contains(rightArray, i)) {
                    rightNode = new TreeNode(i);
                    break;
                }
            }
            root.right = rightNode;
            preorder.remove(preorder.indexOf(rightNode.val));
            buildTree(rightNode, rightArray, preorder);
        }
    }

    public TreeNode buildTree(int[] preorder, int[] inorder) {
        ArrayList<Integer> pre = new ArrayList<Integer>();
        for(int i:preorder)
            pre.add(i);
        if(preorder.length==0)
            return null;
        root = new TreeNode(preorder[0]);
        buildTree(root, inorder, pre);
        return root;

    }
}
```

优化后的代码运行完毕仅需要27ms，这说明我们要时时刻刻注意代码的优化，有时候几行的优化可能会代码效率百倍计的提升。




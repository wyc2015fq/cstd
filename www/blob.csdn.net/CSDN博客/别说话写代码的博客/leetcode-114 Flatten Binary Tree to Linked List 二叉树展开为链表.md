# leetcode-114. Flatten Binary Tree to Linked List 二叉树展开为链表 - 别说话写代码的博客 - CSDN博客





2019年01月26日 22:02:47[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19标签：[Flatten Binary Tree to Linked 																[leetcode																[二叉树展开为链表](https://so.csdn.net/so/search/s.do?q=二叉树展开为链表&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given a binary tree, flatten it to a linked list in-place.For example, given the following tree:    1   / \  2   5 / \   \3   4   6The flattened tree should look like:1 \  2   \    3     \      4       \        5         \          6|给定一个二叉树，[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95/8010757)将它展开为链表。例如，给定二叉树    1   / \  2   5 / \   \3   4   6将其展开为：1 \  2   \    3     \      4       \        5         \          6|
|----|----|

思路：

**第一种**：递归遍历二叉树，先序遍历二叉树。由题目给出的意思就是按先序遍历（从左到右）序列将每个节点的左子树放到节点 和右子树之间。 那就可以先遍历到最左边节点， 将其放到它的父节点与父节点右子树之间，然后一直递归将每个节点的左子树放到节点和 右子树之间。

![](https://img-blog.csdnimg.cn/20190126215211168.png)

```cpp
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
    void flatten(TreeNode* root) {
        if(!root) return ;
        if(root->left) flatten(root->left);    //递归遍历左右子树
        if(root->right) flatten(root->right);
        TreeNode* tmp = root->right;    //临时节点指向左子树
        root->right = root->left;        //改变指针指向
        root->left = NULL;  
        while(root->right) root=root->right;    //找到左子树的最右子节点
        root->right = tmp;    //将根节点右子树连到原来左子节点后面
    }
};
```

**第二种**： 这种思路刚好和上面相反，从右往左来，先判断跟节点是否有左子树，如果有就将其挂到跟节点与右子树之间，然后依次往后遍历

![](https://img-blog.csdnimg.cn/20190126220232912.png)

```cpp
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
    void flatten(TreeNode* root) {
        TreeNode* tree = root;
        while(tree)
        {
            if(tree->left)
            {
                TreeNode* tmp=tree->left;   //临时节点指向当前节点左子节点
                while(tmp->right) tmp=tmp->right;   //遍历到左子树的最右节点
                tmp->right = tree->right;      //将其接到根节点与右子树之间
                tree->right = tree->left;
                tree->left = NULL;
            }
            tree = tree->right;    //一直往后遍历
        }
    }
};
```

参考：[http://www.cnblogs.com/grandyang/p/4293853.html](http://www.cnblogs.com/grandyang/p/4293853.html)](https://so.csdn.net/so/search/s.do?q=Flatten Binary Tree to Linked &t=blog)





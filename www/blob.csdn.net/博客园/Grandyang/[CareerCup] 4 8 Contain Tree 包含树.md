# [CareerCup] 4.8 Contain Tree 包含树 - Grandyang - 博客园







# [[CareerCup] 4.8 Contain Tree 包含树](https://www.cnblogs.com/grandyang/p/4712753.html)







4.8 You have two very large binary trees: Tl, with millions of nodes, and T2, with hundreds of nodes. Create an algorithm to decide if T2 is a subtree of Tl. A tree T2 is a subtree of Tl if there exists a node n in Tl such that the subtree of n is identical to T2. That is, if you cut off the tree at node n, the two trees would be identical.



这道题给我们两棵树，T1和T2，其中T1有百万个节点，T2有上百个节点，让我们写算法来判断T2是不是T1的子树。首先可以用的方法是，我们对两棵树分别进行前序和中序遍历，然后把结果分别存入字符串中，如果T2的前序和中序遍历的字符串分别是T1的前序和中序遍历的字符串的子串，那么我们可以判定T2是T1的子树，参见代码如下：



解法一：

```
class Solution {
public:
    bool containTree(TreeNode *root1, TreeNode *root2) {
        string pre1, pre2, in1, in2;
        preorder(root1, pre1);
        preorder(root2, pre2);
        inorder(root1, in1);
        inorder(root2, in2);
        if (pre1.find(pre2) == pre1.size() - pre2.size() && in1.find(in2) == in1.size() - in2.size()) return true;
        else return false;
    }
    void preorder(TreeNode *root, string &res) {
        if (!root) return;
        res.append(to_string(root->val));
        preorder(root->left, res);
        preorder(root->right, res);
    }
    void inorder(TreeNode *root, string &res) {
        if (!root) return;
        inorder(root->left, res);
        res.append(to_string(root->val));
        inorder(root->right, res);
    }
};
```



但是上面这种解法存在例外情况无法正确分辨，比如下面的两棵树：

     3                       3

   /           and            \

3                                 3

它们的中序和前序遍历都相同，所以上述算法会返回true，但我们知道它们是两个不同的树，谁也不包含谁，谁也不是谁的子树。Cracking the Coding Interview 5th Edition这书上第235页上说我们可以标记处空节点，但这种方法麻烦，又占空间，所以这里我不去写它。下面来看书上给出的另一种解法，这种解法的思路是，我们首先判断T2是否为空，为空则直接返回True，因为空树是任何树的子树，然后我们看T1是否为空，T1为空直接返回false，因为空树不可能有非空子树，然后我们看两个根节点的值是否相同，如果相同，则调用matchTree方法，来比较整个T2树，如果完全匹配则返回true，否则继续往下递归，参见代码如下：



解法二：

```
class Solution {
public:
    bool containTree(TreeNode *root1, TreeNode *root2) {
        if (!root2) return true;
        if (!root1) return false;
        if (root1->val == root2->val) {
            if (matchTree(root1, root2)) return true;
        }
        return containTree(root1->left, root2) || containTree(root1->right, root2);
    }
    bool matchTree(TreeNode *root1, TreeNode *root2) {
        if (!root1 && !root2) return true;
        if (!root1 || !root2) return false;
        if (root1->val != root2->val) return false;
        else (matchTree(root1->left, root2->left) && matchTree(root1->right, root2->right));
    }
};
```















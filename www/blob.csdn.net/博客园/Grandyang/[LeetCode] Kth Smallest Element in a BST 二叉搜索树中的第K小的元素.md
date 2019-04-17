# [LeetCode] Kth Smallest Element in a BST 二叉搜索树中的第K小的元素 - Grandyang - 博客园







# [[LeetCode] Kth Smallest Element in a BST 二叉搜索树中的第K小的元素](https://www.cnblogs.com/grandyang/p/4620012.html)







Given a binary search tree, write a function `kthSmallest` to find the **k**th smallest element in it.

**Note: **

You may assume k is always valid, 1 ≤ k ≤ BST's total elements.

**Follow up:**

What if the BST is modified (insert/delete operations) often and you 
need to find the kth smallest frequently? How would you optimize the 
kthSmallest routine?

**Hint:**
- Try to utilize the property of a BST.
- What if you could modify the BST node's structure?
- The optimal runtime complexity is O(height of BST).

**Credits:**
Special thanks to [@ts](https://leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这又是一道关于[二叉搜索树](http://zh.wikipedia.org/wiki/%E4%BA%8C%E5%85%83%E6%90%9C%E5%B0%8B%E6%A8%B9) Binary Search Tree 的题， LeetCode中关于BST的题有[Validate Binary Search Tree 验证二叉搜索树](http://www.cnblogs.com/grandyang/p/4298435.html)， [Recover Binary Search Tree 复原二叉搜索树](http://www.cnblogs.com/grandyang/p/4298069.html)， [Binary Search Tree Iterator 二叉搜索树迭代器](http://www.cnblogs.com/grandyang/p/4231455.html)， [Unique Binary Search Trees 独一无二的二叉搜索树](http://www.cnblogs.com/grandyang/p/4299608.html)， [Unique Binary Search Trees II 独一无二的二叉搜索树之二](http://www.cnblogs.com/grandyang/p/4301096.html)，[Convert Sorted Array to Binary Search Tree 将有序数组转为二叉搜索树](http://www.cnblogs.com/grandyang/p/4295245.html) 和 [Convert Sorted List to Binary Search Tree 将有序链表转为二叉搜索树](http://www.cnblogs.com/grandyang/p/4295618.html)。

那么这道题给的提示是让我们用BST的性质来解题，最重要的性质是就是左<根<右，那么如果用中序遍历所有的节点就会得到一个有序数组。所以解题的关键还是中序遍历啊。关于二叉树的中序遍历可以参见我之前的博客[Binary Tree Inorder Traversal 二叉树的中序遍历](http://www.cnblogs.com/grandyang/p/4297300.html)，里面有很多种方法可以用，我们先来看一种非递归的方法，中序遍历最先遍历到的是最小的结点，那么我们只要用一个计数器，每遍历一个结点，计数器自增1，当计数器到达k时，返回当前结点值即可，参见代码如下：



解法一：

```
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        int cnt = 0;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            p = s.top(); s.pop();
            ++cnt;
            if (cnt == k) return p->val;
            p = p->right;
        }
        return 0;
    }
};
```



当然，此题我们也可以用递归来解，还是利用中序遍历来解，代码如下：



解法二：

```
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        return kthSmallestDFS(root, k);
    }
    int kthSmallestDFS(TreeNode* root, int &k) {
        if (!root) return -1;
        int val = kthSmallestDFS(root->left, k);
        if (k == 0) return val;
        if (--k == 0) return root->val;
        return kthSmallestDFS(root->right, k);
    }
};
```



再来看一种分治法的思路，由于BST的性质，我们可以快速定位出第k小的元素是在左子树还是右子树，我们首先计算出左子树的结点个数总和cnt，如果k小于等于左子树结点总和cnt，说明第k小的元素在左子树中，直接对左子结点调用递归即可。如果k大于cnt+1，说明目标值在右子树中，对右子结点调用递归函数，注意此时的k应为k-cnt-1，应为已经减少了cnt+1个结点。如果k正好等于cnt+1，说明当前结点即为所求，返回当前结点值即可，参见代码如下：



解法三：

```
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        int cnt = count(root->left);
        if (k <= cnt) {
            return kthSmallest(root->left, k);
        } else if (k > cnt + 1) {
            return kthSmallest(root->right, k - cnt - 1);
        }
        return root->val;
    }
    int count(TreeNode* node) {
        if (!node) return 0;
        return 1 + count(node->left) + count(node->right);
    }
};
```



这道题的Follow up中说假设该BST被修改的很频繁，而且查找第k小元素的操作也很频繁，问我们如何优化。其实最好的方法还是像上面的解法那样利用分治法来快速定位目标所在的位置，但是每个递归都遍历左子树所有结点来计算个数的操作并不高效，所以我们应该修改原树结点的结构，使其保存包括当前结点和其左右子树所有结点的个数，这样我们使用的时候就可以快速得到任何左子树结点总数来帮我们快速定位目标值了。定义了新结点结构体，然后就要生成新树，还是用递归的方法生成新树，注意生成的结点的count值要累加其左右子结点的count值。然后在求第k小元素的函数中，我们先生成新的树，然后调用递归函数。在递归函数中，不能直接访问左子结点的count值，因为左子节结点不一定存在，所以我们先判断，如果左子结点存在的话，那么跟上面解法的操作相同。如果不存在的话，当此时k为1的时候，直接返回当前结点值，否则就对右子结点调用递归函数，k自减1，参见代码如下：



解法四：

```
// Follow up
class Solution {
public:
    struct MyTreeNode {
        int val;
        int count;
        MyTreeNode *left;
        MyTreeNode *right;
        MyTreeNode(int x) : val(x), count(1), left(NULL), right(NULL) {}
    };
    
    MyTreeNode* build(TreeNode* root) {
        if (!root) return NULL;
        MyTreeNode *node = new MyTreeNode(root->val);
        node->left = build(root->left);
        node->right = build(root->right);
        if (node->left) node->count += node->left->count;
        if (node->right) node->count += node->right->count;
        return node;
    }
    
    int kthSmallest(TreeNode* root, int k) {
        MyTreeNode *node = build(root);
        return helper(node, k);
    }
    
    int helper(MyTreeNode* node, int k) {
        if (node->left) {
            int cnt = node->left->count;
            if (k <= cnt) {
                return helper(node->left, k);
            } else if (k > cnt + 1) {
                return helper(node->right, k - 1 - cnt);
            }
            return node->val;
        } else {
            if (k == 1) return node->val;
            return helper(node->right, k - 1);
        }
    }
};
```



类似题目：

[Binary Tree Inorder Traversal](http://www.cnblogs.com/grandyang/p/4297300.html)

[Second Minimum Node In a Binary Tree](http://www.cnblogs.com/grandyang/p/7590156.html)



参考资料：

[https://discuss.leetcode.com/topic/17668/what-if-you-could-modify-the-bst-node-s-structure](https://discuss.leetcode.com/topic/17668/what-if-you-could-modify-the-bst-node-s-structure)

[https://discuss.leetcode.com/topic/17810/3-ways-implemented-in-java-python-binary-search-in-order-iterative-recursive](https://discuss.leetcode.com/topic/17810/3-ways-implemented-in-java-python-binary-search-in-order-iterative-recursive)

[https://discuss.leetcode.com/topic/32792/java-divide-and-conquer-solution-considering-augmenting-tree-structure-for-the-follow-up](https://discuss.leetcode.com/topic/32792/java-divide-and-conquer-solution-considering-augmenting-tree-structure-for-the-follow-up)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













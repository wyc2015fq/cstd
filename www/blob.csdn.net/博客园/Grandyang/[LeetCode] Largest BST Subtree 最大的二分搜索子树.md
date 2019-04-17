# [LeetCode] Largest BST Subtree 最大的二分搜索子树 - Grandyang - 博客园







# [[LeetCode] Largest BST Subtree 最大的二分搜索子树](https://www.cnblogs.com/grandyang/p/5188938.html)







Given a binary tree, find the largest subtree which is a Binary Search Tree (BST), where largest means subtree with largest number of nodes in it.

Note:
A subtree must include all of its descendants.
Here's an example:
    10
    / \
   5  15
  / \   \ 
 1   8   7

The Largest BST Subtree in this case is the highlighted one. 
The return value is the subtree's size, which is 3.

Hint:
- You can recursively use algorithm similar to [98. Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/) at each node of the tree, which will result in O(nlogn) time complexity.

Follow up:
Can you figure out ways to solve it with O(n) time complexity?



这道题让我们求一棵二分树的最大二分搜索子树，所谓二分搜索树就是满足左<根<右的二分树，我们需要返回这个二分搜索子树的节点个数。题目中给的提示说我们可以用之前那道 [Validate Binary Search Tree](http://www.cnblogs.com/grandyang/p/4298435.html) 的方法来做，时间复杂度为O(n2)，这种方法是把每个节点都当做根节点，来验证其是否是二叉搜索数，并记录节点的个数，若是二叉搜索树，就更新最终结果，参见代码如下：



解法一：

```
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        int res = 0;
        dfs(root, res);
        return res;
    }
    void dfs(TreeNode *root, int &res) {
        if (!root) return;
        int d = countBFS(root, INT_MIN, INT_MAX);
        if (d != -1) {
            res = max(res, d);
            return;
        }
        dfs(root->left, res);
        dfs(root->right, res);
    }
    int countBFS(TreeNode *root, int mn, int mx) {
        if (!root) return 0;
        if (root->val <= mn || root->val >= mx) return -1;
        int left = countBFS(root->left, mn, root->val);
        if (left == -1) return -1;
        int right = countBFS(root->right, root->val, mx);
        if (right == -1) return -1;
        return left + right + 1;
    }
};
```



下面我们来看一种更简洁的写法，对于每一个节点，都来验证其是否是BST，如果是的话，我们就统计节点的个数即可，参见代码如下：



解法二：

```
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        if (!root) return 0;
        if (isValid(root, INT_MIN, INT_MAX)) return count(root);
        return max(largestBSTSubtree(root->left), largestBSTSubtree(root->right));
    }
    bool isValid(TreeNode* root, int mn, int mx) {
        if (!root) return true;
        if (root->val <= mn || root->val >= mx) return false;
        return isValid(root->left, mn, root->val) && isValid(root->right, root->val, mx);
    }
    int count(TreeNode* root) {
        if (!root) return 0;
        return count(root->left) + count(root->right) + 1;
    }
};
```



题目中的Follow up让我们用O(n)的时间复杂度来解决问题，我们还是采用DFS的思想来解题，由于时间复杂度的限制，只允许我们遍历一次整个二叉树，由于满足题目要求的二叉搜索子树必定是有叶节点的，所以我们的思路就是先递归到最左子节点，然后逐层往上递归，对于每一个节点，我们都记录当前最大的BST的节点数，当做为左子树的最大值，和做为右子树的最小值，当每次遇到左子节点不存在或者当前节点值大于左子树的最大值，且右子树不存在或者当前节点值小于右子树的最小数时，说明BST的节点数又增加了一个，我们更新结果及其参数，如果当前节点不是BST的节点，那么我们更新BST的节点数res为左右子节点的各自的BST的节点数的较大值，参见代码如下：



解法三：

```
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        int res = 0, mn = INT_MIN, mx = INT_MAX;
        isValidBST(root, mn, mx, res);
        return res;
    }
    void isValidBST(TreeNode* root, int& mn, int& mx, int& res) {
        if (!root) return;
        int left_cnt = 0, right_cnt = 0, left_mn = INT_MIN;
        int right_mn = INT_MIN, left_mx = INT_MAX, right_mx = INT_MAX;
        isValidBST(root->left, left_mn, left_mx, left_cnt);
        isValidBST(root->right, right_mn, right_mx, right_cnt);
        if ((!root->left || root->val > left_mx) && (!root->right || root->val < right_mn)) {
            res = left_cnt + right_cnt + 1;
            mn = root->left ? left_mn : root->val;
            mx = root->right ? right_mx : root->val;
        } else {
            res = max(left_cnt, right_cnt);    
        }
    }
};
```



上面的解法在递归函数中定义了大量的变量，难免让人看的眼花缭乱，我们可以稍稍精简一下，将这些变量都放到递归函数的返回值中，此时我们的helper函数返回了一个一维数组，里面有三个数字，分别是以当前结点为根结点的数的最小值，最大值，以及最大的BST子树的结点个数。那么我们就可以在边验证BST的过程中边统计个数，首先判空，若空，则返回一个默认三元组，整型最大值，最小值，和0。那你可能有疑问，定义的不是说第一个值是最小值么？没错，我们后面再解释。若当前结点node存在，我们分别对其左右子结点调用递归函数，那么左子树和右子树的信息都保存到了left和right数组中，就算左右子结点不存在也没关系，由于第一句的判空，我们还是会得到一个默认的三元组。接下来就是根据左右子树的信息来更新结果res了，由于BST的定义，当前结点值肯定是大于左子树的最大值，小于右子树的最小值的。左子树的最大值保存在 left[1] 中，右子树的最小值保存在 right[0] 中，如果这两个条件满足了，说明左右子树都是BST，那么我们返回的三元组的最小值就是当前结点值和左子树最小值中的较小者，最大值就是当前结点值和右子树最大值中的较大值，返回的BST结点个数就是左右子树的结点个数加上1，即算上了当前结点。好，现在解释下为空时返回的三元组为何顺序是整型最大值，整型最小值。如果当前是叶结点，其也算是BST，那么我们肯定希望能进入if从句，从而使得三元组的第三项能加1，但是if的条件是当前结点值要大于左子树中的最大值，现在左子结点是空的，为了保证条件能通过，我们将空的左子树的最大值设置为整型最小值，这样一定能通过，同理，将空的右子树的最小值设置为整型最大值，这就是空结点的三元组的作用。好，继续看else中的内容，如果破坏了BST的规则，那么返回的三元组的最小值就是整型最小值，最大值是整型最大值，BST结点个数并不是0，因为其左右子树中有可能还有BST，所以是左右子树中的BST结点个数中的较大值，参见代码如下：



解法四：

```
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        vector<int> res = helper(root);
        return res[2];
    }
    vector<int> helper(TreeNode* node) {
        if (!node) return {INT_MAX, INT_MIN, 0};
        vector<int> left = helper(node->left), right = helper(node->right);
        if (node->val > left[1] && node->val < right[0]) {
            return {min(node->val, left[0]), max(node->val, right[1]), left[2] + right[2] + 1};
        } else {
            return {INT_MIN, INT_MAX, max(left[2], right[2])};
        }
    }
};
```



类似题目：

[Validate Binary Search Tree](http://www.cnblogs.com/grandyang/p/4298435.html)



参考资料：

[https://leetcode.com/problems/largest-bst-subtree/](https://leetcode.com/problems/largest-bst-subtree/)

[https://leetcode.com/problems/largest-bst-subtree/discuss/78892/12ms-C%2B%2B-solution](https://leetcode.com/problems/largest-bst-subtree/discuss/78892/12ms-C%2B%2B-solution)

[https://leetcode.com/problems/largest-bst-subtree/discuss/78899/Very-Short-Simple-Java-O(N)-Solution](https://leetcode.com/problems/largest-bst-subtree/discuss/78899/Very-Short-Simple-Java-O(N)-Solution)

[https://leetcode.com/problems/largest-bst-subtree/discuss/78896/Clean-and-easy-to-understand-Java-Solution](https://leetcode.com/problems/largest-bst-subtree/discuss/78896/Clean-and-easy-to-understand-Java-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Maximum Width of Binary Tree 二叉树的最大宽度 - Grandyang - 博客园







# [[LeetCode] Maximum Width of Binary Tree 二叉树的最大宽度](https://www.cnblogs.com/grandyang/p/7538821.html)







Given a binary tree, write a function to get the maximum width of the given tree. The width of a tree is the maximum width among all levels. The binary tree has the same structure as a full binary tree, but some nodes are null.

The width of one level is defined as the length between the end-nodes (the leftmost and right most non-null nodes in the level, where the `null` nodes between the end-nodes are also counted into the length calculation.

Example 1:
Input: 

           1
         /   \
        3     2
       / \     \  
      5   3     9 

Output: 4
Explanation: The maximum width existing in the third level with the length 4 (5,3,null,9).

Example 2:
Input: 

          1
         /  
        3    
       / \       
      5   3     

Output: 2
Explanation: The maximum width existing in the third level with the length 2 (5,3).

Example 3:
Input: 

          1
         / \
        3   2 
       /        
      5      

Output: 2
Explanation: The maximum width existing in the second level with the length 2 (3,2).

Example 4:
Input: 

          1
         / \
        3   2
       /     \  
      5       9 
     /         \
    6           7
Output: 8
Explanation:The maximum width existing in the fourth level with the length 8 (6,null,null,null,null,null,null,7).



Note: Answer will in the range of 32-bit signed integer.



这道题让我们求二叉树的最大宽度，根据题目中的描述可知，这里的最大宽度不是满树的时候的最大宽度，如果是那样的话，肯定是最后一层的结点数最多。这里的最大宽度应该是两个存在的结点中间可容纳的总的结点个数，中间的结点可以为空。那么其实只要我们知道了每一层中最左边和最右边的结点的位置，我们就可以算出这一层的宽度了。所以这道题的关键就是要记录每一层中最左边结点的位置，我们知道对于一棵完美二叉树，如果根结点是深度1，那么每一层的结点数就是2*n-1，那么每个结点的位置就是[1, 2*n-1]中的一个，假设某个结点的位置是i，那么其左右子结点的位置可以直接算出来，为2*i和2*i+1，可以自行带例子检验。由于之前说过，我们需要保存每一层的最左结点的位置，那么我们使用一个数组start，由于数组是从0开始的，我们就姑且认定根结点的深度为0，不影响结果。我们从根结点进入，深度为0，位置为1，进入递归函数。

首先判断，如果当前结点为空，那么直接返回，然后判断如果当前深度大于start数组的长度，说明当前到了新的一层的最左结点，我们将当前位置存入start数组中。然后我们用idx - start[h] + 1来更新结果res。这里idx是当前结点的位置，start[h]是当前层最左结点的位置。然后对左右子结点分别调用递归函数，注意左右子结点的位置可以直接计算出来，参见代码如下：



解法一：

```
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        int res = 0;
        vector<int> start;
        helper(root, 0, 1, start, res);
        return res;
    }
    void helper(TreeNode* node, int h, int idx, vector<int>& start, int& res) {
        if (!node) return;
        if (h >= start.size()) start.push_back(idx);
        res = max(res, idx - start[h] + 1);
        helper(node->left, h + 1, idx * 2, start, res);
        helper(node->right, h + 1, idx * 2 + 1, start, res);
    }
};
```



下面这种解法还是递归，比上面的解法稍微简洁一些，没有用结果res变量，而是递归函数直接返回最大宽度了，但是解题思路没有啥区别，参见代码如下：



解法二：

```
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        vector<int> start;
        return helper(root, 0, 1, start);
    }
    int helper(TreeNode* node, int h, int idx, vector<int>& start) {
        if (!node) return 0;
        if (h >= start.size()) start.push_back(idx);
        return max({idx - start[h] + 1, helper(node->left, h + 1, idx * 2, start), helper(node->right, h + 1, idx * 2 + 1, start)});
    }
};
```



下面这个解法用的是层序遍历，迭代的方法来写的，注意这里使用了队列queue来辅助运算，queue里存的是一个pair，结点和其当前位置，在进入新一层的循环时，首先将首结点的位置保存出来当作最左位置，然后对于遍历到的结点，都更新右结点的位置，遍历一层的结点后来计算宽度更新结果res，参见代码如下：



解法三：

```
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        queue<pair<TreeNode*,int>> q;
        q.push({root, 1});
        while (!q.empty()) {
            int left = q.front().second, right = left, n = q.size();
            for (int i = 0; i < n; ++i) {
                auto t = q.front().first; 
                right = q.front().second; q.pop();
                if (t->left) q.push({t->left, right * 2});
                if (t->right) q.push({t->right, right * 2 + 1});
            }
            res = max(res, right - left + 1);
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/100145/java-c-very-simple-dfs-solution](https://discuss.leetcode.com/topic/100145/java-c-very-simple-dfs-solution)

[https://discuss.leetcode.com/topic/100149/c-java-bfs-dfs-3liner-clean-code-with-explanation](https://discuss.leetcode.com/topic/100149/c-java-bfs-dfs-3liner-clean-code-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













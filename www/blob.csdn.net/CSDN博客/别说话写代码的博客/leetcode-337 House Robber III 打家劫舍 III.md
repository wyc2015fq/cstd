# leetcode-337. House Robber III 打家劫舍 III - 别说话写代码的博客 - CSDN博客





2019年01月27日 13:23:25[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[打家劫舍 III																[House Robber III																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=House Robber III&t=blog)



|The thief has found himself a new place for his thievery again. There is only one entrance to this area, called the "root." Besides the root, each house has one and only one parent house. After a tour, the smart thief realized that "all houses in this place forms a binary tree". It will automatically contact the police if two directly-linked houses were broken into on the same night.Determine the maximum amount of money the thief can rob tonight without alerting the police.**Example 1:****Input: **[3,2,3,null,3,null,1]     3    / \   2   3    \   \      3   1**Output:** 7 **Explanation:** Maximum amount of money the thief can rob = 3 + 3 + 1 = **7**.**Example 2:****Input: **[3,4,5,1,3,null,1]     3    / \   45  / \   \  1   3   1**Output:** 9**Explanation:** Maximum amount of money the thief can rob = 4 + 5 = **9**.|在上次打劫完一条街道之后和一圈房屋后，小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为“根”。 除了“根”之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果两个直接相连的房子在同一天晚上被打劫，房屋将自动报警。计算在不触动警报的情况下，小偷一晚能够盗取的最高金额。**示例 1:****输入: **[3,2,3,null,3,null,1]     **3**    / \   2   3    \   \      **3****1****输出:** 7 **解释:** 小偷一晚能够盗取的最高金额 = 3 + 3 + 1 = **7**.**示例 2:****输入: **[3,4,5,1,3,null,1]     3    / \   **4****5**  / \   \  1   3   1**输出:** 9**解释:** 小偷一晚能够盗取的最高金额 = **4** + **5** = **9**.|
|----|----|

思路：这种树的题目如果没有看明白，一定要想到递归思路，利用回溯法做。题目给的限制条件是打劫位置的父节点与 子节点不能挨着，所以可能有 多重情况，比如下图为例，可能情况有4-2,1-3,4-3,很明显这棵树盗取的 最大金额是7.

![](https://img-blog.csdnimg.cn/20190127125721840.png)

**第一种**：树从上往下递归会重复计算很多节点，所以我们可以用个哈希表 将其保存起来。对某一节点，如果其左子节点存在，通过递归调用函数，算出不包含左子节点的返回值，同理如果右子节点存在，通过递归调用函数，算出不包含右子节点的返回值。那么此节点的值有两种可能，一种是该节点的值加上不包含左子节点和右子节点的返回值之和，另一种是其左子节点 和右子节点的 返回值之和，然后当前节点的值为这两种的最大值。

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
    int rob(TreeNode* root) {
        unordered_map<TreeNode *,int> m;
        return dfs(root,m);
    }
    int dfs(TreeNode *root,unordered_map<TreeNode*,int> &m)
    {
        if(!root) return 0;
        if(m.count(root)) return m[root];
        int value=0;
        if(root->left)
            value+=dfs(root->left->left,m)+dfs(root->left->right,m);
        if(root->right)
            value+=dfs(root->right->left,m)+dfs(root->right->right,m);
        m[root]=max(value+root->val,dfs(root->left,m)+dfs(root->right,m));
        return m[root];
    }
};
```

第二种：使用简单递归，dfs函数返回值就是当前能打劫到的最大金额。l和r分别表示能从左子节点和 右子节点打劫到 的最大金额。dfs函数内的ll和lr，rl和rr分别表示左子节点的左子节点，左子节点的右子节点，右子节点的左子节点，右子节点的右子节点能打劫的最大钱数，返回值为（抢了当前房屋和左子节点和右子节点的子树）和（抢了左子节点和右子节点）之间的最大值。

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
    int rob(TreeNode* root) {
        int l=0,r=0;
        return dfs(root,l,r);
    }
    int dfs(TreeNode* root,int &l,int &r)
    {
        if(!root) return 0;
        int ll=0,lr=0,rl=0,rr=0;
        l=dfs(root->left,ll,lr);
        r=dfs(root->right,rl,rr);
        return max(root->val+ll+lr+rr+rl,l+r);
    }
};
```](https://so.csdn.net/so/search/s.do?q=打家劫舍 III&t=blog)





# LeetCode 958. 二叉树的完全性检验 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 958. 二叉树的完全性检验

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-115/problems/check-completeness-of-a-binary-tree/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-115/)







- 用户通过次数119
- 用户尝试次数157
- 通过次数123
- 提交次数378
- 题目难度Medium



给定一个二叉树，确定它是否是一个*完全二叉树*。

[百度百科](https://baike.baidu.com/item/%E5%AE%8C%E5%85%A8%E4%BA%8C%E5%8F%89%E6%A0%91/7773232?fr=aladdin)中对完全二叉树的定义如下：

若设二叉树的深度为 h，除第 h 层外，其它各层 (1～h-1) 的结点数都达到最大个数，第 h 层所有的结点都连续集中在最左边，这就是完全二叉树。（注：第 h 层可能包含 1~ 2h 个节点。）



示例 1：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/15/complete-binary-tree-1.png)
输入：[1,2,3,4,5,6]
输出：true
解释：最后一层前的每一层都是满的（即，结点值为 {1} 和 {2,3} 的两层），且最后一层中的所有结点（{4,5,6}）都尽可能地向左。

示例 2：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/15/complete-binary-tree-2.png)
输入：[1,2,3,4,5,null,7]
输出：false
解释：值为 7 的结点没有尽可能靠向左侧。



提示：
- 树中将会有 1 到 100 个结点。



```
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
    bool isCompleteTree(TreeNode* root) {
        queue<TreeNode*>que;
        que.push(root);
        int flag = 1;   //flag == 0 表示到最后一个节点了，再也不能往队列里加节点了。
        while(!que.empty()){
            TreeNode* t = que.front();que.pop();
            if(flag == 1){
                if(t->left == NULL && t->right != NULL) return false;  //题中情况
                if(t->left == NULL || t->right == NULL) flag = 0;
                if(t->left) que.push(t->left);
                if(t->right) que.push(t->right);
            }
            else{
                if(t->left != NULL||t->right != NULL)return false;
            }
            
        }
        return true;
    }
};
```

————今天脑子有点不好使，贼傻逼。















# Leetcode 1008. 先序遍历构造二叉树 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1008. 先序遍历构造二叉树

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-127/problems/construct-binary-search-tree-from-preorder-traversal/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-127/)







- 用户通过次数169
- 用户尝试次数183
- 通过次数171
- 提交次数247
- 题目难度Medium



返回与给定先序遍历 `preorder` 相匹配的二叉搜索树（binary search tree）的根结点。

*(回想一下，二叉搜索树是二叉树的一种，其每个节点都满足以下规则，对于 `node.left` 的任何后代，值总 `<``node.val`，而 `node.right` 的任何后代，值总 `>``node.val`。此外，先序遍历首先显示节点的值，然后遍历 `node.left`，接着遍历 `node.right`。）*



示例：
输入：[8,5,1,7,10,12]
输出：[8,5,10,1,7,null,12]
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/03/08/1266.png)


提示：
- `1 <= preorder.length <= 100`
- 先序 `preorder` 中的值是不同的。

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
    
    TreeNode* dfs(vector<int> v,int start,int end){
        if(start > end) return NULL;
        TreeNode* p = new TreeNode(v[start]);
        if(start == end) return p;
        int cnt=start;
        for(int i=start+1;i <= end;i++){
            if(v[i]<v[start])cnt = i;
            else break;
        }
        p->left = dfs(v,start+1,cnt);
        p->right = dfs(v,cnt+1,end);
        return p;
    }
    
    
    TreeNode* bstFromPreorder(vector<int>& preorder) {
        return dfs(preorder,0,preorder.size()-1);
    }
};
```

_我写的和大佬写的

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
    TreeNode* bstFromPreorder(vector<int>& a) {
        int n = a.size();
        if (n == 0) return NULL;
        TreeNode *it = new TreeNode(a[0]);
        vector<int> L, R;
        for (auto x : a)
        {
            if (x < a[0]) L.push_back(x);
            if (x > a[0]) R.push_back(x);
        }
        it->left = bstFromPreorder(L);
        it->right = bstFromPreorder(R);
        return it;
    }
};
```

——这写法也太吊了吧，太美了！















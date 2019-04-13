
# 【LeetCode】104. Maximum Depth of Binary Tree（DFS|BFS） - jiange_zh的博客 - CSDN博客


2016年01月07日 10:05:08[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：907标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[bfs																](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)[dfs																](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)[递归																](https://so.csdn.net/so/search/s.do?q=递归&t=blog)[非递归																](https://so.csdn.net/so/search/s.do?q=非递归&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=递归&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)
[
																								](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)

## 原题
Given a binary tree, find its maximum depth.
The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
## 分析
很简单的基础题，可以使用BFS, DFS来实现。
主要可以用来练习非递归方式的编写。
## 我的解答
C++ BFS版本：
```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
```
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
int
```
```python
bfs(
```
```python
struct
```
```python
TreeNode* root) {
```
```python
int
```
```python
ans =
```
```python
1
```
```python
;
```
```python
queue
```
```python
< pair<TreeNode*,
```
```python
int
```
```python
>
```
```python
> q;
        q.push({root,
```
```python
1
```
```python
});
```
```python
while
```
```python
(!q.empty()) {
            TreeNode *cur = q.front().first;
```
```python
int
```
```python
dep = q.front().second;
            q.pop();
```
```python
if
```
```python
(dep > ans)  ans = dep;
```
```python
if
```
```python
(cur->left)  q.push({cur->left, dep+
```
```python
1
```
```python
});
```
```python
if
```
```python
(cur->right) q.push({cur->right, dep+
```
```python
1
```
```python
});
        }
```
```python
return
```
```python
ans;
    }
```
```python
int
```
```python
maxDepth(TreeNode* root) {
```
```python
if
```
```python
(root)
```
```python
return
```
```python
bfs(root);
```
```python
return
```
```python
0
```
```python
;
    }
};
```
C++ DFS非递归版本：
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
int
```
```python
dfs(
```
```python
struct
```
```python
TreeNode* root) {
```
```python
int
```
```python
ans =
```
```python
1
```
```python
;
```
```python
stack
```
```python
< pair<TreeNode*,
```
```python
int
```
```python
>
```
```python
> s;
        s.push({root,
```
```python
1
```
```python
});
```
```python
while
```
```python
(!s.empty()) {
            TreeNode *cur = s.top().first;
```
```python
int
```
```python
dep = s.top().second;
            s.pop();
```
```python
if
```
```python
(dep > ans)  ans = dep;
```
```python
if
```
```python
(cur->left)  s.push({cur->left, dep+
```
```python
1
```
```python
});
```
```python
if
```
```python
(cur->right) s.push({cur->right, dep+
```
```python
1
```
```python
});
        }
```
```python
return
```
```python
ans;
    }
```
```python
int
```
```python
maxDepth(TreeNode* root) {
```
```python
if
```
```python
(root)
```
```python
return
```
```python
dfs(root);
```
```python
return
```
```python
0
```
```python
;
    }
};
```
哈哈，有没有发现，其实DFS和BFS的代码基本是一样的，只是一个用队列，一个用栈。
## 网友的解答
C++ DFS递归版：
```python
class Solution {
```
```python
public
```
```python
:
```
```python
int
```
```python
maxDepth
```
```python
(TreeNode* root) {
```
```python
if
```
```python
(root)
```
```python
return
```
```python
1
```
```python
+ max(maxDepth(root->left), maxDepth(root->right));
```
```python
return
```
```python
0
```
```python
;
    }
};
```
Python:
```python
def
```
```python
maxDepth
```
```python
(self, root)
```
```python
:
```
```python
return
```
```python
1
```
```python
+ max(map(self.maxDepth, (root.left, root.right)))
```
```python
if
```
```python
root
```
```python
else
```
```python
0
```
等价于下面：
```python
if
```
```python
root ==
```
```python
None
```
```python
:
```
```python
return
```
```python
0
```
```python
return
```
```python
max(self.maxDepth(root.left),self.maxDepth(root.right)) +
```
```python
1
```


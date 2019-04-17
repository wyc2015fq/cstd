# [LeetCode] Add One Row to Tree 二叉树中增加一行 - Grandyang - 博客园







# [[LeetCode] Add One Row to Tree 二叉树中增加一行](https://www.cnblogs.com/grandyang/p/7070182.html)







Given the root of a binary tree, then value `v` and depth `d`, you need to add a row of nodes with value `v` at the given depth `d`. The root node is at depth 1.

The adding rule is: given a positive integer depth `d`, for each NOT null tree nodes `N` in depth `d-1`, create two tree nodes with value `v` as `N's` left subtree root and right subtree root. And `N's` original left subtree should be the left subtree of the new left subtree root, its original right subtree should be the right subtree of the new right subtree root. If depth `d` is 1 that means there is no depth d-1 at all, then create a tree node with value v as the new root of the whole original tree, and the original tree is the new root's left subtree.

Example 1:
Input: 
A binary tree as following:
       4
     /   \
    2     6
   / \   / 
  3   1 5   

v = 1

d = 2

Output: 
       4
      / \
     1   1
    /     \
   2       6
  / \     / 
 3   1   5   




Example 2:
Input: 
A binary tree as following:
      4
     /   
    2    
   / \   
  3   1    

v = 1

d = 3

Output: 
      4
     /   
    2
   / \    
  1   1
 /     \  
3       1



Note:
- The given d is in range [1, maximum depth of the given tree + 1].
- The given binary tree has at least one tree node.



这道题让我们给二叉树增加一行，给了我们需要增加的值，还有需要增加的位置深度，题目中给的例子也比较能清晰的说明问题。但是漏了一种情况，那就是当d=1时，这该怎么加？这时候就需要替换根结点了。其他情况的处理方法都一样，这里博主第一映像觉得应该用层序遍历来做，没遍历完一层，d自减1，我们探测，当d==1时，那么我们需要对于当前层的每一个结点，首先用临时变量保存其原有的左右子结点，然后新建值为v的左右子结点，将原有的左子结点连到新建的左子结点的左子结点上，将原有的右子结点连到新建的右子结点的右子结点，是不是很绕-.-|||。如果d不为1，那么就是层序遍历原有的排入队列操作，记得当检测到d为0时，直接返回，因为添加操作已经完成，没有必要遍历完剩下的结点，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int v, int d) {
        if (!root) return NULL;
        if (d == 1) {
            TreeNode *newRoot = new TreeNode(v);
            newRoot->left = root;
            return newRoot;
        }
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            if (--d == 0) return root;
            int n = q.size();
            for (int i = 0; i < n; ++i) {
                auto t = q.front(); q.pop();
                if (d == 1) {
                    TreeNode *left = t->left;
                    TreeNode *right = t->right;
                    t->left = new TreeNode(v);
                    t->right = new TreeNode(v);
                    t->left->left = left;
                    t->right->right = right;
                } else {
                    if (t->left) q.push(t->left);
                    if (t->right) q.push(t->right);
                }
            }
        }
        return root;
    }
};
```



虽然博主一贯的理念是二叉树问题肯定首选递归来解，但是这道题博主刚开始以为递归没法解，结果看了大神们的帖子，才发现自己还是图样图森破，难道二叉树的问题皆可递归？反正这道题是可以的，而且写法so简洁，乍一看上去，会有疑问，题目中明明d的范围是从1开始的，为何要考虑d为0的情况，后来读懂了整个解法后，才为原作者的聪慧叹服。这里d的0和1，其实相当于一种flag，如果d为1的话，那么将root连到新建的结点的左子结点上；反之如果d为0，那么将root连到新建的结点的右子结点上，然后返回新建的结点。如果root存在且d大于1的话，那么对root的左子结点调用递归函数，注意此时若d的值正好为2，那么我们就不能直接减1，而是根据左右子结点的情况分别赋值1和0，这样才能起到flag的作用嘛，叼的飞起，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int v, int d) {
        if (d == 0 || d == 1) {
            TreeNode *newRoot = new TreeNode(v);
            (d ? newRoot->left : newRoot->right) = root;
            return newRoot;
        }
        if (root && d > 1) {
            root->left = addOneRow(root->left, v, d > 2 ? d - 1 : 1);
            root->right = addOneRow(root->right, v, d > 2 ? d - 1 : 0);
        }
        return root;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/92876/c-java-10-line-solution-no-helper](https://discuss.leetcode.com/topic/92876/c-java-10-line-solution-no-helper)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













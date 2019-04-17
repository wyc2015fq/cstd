# [LeetCode] Closest Leaf in a Binary Tree 二叉树中最近的叶结点 - Grandyang - 博客园







# [[LeetCode] Closest Leaf in a Binary Tree 二叉树中最近的叶结点](https://www.cnblogs.com/grandyang/p/8245586.html)








Given a binary tree where every node has a unique value, and a target key `k`, find the value of the nearest leaf node to target `k` in the tree.

Here, *nearest* to a leaf means the least number of edges travelled on the binary tree to reach any leaf of the tree. Also, a node is called a *leaf* if it has no children.

In the following examples, the input tree is represented in flattened form row by row. The actual `root` tree given will be a TreeNode object.

Example 1:
Input:
root = [1, 3, 2], k = 1
Diagram of binary tree:
          1
         / \
        3   2

Output: 2 (or 3)

Explanation: Either 2 or 3 is the nearest leaf node to the target of 1.



Example 2:
Input:
root = [1], k = 1
Output: 1

Explanation: The nearest leaf node is the root node itself.



Example 3:
Input:
root = [1,2,3,4,null,null,null,5,null,6], k = 2
Diagram of binary tree:
             1
            / \
           2   3
          /
         4
        /
       5
      /
     6

Output: 3
Explanation: The leaf node with value 3 (and not the leaf node with value 6) is nearest to the node with value 2.



Note:
- `root` represents a binary tree with at least `1` node and at most `1000` nodes.
- Every node has a unique `node.val` in range `[1, 1000]`.
- There exists some node in the given binary tree for which `node.val == k`.




这道题让我们找二叉树中最近的叶结点，叶结点就是最底端没有子结点的那个。我们观察题目中的例子3，发现结点2的最近叶结点是其右边的那个结点3，那么传统的二叉树的遍历只能去找其子结点中的叶结点，像这种同一层水平的结点该怎么弄呢？我们知道树的本质就是一种无向图，但是树只提供了父结点到子结点的连接，反过来就不行了，所以只要我们建立了反向连接，就可以用BFS来找最近的叶结点了。明白了这一点后，我们就先来做反向连接吧，用一个哈希map，建立子结点与其父结点之间的映射，其实我们不用做完所有的反向连接，而是做到要求的结点k就行了，因为结点k的子结点可以直接访问，不需要再反过来查找。我们用DFS来遍历结点，并做反向连接，直到遇到结点k时，将其返回。此时我们得到了结点k，并且做好了结点k上面所有结点的反向连接，那么就可以用BFS来找最近的叶结点了，将结点k加入队列queue和已访问集合visited中，然后开始循环，每次取出队首元素，如果是叶结点，说明已经找到了最近叶结点，直接返回；如果左子结点存在，并且不在visited集合中，那么先将其加入集合，然后再加入队列，同理，如果右子结点存在，并且不在visited集合中，那么先将其加入集合，然后再加入队列；再来看其父结点，如果不在visited集合中，那么先将其加入集合，然后再加入队列。因为题目中说了一定会有结点k，所以在循环内部就可以直接返回了，不会有退出循环的可能，但是为表尊重，我们最后还是加上return -1吧， 参见代码如下：



解法一：

```
class Solution {
public:
    int findClosestLeaf(TreeNode* root, int k) {
        unordered_map<TreeNode*, TreeNode*> back;
        TreeNode *kNode = find(root, k, back);
        queue<TreeNode*> q{{kNode}};
        unordered_set<TreeNode*> visited{{kNode}};
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            if (!t->left && !t->right) return t->val;
            if (t->left && !visited.count(t->left)) {
                visited.insert(t->left);
                q.push(t->left);
            }
            if (t->right && !visited.count(t->right)) {
                visited.insert(t->right);
                q.push(t->right);
            }
            if (back.count(t) && !visited.count(back[t])) {
                visited.insert(back[t]);
                q.push(back[t]);
            }
        }
        return -1;
    }
    TreeNode* find(TreeNode* node, int k, unordered_map<TreeNode*, TreeNode*>& back) {
        if (node->val == k) return node;
        if (node->left) {
            back[node->left] = node;
            TreeNode *left = find(node->left, k, back);
            if (left) return left;
        }
        if (node->right) {
            back[node->right] = node;
            TreeNode *right = find(node->right, k, back);
            if (right) return right;
        }
        return NULL;
    }
};
```



下面这种解法也挺巧妙的，虽然没有像上面的解法那样建立所有父结点的反向连接，但是这种解法直接提前算出来了所有父结点到结点k的距离，就比如说例子3中，结点k的父结点只有一个，即为结点1，那么算出其和结点k的距离为1，即建立结点1和距离1之间的映射，另外建立结点k和0之间的映射，这样便于从结点k开始像叶结点统计距离。接下来，我们维护一个最小值mn，表示结点k到叶结点的最小距离，还有结果res，指向那个最小距离的叶结点。下面就开始再次遍历二叉树了，如果当前结点为空， 直接返回。否则先在哈希map中看当前结点是否有映射值，有的话就取出来（如果有，则说明当前结点可能k或者其父结点），如果当前结点是叶结点了，那么我们要用当前距离cur和最小距离mn比较，如果cur更小的话，就将mn更新为cur，将结果res更新为当前结点。否则就对其左右子结点调用递归函数，注意cur要加1，参见代码如下：



解法二：

```
class Solution {
public:
    int findClosestLeaf(TreeNode* root, int k) {
        int res = -1, mn = INT_MAX;
        unordered_map<int, int> m;
        m[k] = 0;
        find(root, k, m);
        helper(root, -1, m, mn, res);
        return res;
    }
    int find(TreeNode* node, int k, unordered_map<int, int>& m) {
        if (!node) return -1;
        if (node->val == k) return 1;
        int r = find(node->left, k, m);
        if (r != -1) {
            m[node->val] = r; 
            return r + 1;
        }
        r = find(node->right, k, m);
        if (r != -1) {
            m[node->val] = r;
            return r + 1;
        }
        return -1;
    }
    void helper(TreeNode* node, int cur, unordered_map<int, int>& m, int& mn, int& res) {
        if (!node) return;
        if (m.count(node->val)) cur = m[node->val];
        if (!node->left && !node->right) {
            if (mn > cur) {
                mn = cur; 
                res = node->val;
            }
        }
        helper(node->left, cur + 1, m, mn, res);
        helper(node->right, cur + 1, m, mn, res);
    }
};
```



参考资料：

[https://leetcode.com/problems/closest-leaf-in-a-binary-tree/](https://leetcode.com/problems/closest-leaf-in-a-binary-tree/)

[https://leetcode.com/problems/closest-leaf-in-a-binary-tree/discuss/109960/java-dfs-bfs-27ms](https://leetcode.com/problems/closest-leaf-in-a-binary-tree/discuss/109960/java-dfs-bfs-27ms)

[https://leetcode.com/problems/closest-leaf-in-a-binary-tree/discuss/109963/java-short-solution28-ms-solution](https://leetcode.com/problems/closest-leaf-in-a-binary-tree/discuss/109963/java-short-solution28-ms-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













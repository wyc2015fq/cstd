# [LeetCode] Encode N-ary Tree to Binary Tree 将N叉树编码为二叉树 - Grandyang - 博客园







# [[LeetCode] Encode N-ary Tree to Binary Tree 将N叉树编码为二叉树](https://www.cnblogs.com/grandyang/p/9945345.html)







Design an algorithm to encode an N-ary tree into a binary tree and decode the binary tree to get the original N-ary tree. An N-ary tree is a rooted tree in which each node has no more than N children. Similarly, a binary tree is a rooted tree in which each node has no more than 2 children. There is no restriction on how your encode/decode algorithm should work. You just need to ensure that an N-ary tree can be encoded to a binary tree and this binary tree can be decoded to the original N-nary tree structure.

For example, you may encode the following `3-ary` tree to a binary tree in this way:



![](https://assets.leetcode.com/uploads/2018/10/12/narytreebinarytreeexample.png)



Note that the above is just an example which *might or might not* work. You do not necessarily need to follow this format, so please be creative and come up with different approaches yourself.



Note:
- `N` is in the range of `[1, 1000]`
- Do not use class member/global/static variables to store states. Your encode and decode algorithms should be stateless.



这道题让我们将一棵N叉树编码为二叉树，其实还需要将二叉树解码回N叉树。题目中说了具体的编解码的方法无所谓，那么就怎么简单怎么来呗。首先想一下这道题的难点是什么，N叉树的特点的每个结点最多有N个子结点，而二叉树的每个结点最多只能有两个子结点，那么多余的子结点怎么办，当然只能继续子结点下继续累加，就像泡泡龙游戏一样，一个挂一个的。如何累，得确定一套具体的规则，这样解码的时候，反向来一遍就可以了。对于当前结点root的N个子结点的处理办法是，将第一个结点挂到二叉树的左子结点上，然后将后面的结点依次挂到右子结点，和右子结点的右子结点上，这样做的好处是，同一层的子结点都挂在右子结点上，而这些子结点自己的子结点都会挂在左子结点上，听起来很晕是么，那就举例说明一下吧，就用题目中的例子好了。



```
N-ary Tree:

      1
   /  |  \
  3   2   4
 / \
5   6


Binary Tree:

    1
   /
  3
 / \
5   2
 \   \
  6   4
```



我们可以看出，N叉树根结点1的第一个子结点3被挂到了二叉树的左子结点上，同一层的结点2挂到了结点3的右子结点上，同一层的结点4被挂到了结点2的右子结点上。而结点3本身的子结点也按照这个规律，第一个子结点5挂到了结点3的左子结点上，而同一排的结点6挂到了结点5的右子结点上。

对于解码，也是同样的规律，先根据根结点值新建一个空的N叉树结点，由于我们的编码规律，根结点是一定没有右子结点的，所以取出左子结点cur，并且开始循环，如果cur结点存在，那么我们对cur递归调用解码函数，将返回的结点加入当前N叉树结点的children数组中，然后cur再赋值为其右子结点，继续递归调用解码函数，再加入children数组，如此便可将二叉树还原为之前的N叉树，参见代码如下：



```
class Codec {
public:

    // Encodes an n-ary tree to a binary tree.
    TreeNode* encode(Node* root) {
        if (!root) return NULL;
        TreeNode *res = new TreeNode(root->val);
        if (!root->children.empty()) {
            res->left = encode(root->children[0]);
        }
        TreeNode *cur = res->left;
        for (int i = 1; i < root->children.size(); ++i) {
            cur->right = encode(root->children[i]);
            cur = cur->right;
        }
        return res;
    }

    // Decodes your binary tree to an n-ary tree.
    Node* decode(TreeNode* root) {
        if (!root) return NULL;
        Node *res = new Node(root->val, {});
        TreeNode *cur = root->left;
        while (cur) {
            res->children.push_back(decode(cur));
            cur = cur->right;
        }
        return res;
    }
};
```



类似题目：

[Serialize and Deserialize N-ary Tree](https://www.cnblogs.com/grandyang/p/9945453.html)



参考资料：

[https://leetcode.com/problems/encode-n-ary-tree-to-binary-tree/](https://leetcode.com/problems/encode-n-ary-tree-to-binary-tree/)

[https://leetcode.com/problems/encode-n-ary-tree-to-binary-tree/discuss/153061/Java-Solution-(Next-Level-greater-left-Same-Level-greater-right)](https://leetcode.com/problems/encode-n-ary-tree-to-binary-tree/discuss/153061/Java-Solution-(Next-Level-greater-left-Same-Level-greater-right))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













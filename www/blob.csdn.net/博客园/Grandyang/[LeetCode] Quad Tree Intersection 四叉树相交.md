# [LeetCode] Quad Tree Intersection 四叉树相交 - Grandyang - 博客园







# [[LeetCode] Quad Tree Intersection 四叉树相交](https://www.cnblogs.com/grandyang/p/9846991.html)







A quadtree is a tree data in which each internal node has exactly four children: `topLeft`, `topRight`, `bottomLeft` and `bottomRight`. Quad trees are often used to partition a two-dimensional space by recursively subdividing it into four quadrants or regions.

We want to store True/False information in our quad tree. The quad tree is used to represent a `N * N` boolean grid. For each node, it will be subdivided into four children nodes until the values in the region it represents are all the same. Each node has another two boolean attributes : `isLeaf` and `val`. `isLeaf`is true if and only if the node is a leaf node. The `val` attribute for a leaf node contains the value of the region it represents.

For example, below are two quad trees A and B:
A:
+-------+-------+   T: true
|       |       |   F: false
|   T   |   T   |
|       |       |
+-------+-------+
|       |       |
|   F   |   F   |
|       |       |
+-------+-------+
topLeft: T
topRight: T
bottomLeft: F
bottomRight: F

B:               
+-------+---+---+
|       | F | F |
|   T   +---+---+
|       | T | T |
+-------+---+---+
|       |       |
|   T   |   F   |
|       |       |
+-------+-------+
topLeft: T
topRight:
     topLeft: F
     topRight: F
     bottomLeft: T
     bottomRight: T
bottomLeft: T
bottomRight: F



Your task is to implement a function that will take two quadtrees and return a quadtree that represents the logical OR (or union) of the two trees.
A:                 B:                 C (A or B):
+-------+-------+  +-------+---+---+  +-------+-------+
|       |       |  |       | F | F |  |       |       |
|   T   |   T   |  |   T   +---+---+  |   T   |   T   |
|       |       |  |       | T | T |  |       |       |
+-------+-------+  +-------+---+---+  +-------+-------+
|       |       |  |       |       |  |       |       |
|   F   |   F   |  |   T   |   F   |  |   T   |   F   |
|       |       |  |       |       |  |       |       |
+-------+-------+  +-------+-------+  +-------+-------+

Note:
- Both `A` and `B` represent grids of size `N * N`.
- `N` is guaranteed to be a power of 2.
- If you want to know more about the quad tree, you can refer to its [wiki](https://en.wikipedia.org/wiki/Quadtree).
- The logic OR operation is defined as this: "A or B" is true if `A is true`, or if `B is true`, or if `both A and B are true`.



这道题又是一道四叉树的题，说是给了我们两个四叉树，然后让我们将二棵树相交形成了一棵四叉树，相交的机制采用的是或，即每个自区域相‘或’，题目中给的例子很好的说明了一些相‘或’的原则，比如我们看A和B中的右上结点，我们发现A树的右上结点已经是一个值为true的叶结点，而B的右上结点还是一个子树，那么此时不论子树里有啥内容，我们相交后的树的右上结点应该跟A树的右上结点保持一致，假如A树的右上结点值是false的话，相‘或’起不到任何作用，那么相交后的树的右上结点应该跟B树的右上结点保持一致。那么我们可以归纳出，只有某一个结点是叶结点了，我们看其值，如果是true，则相交后的结点和此结点保持一致，否则跟另一个结点保持一致。比较麻烦的情况是当两个结点都不是叶结点的情况，此时我们需要对相对应的四个子结点分别调用递归函数，调用之后还需要进行进一步处理，因为一旦四个子结点的值相同，且都是叶结点的话，那么此时应该合并为一个大的叶结点，参见代码如下：



```
class Solution {
public:
    Node* intersect(Node* quadTree1, Node* quadTree2) {
          if (quadTree1->isLeaf) return quadTree1->val ? quadTree1 : quadTree2;
          if (quadTree2->isLeaf) return quadTree2->val ? quadTree2 : quadTree1;
          Node *tl = intersect(quadTree1->topLeft, quadTree2->topLeft);
          Node *tr = intersect(quadTree1->topRight, quadTree2->topRight);
          Node *bl = intersect(quadTree1->bottomLeft, quadTree2->bottomLeft);
          Node *br = intersect(quadTree1->bottomRight, quadTree2->bottomRight);
          if (tl->val == tr->val && tl->val == bl->val && tl->val == br->val && tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf) {
              return new Node(tl->val, true, NULL, NULL, NULL, NULL);
          } else {
              return new Node(false, false, tl, tr, bl, br);
          }
    }
};
```



类似题目：

[Construct Quad Tree](https://www.cnblogs.com/grandyang/p/9649348.html)



参考资料：

[https://leetcode.com/problems/quad-tree-intersection/](https://leetcode.com/problems/quad-tree-intersection/)

[https://leetcode.com/problems/quad-tree-intersection/discuss/152730/C%2B%2B-10-line-beat-100](https://leetcode.com/problems/quad-tree-intersection/discuss/152730/C%2B%2B-10-line-beat-100)

[https://leetcode.com/problems/quad-tree-intersection/discuss/157532/Java-concise-code-beat-100](https://leetcode.com/problems/quad-tree-intersection/discuss/157532/Java-concise-code-beat-100)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [CareerCup] 11.8 The Rank of Number 数的排行 - Grandyang - 博客园







# [[CareerCup] 11.8 The Rank of Number 数的排行](https://www.cnblogs.com/grandyang/p/4900214.html)







11.8 Imagine you are reading in a stream of integers. Periodically, you wish to be able to look up the rank of a number x (the number of values less than or equal tox). Implement the data structures and algorithms to support these operations.That is, implement the method track(int x), which is called when each number is generated, and the method getRankOfNumber(int x), which returns the number of values less than or equal to x (not including x itself).
 EXAMPLE
 Stream (in order of appearance): 5, 1, 4, 4, 5, 9, 7, 13, 3
 getRankOfNumber(l) = 0
 getRankOfNumber(3) = 1
 getRankOfNumber(4) = 3



这道题给了我们一个无序数组，让我们求每个数字的排行，排行为几就表示有几个小于或等于该数字的数。我们首先需要用一个数据结构来保存有序数组，用向量的话加数字不高效，用priority_queue或者multiset的话求rank又太麻烦，引文不能直接通过坐标访问元素。那么我们考虑用另一种有序的数据结构，二叉搜索树Binary Search Tree，我们知道BST的性质是左<=中<右，中序遍历一个BST的结果就是有序数组。为了更有效的找出rank，我们在加入新数字的时候，记录一个变量left_size，表示此数字的左子树的节点数。我们来看下面这个例子，每个节点表示当前数字，括号中的数字表示当前节点的左子节点的个数。

```
20(4)
        /       \
       /         \
     15(3)       25(2)
     /          /
    /          /
   10(1)      23(0)
  /  \         \
 /    \         \ 
5(0)    13(0)    24(0)
```

假如我们要找24的rank，我们将24与根节点20相比较，发现24应该在根节点的右边，根节点的左子树有四个节点，加上根节点本身，我们现已知有5个小于24的数，将counter设为5，然后我们再跟25比，发现24在其左侧，不更新counter，然后和23比，24在其右侧，counter加1，因为23没有左子树，最后我们就可知24的rank为6, 参见代码如下：



```
class RankNode {
public:
    int left_size = 0;
    RankNode *left;
    RankNode *right;
    int data = 0;
    RankNode(int d): data(d), left(nullptr), right(nullptr) {}
    void insert(int d) {
        if (d <= data) {
            if (left != nullptr) left->insert(d);
            else left = new RankNode(d);
            ++left_size;
        } else {
            if (right != nullptr) right->insert(d);
            else right = new RankNode(d);
        }
    }
    int getRank(int d) {
        if (d == data) return left_size;
        else if (d < data) {
            return left == nullptr ? -1 : left->getRank(d);
        } else {
            return right == nullptr ? -1 : right->getRank(d) + 1 + left_size;
        }
    }
};

class Solution {
public:
    RankNode *root;
    void track(int number) {
        if (root == nullptr) {
            root = new RankNode(number);
        } else {
            root->insert(number);
        }
    }
    int getRankOfNumber(int number) {
        return root->getRank(number);
    }
};
```















# [LeetCode]  Convert Sorted List to Binary Search Tree 将有序链表转为二叉搜索树 - Grandyang - 博客园







# [[LeetCode]  Convert Sorted List to Binary Search Tree 将有序链表转为二叉搜索树](https://www.cnblogs.com/grandyang/p/4295618.html)







Given a singly linked list where elements are sorted in ascending order, convert it to a height balanced BST.

For this problem, a height-balanced binary tree is defined as a binary tree in which the depth of the two subtrees of *every* node never differ by more than 1.

Example:
Given the sorted linked list: [-10,-3,0,5,9],

One possible answer is: [0,-3,9,-10,null,5], which represents the following height balanced BST:

      0
     / \
   -3   9
   /   /
 -10  5


这道题是要求把有序链表转为二叉搜索树，和之前那道 [Convert Sorted Array to Binary Search Tree](http://www.cnblogs.com/grandyang/p/4295245.html) 思路完全一样，只不过是操作的数据类型有所差别，一个是数组，一个是链表。数组方便就方便在可以通过index直接访问任意一个元素，而链表不行。由于二分查找法每次需要找到中点，而链表的查找中间点可以通过快慢指针来操作，可参见之前的两篇博客 [Reorder List](http://www.cnblogs.com/grandyang/p/4254860.html) 和 [Linked List Cycle II](http://www.cnblogs.com/grandyang/p/4137302.html) 有关快慢指针的应用。找到中点后，要以中点的值建立一个数的根节点，然后需要把原链表断开，分为前后两个链表，都不能包含原中节点，然后再分别对这两个链表递归调用原函数，分别连上左右子节点即可。代码如下：



解法一：

```
class Solution {
public:
    TreeNode *sortedListToBST(ListNode* head) {
        if (!head) return NULL;
        if (!head->next) return new TreeNode(head->val);
        ListNode *slow = head, *fast = head, *last = slow;
        while (fast->next && fast->next->next) {
            last = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        fast = slow->next;
        last->next = NULL;
        TreeNode *cur = new TreeNode(slow->val);
        if (head != slow) cur->left = sortedListToBST(head);
        cur->right = sortedListToBST(fast);
        return cur;
    }
};
```



我们也可以采用如下的递归方法，重写一个递归函数，有两个输入参数，子链表的起点和终点，因为知道了这两个点，链表的范围就可以确定了，而直接将中间部分转换为二叉搜索树即可，递归函数中的内容跟上面解法中的极其相似，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) return NULL;
        return helper(head, NULL);
    }
    TreeNode* helper(ListNode* head, ListNode* tail) {
        if (head == tail) return NULL;
        ListNode *slow = head, *fast = head;
        while (fast != tail && fast->next != tail) {
            slow = slow->next;
            fast = fast->next->next;
        }
        TreeNode *cur = new TreeNode(slow->val);
        cur->left = helper(head, slow);
        cur->right = helper(slow->next, tail);
        return cur;
    }
};
```



类似题目：

[Convert Sorted Array to Binary Search Tree](http://www.cnblogs.com/grandyang/p/4295245.html)



参考资料：

[https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/](https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/)

[https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/discuss/35476/Share-my-JAVA-solution-1ms-very-short-and-concise.](https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/discuss/35476/Share-my-JAVA-solution-1ms-very-short-and-concise.)

[https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/discuss/35470/Recursive-BST-construction-using-slow-fast-traversal-on-linked-list](https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/discuss/35470/Recursive-BST-construction-using-slow-fast-traversal-on-linked-list)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













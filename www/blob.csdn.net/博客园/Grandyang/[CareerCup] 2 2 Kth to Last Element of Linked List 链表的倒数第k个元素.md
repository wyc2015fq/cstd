# [CareerCup] 2.2 Kth to Last Element of Linked List 链表的倒数第k个元素 - Grandyang - 博客园







# [[CareerCup] 2.2 Kth to Last Element of Linked List 链表的倒数第k个元素](https://www.cnblogs.com/grandyang/p/4658430.html)







2.2 Implement an algorithm to find the kth to last element of a singly linked list.



这道题让我们求链表中倒数第k个元素，LeetCode中相类似的题目有[Kth Largest Element in an Array 数组中第k大的数字](http://www.cnblogs.com/grandyang/p/4539757.html) 和 [Kth Smallest Element in a BST 二叉搜索树中的第K小的元素](http://www.cnblogs.com/grandyang/p/4620012.html)。但那两道题和这题又不一样，首先这道题是要在链表中操作，链表的特点就是不能通过下标来直接访问元素，而且要知道链表长度的话只能遍历，当然如果这道题先遍历一遍知道了长度，再来找倒数第k个就没啥挑战，也不是这道题考察的初衷。这道题可以用递归和非递归两种方法来解。我们先来看看递归的方法，方法是先无脑递归到最末尾，然后开始回去，每回一个计数器累加１，直到回到ｋ返回节点即可，此方法的时间复杂度和空间复杂度均为O(n)，代码如下：



解法一：

```
// Recursion
class Solution {
public:
    ListNode *kthToLast(ListNode *head, int k) {
        int i = 0;
        return kthToLastDFS(head, k, i);
    }
    ListNode *kthToLastDFS(ListNode *head, int k, int &i) {
        if (!head) return head;
        ListNode *node = kthToLastDFS(head->next, k, i);
        ++i;
        if (i == k) return head;
        return node;
    }
};
```



下面我们来看非递归的方法，这种方法就巧妙的多，需要用两个指针，其中一个指针先向前走k个，然后两个指针同时走，当先走那个到达末尾时，另外一个指针指向的元素就是倒数第k个，时间复杂度为O(n)，空间复杂度为O(1)，代码如下：



解法二：

```
// Non-recursion
class Solution {
public:
    ListNode *kthToLast(ListNode *head, int k) {
        ListNode *pre = head, *cur = head;
        for (int i = 0; i < k; ++i) {
            if (cur) cur = cur->next;
        }
        while (cur) {
            cur = cur->next;
            pre = pre->next;
        }
        return pre;
    }
};
```















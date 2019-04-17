# [LeetCode] Reverse Linked List 倒置链表 - Grandyang - 博客园







# [[LeetCode] Reverse Linked List 倒置链表](https://www.cnblogs.com/grandyang/p/4478820.html)







Reverse a singly linked list.

Example:
Input: 1->2->3->4->5->NULL
Output: 5->4->3->2->1->NULL

Follow up:

A linked list can be reversed either iteratively or recursively. Could you implement both?




之前做到 [Reverse Linked List II](http://www.cnblogs.com/grandyang/p/4306611.html) 的时候我还纳闷怎么只有二没有一呢，原来真是忘了啊，现在才加上，这道题跟之前那道比起来简单不少，题目为了增加些许难度，让我们分别用迭代和递归来实现，但难度还是不大。我们先来看迭代的解法，思路是在原链表之前建立一个空的newHead，因为首节点会变，然后从head开始，将之后的一个节点移到newHead之后，重复此操作直到head成为末节点为止，代码如下：




解法一：

```
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *newHead = NULL;
        while (head) {
            ListNode *t = head->next;
            head->next = newHead;
            newHead = head;
            head = t;
        }
        return newHead;
    }
};
```



下面我们来看递归解法，代码量更少，递归解法的思路是，不断的进入递归函数，直到head指向倒数第二个节点，因为head指向空或者是最后一个结点都直接返回了，newHead则指向对head的下一个结点调用递归函数返回的头结点，此时newHead指向最后一个结点，然后head的下一个结点的next指向head本身，这个相当于把head结点移动到末尾的操作，因为是回溯的操作，所以head的下一个结点总是在上一轮被移动到末尾了，但head之后的next还没有断开，所以可以顺势将head移动到末尾，再把next断开，最后返回newHead即可，代码如下：



解法二：

```
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode *newHead = reverseList(head->next);
        head->next->next = head;
        head->next = NULL;
        return newHead;
    }
};
```



类似题目：

[Reverse Linked List II](http://www.cnblogs.com/grandyang/p/4306611.html)

[Binary Tree Upside Down](http://www.cnblogs.com/grandyang/p/5172838.html)

[Palindrome Linked List](http://www.cnblogs.com/grandyang/p/4635425.html)



参考资料：

[https://leetcode.com/problems/reverse-linked-list/](https://leetcode.com/problems/reverse-linked-list/)

[https://leetcode.com/problems/reverse-linked-list/discuss/58156/My-Java-recursive-solution](https://leetcode.com/problems/reverse-linked-list/discuss/58156/My-Java-recursive-solution)

[https://leetcode.com/problems/reverse-linked-list/discuss/58337/Fast-Recursive-Java-solution](https://leetcode.com/problems/reverse-linked-list/discuss/58337/Fast-Recursive-Java-solution)

[https://leetcode.com/problems/reverse-linked-list/discuss/58125/In-place-iterative-and-recursive-Java-solution](https://leetcode.com/problems/reverse-linked-list/discuss/58125/In-place-iterative-and-recursive-Java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Linked List Cycle II 单链表中的环之二 - Grandyang - 博客园







# [[LeetCode] Linked List Cycle II 单链表中的环之二](https://www.cnblogs.com/grandyang/p/4137302.html)







Given a linked list, return the node where the cycle begins. If there is no cycle, return `null`.

To represent a cycle in the given linked list, we use an integer `pos` which represents the position (0-indexed) in the linked list where tail connects to. If `pos` is `-1`, then there is no cycle in the linked list.

Note: Do not modify the linked list.



Example 1:
Input: head = [3,2,0,-4], pos = 1
Output: tail connects to node index 1
Explanation: There is a cycle in the linked list, where tail connects to the second node.

![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist.png)

Example 2:
Input: head = [1,2], pos = 0
Output: tail connects to node index 0
Explanation: There is a cycle in the linked list, where tail connects to the first node.

![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test2.png)

Example 3:
Input: head = [1], pos = -1
Output: no cycle
Explanation: There is no cycle in the linked list.

![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test3.png)



Follow up:
Can you solve it without using extra space?



这个求单链表中的环的起始点是之前那个判断单链表中是否有环的延伸，可参之前那道[Linked List Cycle](http://www.cnblogs.com/grandyang/p/4137187.html)。这里还是要设快慢指针，不过这次要记录两个指针相遇的位置，当两个指针相遇了后，让其中一个指针从链表头开始，一步两步，一步一步似爪牙，似魔鬼的步伐。。。哈哈，打住打住。。。此时再相遇的位置就是链表中环的起始位置，为啥是这样呢，这里直接贴上热心网友「飞鸟想飞」的解释哈，因为快指针每次走2，慢指针每次走1，快指针走的距离是慢指针的两倍。而快指针又比慢指针多走了一圈。所以head到环的起点+环的起点到他们相遇的点的距离 与 环一圈的距离相等。现在重新开始，head运行到环起点 和 相遇点到环起点 的距离也是相等的，相当于他们同时减掉了 环的起点到他们相遇的点的距离。代码如下：



```
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) break;
        }
        if (!fast || !fast->next) return NULL;
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return fast;
    }
};
```



讨论：单链表中的环的问题还有许多扩展，比如求环的长度，或者是如何解除环等等，可参见网上大神的[这个总结贴](http://www.cnblogs.com/hiddenfox/p/3408931.html)。



类似题目：

[Linked List Cycle](http://www.cnblogs.com/grandyang/p/4137187.html)

[Find the Duplicate Number](http://www.cnblogs.com/grandyang/p/4843654.html)



参考资料：

[https://leetcode.com/problems/linked-list-cycle-ii/](https://leetcode.com/problems/linked-list-cycle-ii/)

[https://leetcode.com/problems/linked-list-cycle-ii/discuss/44793/O(n)-solution-by-using-two-pointers-without-change-anything](https://leetcode.com/problems/linked-list-cycle-ii/discuss/44793/O(n)-solution-by-using-two-pointers-without-change-anything)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













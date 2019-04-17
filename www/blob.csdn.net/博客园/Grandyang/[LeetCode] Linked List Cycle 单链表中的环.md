# [LeetCode] Linked List Cycle 单链表中的环 - Grandyang - 博客园







# [[LeetCode] Linked List Cycle 单链表中的环](https://www.cnblogs.com/grandyang/p/4137187.html)







Given a linked list, determine if it has a cycle in it.

To represent a cycle in the given linked list, we use an integer `pos` which represents the position (0-indexed) in the linked list where tail connects to. If `pos` is `-1`, then there is no cycle in the linked list.




Example 1:
Input: head = [3,2,0,-4], pos = 1
Output: true
Explanation: There is a cycle in the linked list, where tail connects to the second node.



![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist.png)

Example 2:
Input: head = [1,2], pos = 0
Output: true
Explanation: There is a cycle in the linked list, where tail connects to the first node.



![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test2.png)

Example 3:
Input: head = [1], pos = -1
Output: false
Explanation: There is no cycle in the linked list.


![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test3.png)



Follow up:

Can you solve it using *O(1)* (i.e. constant) memory?



这道题是快慢指针的经典应用。只需要设两个指针，一个每次走一步的慢指针和一个每次走两步的快指针，如果链表里有环的话，两个指针最终肯定会相遇。实在是太巧妙了，要是我肯定想不出来。代码如下：



C++ 解法：

```
class Solution {
public:
    bool hasCycle(ListNode *head) {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }
};
```



Java 解法：

```
public class Solution {
    public boolean hasCycle(ListNode head) {
        ListNode slow = head, fast = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
            if (slow == fast) return true;
        }
        return false;
    }
}
```



类似题目：

[Linked List Cycle II](http://www.cnblogs.com/grandyang/p/4137302.html)

[Happy Number](http://www.cnblogs.com/grandyang/p/4447233.html)



参考资料：

[https://leetcode.com/problems/linked-list-cycle/](https://leetcode.com/problems/linked-list-cycle/)

[https://leetcode.com/problems/linked-list-cycle/discuss/44489/O(1)-Space-Solution](https://leetcode.com/problems/linked-list-cycle/discuss/44489/O(1)-Space-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













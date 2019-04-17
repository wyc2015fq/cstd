# [LeetCode] Partition List 划分链表 - Grandyang - 博客园







# [[LeetCode] Partition List 划分链表](https://www.cnblogs.com/grandyang/p/4321292.html)







Given a linked list and a value *x*, partition it such that all nodes less than *x* come before nodes greater than or equal to *x*.

You should preserve the original relative order of the nodes in each of the two partitions.

For example,
Given `1->4->3->2->5->2` and *x* = 3,
return `1->2->2->4->3->5`.



这道题要求我们划分链表，把所有小于给定值的节点都移到前面，大于该值的节点顺序不变，相当于一个局部排序的问题。那么可以想到的一种解法是首先找到第一个大于或等于给定值的节点，用题目中给的例子来说就是先找到4，然后再找小于3的值，每找到一个就将其取出置于4之前即可，代码如下：

解法一

```
class Solution {
public:
    ListNode *partition(ListNode *head, int x) {
        ListNode *dummy = new ListNode(-1);
        dummy->next = head;
        ListNode *pre = dummy, *cur = head;;
        while (pre->next && pre->next->val < x) pre = pre->next;
        cur = pre;
        while (cur->next) {
            if (cur->next->val < x) {
                ListNode *tmp = cur->next;
                cur->next = tmp->next;
                tmp->next = pre->next;
                pre->next = tmp;
                pre = pre->next;
            } else {
                cur = cur->next;
            }
        }
        return dummy->next;
    }
};
```





这种解法的链表变化顺序为：

1 -> 4 -> 3 -> 2 -> 5 -> 2 

1 -> 2 -> 4 -> 3 -> 5 -> 2 

1 -> 2 -> 2 -> 4 -> 3 -> 5



此题还有一种解法，就是将所有小于给定值的节点取出组成一个新的链表，此时原链表中剩余的节点的值都大于或等于给定值，只要将原链表直接接在新链表后即可，代码如下：

解法二



```
class Solution {
public:
    ListNode *partition(ListNode *head, int x) {
        if (!head) return head;
        ListNode *dummy = new ListNode(-1);
        ListNode *newDummy = new ListNode(-1);
        dummy->next = head;
        ListNode *cur = dummy, *p = newDummy;
        while (cur->next) {
            if (cur->next->val < x) {
                p->next = cur->next;
                p = p->next;
                cur->next = cur->next->next;
                p->next = NULL;
            } else {
                cur = cur->next;
            }
        }
        p->next = dummy->next;
        return newDummy->next;
    }
};
```



此种解法链表变化顺序为：

Original: 1 -> 4 -> 3 -> 2 -> 5 -> 2 

New:



Original: 4 -> 3 -> 2 -> 5 -> 2 

New:　  1



Original: 4 -> 3 -> 5 -> 2 

New:　  1 -> 2



Original: 4 -> 3 -> 5 

New:　  1 -> 2 -> 2



Original: 

New:　  1 -> 2 -> 2 -> 4 -> 3 -> 5 



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













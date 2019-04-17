# [LeetCode] Merge Two Sorted Lists 混合插入有序链表 - Grandyang - 博客园







# [[LeetCode] Merge Two Sorted Lists 混合插入有序链表](https://www.cnblogs.com/grandyang/p/4086297.html)







Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.



这道混合插入有序链表和我之前那篇混合插入有序数组非常的相似[Merge Sorted Array](http://www.cnblogs.com/grandyang/p/4059650.html)，仅仅是数据结构由数组换成了链表而已，代码写起来反而更简洁。具体思想就是新建一个链表，然后比较两个链表中的元素值，把较小的那个链到新链表中，由于两个输入链表的长度可能不同，所以最终会有一个链表先完成插入所有元素，则直接另一个未完成的链表直接链入新链表的末尾。代码如下：



C++ 解法一：

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                cur->next = l1;
                l1 = l1->next;
            } else {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next;
        }
        cur->next = l1 ? l1 : l2;
        return dummy->next;
    }
};
```



Java 解法一：

```
public class Solution {
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        ListNode dummy = new ListNode(-1), cur = dummy;
        while (l1 != null && l2 != null) {
            if (l1.val < l2.val) {
                cur.next = l1;
                l1 = l1.next;
            } else {
                cur.next = l2;
                l2 = l2.next;
            }
            cur = cur.next;
        }
        cur.next = (l1 != null) ? l1 : l2;
        return dummy.next;
    }
}
```



下面我们来看递归的写法，当某个链表为空了，就返回另一个。然后核心还是比较当前两个节点值大小，如果l1的小，那么对于l1的下一个节点和l2调用递归函数，将返回值赋值给l1.next，然后返回l1；否则就对于l2的下一个节点和l1调用递归函数，将返回值赋值给l2.next，然后返回l2，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1->val < l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
    }
};
```



Java 解法二：

```
public class Solution {
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        if (l1 == null) return l2;
        if (l2 == null) return l1;
        if (l1.val < l2.val) {
            l1.next = mergeTwoLists(l1.next, l2);
            return l1;
        } else {
            l2.next = mergeTwoLists(l1, l2.next);
            return l2;
        }
    }
}
```



下面这种递归的写法去掉了if从句，看起来更加简洁一些，但是思路并没有什么不同：



C++ 解法三：

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        ListNode *head = l1->val < l2->val ? l1 : l2;
        ListNode *nonhead = l1->val < l2->val ? l2 : l1;
        head->next = mergeTwoLists(head->next, nonhead);
        return head;
    }
};
```



Java 解法三：

```
public class Solution {
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        if (l1 == null) return l2;
        if (l2 == null) return l1;
        ListNode head = (l1.val < l2.val) ? l1 : l2;
        ListNode nonhead = (l1.val < l2.val) ? l2 : l1;
        head.next = mergeTwoLists(head.next, nonhead);
        return head;
    }
}
```



 我们还可以三行搞定，简直丧心病狂有木有！



C++ 解法四：

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1 || (l2 && l1->val > l2->val)) swap(l1, l2);
        if (l1) l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};
```



Java 解法四：

```
public class Solution {
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        if (l1 == null || (l2 != null && l1.val > l2.val)) {
            ListNode t = l1; l1 = l2; l2 = t;
        }
        if (l1 != null) l1.next = mergeTwoLists(l1.next, l2);
        return l1;
    }
}
```



类似题目：

[Merge Sorted Array](http://www.cnblogs.com/grandyang/p/4059650.html)



参考资料：

[https://discuss.leetcode.com/topic/2513/a-recursive-solution](https://discuss.leetcode.com/topic/2513/a-recursive-solution)

[https://discuss.leetcode.com/topic/18709/3-lines-c-12ms-and-c-4ms/2](https://discuss.leetcode.com/topic/18709/3-lines-c-12ms-and-c-4ms/2)

[https://discuss.leetcode.com/topic/32953/java-recursive-solution-in-6-lines](https://discuss.leetcode.com/topic/32953/java-recursive-solution-in-6-lines)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













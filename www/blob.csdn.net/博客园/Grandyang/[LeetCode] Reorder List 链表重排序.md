# [LeetCode] Reorder List 链表重排序 - Grandyang - 博客园







# [[LeetCode] Reorder List 链表重排序](https://www.cnblogs.com/grandyang/p/4254860.html)







Given a singly linked list *L*: *L*0→*L*1→…→*L**n*-1→*L*n,
reorder it to: *L*0→*L**n*→*L*1→*L**n*-1→*L*2→*L**n*-2→…

You may not modify the values in the list's nodes, only nodes itself may be changed.

Example 1:
Given 1->2->3->4, reorder it to 1->4->2->3.
Example 2:
Given 1->2->3->4->5, reorder it to 1->5->2->4->3.


这道链表重排序问题可以拆分为以下三个小问题：

1. 使用快慢指针来找到链表的中点，并将链表从中点处断开，形成两个独立的链表。

2. 将第二个链翻转。

3. 将第二个链表的元素间隔地插入第一个链表中。



解法一：

```
class Solution {
public:
    void reorderList(ListNode *head) {
        if (!head || !head->next || !head->next->next) return;
        ListNode *fast = head, *slow = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode *mid = slow->next;
        slow->next = NULL;
        ListNode *last = mid, *pre = NULL;
        while (last) {
            ListNode *next = last->next;
            last->next = pre;
            pre = last;
            last = next;
        }
        while (head && pre) {
            ListNode *next = head->next;
            head->next = pre;
            pre = pre->next;
            head->next->next = next;
            head = next;
        }
    }
};
```



我们尝试着看能否写法上简洁一些，上面的第二步是将后半段链表翻转，那么我们其实可以借助栈的后进先出的特性来做，如果我们按顺序将所有的结点压入栈，那么出栈的时候就可以倒序了，实际上就相当于翻转了链表。由于只需将后半段链表翻转，那么我们就要控制出栈结点的个数，还好栈可以直接得到结点的个数，我们减1除以2，就是要出栈结点的个数了。然后我们要做的就是将每次出栈的结点隔一个插入到正确的位置，从而满足题目中要求的顺序，链表插入结点的操作就比较常见了，这里就不多解释了，最后记得断开栈顶元素后面的结点，比如对于 1->2->3->4，栈顶只需出一个结点4，然后加入原链表之后为 1->4->2->3->(4)，因为在原链表中结点3之后是连着结点4的，虽然我们将结点4取出插入到结点1和2之间，但是结点3后面的指针还是连着结点4的，所以我们要断开这个连接，这样才不会出现环，由于此时结点3在栈顶，所以我们直接断开栈顶结点即可，参见代码如下：



解法二：

```
class Solution {
public:
    void reorderList(ListNode *head) {
        if (!head || !head->next || !head->next->next) return;
        stack<ListNode*> st;
        ListNode *cur = head;
        while (cur) {
            st.push(cur);
            cur = cur->next;
        }
        int cnt = ((int)st.size() - 1) / 2;
        cur = head;
        while (cnt-- > 0) {
            auto t = st.top(); st.pop();
            ListNode *next = cur->next;
            cur->next = t;
            t->next = next;
            cur = next;
        }
        st.top()->next = NULL;
    }
};
```



参考资料：

[https://leetcode.com/problems/reorder-list/](https://leetcode.com/problems/reorder-list/)

[https://leetcode.com/problems/reorder-list/discuss/45175/Java-solution-with-stack](https://leetcode.com/problems/reorder-list/discuss/45175/Java-solution-with-stack)

[https://leetcode.com/problems/reorder-list/discuss/44992/Java-solution-with-3-steps](https://leetcode.com/problems/reorder-list/discuss/44992/Java-solution-with-3-steps)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













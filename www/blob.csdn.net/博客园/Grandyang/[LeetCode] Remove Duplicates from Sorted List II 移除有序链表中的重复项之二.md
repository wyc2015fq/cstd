# [LeetCode] Remove Duplicates from Sorted List II 移除有序链表中的重复项之二 - Grandyang - 博客园







# [[LeetCode] Remove Duplicates from Sorted List II 移除有序链表中的重复项之二](https://www.cnblogs.com/grandyang/p/4069003.html)







Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only *distinct* numbers from the original list.

Example 1:
Input: 1->2->3->3->4->4->5
Output: 1->2->5

Example 2:
Input: 1->1->1->2->3
Output: 2->3


和之前那道 [Remove Duplicates from Sorted List](http://www.cnblogs.com/grandyang/p/4066453.html) 不同的地方是这里要删掉所有的重复项，由于链表开头可能会有重复项，被删掉的话头指针会改变，而最终却还需要返回链表的头指针。所以需要定义一个新的节点，然后链上原链表，然后定义一个前驱指针和一个现指针，每当前驱指针指向新建的节点，现指针从下一个位置开始往下遍历，遇到相同的则继续往下，直到遇到不同项时，把前驱指针的next指向下面那个不同的元素。如果现指针遍历的第一个元素就不相同，则把前驱指针向下移一位。代码如下：



解法一：

```
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        if (!head || !head->next) return head;
        ListNode *dummy = new ListNode(-1), *pre = dummy;
        dummy->next = head;
        while (pre->next) {
            ListNode *cur = pre->next;
            while (cur->next && cur->next->val == cur->val) {
                cur = cur->next;
            }
            if (cur != pre->next) pre->next = cur->next;
            else pre = pre->next;
        }
        return dummy->next;
    }
};
```



同样，我们也可以使用递归来做，首先判空，如果head为空，直接返回。然后判断，若head之后的结点存在，且值相等，那么我们先进行一个while循环，跳过后面所有值相等的结点，到最后一个值相等的点停下。比如对于例子2来说，head停在第三个结点1处，然后我们对后面一个结点调用递归函数，即结点2，这样做的好处是，返回的值就完全把所有的结点1都删掉了。若head之后的结点值不同，那么我们还是对head之后的结点调用递归函数，将返回值连到head的后面，这样head结点还是保留下来了，因为值不同嘛，最后返回head即可，参见代码如下：



解法二：

```
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        if (!head) return head;
        if (head->next && head->val == head->next->val) {
            while (head->next && head->val == head->next->val) {
                head = head->next;
            }
            return deleteDuplicates(head->next);
        }
        head->next = deleteDuplicates(head->next);
        return head;
    }
};
```



类似题目：

[Remove Duplicates from Sorted List](http://www.cnblogs.com/grandyang/p/4066453.html)



参考资料：

[https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/](https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/)

[https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/discuss/28335/My-accepted-Java-code](https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/discuss/28335/My-accepted-Java-code)

[https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/discuss/28339/My-Recursive-Java-Solution](https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/discuss/28339/My-Recursive-Java-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Remove Duplicates from Sorted List 移除有序链表中的重复项 - Grandyang - 博客园







# [[LeetCode] Remove Duplicates from Sorted List 移除有序链表中的重复项](https://www.cnblogs.com/grandyang/p/4066453.html)







Given a sorted linked list, delete all duplicates such that each element appear only *once*.

Example 1:
Input: 1->1->2
Output: 1->2

Example 2:
Input: 1->1->2->3->3
Output: 1->2->3


这道题让我们移除给定有序链表的重复项，那么我们可以遍历这个链表，每个结点和其后面的结点比较，如果结点值相同了，我们只要将前面结点的next指针跳过紧挨着的相同值的结点，指向后面一个结点。这样遍历下来，所有重复的结点都会被跳过，留下的链表就是没有重复项的了，代码如下：



解法一：

```
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode *cur = head;
        while (cur && cur->next) {
            if (cur->val == cur->next->val) {
                cur->next = cur->next->next;
            } else {
                cur = cur->next;
            }
        }
        return head;
    }
};
```



我们也可以使用递归的方法来做，首先我们判断是否至少有两个结点，若不是的话，直接返回head。否则对head->next调用递归函数，并赋值给head->next。这里可能比较晕，我们先看后面一句，返回的时候，head结点先跟其身后的结点进行比较，如果值相同，那么返回后面的一个结点，当前的head结点就被跳过了，而如果不同的话，还是返回head结点。我们发现了，进行实质上的删除操作是在最后一句进行了，再来看第二句，我们对head后面的结点调用递归函数，那么就应该suppose返回来的链表就已经没有重复项了，此时接到head结点后面，再第三句的时候再来检查一下head是否又duplicate了，实际上递归一直走到了末尾结点，再不断的回溯回来，进行删除重复结点，参见代码如下：



解法二：

```
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head || !head->next) return head;
        head->next = deleteDuplicates(head->next);
        return (head->val == head->next->val) ? head->next : head;
    }
};
```



类似题目：

[Remove Duplicates from Sorted List II](http://www.cnblogs.com/grandyang/p/4069003.html)



参考资料：

[https://leetcode.com/problems/remove-duplicates-from-sorted-list/](https://leetcode.com/problems/remove-duplicates-from-sorted-list/)

[https://leetcode.com/problems/remove-duplicates-from-sorted-list/discuss/28614/My-pretty-solution.-Java.](https://leetcode.com/problems/remove-duplicates-from-sorted-list/discuss/28614/My-pretty-solution.-Java.)

[https://leetcode.com/problems/remove-duplicates-from-sorted-list/discuss/28625/3-Line-JAVA-recursive-solution](https://leetcode.com/problems/remove-duplicates-from-sorted-list/discuss/28625/3-Line-JAVA-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













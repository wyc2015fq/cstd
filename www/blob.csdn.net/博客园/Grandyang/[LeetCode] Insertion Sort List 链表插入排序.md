# [LeetCode] Insertion Sort List 链表插入排序 - Grandyang - 博客园







# [[LeetCode] Insertion Sort List 链表插入排序](https://www.cnblogs.com/grandyang/p/4250107.html)







Sort a linked list using insertion sort.

![](https://upload.wikimedia.org/wikipedia/commons/0/0f/Insertion-sort-example-300px.gif)
A graphical example of insertion sort. The partial sorted list (black) initially contains only the first element in the list.
With each iteration one element (red) is removed from the input data and inserted in-place into the sorted list


Algorithm of Insertion Sort:
- Insertion sort iterates, consuming one input element each repetition, and growing a sorted output list.
- At each iteration, insertion sort removes one element from the input data, finds the location it belongs within the sorted list, and inserts it there.
- It repeats until no input elements remain.


Example 1:
Input: 4->2->1->3
Output: 1->2->3->4

Example 2:
Input: -1->5->3->4->0
Output: -1->0->3->4->5


链表的插入排序实现原理很简单，就是一个元素一个元素的从原链表中取出来，然后按顺序插入到新链表中，时间复杂度为O(n2)，是一种效率并不是很高的算法，但是空间复杂度为O(1)，以高时间复杂度换取了低空间复杂度。代码如下：



```
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        while (head) {
            ListNode *t = head->next;
            cur = dummy;
            while (cur->next && cur->next->val <= head->val) {
                cur = cur->next;
            }
            head->next = cur->next;
            cur->next = head;
            head = t;
        }
        return dummy->next;
    }
};
```



类似题目：

[Sort List](http://www.cnblogs.com/grandyang/p/4249905.html)

[Insert into a Cyclic Sorted List](https://www.cnblogs.com/grandyang/p/9981163.html)



参考资料：

[https://leetcode.com/problems/insertion-sort-list/](https://leetcode.com/problems/insertion-sort-list/)

[https://leetcode.com/problems/insertion-sort-list/discuss/46423/Explained-C%2B%2B-solution-(24ms)](https://leetcode.com/problems/insertion-sort-list/discuss/46423/Explained-C%2B%2B-solution-(24ms))

[https://leetcode.com/problems/insertion-sort-list/discuss/46420/An-easy-and-clear-way-to-sort-(-O(1)-space-)](https://leetcode.com/problems/insertion-sort-list/discuss/46420/An-easy-and-clear-way-to-sort-(-O(1)-space-))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













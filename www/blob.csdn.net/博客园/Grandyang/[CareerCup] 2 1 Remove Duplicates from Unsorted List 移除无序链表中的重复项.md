# [CareerCup] 2.1 Remove Duplicates from Unsorted List 移除无序链表中的重复项 - Grandyang - 博客园







# [[CareerCup] 2.1 Remove Duplicates from Unsorted List 移除无序链表中的重复项](https://www.cnblogs.com/grandyang/p/4656399.html)







2.1 Write code to remove duplicates from an unsorted linked list.
FOLLOW UP
How would you solve this problem if a temporary buffer is not allowed?



这道题让我们移除无序链表中的重复项，在LeetCode中有两道类似的题是[Remove Duplicates from Sorted List 移除有序链表中的重复项](http://www.cnblogs.com/grandyang/p/4066453.html) 和 [Remove Duplicates from Sorted List II 移除有序链表中的重复项之二](http://www.cnblogs.com/grandyang/p/4069003.html)。这两道都是针对有序链表的，而这道题是针对无序链表的，其实难度也不是很大。很多对于链表的处理的题都需要在头结点前建立一个dummy node，目的是为了防止头结点被移除，没法返回新的头结点位置。而这道题不用，因为此题让我们删除重复的节点，不是全删掉，而是会保留一个，那么不管头结点有没有重复项，都会保留下来。这题我们可以用哈希表来解，思路是对于每一个节点，如果在哈希表中存在，则删掉，若不存在，则加入哈希表，参见代码如下：



```
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        ListNode *pre = NULL, *cur = head;
        int m[256] = {0};
        while (cur) {
            if (m[cur->val] > 0) {
                pre->next = cur->next;
            } else {
                ++m[cur->val];
                pre = cur;
            }
            cur = cur->next;
        }
        return head;
    }
};
```



这道题的Follow Up让我们不要用额外空间，即空间复杂度应为O(1)，那么我们需要两个while循环来解，同时需要两个指针，第一个指针指向一个节点，第二个指针从下一个为位置开始遍历到链表末尾，遇到相同的就删掉。以此类推直到第一个指针完成链表的遍历即可删掉所有的重复项，整体的思路和冒泡排序有些类似，但是这种方法的时间复杂度为O(n2)，是一种以时间来换取空间的方法，参见代码如下：



```
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        ListNode *pre = head, *cur = head;
        while (pre) {
            cur = pre->next;
            while (cur) {
                if (cur->val == pre->val) {
                    pre->next = cur->next;
                }
                cur = cur->next;
            }
            pre = pre->next;
        }
        return head;
    }
};
```















# [LeetCode] Remove Linked List Elements 移除链表元素 - Grandyang - 博客园







# [[LeetCode] Remove Linked List Elements 移除链表元素](https://www.cnblogs.com/grandyang/p/4452252.html)







Remove all elements from a linked list of integers that have value ***val***.

**Example**
***Given:*** 1 --> 2 --> 6 --> 3 --> 4 --> 5 --> 6,  ***val*** = 6
***Return:*** 1 --> 2 --> 3 --> 4 --> 5


**Credits:**
Special thanks to [@mithmatt](https://leetcode.com/discuss/user/mithmatt) for adding this problem and creating all test cases.



这道移除链表元素是链表的基本操作之一，没有太大的难度，就是考察了基本的链表遍历和设置指针的知识点，我们只需定义几个辅助指针，然后遍历原链表，遇到与给定值相同的元素，将该元素的前后连个节点连接起来，然后删除该元素即可，要注意的是还是需要在链表开头加上一个dummy node，具体实现参见代码如下：



解法一：

```
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode *dummy = new ListNode(-1), *pre = dummy;
        dummy->next = head;
        while (pre->next) {
            if (pre->next->val == val) {
                ListNode *t = pre->next;
                pre->next = t->next;
                t->next = NULL;
                delete t;
            } else {
                pre = pre->next;
            }
        }
        return dummy->next;
    }
};
```



如果只是为了通过OJ，不用写的那么严格的话，下面这种方法更加简洁，当判断下一个结点的值跟给定值相同的话，直接跳过下一个结点，将next指向下下一个结点，而根本不断开下一个结点的next，更不用删除下一个结点了。最后还要验证头结点是否需要删除，要的话直接返回下一个结点，参见代码如下：



解法二：

```
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        if (!head) return NULL;
        ListNode *cur = head;
        while (cur->next) {
            if (cur->next->val == val) cur->next = cur->next->next;
            else cur = cur->next;
        }
        return head->val == val ? head->next : head;
    }
};
```



我们也可以用递归来解，写法很简洁，通过递归调用到链表末尾，然后回来，需要要删的元素，将链表next指针指向下一个元素即可：



解法三：

```
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        if (!head) return NULL;
        head->next = removeElements(head->next, val);
        return head->val == val ? head->next : head;
    }
};
```



类似题目：

[Remove Element](http://www.cnblogs.com/grandyang/p/4606700.html)

[Delete Node in a Linked List](http://www.cnblogs.com/grandyang/p/4647576.html)



参考资料：

[https://leetcode.com/problems/remove-linked-list-elements/](https://leetcode.com/problems/remove-linked-list-elements/)

[https://leetcode.com/problems/remove-linked-list-elements/discuss/57324/AC-Java-solution](https://leetcode.com/problems/remove-linked-list-elements/discuss/57324/AC-Java-solution)

[https://leetcode.com/problems/remove-linked-list-elements/discuss/57306/3-line-recursive-solution](https://leetcode.com/problems/remove-linked-list-elements/discuss/57306/3-line-recursive-solution)

[https://leetcode.com/problems/remove-linked-list-elements/discuss/57331/Accepted-7-line-clean-java-solution](https://leetcode.com/problems/remove-linked-list-elements/discuss/57331/Accepted-7-line-clean-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













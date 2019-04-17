# [LeetCode] Palindrome Linked List 回文链表 - Grandyang - 博客园







# [[LeetCode] Palindrome Linked List 回文链表](https://www.cnblogs.com/grandyang/p/4635425.html)







Given a singly linked list, determine if it is a palindrome. Follow up: Could you do it in O(n) time and O(1) space?

这道题让我们判断一个链表是否为回文链表，LeetCode中关于回文串的题共有六道，除了这道，其他的五道为[Palindrome Number 验证回文数字](http://www.cnblogs.com/grandyang/p/4125510.html)，[Validate Palindrome 验证回文字符串](http://www.cnblogs.com/grandyang/p/4030114.html)，[Palindrome Partitioning 拆分回文串](http://www.cnblogs.com/grandyang/p/4270008.html)，[Palindrome Partitioning II 拆分回文串之二](http://www.cnblogs.com/grandyang/p/4271456.html) 和 [Longest Palindromic Substring 最长回文串](http://www.cnblogs.com/grandyang/p/4464476.html).链表比字符串难的地方就在于不能通过坐标来直接访问，而只能从头开始遍历到某个位置。那么根据回文串的特点，我们需要比较对应位置的值是否相等，那么我们首先需要找到链表的中点，这个可以用快慢指针来实现，使用方法可以参见之前的两篇[Convert Sorted List to Binary Search Tree 将有序链表转为二叉搜索树](http://www.cnblogs.com/grandyang/p/4295618.html) 和 [Reorder List 链表重排序](http://www.cnblogs.com/grandyang/p/4254860.html)，我们使用快慢指针找中点的原理是fast和slow两个指针，每次快指针走两步，慢指针走一步，等快指针走完时，慢指针的位置就是中点。我们还需要用栈，每次慢指针走一步，都把值存入栈中，等到达中点时，链表的前半段都存入栈中了，由于栈的后进先出的性质，就可以和后半段链表按照回文对应的顺序比较了。代码如下：



解法一：

```
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode *slow = head, *fast = head;
        stack<int> s;
        s.push(head->val);
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
            s.push(slow->val);
        }
        if (!fast->next) s.pop();
        while (slow->next) {
            slow = slow->next;
            int tmp = s.top(); s.pop();
            if (tmp != slow->val) return false;
        }
        return true;
    }
};
```



这道题的Follow Up让我们用O(1)的空间，那就是说我们不能使用stack了，那么如果代替stack的作用呢，用stack的目的是为了利用其后进先出的特点，好倒着取出前半段的元素。那么现在我们不用stack了，如何倒着取元素呢。我们可以在找到中点后，将后半段的链表翻转一下，这样我们就可以按照回文的顺序比较了，参见代码如下：



解法二：

```
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode *slow = head, *fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode *last = slow->next, *pre = head;
        while (last->next) {
            ListNode *tmp = last->next;
            last->next = tmp->next;
            tmp->next = slow->next;
            slow->next = tmp;
        }
        while (slow->next) {
            slow = slow->next;
            if (pre->val != slow->val) return false;
            pre = pre->next;
        }
        return true;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













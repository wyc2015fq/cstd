# [CareerCup] 2.5 Add Two Numbers 两个数字相加 - Grandyang - 博客园







# [[CareerCup] 2.5 Add Two Numbers 两个数字相加](https://www.cnblogs.com/grandyang/p/4662599.html)







2.5 You have two numbers represented by a linked list, where each node contains a single digit. The digits are stored in reverse order, such that the 1's digit is at the head of the list. Write a function that adds the two numbers and returns the sum as a linked list.
EXAMPLE
Input: (7-> 1 -> 6) + (5 -> 9 -> 2).That is, 617 + 295.
Output: 2 -> 1 -> 9.That is, 912.
FOLLOW UP
Suppose the digits are stored in forward order. Repeat the above problem.
EXAMPLE
Input: (6 -> 1 -> 7) + (2 -> 9 -> 5).That is, 617 + 295.
Output: 9 -> 1 -> 2.That is, 912.



LeetCode上的原题，请参见我之前的博客[Add Two Numbers 两个数字相加](http://www.cnblogs.com/grandyang/p/4129891.html)。

跟那道LeetCode有所不同的是，这道题还有个Follow Up，把链表存的数字方向变了，原来是表头存最低位，现在是表头存最高位。既然是翻转了链表，那么一种直接的解法是把两个输入链表都各自翻转一下，然后用之前的方法相加完成，再把得到的结果翻转一次，就是结果了，翻转链表的方法可以参见我之前的博客[Reverse Linked List 倒置链表](http://www.cnblogs.com/grandyang/p/4478820.html)。代码如下：



解法一：

```
// Follow up
class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        ListNode *dummy = new ListNode(-1);
        ListNode *cur = dummy;
        int carry = 0;
        l1 = reverseList(l1);
        l2 = reverseList(l2);
        while (l1 || l2) {
            int n1 = l1 ? l1->val : 0;
            int n2 = l2 ? l2->val : 0;
            int sum = n1 + n2 + carry;
            carry = sum / 10;
            cur->next = new ListNode(sum % 10);
            cur = cur->next;
            if (l1) l1 = l1->next;
            if (l2) l2 = l2->next;
        }
        if (carry) cur->next = new ListNode(1);
        return reverseList(dummy->next);
    }
    ListNode *reverseList(ListNode *head) {
        if (!head) return head;
        ListNode *dummy = new ListNode(-1);
        dummy->next = head;
        ListNode *cur = head;
        while (cur->next) {
            ListNode *tmp = cur->next;
            cur->next = tmp->next;
            tmp->next = dummy->next;
            dummy->next = tmp;
        }
        return dummy->next;
    }
};
```



如果我们不采用翻转链表的方法该怎么做呢，这就比较复杂了。首先我们要县分别计算出两个链表的长度，然后给稍短一点的链表前面补0，补到和另一个链表相同的长度。由于要从低位开始相加，而低位是链表的末尾，所以我们采用递归来处理，先遍历到链表的末尾，然后从后面相加，进位标示符carry用的是引用，这样保证了再递归回溯时值可以正确传递，每次计算的节点后面接上上一次回溯的节点，直到回到首节点完成递归。最后还是处理最高位的进位问题。代码如下：



解法二：

```
// Follow up
class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        int n1 = 0, n2 = 0, carry = 0;;
        n1 = getLength(l1);
        n2 = getLength(l2);
        if (n1 > n2) l2 = padList(l2, n1 - n2);
        if (n2 > n1) l1 = padList(l1, n2 - n1);
        ListNode *res = addTwoNumbersDFS(l1, l2, carry);
        if (carry == 1) {
            ListNode *tmp = new ListNode(1);
            tmp->next = res;
            res = tmp;
        }
        return res;
    }
    ListNode *addTwoNumbersDFS(ListNode *l1, ListNode *l2, int &carry) {
        if (!l1 && !l2) return NULL;
        ListNode *list = addTwoNumbersDFS(l1->next, l2->next, carry);
        int sum = l1->val + l2->val + carry;
        ListNode *res = new ListNode(sum % 10);
        res->next = list;
        carry = sum / 10;
        return res;
    }
    ListNode *padList(ListNode *list, int len) {
        ListNode *dummy = new ListNode(-1);
        ListNode *cur = dummy;
        for (int i = 0; i < len; ++i) {
            cur->next = new ListNode(0);
            cur = cur->next;
        }
        cur->next = list;
        return dummy->next;
    }
    int getLength(ListNode *list) {
        ListNode *cur = list;
        int res = 0;
        while (cur) {
            ++res;
            cur = cur->next;
        }
        return res;
    }
};
```















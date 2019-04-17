# [LeetCode] Add Two Numbers 两个数字相加 - Grandyang - 博客园







# [[LeetCode] Add Two Numbers 两个数字相加](https://www.cnblogs.com/grandyang/p/4129891.html)







You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.


这道并不是什么难题，算法很简单，链表的数据类型也不难，就是建立一个新链表，然后把输入的两个链表从头往后撸，每两个相加，添加一个新节点到新链表后面。为了避免两个输入链表同时为空，我们建立一个dummy结点，将两个结点相加生成的新结点按顺序加到dummy结点之后，由于dummy结点本身不能变，所以我们用一个指针cur来指向新链表的最后一个结点。好，可以开始让两个链表相加了，这道题好就好在最低位在链表的开头，所以我们可以在遍历链表的同时按从低到高的顺序直接相加。while循环的条件两个链表中只要有一个不为空行，由于链表可能为空，所以我们在取当前结点值的时候，先判断一下，若为空则取0，否则取结点值。然后把两个结点值相加，同时还要加上进位carry。然后更新carry，直接 sum/10 即可，然后以 sum%10 为值建立一个新结点，连到cur后面，然后cur移动到下一个结点。之后再更新两个结点，若存在，则指向下一个位置。while循环退出之后，最高位的进位问题要最后特殊处理一下，若carry为1，则再建一个值为1的结点，代码如下：



C++ 解法： 

```
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        int carry = 0;
        while (l1 || l2) {
            int val1 = l1 ? l1->val : 0;
            int val2 = l2 ? l2->val : 0;
            int sum = val1 + val2 + carry;
            carry = sum / 10;
            cur->next = new ListNode(sum % 10);
            cur = cur->next;
            if (l1) l1 = l1->next;
            if (l2) l2 = l2->next;
        }
        if (carry) cur->next = new ListNode(1);
        return dummy->next;
    }
};
```



Java 解法：

```
public class Solution {
    public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
        ListNode dummy = new ListNode(-1);
        ListNode cur = dummy;
        int carry = 0;
        while (l1 != null || l2 != null) {
            int d1 = l1 == null ? 0 : l1.val;
            int d2 = l2 == null ? 0 : l2.val;
            int sum = d1 + d2 + carry;
            carry = sum >= 10 ? 1 : 0;
            cur.next = new ListNode(sum % 10);
            cur = cur.next;
            if (l1 != null) l1 = l1.next;
            if (l2 != null) l2 = l2.next;
        }
        if (carry == 1) cur.next = new ListNode(1);
        return dummy.next;
    }
}
```



在CareerCup上的这道题还有个Follow Up，把链表存的数字方向变了，原来是表头存最低位，现在是表头存最高位，请参见我的另一篇博客[2.5 Add Two Numbers 两个数字相加](http://www.cnblogs.com/grandyang/p/4662599.html) 。



类似题目：

[Multiply Strings](http://www.cnblogs.com/grandyang/p/4395356.html)

[Add Binary](http://www.cnblogs.com/grandyang/p/4084971.html)

[Sum of Two Integers](http://www.cnblogs.com/grandyang/p/5631814.html)

[Add Strings](http://www.cnblogs.com/grandyang/p/5944311.html)

[Add Two Numbers II](http://www.cnblogs.com/grandyang/p/6216480.html)



参考资料：

[https://leetcode.com/problems/add-two-numbers/](https://leetcode.com/problems/add-two-numbers/)

[https://leetcode.com/problems/add-two-numbers/discuss/997/c%2B%2B-Sharing-my-11-line-c%2B%2B-solution-can-someone-make-it-even-more-concise](https://leetcode.com/problems/add-two-numbers/discuss/997/c%2B%2B-Sharing-my-11-line-c%2B%2B-solution-can-someone-make-it-even-more-concise)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













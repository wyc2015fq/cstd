# leetcode(3):add - 刘炫320的博客 - CSDN博客
2018年03月05日 20:48:10[刘炫320](https://me.csdn.net/qq_35082030)阅读数：95
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
在这一讲中，我们主要讲解关于加法的相关题目。
# 1.leetcode#2Add Two Numbers
## 1.1题目描述
> 
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list. 
  You may assume the two numbers do not contain any leading zero, except the number 0 itself.
Example
> 
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4) 
  Output: 7 -> 0 -> 8 
  Explanation: 342 + 465 = 807.
## 1.2思路
这道题就是模拟加法的过程，熟悉加法器的同学都知道这个过程是什么样子的。只需要设置一个进位就可以了。但是这里需要考虑一个问题，就是如果两个数不一般长的时候，如何处理。另外一个要注意的地方是如果最后的和比原来最长的数还要进位，该如何处理。
## 1.3代码
```java
public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
        //如果都为空
        if (l1 == null && l2 == null)
            return null;
        ListNode sum_head = null;
        ListNode sum_tail = null;
        int carry = 0;
        //如果两个都不为空
        while (l1 != null && l2 != null) {
            int s = l1.val + l2.val + carry;
            carry = s / 10;
            int value = s % 10;
            if (sum_head == null) {
                sum_head = new ListNode(value);
                sum_tail = sum_head;
            } else {
                sum_tail.next = new ListNode(value);
                sum_tail = sum_tail.next;
            }
            l1 = l1.next;
            l2 = l2.next;
        }
        //找到较长的那个
        ListNode longest = null;
        if (l1 != null) {
            longest = l1;
        } else if (l2 != null) {
            longest = l2;
        }
        //如果两个中有较长的
        while (longest != null) {
            int s = longest.val + carry;
            carry = s / 10;
            int value = s % 10;
            if (sum_head == null) {
                sum_head = new ListNode(value);
                sum_tail = sum_head;
            } else {
                sum_tail.next = new ListNode(value);
                sum_tail = sum_tail.next;
            }
            longest = longest.next;
        }
        //观察进位情况
        if (carry != 0) {
            sum_tail.next = new ListNode(carry);
            return sum_head;
        }
        return sum_head;
    }
```
# 2leetcode#445Add Two Numbers II
## 2.1题目描述
> 
You are given two non-empty linked lists representing two non-negative integers. The most significant digit comes first and each of their nodes contain a single digit. Add the two numbers and return it as a linked list. 
  You may assume the two numbers do not contain any leading zero, except the number 0 itself. 
  Follow up: 
  What if you cannot modify the input lists? In other words, reversing the lists is not allowed.
Example:
> 
Input: (7 -> 2 -> 4 -> 3) + (5 -> 6 -> 4) 
  Output: 7 -> 8 -> 0 -> 7
## 2.2思路
这道题难就难在不能够让列表反转，不然的话，就可以使用上面题目了。因为链表反转是比较容易操作的。但是可以使用其他隐性的反转，比如使用栈来表示倒序，是一个非常不错的方法。
## 2.3代码
```java
Stack<Integer> s1 = new Stack<Integer>();
        Stack<Integer> s2 = new Stack<Integer>();
        //把链表压入栈中用以倒序
        while(l1 != null) {
            s1.push(l1.val);
            l1 = l1.next;
        };
        while(l2 != null) {
            s2.push(l2.val);
            l2 = l2.next;
        }
        int sum = 0;
        ListNode list = new ListNode(0);
        //正常的输出就可以了。
        while (!s1.empty() || !s2.empty()) {
            if (!s1.empty()) sum += s1.pop();
            if (!s2.empty()) sum += s2.pop();
            list.val = sum % 10;
            ListNode head = new ListNode(sum / 10);
            head.next = list;
            list = head;
            sum /= 10;
        }
        return list.val == 0 ? list.next : list;
```
# 3.小结
本节我们只介绍了2个题目，收获如下： 
1.一定弄清楚本质，然后使用计算机来进行模拟。这也是计算机诞生之初所做的事情。 
2.一定要熟悉常用的数据结构，比如链表的操作（逆指、快慢指针、插入、删除、合并等），队列和栈，树（二叉检索树，树的插入和删除，深度优先广度优先搜索）以及HashMap等。

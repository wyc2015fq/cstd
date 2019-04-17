# 翻转链表 II - guoziqing506的博客 - CSDN博客





2016年05月09日 14:28:51[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1562
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：翻转链表中第m个节点到第n个节点的部分；注意事项：m，n满足1 ≤ m ≤ n ≤ 链表长度

样例：给出链表1->2->3->4->5->null， m = 2 和n = 4，返回1->4->3->2->5->null





之前，已经实现过普通的，对于整个链表的翻转（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51295722)），这里增加了一点难度，设置了翻转的位置。但是基本方法没变。难度主要在于“摘链”和“链接”的操作。

所以，我们不妨仿照之前“回文链表”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51340547)）的解决方法，将链表断开，再处理。这是一种偷懒的方法，将问题难度降低了。

简单说，拿样例举例吧，可以先找到翻转的开始位置，这里是2，然后根据这个位置将链表断开：1->null, 2->3->4->5->null，这就形成了两个链表。

然后，将第二个链表依次取头结点，放在1的后面：

1. 1->2->null, 3->4->5->null

2. 1->3->2->null, 4->5->null

。。。

这样的循环进行几次呢，循环3次，也就是n - m + 1次

之后再将现在的两个链表合并即可。代码就可以得到了：



```python
"""
Definition of ListNode

class ListNode(object):

    def __init__(self, val, next=None):
        self.val = val
        self.next = next
"""
class Solution:
    """
    @param head: The head of linked list
    @param m: start position
    @param n: end position
    """
    def reverseBetween(self, head, m, n):
        dummy = ListNode(-1)
        dummy.next = head
        pre = dummy
        count = 1
        # 找到要翻转部分的开始
        while count != m:
            pre = pre.next
            count += 1
        # gap为循环的次数
        gap = n - m + 1
        # 第二部分
        second = pre.next
        # 设置第一部分的尾节点，目的在于最后的合并
        tail = second
        pre.next = None
        while gap != 0:
            cur = second
            second = second.next
            temp = pre.next
            pre.next = cur
            cur.next = temp
            gap -= 1
        # 两部分合并
        tail.next = second
        return dummy.next
        # write your code here
```






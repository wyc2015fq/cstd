# 链表倒数第n个节点 - guoziqing506的博客 - CSDN博客





2016年04月30日 19:33:04[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：955标签：[链表](https://so.csdn.net/so/search/s.do?q=链表&t=blog)
个人分类：[lintcode](https://blog.csdn.net/guoziqing506/article/category/6289140)

所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：找到单链表倒数第n个节点，保证链表中节点的最少数量为n。

样例：给出链表 3->2->1->5->null和n = 2，返回倒数第二个节点的值1.

因为链表是不能回溯的，同时也不像数组那样，每个元素都存有一个自然的索引，可以实现快速查找，所以，我们在这里，通过两个指针来解决这个问题。

想一想之前在“排序链表转换为二分查找树”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51212234)）中，我们求取链表中间节点值的方法，是通过快慢指针，快指针为慢指针速率两倍，通过一次遍历，查到中间节点的。这里，可以借助这种思路，但是稍微改变一下就行：令快指针先走n步，然后慢指针出发，快慢指针保持速率一致（其实严格说不能叫“快慢”了，叫“先后”更为合理），直到快指针为空，慢指针的位置正好是倒数第n个节点。

给出代码：



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
    @param head: The first node of linked list.
    @param n: An integer.
    @return: Nth to last node of a singly linked list. 
    """
    def nthToLast(self, head, n):
        slow, fast = head, head
        for i in range(n):
            fast = fast.next
        while fast:
            slow = slow.next
            fast = fast.next
        return slow
        # write your code here
```
很简单的两指针应用，其实要学会举一反三，通过查找中间节点的方法，启发得到本题的解法。







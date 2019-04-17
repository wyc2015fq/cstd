# 删除排序链表中的重复数字 II - guoziqing506的博客 - CSDN博客





2016年05月02日 22:29:15[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：921
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个排序链表，删除所有重复的元素只留下原链表中没有重复的元素。

样例：给出 1->2->3->3->4->4->5->null，返回 1->2->5->null；给出 1->1->1->2->3->null，返回 2->3->null

还记得之前做过一道删除链表中重复元素的题（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51295495)），那道题中，我用了一个集合来存储出现过的元素，从而在遍历链表过程中决定节点的去留。但是这道题不适用这种方法了，因为只要出现重复就全部删掉。如果还用集合的方法，只能遍历两遍，这显然不够高效。好在这道题还给了另外一个条件：那就是这个链表是排好序的。所以，我们也没必要用一个集合了，直接用一个数存储可能重复的值即可。

具体的思路是这样的：记录第一个节点的值，然后循环，找出所有重复的节点（找的原则就是看此后的节点的值和刚才记录的值是否相等），如果只有一个节点满足相等的条件（就是说明不重复，只有一个节点含有这个值），就继续记录下一个节点的值；如果不止一个节点满足条件（说明含有这个值的节点有重复），那就全部删除，记录删完之后的第一个节点的值。

依照上面的思路遍历链表，直到全部处理完。

代码如下：



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
    @param head: A ListNode
    @return: A ListNode
    """
    def deleteDuplicates(self, head):
        if head is None:
            return head
        dummy = ListNode(-1)
        dummy.next = head
        pre = dummy
        cur = head
        # 从头结点开始遍历
        while cur:

            # 计数
            count = 0

            # 记录用于对比的值
            temp = cur.val

            # 循环至少进行一次，count值至少为1
            # 进行完之后，cur指向的是下一个值不同节点，或者空
            while cur and cur.val == temp:
                count += 1
                cur = cur.next

            # 有重复，直接删除
            if count > 1:
                pre.next = cur
            # 无重复，将pre前移
            else:
                pre = pre.next
        return dummy.next
        # write your code here
```







# LeetCode —— 链表相关（206、141） - Zhang's Wikipedia - CSDN博客


2018年11月03日 22:29:23[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：233



## 0. 数据结构定义
链表节点：
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None
## 1. 链表
206，翻转链表，[206. Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/description/)：
def reverseList(head):
	cur, prev = head, None
	while cur:
		cur.next, prev, cur = prev, cur, cur.next
	return prev
			\# 跳出while循环时，cur 为 None
141，检测链表是否有环：[Linked List Cycle](https://leetcode.com/problems/linked-list-cycle/description/)
链表不可能有多个环，因为一个节点不能有两个后继指针；
可行方案：1、遍历是否能够出现 NIL 空指针；
2、使用 Set 存储已访问过的节点，如果当前待遍历的节点已存储在Set集合中，则证明有环；
3、快慢指针：慢指针一次走一步，快指针一次走两步，如果有环，两者一定会相遇；
def hasCycle(head):
	slow = fast  = head
	while slow and fast and fast.next:
		slow = slow.next
		fast = fast.next.next
		if slow == fast:
			return True
	return False


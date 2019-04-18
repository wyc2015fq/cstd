# 【21-25】LeetCode：Python解题 - Joe的博客 - CSDN博客





2017年12月16日 11:46:14[Joe-Han](https://me.csdn.net/u010089444)阅读数：1091








## **21. Merge Two Sorted Lists**【Easy】

Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.

**Solution 1:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def mergeTwoLists(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        if not l1:
            return l2 
        if not l2:
            return l1 
        head = node = ListNode(0)
        while l1 and l2:
            if l1.val < l2.val:
                node.next = ListNode(l1.val)
                l1 = l1.next
            else:
                node.next = ListNode(l2.val)
                l2 = l2.next
            node = node.next
        if l1:
            while l1:
                node.next = ListNode(l1.val)
                l1 = l1.next
                node = node.next
        elif l2:
            while l2:
                node.next = ListNode(l2.val)
                l2 = l2.next
                node = node.next
        return head.next
```

**Solution 2（递归）:**

```python
class Solution(object):
    def mergeTwoLists(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        if not l1 or l2 and l1.val > l2.val:
            l1, l2 = l2, l1
        if l1:
            l1.next = mergeTwoLists(l1.next, l2)

        return l1
```

**Discussion:**

另一种简洁的思路：[https://discuss.leetcode.com/topic/15242/simple-5-lines-python/4](https://discuss.leetcode.com/topic/15242/simple-5-lines-python/4)

## **22. Generate Parentheses**【Medium】

Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

For example, given n = 3, a solution set is:

```
[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
```

**Solution:**

```python
class Solution(object):
    def recur(self, s, l, r, result):
        if l == 0 and r == 0:
            result.append(''.join(s))
            return 
        if l > 0:
            s.append("(")
            self.recur(s,l-1,r,result)
            s.pop(-1)
        if l < r:
            s.append(")")
            self.recur(s,l,r-1,result)
            s.pop(-1)
    def generateParenthesis(self, n):
        """
        :type n: int
        :rtype: List[str]
        """
        result = []
        self.recur([],n,n,result)
        return result
```

**Discussion:**

采用递归方式，插( 或 )的数量不超过n且可以插入 ） 的前提是 ( 的数量大于 ）。

## **23. Merge k Sorted Lists**【Hard】

Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.

**Solution 1:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def mergeTwo(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        if not l1:
            return l2 
        if not l2:
            return l1 
        head = node = ListNode(0)
        while l1 and l2:
            if l1.val < l2.val:
                node.next = ListNode(l1.val)
                l1 = l1.next
            else:
                node.next = ListNode(l2.val)
                l2 = l2.next
            node = node.next
        if l1:
            while l1:
                node.next = ListNode(l1.val)
                l1 = l1.next
                node = node.next
        elif l2:
            while l2:
                node.next = ListNode(l2.val)
                l2 = l2.next
                node = node.next
        return head.next

    def partition(self, lists, low, high):
        if low == high:
            return lists[low]
        mid = (low+high)/2
        left = self.partition(lists, low, mid)
        right = self.partition(lists, mid+1, high)
        return self.mergeTwo(left, right)

    def mergeKLists(self, lists):
        """
        :type lists: List[ListNode]
        :rtype: ListNode
        """
        n = len(lists)
        if n == 0:
            return []
        return self.partition(lists, 0, n-1)
```

**Solution 2:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None
import heapq

class Solution(object):
    def mergeKLists(self, lists):
        """
        :type lists: List[ListNode]
        :rtype: ListNode
        """
        h = [(l.val, l) for l in lists if l]
        start = node = ListNode(0)
        heapq.heapify(h)
        while h:
            v, n = heapq.heappop(h)
            if n.next:
                heapq.heappush(h, (n.next.val, n.next))
            node.next = n
            node = node.next
        return start.next
```

**Discussion:**

思路1：递归地将lists进行二分，最后合并两个有序数组。思路2：使用heapq，用k个有序列表的第一个元祖作为树的节点构造小顶堆，pop堆顶元素并调整堆，直到堆为空。

## **24. Swap Nodes in Pairs**【Medium】

Given a linked list, swap every two adjacent nodes and return its head.

For example, 

Given 1->2->3->4, you should return the list as 2->1->4->3.

Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.

**Solution:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def swapPairs(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head or not head.next:
            return head
        start = ListNode(0)
        start.next = head
        n0, n1, n2 = start, head, head.next
        while True:
            n0.next, n1.next, n2.next = n2, n2.next, n1
            if n1.next and n1.next.next:
                n0, n1, n2 = n1, n1.next, n1.next.next
            else:
                break
        return start.next
```

## **25. Reverse Nodes in k-Group**【Hard】

Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.

k is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.

You may not alter the values in the nodes, only nodes itself may be changed.

Only constant memory is allowed.

For example, 

Given this linked list: 1->2->3->4->5

For k = 2, you should return: 2->1->4->3->5

For k = 3, you should return: 3->2->1->4->5

**Solution 1:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def reverseKGroup(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        start = preNode =ListNode(0)
        start.next = l = r = head
        while True:
            for i in range(k):
                if r:
                    r = r.next
                else:
                    return start.next
            cur, nex = l, r
            for i in range(k):
                cur.next, cur, nex = nex, cur.next, cur
            l, preNode.next, preNode = r, nex, l
```

**Solution 2:（对Solution 1优化）**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def reverseKGroup(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        start = preNode =ListNode(0)
        start.next = l = r = head
        flag = True
        for i in range(k):
            if r:
                r = r.next
            else:
                return start.next        
        while True:
            if not flag:
                return start.next
            cur, nextNode = l, r
            for i in range(k):
                if r:
                    r = r.next
                else:
                    flag = False                
                cur.next, cur, nextNode = nextNode, cur.next, cur
            l, preNode.next, preNode = l.next, nextNode, l
```

**Discussion:**

将list的每k个节点作为一组，依次对每组进行翻转，当某组的节点数小于k时，直接返回head。





# 148. Sort List - OraYang的博客 - CSDN博客

2018年03月04日 14:06:51[OraYang](https://me.csdn.net/u010665216)阅读数：207标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[sort																](https://so.csdn.net/so/search/s.do?q=sort&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Sort a linked list in O(n log n) time using constant space complexity.
# 思路
本题要求将一个链表进行排序，空间复杂度为O(1)$O(1)$，我们采用归并排序的方法。首先我们实现一个merge函数，然后实现一个getmid函数（用来找中间结点）,最后用递归实现。
# 代码
```python
/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode(int x) { val = x; }
 * }
 */
```
```python
class Solution {
```
```python
private
```
```python
static
```
```python
boolean
```
```python
less
```
```python
(ListNode w,ListNode v)
    {
```
```python
return
```
```python
w.val<v.val;
    }
```
```python
private
```
```python
static
```
```python
ListNode
```
```python
merge
```
```python
(ListNode first,ListNode second)
    {
        ListNode head =
```
```python
new
```
```python
ListNode(
```
```python
0
```
```python
);
        ListNode tmp = head;
```
```python
while
```
```python
(first!=
```
```python
null
```
```python
||second!=
```
```python
null
```
```python
)
        {
```
```python
if
```
```python
(first==
```
```python
null
```
```python
) {head.next = second;
```
```python
break
```
```python
;}
```
```python
else
```
```python
if
```
```python
(second==
```
```python
null
```
```python
) {head.next = first;
```
```python
break
```
```python
;}
```
```python
else
```
```python
if
```
```python
(less(first,second))
            {
                head.next = first;
                first = first.next;
                head = head.next;
            }
```
```python
else
```
```python
{
                head.next = second;
                second = second.next;
                head = head.next;
            }
        }
        tmp = tmp.next;
```
```python
return
```
```python
tmp;
    }
```
```python
private
```
```python
static
```
```python
ListNode
```
```python
getmid
```
```python
(ListNode head)
    {
        ListNode first = head;
        ListNode second = head;
        ListNode prev = head;
```
```python
while
```
```python
(second.next!=
```
```python
null
```
```python
)
        {
            second = second.next;
            prev = first;
            first = first.next;
```
```python
if
```
```python
(second.next ==
```
```python
null
```
```python
)
            {
```
```python
break
```
```python
;
            }
            second = second.next;
        }
        prev.next =
```
```python
null
```
```python
;
```
```python
return
```
```python
first;
    }
```
```python
public
```
```python
ListNode
```
```python
sortList
```
```python
(ListNode head) {
```
```python
if
```
```python
(head==
```
```python
null
```
```python
||head.next==
```
```python
null
```
```python
)
```
```python
return
```
```python
head;
        ListNode mid = getmid(head);
        ListNode head1;
        ListNode head2;
        head1 = sortList(head);
        head2 = sortList(mid);
```
```python
return
```
```python
merge(head1,head2);
    }
}
```


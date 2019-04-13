
# 147. Insertion Sort List - OraYang的博客 - CSDN博客

2018年03月04日 14:16:27[OraYang](https://me.csdn.net/u010665216)阅读数：190所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Sort a linked list using insertion sort.
# 思路
典型的插入排序，方法直接看代码
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
public
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
public
```
```python
ListNode
```
```python
insertionSortList
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
        ListNode helper =
```
```python
new
```
```python
ListNode(Integer.MIN_VALUE);
        ListNode cur = head;
        ListNode pre = helper;
        ListNode next =
```
```python
null
```
```python
;
```
```python
while
```
```python
(cur!=
```
```python
null
```
```python
)
        {
            next = cur.next;
```
```python
//store cur.next
```
```python
while
```
```python
(pre.next!=
```
```python
null
```
```python
&&less(pre.next,cur))
            {
                pre = pre.next;
            }
```
```python
//insert cur between pre and pre.next
```
```python
cur.next = pre.next;
            pre.next = cur;
            pre = helper;
            cur = next;
        }
```
```python
return
```
```python
helper.next;
    }
}
```


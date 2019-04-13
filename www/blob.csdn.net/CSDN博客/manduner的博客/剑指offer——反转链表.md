
# 剑指offer——反转链表 - manduner的博客 - CSDN博客


2019年03月17日 14:46:36[Manduner_TJU](https://me.csdn.net/manduner)阅读数：120


# 1，问题描述
输入一个链表，反转链表后，输出新链表的表头。
# 2，解题思路
（1）首先，保留链表头节点的next节点，其中，设头结点为aNode（aNode = head），它的下一个节点为bNode = aNode.next。
（2）然后，将aNode.next置空，即，aNode.next = null。此时aNode就表示一个新链表（只有一个节点），aNode也是该链表的头结点。
（3）最终，bNode不为空的情况下循环执行以下步骤：
保留bNode的下一个节点，即，ListNode temp = bNode.next;
将bNode指向新链表的头结点，即，bNode.next = aNode。
再将bNode表示成该新链表的头结点，即，aNode = bNode；也就是用aNode这个变量一直表示新链表的头结点。
最后将bNode设置成旧链表的头结点。
**整个过程就是，先通过（1）（2）将原来的链表拆分成一个新链表和一个旧链表，再通过（3）依次将旧链表的头结点（bNode）指向新链表的头结点（aNode）的过程。**
# 3，源码
```python
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
public class Solution {
    public ListNode ReverseList(ListNode head) {
        if(head == null) return null;
        ListNode aNode = head;
        ListNode bNode = aNode.next;
        aNode.next = null;
        while(bNode != null){
            ListNode temp = bNode.next;
            bNode.next = aNode;
            aNode = bNode;
            bNode = temp;
         }
        return aNode;
        }
    }
```



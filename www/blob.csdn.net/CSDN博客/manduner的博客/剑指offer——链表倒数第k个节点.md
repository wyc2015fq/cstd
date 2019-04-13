
# 剑指offer——链表倒数第k个节点 - manduner的博客 - CSDN博客


2019年03月15日 23:20:24[Manduner_TJU](https://me.csdn.net/manduner)阅读数：7


# 1，问题描述
输入一个链表，输出该链表中倒数第k个结点。
# 2，解题思路
倒数第k个节点和最后一个节点的距离是k-1，假设现在有两个指针分别指向倒数第k个节点和最后一个节点，那么指向倒数第k个节点的指针走k-1步就能到达最后一个节点，也就是说两个指针的间隔是k-1。
所以我们可以先设定两个同时指向头结点的指针a和b，先让a指针走k-1步，此时a和b的间隔就是k-1；然后让a和b同时向后走，直到a到达最后一个节点，此时b所指向的节点就是倒数第k个节点。
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
    public ListNode FindKthToTail(ListNode head,int k) {
        if(head==null || k<=0) return null;
        ListNode aNode = head;
        ListNode bNode = head;
        for(int i=0; i<k-1;i++){
            if(aNode.next != null){
                aNode = aNode.next;
            }else{
                return null; //k超过了链表长度
            }
        }
        
        while(aNode.next != null){
            aNode = aNode.next;
            bNode = bNode.next;
        }
        return bNode;
    }
}
```



# Lintcode174——删除链表中倒数第n个节点 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:05:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：86标签：[Lintcode																[算法																[链表																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)





## 题目：

> 
给定一个链表，删除链表中倒数第n个节点，返回链表的头节点。


又get了一方法

## 题解：

```
class ListNode {
      int val;
      ListNode next;
      ListNode(int val) {
          this.val = val;
          this.next = null;
      }
  }
public class Solution {
    public static ListNode removeNthFromEnd(ListNode head, int n) {
         if (n <= 0) {
            return null;
        }

        ListNode dummy = new ListNode(0);
        dummy.next = head;

        ListNode preDelete = dummy;
        for (int i = 0; i < n; i++) {
            if (head == null) {
                return null;
            }
            head = head.next;
        }
        while (head != null) {
            head = head.next;
            preDelete = preDelete.next;
        }
        preDelete.next = preDelete.next.next;
        return dummy.next;
    }
    public static void main(String[] args){
        ListNode head=new ListNode(1);
        ListNode head1=new ListNode(2);
        ListNode head2=new ListNode(3);
        ListNode head3=new ListNode(4);
        ListNode head4=new ListNode(5);
        head1.next=head2;
        head2.next=head3;
        head3.next=head4;
        head4.next=null;
        removeNthFromEnd(head,2);
        System.out.println(head.val+" "+head1.val+" "+head2.val);
    }
}
```

## 思路

链表从后遍历到第n个———-> 

a)先用一个节点dummy保存head节点的前一节点，最后返回dummy.next 

b)用一个循环让head节点右移n个节点 

c)用一和dummy同指向的节点preDelete，和head节点同步移动，直到head移动到null(尾节点.next) 

d)用preDelete.next=preDelete.next.next删除该节点](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)





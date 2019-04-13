
# 剑指offer——合并两个排序的链表 - manduner的博客 - CSDN博客


2019年03月17日 23:10:11[Manduner_TJU](https://me.csdn.net/manduner)阅读数：18


# 1，问题描述
输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
注：em。。。。。。。。。。这是我面试的一家公司的编程题，相对来说还是挺简单的，遇到这样的面试官，就偷着乐吧0.0
# 2，解题思路
有两种思路，一种是：遍历一个链表，插入另一个链表里。另一种是采用递归的方式，比较两个链表头结点的值的大小，返回值较小的那个节点。相对来说，更推荐采用递归方法的思路，因为它既清晰又简单，运行起来还快。
## 第一种方法详述：
不用详述，就是每次比较两个链表的头结点的值的大小，返回值较小的那个节点，然后采用递归的方式继续进行同样过程。看源码就能秒懂的。
## 第二种方法详述：
（1）先进行临界值判断，如果其中一个为空，返回另一个。
（2）然后aNode = list1, bNode=list2（这样做的目的是在程序执行过程中，能够一直记住两个输入链表的头结点，方便确定最后需要返回哪一个链表）；
（3）接着遍历aNode，如果aNode不为空：
判断aNode与bNode值的大小。
如果aNode.val <= bNode.val，将aNode节点插到bNode的前边。(此时就能确定最终返回的是list1）。
否则，将aNode节点插到bNode的后边。（此时能确定最后返回的应该是list2）。
（4）整个流程如下图所示（其中虚线表示原链表中的链接，实线表示合并过程）：
![](https://img-blog.csdnimg.cn/20190317225939433.png)
# 3，源码
## 第一种方法：
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
    public ListNode Merge(ListNode list1,ListNode list2) {
        if(list1 == null) return list2;
        if(list2 == null) return list1;
        ListNode head = null;
        if(list1.val <= list2.val){
            head = list1;
            head.next = Merge(list1.next,list2);
        }else{
            head = list2;
            head.next = Merge(list1,list2.next);
        }
        return head;
    }
}
```
## 第二种方法：
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
    public ListNode Merge(ListNode list1,ListNode list2) {
        ListNode aNode = list1;
        ListNode bNode = list2;
        if(aNode == null) return bNode;
        if(bNode == null) return aNode;
        ListNode list3 = null;
        while(aNode != null){
            if(aNode.val <= bNode.val){
                ListNode temp = aNode.next;
                aNode.next = bNode;
                list3 = list1;
                aNode = temp;
            }else{
                if(bNode.next == null) break;
                ListNode temp = bNode.next;
                bNode.next = aNode;
                list3 = list2;
                bNode = temp;
            }
        }
        return list3;
    }
}
```



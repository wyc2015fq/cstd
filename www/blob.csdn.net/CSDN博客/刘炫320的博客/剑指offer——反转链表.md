# 剑指offer——反转链表 - 刘炫320的博客 - CSDN博客
2017年05月04日 21:10:18[刘炫320](https://me.csdn.net/qq_35082030)阅读数：302标签：[剑指offer-java实现																[反转链表																[栈																[递归](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=栈&t=blog)](https://so.csdn.net/so/search/s.do?q=反转链表&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 题目描述
> 
输入一个链表，反转链表后，输出链表的所有元素。
# 2. 解决方法
反转链表是一个非常基础的操作，但是也是一个非常有技术含量的操作。能看出一个人的基本功扎不扎实。
## 2.1 level1
反转链表，反转，第一眼应当想到的是栈，因为栈具有这样的特性。所以我们可以使用栈来辅助我们完成这个操作。代码如下：
```java
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
import java.util.Stack;
public class Solution {
    public ListNode ReverseList(ListNode head) {
        Stack<ListNode> stack=new Stack<ListNode>();
        ListNode tempnode=head;
        while(tempnode!=null){
            stack.push(tempnode);
            tempnode=tempnode.next;
        }
        ListNode newhead=null;
        while (!stack.isEmpty()) {
            ListNode newtemp=stack.pop() ;
            if(newhead==null){
                newhead=newtemp;
                //获取头
                tempnode=newhead;
            }
            else {
                //迭代
                newhead.next=newtemp;
                newhead=newhead.next;
            }
        }
        if(newhead!=null){
            //链表结束符
            newhead.next=null;
        }
        return tempnode;
    }
}
```
## 2.2 level2
但是，这种算法时间复杂度虽然是O（n）但是其实是2n，因为这需要入栈，出栈。最重要的，它需要额外的数据结构Stack。如果不允许使用额外的数据结构呢？ 
那就一个一个来做呗。 
事实上，这种方法所遇到的困难就是指针，我们先考虑3个节点之间的反转，也就是以中间一个结点反转它的指针。
第一个问题就是当撤除了第一个结点的指针后，第一个结点就有可能处于游离状态，那么我们需要一个指针来指向它，然后再把第二个结点的next指针指向第一个的结点。
第二个问题就是当你把第二个结点的next指针指向第一个结点的时候，那么第三个结点就有可能处于游离态，因此需要在断开第二个、第三个结点之前，需要记住第三个结点，因此我们需要一个指针记住它。这样，我们就完成了所有的暂存，可以放心的转移我们的next指针了。当转移后，不要忘记更新各个指针，方便下一次迭代。这样只需要遍历一次就可以，时间复杂度为O（n）。
代码如下：
```java
public static ListNode ReverseList(ListNode head) {
        ListNode pre = null;
        ListNode next = null;
        while (head != null) {
            next = head.next;
            head.next = pre;
            pre = head;
            head = next;
        }
        return pre;
```
## 2.3 level3
上一级别中，我们考虑的是3个结点之间的关系。其实细细想来，只需要考虑两个节点之间的互换即可，如果使用递归的话，可以让事情更简单，代码如下所示：
```java
public static ListNode ReverseList(ListNode head) {
        //如果链表为空或者链表中只有一个元素 
        if(head==null||head.next==null) 
            return head;
        //先反转后面的链表，走到链表的末端结点
        ListNode pReverseNode=ReverseList(head.next);
        //再将当前节点设置为后面节点的后续节点
        head.next.next=head;
        head.next=null;
        //返回结点
        return pReverseNode;
    }
```
# 3 一点思考
正如之前所说，最快的路径一定是你走的最熟的路径，但是真正最短的路径，是把所有表面都拨开，看见本质的时候。当把链表反转看到只是2个结点反转的时候，就已经接近答案了！

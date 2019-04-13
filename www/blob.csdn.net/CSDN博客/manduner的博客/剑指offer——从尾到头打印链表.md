
# 剑指offer——从尾到头打印链表 - manduner的博客 - CSDN博客


2018年12月19日 23:21:26[Manduner_TJU](https://me.csdn.net/manduner)阅读数：47标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
输入一个链表，按链表值从尾到头的顺序返回一个ArrayList。
# 2，求解思路
## （1）理解题意
刚开始看这个题的时候是懵的，因为还没接触过java中链表类型，想象不出输入的链表是啥样的。后来观察到原题中给出了链表中节点类的定义。在此回忆下数据结构中的链表：一个链表由多个节点组成，每个节点（ListNode）由两部分组成，即节点值（value）和指针（next），如下图所示：
![](https://img-blog.csdnimg.cn/20181219230022374.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
但是，仔细看一下java中是怎么定义链表节点的，如源码中所示，val表示节点值，next表示ListNode节点类，即代表该节点下一个节点的类名，例如，定义链表中的第一个节点，如下：
ListNode1.val = 10
ListNode1.next = ListNode2
其余节点定义类似。最后一个节点(假设有100个节点)的定义如下：
ListNode100.val = 100
ListNode100.next = null
## （2）解题思路分析
定义一个ArrayList列表
从头到尾获取链表中每个节点的值，并添加到列表中
反转列表
# 3，源码
```python
/**
*    public class ListNode {
*        int val;
*        ListNode next = null;
*
*        ListNode(int val) {
*            this.val = val;
*        }
*    }
*
*/
import java.util.ArrayList;
import java.util.Collections;
public class Solution {
    public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
        ArrayList<Integer> result = new ArrayList<Integer>();
        while(listNode != null){
            result.add(listNode.val);
            listNode = listNode.next;
        }
        //反转列表
        Collections.reverse(result);
        return result;
    }
}
```



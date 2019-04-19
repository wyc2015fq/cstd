# 剑指offer——从尾到头打印链表 - 刘炫320的博客 - CSDN博客
2017年07月05日 18:55:09[刘炫320](https://me.csdn.net/qq_35082030)阅读数：225标签：[链表																[反转																[头插法																[递归																[栈](https://so.csdn.net/so/search/s.do?q=栈&t=blog)](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=头插法&t=blog)](https://so.csdn.net/so/search/s.do?q=反转&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入一个链表，从尾到头打印链表每个节点的值。
# 2. 解决思路
题目已经给出了一个预先定义好的链表：
```java
public class ListNode {
        int val;
        ListNode next = null;
        ListNode(int val) {
           this.val = val;
      }
   }
```
那么这道题和反转链表就比较像了，但其输出不是链表而是数组的形式，但是其实不是啦，其实也是链表的形式，只不过是数组链表。这就很容易做了。
## 2.1 level1
最简单的，一般这种肯定想到的是栈，因为栈具有先进后出的效果，能够进行反置。于是就有了下面这样的代码：
```
public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
        ArrayList<Integer> res = new ArrayList<Integer>();
        ArrayDeque<Integer> stack = new ArrayDeque<Integer>();
        while (listNode != null) {
            stack.push(listNode.val);
            listNode = listNode.next;
        }
        while (!stack.isEmpty()) {
            res.add(stack.pop());
        }
        return res;
    }
```
## 2.2 level2
但是这个代码比较冗余，那么怎么才能够简洁一些呢，当然是递归了，递归同样是具有反转的效果的，只需要把赋值语句放在递归后面即可，代码如下：
```
ArrayList<Integer> arrayList=new ArrayList<Integer>();
        if(listNode==null){
            return arrayList;
        }
        arrayList.addAll(printListFromTailToHead(listNode.next));
        arrayList.add(listNode.val);
        return arrayList;
```
# 2.3 level3
但是其实我们忽略了一个重要的部分，那就是返回的形式，是arraylist的形式，这就意味着返回类型同样是链表，那么同样具有反转特性的头插法链表就应运而生了：
```
ArrayList<Integer> res = new ArrayList<Integer>();
        ListNode tmd=null;
        while((tmd=listNode)!=null){
            res.add(0,tmd.val);//头插法
            listNode=tmd.next;
        }
        return res;
```
# 3. 小结
这里的题目不难，主要是讲解有关链表反转的思路，在另一个[反转链表](http://blog.csdn.net/qq_35082030/article/details/71188901)里，我们也讲解了3种层次的反转。这里可以互相比较一下。这里提供的是三种思路，一种使用栈，第二种使用递归，第三种使用头插法，都可以解决链表反转的问题。

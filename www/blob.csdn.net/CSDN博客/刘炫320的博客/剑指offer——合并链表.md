# 剑指offer——合并链表 - 刘炫320的博客 - CSDN博客
2017年05月06日 11:27:46[刘炫320](https://me.csdn.net/qq_35082030)阅读数：276
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
# 2. 解决方法
合并链表应该是数据结构里最经典的一道题目。两个链表合并，当然是要一次对比两个链表开头谁更小，谁就当做新的头，然后依次类推下去。
## 2.1 level1
还记得当时不是用递归写的，一点一点合并的。现在知道怎么用递归了，就用递归写一个吧。代码看起来是真的挺简洁的。
```java
public static ListNode Merge2(ListNode list1,ListNode list2) {
        if(list1==null){
            return list2;
        }
        if(list2==null){
            return list1;
        }
        //如果list1比2小于等于
        if(list1.val<=list2.val){
            list1.next=Merge2(list1.next, list2);
            return list1;
        }
        else{
            list2.next=Merge2(list1, list2.next);
            return list2;
        }
    }
```
## 2.2 level2
这时候可能会出现这样一个问题，这时候每次合并都是一个一个合并，一个一个递归，可不可以批量递归呢？比如如果遇到这样的两个链表： 
A={1,2,3,8,9,10},B={4,5,6,7,11,12,13}。
于是我们要做进一步改进，算法如下所示：
```java
public static ListNode Merge3(ListNode list1,ListNode list2) {
        if(list1==null){
            return list2;
        }
        if(list2==null){
            return list1;
        }
        if(list1.val<=list2.val){
            ListNode temp=list1;
            ListNode pre=null;
            //如果list1比2小于等于
            while(temp.val<=list2.val){
                //如果是最后一个
                if(temp.next==null){
                    temp.next=list2;
                    return list1;
                }
                //否则记录上一个结点。
                pre=temp;
                temp=temp.next;
            }
            pre.next=Merge3(temp, list2);
            return list1;   
        }
        else{
            ListNode temp=list2;
            ListNode pre=null;
            //如果list2比1小于等于
            while(temp.val<=list1.val){
                //如果是最后一个
                if(temp.next==null){
                    temp.next=list1;
                    return list2;
                }
                //否则记录上一个结点。
                pre=temp;
                temp=temp.next;
            }
            pre.next=Merge3(temp, list1);
            return list2;
        }
    }
```
## 2.3 level3
但是这样对于小数据量的还好说，对于大数据量的链表来讲，递归就是噩梦啊。因此还是要写非递归的程序。如下所示：
```java
public static ListNode Merge(ListNode list1,ListNode list2) {
         if (list1 ==null)
            return list2;
         if (list2 ==null)
            return list1;
         ListNode head = null;
         ListNode tmp= null;
         if (list1.val < list2.val)
         {
            head = list1;
            list1 = list1.next;
         }
         else 
         {
            head = list2;
            list2 = list2.next;
         }
         tmp = head;
         //都没剩的话
         while (list1 != null && list2 != null)
            {
                if (list1.val < list2.val)
                {    
                    tmp.next = list1;
                    list1 = list1.next;
                    tmp = tmp.next; 
                }
                else 
                {
                    tmp.next = list2;
                    list2 = list2.next;
                    tmp = tmp.next;
                }
            }
         //如果还有剩的
         while (list1 != null || list2 != null)
            {
                if (list1 ==null && list2 != null)
                {
                    tmp.next = list2;
                    list2 = list2.next;
                    tmp = tmp.next;
                }
                if (list2 ==null && list1 != null)
                {
                    tmp.next = list1;
                    list1 = list1.next;
                    tmp = tmp.next;
                }
           }
            return head;
    }
```
事实证明，在极端情况下，level2可以比level1和level3快上一个数量级。但是，level1和level2的递归在长链表中，容易出现StackOverFlow的情况。因此非递归的手段也应该掌握。
# 3. 一点思考
同样的问题，我们有时候喜欢递归的，有时候喜欢非递归的。这完全取决于问题的具体情况。就编码人员而言，当然更喜欢递归的，因为这样代码逻辑清晰。但是这并不是简化了程序，只是简化了程序员这面的工作，对于编译器来讲，将是沉重的负担。因此也应当平衡一下两方面的工作量，这样才能达到效率最高。


# 剑指offer——复杂链表的复制 - manduner的博客 - CSDN博客


2019年04月02日 17:14:20[Manduner_TJU](https://me.csdn.net/manduner)阅读数：13标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题的描述
输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意一个节点），返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则判题程序会直接返回空）
# 2，解题思路（转自牛客网）
（1）遍历链表，复制每个节点并将该复制后的新节点放至旧节点之后，先不用管每个节点的随机节点
（2）重新遍历链表，复制旧节点的随机指针给新节点
（3）拆分链表，将链表拆分为原链表和复制后的链表
![](https://img-blog.csdnimg.cn/20190402171218259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，源码
```python
/*
public class RandomListNode {
    int label;
    RandomListNode next = null;
    RandomListNode random = null;
    RandomListNode(int label) {
        this.label = label;
    }
}
*/
public class Solution {
    public RandomListNode Clone(RandomListNode pHead)
    {
        if(pHead==null) return null;
        //1,遍历链表，复制每个节点并将该复制后的新节点放至旧节点之后
        RandomListNode currentHead = pHead;
        while(currentHead!=null) {
            RandomListNode copyNode = new RandomListNode(currentHead.label);
            RandomListNode nextNode = currentHead.next;
            copyNode.next = nextNode;
            currentHead.next = copyNode;
            currentHead = nextNode;
        }
        //2,重新遍历链表，复制旧节点的随机指针给新节点
        currentHead = pHead;
        while(currentHead!=null) {
            currentHead.next.random = currentHead.random==null?null:currentHead.random.next;
            currentHead = currentHead.next.next;
        }
        //3，拆分链表，将链表拆分为原链表和复制后的链表
        currentHead = pHead;
        RandomListNode cloneHead = currentHead.next;
        while(currentHead!=null) {
            RandomListNode cloneNode = currentHead.next;
            currentHead.next = cloneNode.next;
            cloneNode.next = currentHead.next==null?null:currentHead.next.next;
            currentHead = currentHead.next;
        }
        return cloneHead;
    }
}
```



# 剑指offer系列-T26复杂链表的复制 - 右上角同学的专栏 - CSDN博客
2016年07月15日 16:31:12[右上角徐](https://me.csdn.net/u011032983)阅读数：198
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl 
 * 复杂链表的复制 
 * 题目描述 
 * 输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意
 * 一个节点），返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则
 * 判题程序会直接返回空）
 * 
 * 思路：第一步是根据原始链表的每个结点N，创建对应的N’。把新创建的每个结点N’链接在原先结点N的后面
 * 第二步是设置我们复制出来的链表上的结点的m_pSibling。假设原始链表上的N的m_pSibling指向结点S，那
 * 么其对应复制出来的N’是N->m_pNext，同样S’也是S->m_pNext。这就是我们在上一步中把每个结点复制出来
 * 的结点链接在原始结点后面的原因。
 * 第三步是把这个长链表拆分成两个。
 */
public class offerT26 {
	/**
	 * @param args
	 */
	 public RandomListNode Clone(RandomListNode pHead)
	    {
		 RandomListNode Head=pHead;
		 RandomListNode Head2=pHead;
		 RandomListNode Head3=pHead;
		 /*为每个节点复制附属节点，插入到每个节点后面*/
		 while(Head!=null){
			 RandomListNode clone=new RandomListNode(Head.label);
			  clone.next=Head.next;
			  Head.next=clone;
			  Head=clone.next;
		 }
		 /*定义附属节点的random指针，即指向原节点的random节点的附属节点，Head2.next.random=Hea<span style="white-space:pre">		</span>d2.random.next;*/
		 while(Head2!=null){
			 if(Head2.random!=null){
			 Head2.next.random=Head2.random.next;
			 }
			 Head2=Head2.next.next;
		 }
		 
		 /*改变附属节点的next指针指向，断开两个链表*/
		 while(Head3.next.next!=null){/*最后一个节点（没复制之前的最后一个）不做操作，不用改变                  其附属节点的next*/
			 RandomListNode clone=Head3.next;
			 RandomListNode Head3next=Head3.next.next;
			 clone.next=clone.next.next;
			 Head3=Head3next;
		 }
		return pHead.next;
		 
	    }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT26 o = new offerT26();
		RandomListNode node1 = new RandomListNode(1);
		RandomListNode node2 = new RandomListNode(2);
		RandomListNode node3 = new RandomListNode(3);
		RandomListNode node4 = new RandomListNode(4);
		RandomListNode node5 = new RandomListNode(5);
		
		node1.next = node2;
		node1.random=node4;
		
		node2.next = node3;
		node2.random=node1;
		
		node3.next = node4;
		node3.random=node2;
		
		node4.next = node5;
		node4.random=node5;
		
		node5.next = null;
		node5.random=node2;
		RandomListNode pnode = o.Clone(node1);
		while(pnode!=null){
			System.out.println(pnode.toString());
			pnode=pnode.next;
		}
	}
}
class RandomListNode {
    int label;
    RandomListNode next = null;
    RandomListNode random = null;
    RandomListNode(int label) {
        this.label = label;
    }
    public String toString() {
    	String s1=next == null?"null" : String.valueOf(next.label);
    	String s2=random == null?"null" : String.valueOf(random.label);
        String s = ("value = " + label+", next = " + s1+", random = " + s2);
        return s;
    }
}
```

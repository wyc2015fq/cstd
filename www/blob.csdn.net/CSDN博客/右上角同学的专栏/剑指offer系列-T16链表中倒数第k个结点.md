# 剑指offer系列-T16链表中倒数第k个结点 - 右上角同学的专栏 - CSDN博客
2016年07月11日 16:31:45[右上角徐](https://me.csdn.net/u011032983)阅读数：141
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 链表中倒数第k个结点
 * 题目描述
 * 输入一个链表，输出该链表中倒数第k个结点。
 * 思路：为避免因改变指针方向导致链表断开，需要保存三个节点，pAhead、point、pBehind
 * 另外要考虑到空链表和只有一个元素的链表（其实是两个节点，带值的节点和null）
 */
public class offerT16 {
	/**
	 * @param args
	 */
	public ListNode ReverseList(ListNode head) {
		if(head==null){
			return null;
		}
		if(head.next==null){
			return head;
		}
		ListNode pAhead=head;
		ListNode point=pAhead.next;
		ListNode pBehind=point.next;
		point.next=pAhead;
		head.next=null;
		
		while(pBehind!=null){
			pAhead=point;
			point=pBehind;
			pBehind=point.next;
			point.next=pAhead;
		}
		return point;
    }
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT16 o = new offerT16();
		ListNode node1 = new ListNode(1);
		ListNode node2 = new ListNode(2);
		ListNode node3 = new ListNode(3);
		ListNode node4 = new ListNode(4);
		ListNode node5 = new ListNode(5);
		node1.next = node2;
		node2.next = node3;
		node3.next = node4;
		node4.next = node5;
		node5.next = null;
		ListNode pnode = o.ReverseList(node1);
		while(pnode!=null){
			System.out.println(pnode.val);
			pnode=pnode.next;
		}
		
	}
}
//因为在offerT5已经定义了class ListNode，同一个包中不能有相同的类名
/*
 * class ListNode {// int value; ListNode next = null;
 * 
 * ListNode(int val) { value = val; } }
 */
```

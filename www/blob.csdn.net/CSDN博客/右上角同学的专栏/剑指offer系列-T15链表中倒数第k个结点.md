# 剑指offer系列-T15链表中倒数第k个结点 - 右上角同学的专栏 - CSDN博客
2016年07月09日 22:14:25[右上角徐](https://me.csdn.net/u011032983)阅读数：169
```java
/**
 * @author xhl 链表中倒数第k个结点 
 * 题目描述 
 * 输入一个链表，输出该链表中倒数第k个结点。
 * 
 */
public class offerT15 {
	/**
	 * @param args
	 */
	public ListNode FindKthToTail(ListNode head, int k) {
		ListNode pAhead = head;
		ListNode pBehind = head;
		if (k == 0 || head == null)//判断倒数第0个或空链表
			return null;
		while (k-- > 1) {//前面的指针先移动k-1步
			if (pAhead.next == null)//还没到指定位置，已经到尾节点即节点个数<k
				return null;
			pAhead = pAhead.next;
			// k--;
		}
		while (pAhead.next != null) {//两个指针一起移动
			pAhead = pAhead.next;
			pBehind = pBehind.next;
		}
		return pBehind;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT15 o = new offerT15();
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
		ListNode pnode = o.FindKthToTail(node1, 3);
		System.out.println(pnode.val);
	}
}
// 因为在offerT5已经定义了class ListNode，同一个包中不能有相同的类名
/*
 * class ListNode {// int value; ListNode next = null;
 * 
 * ListNode(int val) { value = val; } }
 */
```

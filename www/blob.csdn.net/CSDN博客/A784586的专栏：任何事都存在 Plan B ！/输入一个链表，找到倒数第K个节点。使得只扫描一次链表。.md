# 输入一个链表，找到倒数第K个节点。使得只扫描一次链表。 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年02月20日 19:29:34[QuJack](https://me.csdn.net/A784586)阅读数：264
个人分类：[算法																[Java](https://blog.csdn.net/A784586/article/category/6556682)](https://blog.csdn.net/A784586/article/category/6556570)








输入一个链表，找到倒数第K个节点。使得只扫描一次链表。


思路：两个指针p,q。p先扫K个，然后p,q同时向后，p到表尾的时候，q所指向的就是倒数k个节点。注意代码的鲁棒性（健壮性）排除异常输入。









```java
package com.mytest.mymain;

class ListNode {
	int val;
	ListNode next = null;

	ListNode(int val) {
		this.val = val;
	}
}

public class FindKthtotail {
	public static void main(String[] args) {
		FindKthtotail findKthtotail=new FindKthtotail();
		ListNode One =new ListNode(2);
		ListNode Two =new ListNode(3);
		ListNode Third =new ListNode(4);
		ListNode Four =new ListNode(5);
		ListNode Five =new ListNode(6);
		One.next=Two;
		Two.next= Third;
		Third.next=Four;
		Four.next=Five;
		ListNode head=new ListNode(1);
		head.next=One;
		ListNode result=findKthtotail.FindKthToTail(head,3);
		System.out.println(result.val);
		
	}
	public ListNode FindKthToTail(ListNode head, int k) {
		if (head == null || k <= 0)
			return null;

		ListNode pListNode = head, qListNode = head;

		for (int i = 1; i < k; ++i) {
			if (pListNode.next != null) {
				pListNode = pListNode.next;
			} else {
				return null;
			}
		}

		while (pListNode.next != null) {
			qListNode = qListNode.next;
			pListNode = pListNode.next;
		}

		return qListNode;

	}

}
```









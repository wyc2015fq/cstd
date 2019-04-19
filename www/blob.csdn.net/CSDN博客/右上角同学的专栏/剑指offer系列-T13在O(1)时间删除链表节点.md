# 剑指offer系列-T13在O(1)时间删除链表节点 - 右上角同学的专栏 - CSDN博客
2016年07月07日 20:54:04[右上角徐](https://me.csdn.net/u011032983)阅读数：266
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 题目：在O(1)时间删除链表节点
 * 题目描述：给定单向链表的头指针和一个结点指针，定义一个函数在O(1)时间删除该结点。
 * 方法思路：链表操作时，要想删除某节点，必须知道该节点的前一个节点的指针，并让该指针跳过要删节点，指向下一节点；
 * 本题要求时间复杂度O(1)，思路是：将要删除节点的下一个节点的值复制到该节点，再删除下一个节点，变相删除要求节点
 * 该方法不难，数据结构课时老师就讲过，但是，要有很多假设情况，所以需要很多if语句判断
 * 
 */
public class offerT13 {
	/**
	 * @param args
	 */
	ListNode DeleteNode(ListNode pHead, ListNode pToBeDeleted) {
		if (pHead == null)// 原链表为空或要删节点为空
			return null;
		if (pToBeDeleted == null)
			return pHead;// 要删节点为空
		if (pToBeDeleted.next != null) {// 一般情况，即要删除的节点不是最后一个，且原链表不是只含有一个元素，
			pToBeDeleted.val = pToBeDeleted.next.val;// 复制后面的一个节点值到要删除的节点处，再把后面的节点删除
			pToBeDeleted.next = pToBeDeleted.next.next;
		} else if (pHead == pToBeDeleted) {// 是最后一个节点，且该链表只有该节点一个元素
			pHead = null;
		} else {// 是最后一个节点，但不止一个元素，这必须从头遍历至该节点的前一个节点，必须时间复杂度o(logn)
			while (pHead.next != pToBeDeleted) {
				pHead = pHead.next;
			}
			pHead.next = null;
		}
		return pHead;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT13 o = new offerT13();
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
		ListNode pnode = o.DeleteNode(node1, node4);
		if (pnode != null) {
			while (pnode != null) {
				System.out.println(pnode.val);
				pnode = pnode.next;
			}
		} else
			System.out.println("null");
	}
}
```

# 剑指offer系列-从尾到头打印链表 - 右上角同学的专栏 - CSDN博客
2016年06月28日 21:53:30[右上角徐](https://me.csdn.net/u011032983)阅读数：475
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
	 * 从尾到头打印链表
	 * 题目描述：输入一个链表，从尾到头打印链表每个节点的值。 
		输入描述:输入为链表的表头
		输出描述:输出为需要打印的“新链表”的表头
	 */
import java.util.ArrayList;
import java.util.Stack;
/**
 * public class ListNode { int val; ListNode next = null;
 * 
 * ListNode(int val) { this.val = val; } }
 * 
 */
class ListNode {
	int val;
	ListNode next = null;
	ListNode(int val) {
		this.val = val;
	}
}
public class offerT5 {
	public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
		Stack<Integer> stack = new Stack<Integer>();
		// 队列和栈是一对好基友，从尾到头打印链表，当然离不开借助栈的帮忙啦
		// 所以，先把链表里的东西，都放到一个栈里去，然后按顺序把栈里的东西pop出来，就这么简单
		ArrayList<Integer> list = new ArrayList<Integer>();
		while (listNode == null) {//判断是否空链表，空则返回空ArrayList
			return list;
		}
		while (listNode != null) {
			stack.push(listNode.val);
			listNode = listNode.next;
		}
		while (!stack.isEmpty()) {
			int member = stack.pop();//Integer封装类型自动转为int基本数据类型
			list.add(member);
		}
		return list;
	}
	public static void main(String[] args) {
		ArrayList<ListNode> list = new ArrayList<ListNode>();
		ListNode a = new ListNode(1);
		ListNode b = new ListNode(2);
		ListNode c = new ListNode(1);
		ListNode d = new ListNode(3);
		ListNode e = new ListNode(4);
		list.add(a);
		list.add(b);
		list.add(c);
		list.add(d);
		list.add(e);
		for (int i = 0; i < list.size() - 1; i++) {//建立指针连接
			list.get(i).next = list.get(i + 1);
		}
		list.get(list.size() - 1).next = null;
		offerT5 T5 = new offerT5();
		ArrayList<Integer> list2 = T5.printListFromTailToHead(a);
		for (Integer i : list2) {//增强型for循环
			System.out.print(i.intValue());
		}
	}
}
```

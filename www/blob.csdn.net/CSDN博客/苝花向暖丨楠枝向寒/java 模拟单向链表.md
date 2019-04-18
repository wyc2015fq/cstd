# java 模拟单向链表 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月21日 22:34:24[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：33



链表 类

```java
public class LinkList {

	//头结点
	public Node first;
	//临时node节点
	Node tmp;
	
	public LinkList() {
		first = null;
	}
	
	/**
	 * 插入一个结点 ，正序插入
	 */
	public void insertFirst(long value) {
		
		Node node = new Node(value);
		if(first!=null) {
			tmp.next = node;
			tmp = node;
		}else {
			tmp = node;	
			first = node;
		}
	}
	
	/**
	 * 插入一个结点 ，倒序插入
	 */
	public void reInsertFirst(long value) {
		
		Node node = new Node(value);
		node.next = first;
		first = node;		
	}
	
	/**
	 * 删除第一个节点
	 */
	public Node deleteFirst() {
		Node tmp = first;
		//将头结点指向 下一个结点所指向的位置
		first = tmp.next;
		//返回删除的结点
		return tmp;
	}
	
	/**
	 * 删除。根据数据域来进行删除
	 */
	
	public Node delete(long value) {
		Node privious = first;
		Node current = first;
		int index = 0;
		while(current!=null) {
			
			if(current.data == value) {
				
				if(current == first) {
					first = first.next;
				}else {
					privious.next = current.next;
				}
				
			}
			if(index>0) {
				privious = privious.next;
			}
			current = current.next;
			index++;
		}
		
		return null;
	}
	
	
	/**
	 * 查找方法
	 */
	public Node find(long value) {
		Node current = first;
		while(current.data != value) {
			if(current.next == null) {
				return null;
			}
			current = current.next;
		}
		return current;
	}
	
	
	/**
	 * 显示方法
	 */
	public void display() {
		Node current = first;
		while(current != null) {
			current.display();
			current =current.next;
		}
	}
}
```

结点类

```java
public class Node {
	//数据域
	public long data;
	//指针域
	public Node next;
	
	public Node(long value) {
		this.data = value;
	}
	
	/**
	 * 显示方法
	 */
	public void display() {
		System.out.print(data + " ");
	}
}
```



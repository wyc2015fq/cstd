# 剑指offer系列-用两个队列实现栈 - 右上角同学的专栏 - CSDN博客
2016年06月29日 22:23:22[右上角徐](https://me.csdn.net/u011032983)阅读数：174
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
/**
 * @author xhl 用两个队列实现栈 获得queue对象时，要明确，java.util.Queue是接口，
 *         要找到该接口的的实现类，这里用LinkedList类，LinkedList实现了 Deque接口，Deque又实现了Queue接口；
 *         该程序的思想就是两个队列，一个盛放元素，一个为空，空队列作为有元素的队列的
 *         中转容器，所以在实现push时先判断哪个不空再offer；在实现pop时，先把有元素的
 *         队列的前n-1个元素pull再offer到空队列中，再pull出最后进去的第n个元素
 * 
 */
public class offerT7_2 {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		stack o = new stack();
		o.push(1);
		o.push(2);
		o.push(3);
		ArrayList<Integer> list = new ArrayList<Integer>();
		list.add(o.pop());
		list.add(o.pop());
		list.add(o.pop());
		// list.add(o.pop());
		for (int i : list) {
			System.out.println(i);
		}
	}
}
class stack {
	private Queue<Integer> queue1 = new LinkedList<Integer>();
	private Queue<Integer> queue2 = new LinkedList<Integer>();
	int pop() {
		Integer popnode = null;
		// Queue<Integer> use=null;
		if (queue1.isEmpty() == true) {
			if (queue2.isEmpty() != true) {
				while (queue2.size() > 1)
					queue1.offer(queue2.poll());
				popnode = queue2.poll();
			}
		} else if (queue2.isEmpty() == true) {
			while (queue1.size() > 1)
				queue2.offer(queue1.poll());
			popnode = queue1.poll();
		}
		return popnode;
	}
	void push(int pushnode) {
		if (queue1.isEmpty() == true) {
			queue2.offer(pushnode);
		} else if (queue2.isEmpty() == true)
			queue1.offer(pushnode);
		else
			System.out.print(false);
	}
}
```

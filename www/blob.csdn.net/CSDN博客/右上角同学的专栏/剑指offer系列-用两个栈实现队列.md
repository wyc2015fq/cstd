# 剑指offer系列-用两个栈实现队列 - 右上角同学的专栏 - CSDN博客
2016年06月29日 22:18:54[右上角徐](https://me.csdn.net/u011032983)阅读数：185
     本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.Stack;
/**
	 * 用两个栈实现队列
	 * 题目描述
	 * 用两个栈来实现一个队列，
	 * 完成队列的Push和Pop操作。 队列中的元素为int类型。
	 */
public class offerT7 {
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		queue o = new queue();
		for (int i = 1; i < 11; i++)
			o.push(i);
		for (int j = 1; j < 11; j++)
			System.out.println(o.pop());
	}
}
class queue {
	private Stack<Integer> stack1 = new Stack<Integer>();
	private Stack<Integer> stack2 = new Stack<Integer>();
	public void push(int node) {
		stack1.push(node);
	}
	public int pop() {
		Integer popnum =null;
		if (stack2.isEmpty() == true) {
			while (stack1.isEmpty() == false) {
				stack2.push(stack1.pop());
			}
			if(stack2.isEmpty()==false)
			popnum = stack2.pop();
		} else
			popnum = stack2.pop();
		return popnum;
	}
}
```

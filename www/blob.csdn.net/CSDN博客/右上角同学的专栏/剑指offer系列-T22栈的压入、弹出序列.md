# 剑指offer系列-T22栈的压入、弹出序列 - 右上角同学的专栏 - CSDN博客
2016年07月13日 13:39:40[右上角徐](https://me.csdn.net/u011032983)阅读数：183
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.Stack;
/**
 * @author xhl
 * 栈的压入、弹出序列
 * 题目描述
 * 输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否为该栈的弹出顺序。
 * 假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4，5,3,2,1是
 * 该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。
 * （注意：这两个序列的长度是相等的）
 * 思路：一定记住，pop出的元素一定是栈顶元素。所以对popA里的每个元素，与栈顶元素比较，
 * 相等则直接pop出栈顶，并继续popA下个元素，不等，则继续push，直至和栈顶数相等，或者
 * popA中全部都push结束还没找到，则直接return false。如果popA里数能全部在栈顶找到并
 * pop，则返回true。
 * 
 */
public class offerT22 {
	/**
	 * @param args
	 */
	public boolean IsPopOrder(int[] pushA, int[] popA) {
		boolean is = false;
		int i = 0;
		int j = 1;
		Stack<Integer> stack = new Stack<Integer>();
		if(pushA.length==0||popA.length==0||pushA.length!=popA.length)
			return false;
		if (stack.isEmpty()) {//先进栈一个元素，不然stack.peek()显示错误
			stack.push(pushA[0]);
		}
		
		//在栈顶找每个popA元素
		while (i < popA.length) {
			while (stack.peek() != popA[i] && j < pushA.length) {//和栈顶数不等，则继续push
				stack.push(pushA[j]);
				j++;
			}
			if (stack.peek() == popA[i])//如果最后一个元素入栈后，不管和popA是否相等，j < pushA.length都false，所以不能用此作为是否找到的条件
				stack.pop();
			else
				return false;
			i++;
		}
		if (i == popA.length) {//全部在栈顶找到
			is = true;
		}
		return is;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT22 o = new offerT22();
		int[] a = { 1, 2, 3, 4, 5 };
		int[] b1 = { 4, 3, 5, 1, 2 };
		int[] b2 = { 4, 5, 3, 2, 1 };
		System.out.print(o.IsPopOrder(a, b1));
	}
}
```

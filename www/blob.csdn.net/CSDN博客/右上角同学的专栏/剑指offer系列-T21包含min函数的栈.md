# 剑指offer系列-T21包含min函数的栈 - 右上角同学的专栏 - CSDN博客
2016年07月13日 00:35:04[右上角徐](https://me.csdn.net/u011032983)阅读数：137
        本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 包含min函数的栈
 * 题目描述
 * 定义栈的数据结构，要求添加一个min 函数，能够得到栈的最小元素。 
 * 要求函数min、push 以及pop 的时间复杂度都是O(1)。
 * 思路：在定义的栈中，再加入辅助栈，用于记录每次栈元素变化时的最小值变化，
 * 每push进一个元素，辅助栈也push进新元素加入后的最小值（可能还是原最小值，所以会重复）
 * 每pop出一个元素，辅助栈也pop出一个元素，反应减少该元素后，栈的最小值。
 */
public class offerT21 {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		stack sta=new stack();
		sta.push(1);
		sta.push(9);
		sta.push(0);
		sta.push(3);
		System.out.println(sta.min());
		sta.pop();
		System.out.println(sta.min());
		sta.pop();
		System.out.println(sta.min());
		
	}
}
/*定义的栈结构*/
class stack{
	Stack<Integer> stack=new Stack<Integer>();
	Stack<Integer> MinNumberstack=new Stack<Integer>();
	 public void push(int node) {
	        stack.push(node);
	        if(MinNumberstack.isEmpty())//空时直接push
	        	MinNumberstack.push(node);
	        else{//非空时，先判断出当前最小值，再push进最小值
	        	if(MinNumberstack.peek()<node)
	        		MinNumberstack.push(MinNumberstack.peek());
	        	else
	        		MinNumberstack.push(node);
	        }
	    }
	    
	    public void pop() {
	        stack.pop();
	        MinNumberstack.pop();//栈元素变化，最小值也随之变化
	    }
	    
	    public int top() {
	        return stack.peek();
	    }
	    
	    public int min() {
	    	//if(!MinNumberstack.isEmpty())
	    		return MinNumberstack.peek();
	    	//else
	    	//	return null;
	    }
}
```

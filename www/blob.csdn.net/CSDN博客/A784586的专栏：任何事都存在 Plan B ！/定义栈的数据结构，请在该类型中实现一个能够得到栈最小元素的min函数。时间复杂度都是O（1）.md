# 定义栈的数据结构，请在该类型中实现一个能够得到栈最小元素的min函数。时间复杂度都是O（1） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年02月19日 16:56:49[QuJack](https://me.csdn.net/A784586)阅读数：2187
个人分类：[算法																[Java](https://blog.csdn.net/A784586/article/category/6556682)](https://blog.csdn.net/A784586/article/category/6556570)








定义栈的数据结构，请在该类型中实现一个能够得到栈最小元素的min函数。要求：使得时间复杂度都是O（1）

完成如下的函数：



```java
import java.util.Stack;


public class Solution {


    
    public void push(int node) {
        
    }
    
    public void pop() {
        
    }
    
    public int top() {
        
    }
    
    public int min() {
        
    }
}
```






思路:用空间换时间，用一个辅助栈记录当前栈中的最小值。辅助栈元素个数和数据栈保持一样的数目。例如一次压入数据栈数字序列为：

3,2,4,1,5  那么一次压入辅助栈的为：3,2,2,1,1

当每次压入数据栈的元素小余辅助站的元素的时候，才把新元素压入辅助栈，否则把辅助站栈顶元素去到压入辅助栈，保持两个栈元素个数一致。




[](http://blog.csdn.net/jia611/article/details/11200911)[备注](http://blog.csdn.net/jia611/article/details/11200911)：Stack.Peek
 与 stack.pop 的区别


相同点：大家都返回栈顶的值。


不同点：peek 不改变栈的值(不删除栈顶的值)，pop会把栈顶的值删除。






```java
package com.mytest.mymain;
import java.util.Stack;
public class MinStack {
    private Stack<Integer> data_stack=new Stack<Integer>();
    private Stack<Integer> min_stack=new Stack<Integer>();
    
    public void push(int node) {//进栈
        if(min_stack.isEmpty() ||min_stack.peek()>=node){
        	min_stack.push(node);
        }else{
        	min_stack.push(min_stack.peek());
        }
        data_stack.push(node);
    }
    
    public void pop() {//出栈
    	if(data_stack.empty() || min_stack.empty())
    		return;
    	
    	data_stack.pop();
    	min_stack.pop();
    }
    
    public int top() {//取得栈顶元素
    	if(!data_stack.empty()){	
          return data_stack.peek();
    	}
    	return 0;
    }
    
    public int min() {//取得最小值
    	if(!min_stack.empty()){
    		
        return min_stack.peek();
    }
    	return 0;
    }
}
```






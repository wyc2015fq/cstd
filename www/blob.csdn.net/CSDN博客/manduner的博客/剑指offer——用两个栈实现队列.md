
# 剑指offer——用两个栈实现队列 - manduner的博客 - CSDN博客


2018年12月21日 20:00:14[Manduner_TJU](https://me.csdn.net/manduner)阅读数：37标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。
# 2，解题思路
（1）明确栈和队列的含义
栈是先进后出
队列是先进先出
（2）用stack1做入队操作，这一步没有需要特别注意的地方，来一个入一个就行
（3）用stack2做出队操作，基本思想是先把stack1中的元素依次弹出放到stack2中，再把stack2中的元素弹出即可。但是需要注意以下几点：
在stack2执行弹出操作时，必须判断该栈中是否还有元素，如果有，则先弹出该栈中的元素，否则，进行下一步操作
把stack1中的元素依次弹出，并依次放入stack2中，再实行stack2的弹出操作
# 3，源码
```python
import java.util.Stack;
public class Solution {
    Stack<Integer> stack1 = new Stack<Integer>();
    Stack<Integer> stack2 = new Stack<Integer>();
    
    public void push(int node) {
        stack1.push(node);
    }
    
    public int pop() {
        if(!stack2.empty()){
            return stack2.pop();
        }else{
            while(!stack1.empty()){
            stack2.push(stack1.pop());
            }
            return stack2.pop();
        }
    }
}
```



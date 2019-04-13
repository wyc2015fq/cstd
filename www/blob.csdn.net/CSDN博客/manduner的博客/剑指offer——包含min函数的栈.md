
# 剑指offer——包含min函数的栈 - manduner的博客 - CSDN博客


2019年03月27日 23:36:27[Manduner_TJU](https://me.csdn.net/manduner)阅读数：14


# 1，问题描述
定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。
# 2，解题思路
em。。。。我觉得这道题的**难点是题意理解上**，只要把问题理解了，那解题思路还是较简单的。
这道题是让我们设计一个栈结构（此时的Solution类就是我们要设计的栈对象，最后牛客系统测试的时候也是将Solution当作一个栈对象来操作），这个栈除了包含常用的“弹出”、“查看”、“压入”操作，还要求包含一个能够让我们随时查看栈最小元素的min函数，并且时间复杂度为O(1)。
ok，题意理解了，那解题思路就简单了。首先题目要求时间复杂度是O(1)，并且隐含条件是，我们调用min()函数时不能改变栈中元素的顺序，所以我们考虑使用辅助栈。
一共需要两个栈，其中一个是存放数据的（我们命名为数据栈，用stData表示），另一个是存放当前数据栈最小元素的（我们命名为最小栈，用stMin表示）。那么怎么才能随时获取到栈的最小元素呢？
答案如下：**只需要在push操作的时候做些手脚就行**。新来一个元素node，首先将该元素push进数据栈中，然后判断该元素是否小于等于最小栈的栈顶元素，如果满足小于等于，则最小栈push该元素，否则最小栈push最小栈的栈顶元素。这样操作下来，不管如何push，如何pop，数据栈和最小栈的元素总是相等的，而且最小栈的栈顶元素就是当前数据栈中最小的元素。
看源码更简单易懂，这里给出牛客网的一个测试用例：
![](https://img-blog.csdnimg.cn/20190327233241702.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
3，源码
```python
import java.util.Stack;
public class Solution {
    Stack<Integer> stData = new Stack<Integer>();
    Stack<Integer> stMin = new Stack<Integer>();
    public void push(int node) {
        stData.push(node);
        if(!stMin.isEmpty()) {
            if(node<=stMin.peek()) {
                stMin.push(node);
            }else {
                stMin.push(stMin.peek());
            }
        }else {
            stMin.push(node);
        }
    }
    public void pop() {
        stData.pop();
        stMin.pop();
    }
    public int top() {
        return stData.peek();
    }
    public int min() {
        return stMin.peek();
    }
}
```



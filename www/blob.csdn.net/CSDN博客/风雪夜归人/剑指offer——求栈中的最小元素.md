# 剑指offer——求栈中的最小元素 - 风雪夜归人 - CSDN博客
2018年08月23日 23:51:48[cdjccio](https://me.csdn.net/qq_34624951)阅读数：96
**题目**：定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。
**思路**：
1、入栈时判断辅助栈min_data中是否有元素，如果没有元素，则将值压入栈中，如果有元素，比较栈顶元素和value值，如果小于等于栈顶元素，将value压入辅助栈中，否则，不做处理。保证辅助栈中栈顶一直是栈中的最小值。
2、出栈使为了防止辅助栈出现下溢，必须在数据栈和辅助栈栈顶元素相等的情况下才能弹出辅助栈的栈顶元素。
3、栈的弹出只需要弹出数据栈栈顶元素。
4、min()方法返回辅助栈的栈顶元素。
```cpp
class Solution {
public:
    stack<int> data, min_data;
    void push(int value) {
        data.push(value);
        if(min_data.empty() || value <= min_data.top())
        {
            min_data.push(value);
        }
    }
    
    void pop() {
        if(data.top() == min_data.top())
        {
            min_data.pop();
        }
        data.pop();
    }
    int top() {
        return data.top();
    }
    int min() {
        return min_data.top();
    }
};
```

# 剑指offer - 包含min函数的栈 - curryche的博客 - CSDN博客





2018年08月18日 19:56:46[curryche](https://me.csdn.net/whwan11)阅读数：29标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

包含min函数的栈 

时间限制：1秒 空间限制：32768K 热度指数：180971 

本题知识点： 栈
题目描述 

定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。

## 解法

代码

```cpp
class Solution {
public:
    stack<int> data;
    stack<int> mins;
    void push(int value) {
        data.push(value);
        if(mins.empty())
            mins.push(value);
        else
        {
            if(value<mins.top())
                mins.push(value);
            else
                mins.push(mins.top());
        }

    }
    void pop() {
        if(!data.empty())
        {
            data.pop();
            mins.pop();
        }
    }
    int top() {
        if(!data.empty())
            return data.top();
        else
            throw "empty!";

    }
    int min() {
        if(!mins.empty())
            return mins.top();
        else
            throw "empty!";

    }
};
```






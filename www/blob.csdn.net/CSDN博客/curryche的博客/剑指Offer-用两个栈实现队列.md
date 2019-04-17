# 剑指Offer - 用两个栈实现队列 - curryche的博客 - CSDN博客





2018年08月01日 15:28:53[curryche](https://me.csdn.net/whwan11)阅读数：19标签：[剑指offer																[栈																[队列](https://so.csdn.net/so/search/s.do?q=队列&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)





## 题目

用两个栈实现队列 

时间限制：1秒 空间限制：32768K 热度指数：261623 

本题知识点： 队列 栈
题目描述 

用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。

## 解法

代码

```cpp
class Solution
{
public:
    void push(int node) {
        stack1.push(node);
    }

    int pop() {
        int num;
        if(stack2.empty())
        {
            if(stack1.empty())
                throw "this queue is empty!";
            while(!stack1.empty())
            {
                num=stack1.top();
                stack2.push(num);
                stack1.pop();
            }
        }

        num=stack2.top();
        stack2.pop();
        return num;
    }

private:
    stack<int> stack1;
    stack<int> stack2;
};
```](https://so.csdn.net/so/search/s.do?q=栈&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)





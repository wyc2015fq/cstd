# 剑指offer 面试题：两个栈实现一个队列 - 别说话写代码的博客 - CSDN博客





2019年03月03日 14:58:17[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[两个栈实现一个队列																[剑指offer](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=两个栈实现一个队列&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。

思路：两个栈s1和s2，入栈只往s1中入，出栈从s2中出，若出的时候s2空了，将s1所有元素放入s2中。这里就能通过提交。其实这里还要有  更详细的 判断，比如往s1中插入的时候 如果s1满了怎么办，这里没写

```cpp
class Solution
{
public:
    void push(int node) {
        stack1.push(node);
    }
 
    int pop() {
        if(stack2.empty())
        {
            while(!stack1.empty())
            {
                int a = stack1.top();
                stack1.pop();
                stack2.push(a);
            }
        }
        int res = stack2.top();
        stack2.pop();
        return res;
    }
 
private:
    stack<int> stack1;
    stack<int> stack2;
};
```







# 剑指offer 面试题：两个队列模拟一个栈 - 别说话写代码的博客 - CSDN博客





2019年03月06日 10:03:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[剑指offer																[两个队列模拟一个栈](https://so.csdn.net/so/search/s.do?q=两个队列模拟一个栈&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








两个栈模拟一个队列我们知道了：[https://blog.csdn.net/qq_21997625/article/details/88088230](https://blog.csdn.net/qq_21997625/article/details/88088230)。就是用一个栈使用做入队操作，而另外一个栈 始终做出队 操作，当出队操作的栈元素为空，就将入队的栈元素全部 转移过来。

思路：这个题目是两个 队列模拟一个栈，比如有两个队列q1和q2.入 栈操作就比如往q1里面入，入完之后如果想出，那就将q1中前面元素全部放到q2中，然后q1剩下一个元素就可以出栈了。抽象一下就是如果两个队列哪个有 元素就往哪个 里面入，相当于执行入栈操作，出栈的时候如果哪个队列有元素，将其除了最后一个以外其他全部移入另外一个队列中（**队列是先进先出，只需要把先进去的全部移走，只剩下最后一个就可以了，就相当于栈的先进后出**），这样就实现了栈的入栈和出栈操作

![](https://img-blog.csdnimg.cn/20190306094555892.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

比如上图中，先将abc全部压入队列1，然后要c出栈，那就把队列1前两个元素全部移到队列2中，只剩下最后一个就可以啦。如果要继续出，那就再从队列2移动到队列1中就可以，再**入栈的话，就是哪个队列有元素就往哪个里面插，出栈的话，就是哪个队列有元素就将其前面所有都要移动到另外一个队列中只剩一个元素，然后出栈就好了，就是说无论如何都要一个队列为空**

```cpp
class Solution
{
private:
    queue<int> q1;
    queue<int> q2;
public:
    void pushstack(int n)
    {
        if(q1.size()>0 && q2.size()>0) return ; //两个队列都不空那就没法执行
        if(q1.size()) q1.push(n);
        else q2.push(n);
    }
    int popstack()
    {
        if(!q1.size() && !q2.size())  return -1;//两个队列都空没法删除
        int res;
        if(q1.size())  //q1不为空，那就从这个里面移动
        {
            while(q1.size()>1)   //将前面元素全部移动到两一个队列
            {
                int data = q1.front();
                q1.pop();
                q2.push(data);
            }
            res = q1.front();  //剩下的最后一个出队列
            q1.pop();
        }else
        {
            while(q2.size())
            {
                int data = q2.front();
                q2.pop();
                q1.push(data);
            }
            res = q2.front();
            q2.pop();
        } 
        return res;
    }
};
```







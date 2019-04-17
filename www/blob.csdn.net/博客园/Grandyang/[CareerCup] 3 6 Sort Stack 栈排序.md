# [CareerCup] 3.6 Sort Stack 栈排序 - Grandyang - 博客园







# [[CareerCup] 3.6 Sort Stack 栈排序](https://www.cnblogs.com/grandyang/p/4679143.html)







3.6 Write a program to sort a stack in ascending order (with biggest items on top). You may use at most one additional stack to hold items, but you may not copy the elements into any other data structure (such as an array). The stack supports the following operations: push, pop, peek, and isEmpty.



这道题让我们对栈进行排序，栈顶放大的元素，而且限定了我们只能用一个辅助栈。那么我们的思路是，先取出给定栈的栈顶元素存入到一个临时变量中，然后我们看辅助栈的栈顶元素是否大于取出的元素，大的话就把辅助栈的栈顶元素压入给定栈中，直到辅助栈为空或是栈顶元素小于取出值，这时将取出值压入辅助栈，然后继续从给定栈中取值，以此类推直至给定栈取空，这时候辅助栈就是排序完成的结果，返回即可，参见代码如下：



```
class Solution {
public:
    stack<int> sort(stack<int> s) {
        stack<int> res;
        while (!s.empty()) {
            int v = s.top(); s.pop();
            while (!res.empty() && res.top() > v) {
                s.push(res.top());
                res.pop();
            }
            res.push(v);
        }
        return res;
    }
};
```















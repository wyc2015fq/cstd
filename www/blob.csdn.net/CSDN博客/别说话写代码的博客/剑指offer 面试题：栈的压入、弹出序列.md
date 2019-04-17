# 剑指offer 面试题：栈的压入、弹出序列 - 别说话写代码的博客 - CSDN博客





2019年03月03日 15:45:03[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11标签：[栈的压入、弹出序列																[剑指offer](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=栈的压入、弹出序列&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否可能为该栈的弹出顺序。假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。（注意：这两个序列的长度是相等的）

思路：已知一个入栈序列和出栈序列。先定义一个栈，然后分 几种情况：1）如果下一个弹出的数字 刚好是栈顶数字，则直接弹出。2）若下一个弹出的数字不在栈顶，则将入栈序列中元素压栈，知道把下个要出栈元素入栈为止。3）若所有元素都压栈依然没有找到下一个弹出的数字，那该序列不可能是一个弹出序列。最后只需要判断栈是否 为空

```cpp
class Solution {
public:
    bool IsPopOrder(vector<int> pushV,vector<int> popV) {
        if(pushV.empty() || popV.empty() || pushV.size()!=popV.size()) return false;
        int n=pushV.size(),i=0,j=0;
        stack<int> stk;
        for(i=0;i<n;++i)  //控制pop数组
        {
            while(stk.empty() || stk.top()!=popV[i])
            {
                if(j>=n) break; //push数组所有元素都放入栈了
                stk.push(pushV[j++]);
            }
            if(stk.top()!=popV[i]) break;
            stk.pop();
        }
        if(stk.empty()) return true;
        return false;
    }
};
```







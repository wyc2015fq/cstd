# 剑指offer - 栈的压入、弹出序列 - curryche的博客 - CSDN博客





2018年08月21日 09:26:14[curryche](https://me.csdn.net/whwan11)阅读数：47标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

栈的压入、弹出序列 

时间限制：1秒 空间限制：32768K 热度指数：218120 

本题知识点： 栈
题目描述 

输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否可能为该栈的弹出顺序。假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。（注意：这两个序列的长度是相等的）

## 解法

代码

```cpp
class Solution {
public:
    bool IsPopOrder(vector<int> pushV,vector<int> popV) {
        if(pushV.size()==0||popV.size()==0)
            return false;
        int i,j;
        stack<int> nums;
        i=0;
        j=0;
        while(i<pushV.size())
        {
            while(nums.empty()||(nums.top()!=popV[j]))
            {
                nums.push(pushV[i]);
                i++;
            }
            if(!nums.empty()&&nums.top()==popV[j])
            {
                nums.pop();
                j++;
            }
        }

        while(j<popV.size())
        {
            if(!nums.empty()&&nums.top()==popV[j])
            {
                nums.pop();
                j++;
            }
            else
            {
                break;
            }
        }

        if(nums.empty())
            return true;
        else
            return false;
    }
};
```






# 【剑指offer】题22：栈的压入、弹出序列 - xiaxzhou的博客 - CSDN博客





2017年07月02日 11:05:20[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：99








```cpp
class Solution {
public:
    bool IsPopOrder(vector<int> pushV, vector<int> popV) {
        if (pushV.size()==0||popV.size()==0||pushV.size()!=popV.size())
        {
            return false;
        }
        stack<int> my_stack;
        int index(0);
        for (auto i = 0; i < pushV.size();++i)
        {
            my_stack.push(pushV[i]);
            while (my_stack.size()>0&&my_stack.top()==popV[index])
            {
                my_stack.pop();
                index++;
            }
        }
        if (my_stack.size()==0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
```




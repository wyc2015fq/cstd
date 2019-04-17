# 剑指offer 面试题59：队列最大值 滑动窗口最大值 c++ - 别说话写代码的博客 - CSDN博客





2018年11月27日 15:22:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：68
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值。例如，如果输入数组{2,3,4,2,6,2,5,1}及滑动窗口的大小3，那么一共存在6个滑动窗口，他们的最大值分别为{4,4,6,6,6,5}； 针对数组{2,3,4,2,6,2,5,1}的滑动窗口有以下6个： {[2,3,4],2,6,2,5,1}， {2,[3,4,2],6,2,5,1}， {2,3,[4,2,6],2,5,1}， {2,3,4,[2,6,2],5,1}， {2,3,4,2,[6,2,5],1}， {2,3,4,2,6,[2,5,1]}。

思路：（1）使用队列，队列第一个元素放 当前窗口最大值下标，然后一直往后滑，

```cpp
class Solution {
public:
    vector<int> maxInWindows(const vector<int>& num, unsigned int size)
    {
        vector<int> maxinwindow;
        if(num.size()>=size && size>=1)
        {
            deque<int> index;
            for(int i=0;i<size;++i)
            {
                while(!index.empty() && num[i]>=num[index.back()]) //已有数字小于待存入数字
                    index.pop_back();    //已有数字不可能是滑动窗口最大值，则删去
                index.push_back(i);
            }
            for(int i=size;i<num.size();++i)
            {
                maxinwindow.push_back(num[index.front()]);  //获取队列第一个元素为最大值
                while(!index.empty() && num[i]>=num[index.back()])
                    index.pop_back();
                if(!index.empty() && index.front() <= (int)(i-size)) //队头从滑动窗口滑出
                    index.pop_front();
                index.push_back(i);
            }
            maxinwindow.push_back(num[index.front()]);  //将队头元素最大值放入
        }return maxinwindow;
    }
};
```






# 剑指offer - 滑动窗口的最大值 - curryche的博客 - CSDN博客





2018年09月11日 16:02:59[curryche](https://me.csdn.net/whwan11)阅读数：35标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

滑动窗口的最大值 

时间限制：1秒 空间限制：32768K 热度指数：117761

题目描述 

给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值。例如，如果输入数组{2,3,4,2,6,2,5,1}及滑动窗口的大小3，那么一共存在6个滑动窗口，他们的最大值分别为{4,4,6,6,6,5}； 针对数组{2,3,4,2,6,2,5,1}的滑动窗口有以下6个： {[2,3,4],2,6,2,5,1}， {2,[3,4,2],6,2,5,1}， {2,3,[4,2,6],2,5,1}， {2,3,4,[2,6,2],5,1}， {2,3,4,2,[6,2,5],1}， {2,3,4,2,6,[2,5,1]}。

## 解法

代码1

```cpp
class Solution {
public:
    vector<int> maxInWindows(const vector<int>& num, unsigned int size)
    {
        vector<int> result;
        if(num.size()==0||size==0||size>num.size())
            return result;

        if(size==num.size())
        {
            int max;
            max=num[0];
            for(int i=0;i<num.size();i++)
            {
                if(num[i]>max)
                    max=num[i];
            }
            result.push_back(max);
            return result;
        }

        int start,end,max,max_id,sec_id;
        start=0;
        end=size-1;
        max_id=0;

        while(end<num.size())
        {
            if(start==0||start-1==max_id)
            {
                max_id=start;
                for(int i=start;i<=end;i++)
                {
                    if(num[i]>=num[max_id])
                    {
                        max_id=i;
                    }

                }
            }
            else
            {
                if(num[end]>=num[max_id])
                    max_id=end;
            }
            result.push_back(num[max_id]);
            start++;
            end++;
        }
        return result;
    }
};
```

代码2

```cpp
class Solution {
public:
    vector<int> maxInWindows(const vector<int>& num, unsigned int size)
    {
        vector<int> result;
        if(num.size()==0||size==0||size>num.size())
            return result;

        if(size==num.size())
        {
            int max;
            max=num[0];
            for(int i=0;i<num.size();i++)
            {
                if(num[i]>max)
                    max=num[i];
            }
            result.push_back(max);
            return result;
        }

        deque<int> index;
        int i;
        i=0;
        while(i<size)
        {
            while(!index.empty()&&num[index.back()]<num[i])
                index.pop_back();
            index.push_back(i);
            i++;
        }

        for(i=size;i<num.size();i++)
        {
            result.push_back(num[index.front()]);
            while(!index.empty()&&num[index.back()]<num[i])
                index.pop_back();
            index.push_back(i);

            if(index.front()<(i-size+1))
                index.pop_front();
        }
        if(!index.empty())
        {
            result.push_back(num[index.front()]);
        }

        return result;
    }
};
```





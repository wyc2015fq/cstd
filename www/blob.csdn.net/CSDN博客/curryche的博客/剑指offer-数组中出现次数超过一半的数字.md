# 剑指offer - 数组中出现次数超过一半的数字 - curryche的博客 - CSDN博客





2018年09月01日 11:36:44[curryche](https://me.csdn.net/whwan11)阅读数：21标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

数组中出现次数超过一半的数字 

时间限制：1秒 空间限制：32768K 热度指数：224454 

本题知识点： 数组
题目描述 

数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组{1,2,3,2,2,2,5,4,2}。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。如果不存在则输出0。

## 解法

代码

```cpp
class Solution {
public:
    int MoreThanHalfNum_Solution(vector<int> numbers) {
        if(numbers.size()==0)
            return 0;
        int current_record,count;
        current_record=0;
        count=0;
        for(int i=0;i<numbers.size();i++)
        {
            if(count==0)
            {
                current_record=numbers[i];
                count=1;
            }
            else if(numbers[i]==current_record)
                count++;
            else
                count--;
        }

        count=0;
        for(int i=0;i<numbers.size();i++)
        {
            if(numbers[i]==current_record)
                count++;
        }

        if(count>numbers.size()/2)
            return current_record;
        else
            return 0;
    }
};
```






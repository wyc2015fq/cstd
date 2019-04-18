# 剑指offer - 数组中只出现一次的数字 - curryche的博客 - CSDN博客





2018年09月10日 20:21:02[curryche](https://me.csdn.net/whwan11)阅读数：24








## 题目

数组中只出现一次的数字 

时间限制：1秒 空间限制：32768K 热度指数：154585 

本题知识点： 数组
题目描述 

一个整型数组里除了两个数字之外，其他的数字都出现了偶数次。请写程序找出这两个只出现一次的数字。

## 解法

代码 

注意==的优先级高于&

```cpp
class Solution {
public:
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
        if(data.size()<2)
            return;

        int xor_res;
        unsigned int index;
        xor_res=0;
        for(int i=0;i<data.size();i++)
            xor_res^=data[i];
        index=FindFirst1Bit(xor_res);

        *num1=0;
        *num2=0;
        for(int i=0;i<data.size();i++)
        {
            if(JudgeKbitIs1(data[i],index))
                *num1^=data[i];
            else
                *num2^=data[i];
        }

    }

    unsigned int FindFirst1Bit(int num)
    {
        unsigned int i=0;
        while(num>0&& ((num&1)==0))
        {
            num=num>>1;
            i++;
        }
        return i;
    }

    bool JudgeKbitIs1(int num,unsigned int k)
    {
        num=num>>k;
        return (num&1);
    }
};
```





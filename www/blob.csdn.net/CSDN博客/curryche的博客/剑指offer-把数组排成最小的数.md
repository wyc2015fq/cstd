# 剑指offer - 把数组排成最小的数 - curryche的博客 - CSDN博客





2018年09月04日 21:53:09[curryche](https://me.csdn.net/whwan11)阅读数：28标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

把数组排成最小的数 

时间限制：1秒 空间限制：32768K 热度指数：162626 

本题知识点： 数组
题目描述 

输入一个正整数数组，把数组里所有数字拼接起来排成一个数，打印能拼接出的所有数字中最小的一个。例如输入数组{3，32，321}，则打印出这三个数字能排成的最小数字为321323。

## 解法

代码

```cpp
#include<string>
class Solution {
public:
    string PrintMinNumber(vector<int> numbers) {
        string result;
        if(numbers.size()==0)
            return result;
        if(numbers.size()==1)
            return to_string(numbers[0]);

        //myquick_sort(&numbers,0,numbers.size()-1);
        sort(numbers.begin(),numbers.end(),compare_str);
        //result=to_string(numbers[0]);

        for(int i=0;i<numbers.size();i++)
            result+=to_string(numbers[i]);

        return result;
    }

    static bool compare_str(int num1,int num2)
    {
        return (to_string(num1)+to_string(num2))<
            (to_string(num2)+to_string(num1));
    }


};
```






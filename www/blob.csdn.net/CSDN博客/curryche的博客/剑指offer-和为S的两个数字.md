# 剑指offer - 和为S的两个数字 - curryche的博客 - CSDN博客





2018年09月10日 21:05:39[curryche](https://me.csdn.net/whwan11)阅读数：30标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

和为S的两个数字 

时间限制：1秒 空间限制：32768K 热度指数：143157

题目描述 

输入一个递增排序的数组和一个数字S，在数组中查找两个数，使得他们的和正好是S，如果有多对数字的和等于S，输出两个数的乘积最小的。 

输出描述: 

对应每个测试案例，输出两个数，小的先输出。
## 解法

代码

```cpp
class Solution {
public:
    vector<int> FindNumbersWithSum(vector<int> array,int sum) {
        vector<int> result;
        if(array.size()==0)
            return result;
        int i,j;
        i=0;
        j=array.size()-1;
        while(i<j)
        {
            if(array[i]+array[j]==sum)
            {
                if(result.empty())
                {
                    result.push_back(array[i]);
                    result.push_back(array[j]);
                }
                else
                {
                    if(result[0]*result[1]>array[i]*array[j])
                    {
                        result.clear();
                        result.push_back(array[i]);
                        result.push_back(array[j]);
                    }
                }
                i++;
                j--;
            }
            else if(array[i]+array[j]>sum)
            {
                if(j>0)
                    j--;
                else
                    break;
            }
            else
            {
                if(i<array.size()-1)
                    i++; 
                else
                    break;
            }
        }
        return result;
    }
};
```






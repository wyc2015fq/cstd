# 剑指offer - 连续子数组的最大和 - curryche的博客 - CSDN博客





2018年09月04日 20:03:36[curryche](https://me.csdn.net/whwan11)阅读数：12标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

连续子数组的最大和 

时间限制：1秒 空间限制：32768K 热度指数：161425 

本题知识点： 数组
题目描述 

HZ偶尔会拿些专业问题来忽悠那些非计算机专业的同学。今天测试组开完会后,他又发话了:在古老的一维模式识别中,常常需要计算连续子向量的最大和,当向量全为正数的时候,问题很好解决。但是,如果向量中包含负数,是否应该包含某个负数,并期望旁边的正数会弥补它呢？例如:{6,-3,-2,7,-15,1,2,2},连续子向量的最大和为8(从第0个开始,到第3个为止)。给一个数组，返回它的最大连续子序列的和，你会不会被他忽悠住？(子向量的长度至少是1)

## 解法

代码

```cpp
class Solution {
public:
    int FindGreatestSumOfSubArray(vector<int> array) {
        if(array.size()==0)
            return -1;
        int max_sum,last_sum;
        max_sum=array[0];
        last_sum=array[0];
        for(int i=1;i<array.size();i++)
        {
            if(last_sum<=0)
                last_sum=array[i];
            else
                last_sum+=array[i];
            if(last_sum>max_sum)
                max_sum=last_sum;
        }
        return max_sum;
    }
};
```






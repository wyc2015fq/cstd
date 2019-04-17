# 剑指offer 面试题56：数组中只出现一次的数字 - 别说话写代码的博客 - CSDN博客





2018年11月22日 23:40:47[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：27标签：[剑指offer																[面试题35																[数组只出现一次元素																[第一个只出现一次的数](https://so.csdn.net/so/search/s.do?q=第一个只出现一次的数&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)](https://so.csdn.net/so/search/s.do?q=数组只出现一次元素&t=blog)




题目：在一个整数数组中，除了一个数之外，其他的数出现的次数都是两次，求出现一次的数，要求时间复杂度尽可能的小。例如数组{1,2,2,3,3,6,6},出现一次的数是1.

思路：用 异或操作，相同为0相异为1，对所有元素异或然后得到的 结果就是要求的唯一一个只出现一次的数字

```cpp
int getonlyone(vector<int> data)
{
    int k=0;
    for(int i=0;i<data.size();++i)
        k=k^data[i];
    return k;
}
```

题目二：要求的数组里面有两个数出现的次数是1，其他出现的次数都是2

思路：借鉴上面 题目，先对所有 元素异或，得到的值肯定不为0，则至少有一位元素是1，假设第n位为1，可以数组元素分为两部分， 一部分元素第n位是1，另一部分第n为是0，再用上面方法分别求出每个部分的只出现 一次的数组

```cpp
class Solution {
public:
    int comp(vector<int> data)
    {
        int k=0;
        for(int i=0;i<data.size();++i)
            k=k^data[i];
        return k;
    }
    int first(int n)
    {
        int i=0;
        while((n&1)==0)
        {
            ++i;
            n=n>>1;
        }return i;
    }
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
        if(data.size()==0) return;
        int k = comp(data);
        int n=first(k);
        *num1=*num2=0;
        for(int i=0;i<data.size();++i)
        {
            if((data[i]>>n)&1) *num1=*num1^data[i];
            else *num2=*num2^data[i];
        }
    }
};
```](https://so.csdn.net/so/search/s.do?q=面试题35&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)





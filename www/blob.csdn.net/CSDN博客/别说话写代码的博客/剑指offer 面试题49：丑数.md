# 剑指offer 面试题49：丑数 - 别说话写代码的博客 - CSDN博客





2018年11月19日 16:34:44[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[剑指offer																[面试题49丑数](https://so.csdn.net/so/search/s.do?q=面试题49丑数&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








 题目：把只包含质因子2、3和5的数称作丑数（Ugly Number）。例如6、8都是丑数，但14不是，因为它包含质因子7。 习惯上我们把1当做是第一个丑数。求按从小到大的顺序的第N个丑数。

思路：用一个数组，三个指针，index2,index3和index5,给每个元素乘以2,3,5看哪个刚好比当前nextindex位置的元素大，就放进来

```cpp
class Solution {
public:
    int GetUglyNumber_Solution(int index) {
        if(index <=0) return 0;
        int uglynumber[index];
        uglynumber[0]=1;
        int nextindex=1,index2=0,index3=0,index5=0;
        while(nextindex<index)
        {
            uglynumber[nextindex]=min3(uglynumber[index2]*2,uglynumber[index3]*3,
                                      uglynumber[index5]*5);
            while(uglynumber[index2]*2<=uglynumber[nextindex]) ++index2;
            while(uglynumber[index3]*3<=uglynumber[nextindex]) ++index3;
            while(uglynumber[index5]*5<=uglynumber[nextindex]) ++index5;
            ++nextindex;
        }return uglynumber[index-1];
    }
    int min3(int a,int b,int c)
    {
        int m = a<b?a:b;
        return m<c?m:c;
    }
};
```







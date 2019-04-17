# 剑指offer 面试题66：构建乘积数组 c++ - 别说话写代码的博客 - CSDN博客





2018年11月26日 20:36:15[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：44
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：给定一个数组A[0,1,...,n-1],请构建一个数组B[0,1,...,n-1],其中B中的元素B[i]=A[0]*A[1]*...*A[i-1]*A[i+1]*...*A[n-1]。不能使用除法。

思路：（1）暴力循环，复杂度为O（n^2）  (2)构建前向乘积数组C[i] = A[0]*A[1]*...*A[i-1]，即C[i]=C[i-1]*A[i-1]；构建后向乘积数组D[i] = A[n-1]*A[n-2]*...A[n-i+1]，即D[i]=D[i+1]*A[i+1]；通过C[i],D[i]来求B[i]：B[i]=C[i]*D[i]。时间复杂度为O(n)

```cpp
class Solution {
public:
    vector<int> multiply(const vector<int>& A) {
        int len = A.size();
        if(len<2) return A;
        vector<int> b;
        b.push_back(1);
        for(int i=1;i<len;++i)
            b.push_back(b[i-1] * A[i-1]);
        int temp = 1;   //用来保存D
        for(int i= len - 2;i>=0;--i)
        {
            temp = temp * A[i+1];
            b[i] = temp * b[i];
        }return b;
    }
};
```






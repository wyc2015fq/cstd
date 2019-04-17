# leetcode-326. Power of Three 3的幂 - 别说话写代码的博客 - CSDN博客





2019年03月11日 21:58:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：69标签：[3的幂																[Power of Three																[leetcode 326](https://so.csdn.net/so/search/s.do?q=leetcode 326&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Power of Three&t=blog)



|Given an integer, write a function to determine if it is a power of three.**Example 1:****Input:** 27**Output:** true**Example 2:****Input:** 0**Output:** false**Example 3:****Input:** 9**Output:** true**Example 4:****Input:** 45**Output:** false|给定一个整数，写一个函数来判断它是否是 3 的幂次方。**示例 1:****输入:** 27**输出:** true**示例 2:****输入:** 0**输出:** false**示例 3:****输入:** 9**输出:** true**示例 4:****输入:** 45**输出:** false|
|----|----|

思路：[判断2的幂](https://blog.csdn.net/qq_21997625/article/details/88410292)是看二进制中是否只有一个1，判断3,4的次幂可以直接用log函数的性质

第一种：每次除以3，看最后剩余的是不是1

```cpp
class Solution {
public:
    bool isPowerOfThree(int n) {
        while(n && n%3==0)   //若能被3整除就一直除，不能整除肯定不是3的幂，能整除的不一定是，除非最后除的剩余是1
            n/=3;
        return n==1;
    }
};
```

第二种：一个数是3的次方，那log以3为底的某数一定是整数。

```cpp
class Solution {
public:
    bool isPowerOfThree(int n) {
        double a = log10(n) / log10(3);  //浮点数
        return (a - (int)(a) ==0)? true:false;   //判断是不是整数
    }
};
```](https://so.csdn.net/so/search/s.do?q=3的幂&t=blog)





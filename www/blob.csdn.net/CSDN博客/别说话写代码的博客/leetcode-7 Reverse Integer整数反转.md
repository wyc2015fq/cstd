# leetcode-7:Reverse Integer整数反转 - 别说话写代码的博客 - CSDN博客





2018年12月14日 13:09:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19标签：[leetcode-7																[Reverse Integer																[整形翻转](https://so.csdn.net/so/search/s.do?q=整形翻转&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Reverse Integer&t=blog)




题目：
|Given a 32-bit signed integer, reverse digits of an integer.**Example 1:****Input:** 123**Output:** 321**Example 2:****Input:** -123**Output:** -321**Example 3:****Input:** 120**Output:** 21**Note:**			Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−231,  231 − 1]. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.|给出一个 32 位的有符号整数，你需要将这个整数中每位上的数字进行反转。**示例 1:****输入:** 123**输出:** 321** 示例 2:****输入:** -123**输出:** -321**示例 3:****输入:** 120**输出:** 21**注意:**假设我们的环境只能存储得下 32 位的有符号整数，则其数值范围为 [−231,  231 − 1]。请根据这个假设，如果反转后整数溢出那么就返回 0。|
|----|----|

思路：求解每次使用ret =0 ; ret = ret*10 + x%10; 就行，问题在于处理正负号和整形数范围，正负号或者自己先判断处理，或者直接算。整形范围就要自己判断，或者使用long long

方法一：时间慢了点，但是好理解

```cpp
class Solution {
public:
    int reverse(int x) {
        long long ret=0;   //避免超范围使用long long
        bool flag = true;   //判断正负号
        if(x<0)
        {
            flag = false;
            x=-x;
        }
        while(x > 0)
        {
            ret = ret*10 + x%10;
            x/=10;
        }
        if(ret > INT_MAX) return 0;
        if(flag) return ret;
        else return -ret;
    }
};
```

方法二：时间还行，速度快 

```cpp
class Solution {
public:
    int reverse(int x) {
        int ret=0;
        while(x != 0)
        {
            if(abs(ret) > INT_MAX/10) return 0; //因为要执行下面的乘10，所以当前不能超范围
            ret = ret*10 + x%10;
            x/=10;
        }return ret;
    }
};
```

 方法三：速度快也好理解

```cpp
class Solution {
public:
    int reverse(int x) {
        long long ret=0;   //使用long long
        while(x != 0)
        {
            ret = ret*10 + x%10;
            x/=10;
        }
        return (ret > INT_MAX || ret < INT_MIN)?0:ret;  //最后判断是否超过范围
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode-7&t=blog)





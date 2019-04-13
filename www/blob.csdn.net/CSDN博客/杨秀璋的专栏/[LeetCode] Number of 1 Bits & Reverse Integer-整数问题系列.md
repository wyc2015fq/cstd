
# [LeetCode] Number of 1 Bits & Reverse Integer - 整数问题系列 - 杨秀璋的专栏 - CSDN博客

2015年09月14日 05:15:15[Eastmount](https://me.csdn.net/Eastmount)阅读数：1676


**目录：**
1.Number of 1 Bits  - 计算二进制1的个数 [与运算]
2.Contains Duplicate - 是否存在重复数字 [遍历]
3.Reverse Integer - 翻转整数 [int边界问题]
4.Excel Sheet Column Number - Excel字符串转整数 [简单]
5.Power of Two & Happy Number - 计算各个位数字 [%10 /10]

## 一.Number of 1 Bits
**题目概述：**
Write a function that takes an unsigned integer and returns the number of ’1' bits it has (also known as the[Hamming
 weight](http://en.wikipedia.org/wiki/Hamming_weight)).
For example, the 32-bit integer ’11' has binary representation`00000000000000000000000000001011`
so the function should return 3.
**解题方法：**
三种方法包括：
1.依次和0x1进行&与运算，若结果为1则加1，表示1个数，再右移；
2.推荐的方法，n&(n-1)，直到为0，次数为1的个数；
3.n取2模，依次判断个位是否为1，在n/2移位，常规方法。
其中uint32_t为32位无符号类型数据，[参考地址](http://blog.csdn.net/canglingye/article/details/44177479)
Power of Two题目也可以通过return (n > 0) && (!(n & (n - 1)))一句话实现。
Reverse Bits题目也可以<<移位实现。
**我的代码：**

```python
/*
 * uint32_t为32位无符号类型数据 思路：数字移位
 */
int hammingWeight(uint32_t n) {
    //第一种方法 考查移位及与运算&
    int result=0, left=0;
    while(0 != n)
    {
        left = n & 0x1;
        result += left;
        n = n >> 1;
    }
    return result;
    
    //第二种方法 
    int re = 0;
    while(0 != n)
    {
        n = n&(n - 1);
        ++re;
    }
    return re;
    
    //第三种方法  求2模
    int count = 0;  
    while (n) 
    {  
        if (n % 2 == 1) 
        {  
            ++count;  
        }  
        n /= 2;  
    }  
    return count;  
}
```

## 二.Contains Duplicate
**题目概述：**
Given an array of integers, find if the array contains any duplicates. Your function should return true if any value appears at least twice in the
 array, and it should return false if every element is distinct.
**题目解析：**
题目是给定一个整数数组，判断该数组中是否存在重复的数字。简单AC的方法比较简单，两层循环判断；但是如果要求是O(n)的时间和O(1)的空间，怎样实现呢？腾讯的笔试题就考到了。又见重复判断II III题。
**我的代码：**
```python
bool containsDuplicate(int* nums, int numsSize) {
    //最傻的方法循环判断
    int i,j;
    if(numsSize==0)
        return false;
    for(i=0;i<numsSize;i++)
    {
        for(j=i+1;j<numsSize;j++)
        {
            if(nums[i]==nums[j])
            {
                return true;  //表示存在重复的
            }
        }
    }
    return false;
}
```
**推荐代码：**


## 三.Reverse Integer
**题目概述：**
Reverse digits of an integer.
Example1:x
 = 123, return 321
Example2:x
 = -123, return -321
**解题思路：**
该题主要是考察整数的翻转问题，最简单的方法就是：通过"%10"计算个位数字和"/10"循环进行，直到整数为结果0；但是你需要注意的是：
1.负数的转换x=x*(-1)
2.整数越界，int型范围是(-2147483648~2147483647)，4字节。当x=1534236469时，应该输出0而不是9646324351或溢出后的数
3.需要注意一个特殊的用例：x=-2147483648。此时x=x*(-1)=2147483648溢出，结果应是0。
故此处需要把整数范围的判断指定出来讲解，没考虑整数溢出的代码如下：

```python
//翻转数字 显然采用前面做过的%10提取个位和/10方法
int reverse(int x) {
    int i,j;
    int num;      //存储位数
    int result;   //结果
    bool flag;    //是否是负数 true负数
    
    if( (x>=0&&x<10)||(x<0&&x>-10)) return x;
    if(x>0) 
        flag=true;
    else {
        flag=false;
        x=x*(-1);
    }
    result=0;
    while(x!=0) {
        result=result*10+x%10;   //结果
        x=x/10;
    }
    if(flag==true)
        return result;
    else
        return result*(-1);
}
```
**正确代码：**
重点是类似于Java的Integer.MAX_VALUE 这种直接可用的数值型的最大值定义，C语言采用\#include <limits.h>里面的INT_MIN和INT_MAX，而不是写一长串数字。
```python
/**
 * 翻转数字 刚做完翻转二叉树做该题还是感觉数字亲切点
 * 显然采用前面做过的%10提取个位和/10方法
 * 方法很简单 但是需要注意越界和x==-2147483648变成正数时溢出
 */ 
int reverse(int x) {
    int i,j;
    int num;      //存储位数
    int result;   //结果
    bool flag;    //是否是负数 true负数
    
    if( (x>=0&&x<10)||(x<0&&x>-10)) return x;
    if(x==INT_MIN)   //否则扭转溢出 INT_MIN=-2147483648
        return 0;
    if(x>0) { 
        flag=true;
    }
    else {
        flag=false;
        x=x*(-1);
    }
    result=0;
    while(x!=0) {
        if(x!=0&&result>INT_MAX/10) {  //214748364
            return 0;
        }
        result=result*10+x%10;   //结果
        x=x/10;
        //printf("%d\n",result);
    }
    if(flag==true)
        return result;
    else
        return result*(-1);
}
```

## 四.
## Excel Sheet Column Number
**题目概述：**
Related to question[Excel
 Sheet Column Title](https://leetcode.com/problems/excel-sheet-column-title/)
Given a column title as appear in an Excel sheet, return its corresponding column number.
For example:
A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28
**题目解析：**
该类题目比较简单，主要考察字符串遍历和整数进制问题(26进制)，自己一次AC。
```python
int titleToNumber(char* s) {
    int result;  
    int length;
    int i,j;
    
    if(s==NULL) return 0;
    
    length=strlen(s);
    result=0;
    
    //从右向左遍历 个位右
    for(i=0; i<length; i++) {
        result=result*26+(s[i]-'A')+1;
    }
    return result;
}
```

## 五.
## Power of Two & Happy Number
题目概述：
判断数字是否是2的次数数
判断数字是否是happy数，结果为1则返回true
Example: 19 is a happy number
1^2 + 9^2 = 82
8^2 + 2^2 = 68
6^2 + 8^2 = 100
1^2 + 0^2 + 0^2 = 1
主要考察计算数字每个位数的方法，即n%10和n=n/10的方法。
**我的代码：**
Power of Two
强推位操作判断16=10000即高位为1其他为0，或通过一句话即可：
**return****(n >****0****) && (!(n & (n -****1****)))**
http://www.bubuko.com/infodetail-953320.html

```python
bool isPowerOfTwo(int n) {
    int number;
    
    if(n<=0) return false;
    if(n==1) return true;       //1=2^0
    if(n%2!=0) return false;
    while(n>0) {
        if(n==1) {  //最后一个数字是1
            return true;
        }
        if(n%2!=0) {
            return false;
        }
        else {
            n=n/2;
        }
    }
}
```
Happy Number
```python
bool isHappy(int n) { //重点:可能出现无限循环或数组越界情况 哪种情况不是happy数
    int result;       //结果直至0
    int number;
    
    if(n<=0) return false;
    while(result!=1) {
        //计算result
        result = 0;
        while(n>0) {
            number = n%10;
            n = n/10;
            result = result + number*number;
        }
        if(result==1) {
            return true;
        }
        else if(result<10) { //输入2返回false
            return false;
        }
        else {
            n = result;     //下一计算n为上次的结果
        }
    }
}
```


PS：最后希望文章对您有所帮助，这都是自己A题的一些笔记和心得，同时真心建议你自己动手做做LeetCode。以前我也不信邪，现在信了~
(By:Eastmount 2015-9-14 凌晨5点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



# 剑指Offer_编程题 不用加减乘除做加法 - 紫芝的博客 - CSDN博客





2018年10月18日 21:35:47[紫芝](https://me.csdn.net/qq_40507857)阅读数：17








# **不用加减乘除做加法**

时间限制：1秒 空间限制：32768K 热度指数：81997

[算法知识视频讲解](https://www.nowcoder.com/courses/semester/algorithm-senior)

## 题目描述

**写一个函数，求两个整数之和，要求在函数体内不得使用+、-、*、/四则运算符号。**

**首先，十进制是如何计算加法：5+7=12**
- 相加各位的值，不算进位，得到2
- 计算进位的值，得到10，如果进位为0，那么第一步得到的值即为结果
- 重复上述两步，相加的值变成10和2，结果为12

**同样以三步走的方式计算二进制下的加法：5-101，7-111**
- 相加各位的值，不算进位，得到010；二进制相加就是每一位做异或操作：101^111
- 计算进位值，得到1010；相当于每一位做与操作，再左移一位：(101&111)>>1=1010
- 重复上述两步，各位相加：010^1010=1000；进位值：(010&1010)>>1=100；继续重复上述两步，1000^100=1100,进位值为0，退出循环；结果为1100

```java
import java.util.*;
public class Solution{
	public int Add(int num1,int num2) {
		while(num2!=0) {
			int temp=(num1^num2);
			num2=(num1&num2)<<1;
			num1=temp;
		}
		return num1;
	}
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		Solution work=new Solution();
		while(cin.hasNext()) {
			int a=cin.nextInt();
			int b=cin.nextInt();
			System.out.println(work.Add(a,b));
		}
		cin.close();
	}
	
}
```

**Java大数类**

```java
import java.util.*;
import java.math.*;
public class Solution{
	public int Add(int num1,int num2) {
		BigInteger a=BigInteger.valueOf(num1);
		BigInteger sum=a.add(BigInteger.valueOf(num2));
		return sum.intValue();
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		Solution work=new Solution();
		while(cin.hasNext()) {
			int a=cin.nextInt();
			int b=cin.nextInt();
			System.out.println(work.Add(a,b));
		}
		cin.close();
	}
}
```






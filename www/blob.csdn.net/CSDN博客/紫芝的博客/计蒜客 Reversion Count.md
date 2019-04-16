# 计蒜客 Reversion Count - 紫芝的博客 - CSDN博客





2018年06月01日 18:07:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：48








# Reversion Count




**[点击打开链接](https://nanti.jisuanke.com/t/26217)**

**Description:**

There is a positive integer X, X's reversion count is Y. For example, X=123, Y=321; X=1234, Y=4321. Z=(X-Y)/9, Judge if Z is made up of only one number(0,1,2...9), like Z=11,Z=111,Z=222,don't consider '+'and '-'.

**Input:**

Input contains of several test cases. Each test case only contains of a number X, L is the length of X. ( 2 <= L < 100)


**Output:**

Output “YES”or “NO”.




#### 样例输入
10
13


#### 样例输出
YES
YES


#### 题目来源

[2018 ACM-ICPC 中国大学生程序设计竞赛线上赛](https://nanti.jisuanke.com/?kw=2018%20ACM-ICPC%20%E4%B8%AD%E5%9B%BD%E5%A4%A7%E5%AD%A6%E7%94%9F%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E7%AB%9E%E8%B5%9B%E7%BA%BF%E4%B8%8A%E8%B5%9B)


解法一：


首先 **一个数n与他反序数的差的绝对值，一定是9的倍数。**证明如下：

设四位数 ABCD 他的反序数是DBCA

ABCD-DCBA=(1000*A+100*B+10*C+D)-(1000*D+100*C+10*B+A)

=(1000-1)*A+(100-10)*B-(100-10)*C-(1000-1)*D

=999*A+90*B-90*C-999*D

=(111*A+10*B-10*C-111*D)*9




再设五位数 ABCDE 他的反序数是 EDCBA

ABCDE-EDCBA=(10000*A+1000*B+100*C+10*D+E)-(10000*E+1000*D+100*C+10*B+A)

=9999*A+990*B+**0*C**-990*D-9999*E

=(1111*A+110*B-110*D-1111*E)*9




对于任一数都可以按此方法证明。

现在我们可以观察到 要想使最后结果全由同一个数字组成就取决于 **除第一位与最后一位 剩余所有对称位置的数的值是否相等，**若相等则一定是1，11，111，1111...的倍数，不等则一定不是。

如四位数ABCD， 111*A-111*D的结果一定是111的倍数，要使最后结果也是1,11,111,1111...的倍数那么10*B-10*C=10*(B-C)的结果一定要是0(你说B-C的结果可能是11的倍数（除0）？？？)

五位数 ABCDE， 1111*A-1111*E的结果一定是1111的倍数，注意观察到 凡是有奇数位的数最中间的那个数最后一定被消掉了，对于ABCDE来说就是C最后被消掉了，所以最后一定要(110*B-110*D)=(B-D)*110的值为0

另外1到3位数可以直接输出YES，因为他们的结果都是1位数

打表找规律代码

```cpp
#include<bits/stdc++.h>
using namespace std;
int rev(int x){
    int t = 0;
    int value = 0;
    while (x / 10){
        value = 10 * value + x % 10;
        x /=  10;
    }
    return value*10 + x;
}
int main()
{
    int x,y,z;
    for(x=0;x<100000;x++)
    {
        y=rev(x);
        z=(x-y)/9;
        printf("%d     ",x);
        z=abs(z);
        if(z<90)
            printf("YES\n");
        else{
            set<int> s;s.clear();
        while(z){
            s.insert(z%10);
            z/=10;
        }

        if(s.size()==1)
              printf("YES\n");
            else    printf("NO\n");
        }
    }
    return 0;
}
```
AC代码

```cpp
#include<stdio.h>
#include<string.h>

char str[110];

int main()
{
	while(~scanf("%s",str))
	{
		int l=strlen(str);
		if(l<=3)
		{
			printf("YES\n");
			continue;
		}
		int flag=1;
		for(int i=1;i<=l/2-1;i++)
		{
			if(str[i]!=str[l-1-i]) flag=0;
		}
		if(flag) printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
```
解法二：

直接采用大数相加减然后模拟除法

JAVA


```java
import java.math.BigInteger;
import java.util.Scanner;
import java.io.*;
public class Main
{
public static void main(String args[])
{
Scanner cin = new Scanner(System.in);
while(cin.hasNext())
{
BigInteger x,y,z,xx;
x=cin.nextBigInteger();
xx=x;
y=BigInteger.ZERO;
z=BigInteger.ZERO;
BigInteger n,k;
n=BigInteger.ZERO;
k=BigInteger.ZERO;
while(x.compareTo(BigInteger.valueOf(0))!=0)
{
n=x.mod(BigInteger.valueOf(10));
y=y.multiply(BigInteger.valueOf(10));
y=y.add(n);
x=x.divide(BigInteger.valueOf(10));
}
z=xx.subtract(y);
z=z.abs();
z=z.divide(BigInteger.valueOf(9));
n=z.mod(BigInteger.valueOf(10));
z=z.divide(BigInteger.valueOf(10));
k=n;
// System.out.println(n);
// System.out.println(k);
int f=1;
while(z.compareTo(BigInteger.valueOf(0))!=0)
{
n=z.mod(BigInteger.valueOf(10));
if(k.compareTo(n)!=0)
{
f=0;
break;
}
z=z.divide(BigInteger.valueOf(10));
}
if(f==1)System.out.println("YES");
else System.out.println("NO");
}
}
}
```
C++





































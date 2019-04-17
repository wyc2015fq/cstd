# 【水String】#61 A. Petya and Java - CD's Coding - CSDN博客





2014年10月08日 23:07:59[糖果天王](https://me.csdn.net/okcd00)阅读数：622
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Little Petya has recently started attending a programming club. Naturally he is facing the problem of choosing a programming language. After long considerations he realized that Java is the best choice. The main argument in favor of choosing Java was that it
 has a very large integer data type, called BigInteger.


But having attended several classes of the club, Petya realized that not all tasks require using the BigInteger type. It turned out that in some tasks it is much easier to use small data types. That's why a question arises: "Which integer type to use if one
 wants to store a positive integer*n*?"


Petya knows only 5 integer types:


1) byte occupies 1 byte and allows you to store numbers from  - 128 to 127


2) short occupies 2 bytes and allows you to store numbers from  - 32768 to 32767


3) int occupies 4 bytes and allows you to store numbers from  - 2147483648 to 2147483647


4) long occupies 8 bytes and allows you to store numbers from  - 9223372036854775808 to9223372036854775807


5) BigInteger can store any integer number, but at that it is not a primitive type, and operations with it are much slower.


For all the types given above the boundary values are included in the value range.


From this list, Petya wants to choose the smallest type that can store a positive integer *n*. Since BigInteger works much slower, Peter regards it last. Help
 him.




Input


The first line contains a positive number *n*. It consists of no more than 100 digits
 and doesn't contain any leading zeros. The number *n* can't be represented as an empty string.


Please, do not use %lld specificator to read or write 64-bit integers in C++. It is preffered to use cout (also
 you may use %I64d).




Output


Print the first type from the list "byte, short, int, long, BigInteger",
 that can store the natural number *n*, in accordance with the data given above.




Sample test(s)




input
127




output
byte




input
130




output
short




input
123456789101112131415161718192021222324




output
BigInteger












很明显没有一个数据类型能存这么大的数字，但是我们可以存这么长的字符串



string读入，然后预设置好每两种的界限，string类的大小于符号判断是字典序，所以正好可以判断，那么，直接if/else看就可以了






```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
using namespace std;

int main()
{
	char now;
	string s;
	string _byte="127";
	string _short="32767";
	string _int="2147483647";
	string _long="9223372036854775807";
	long long n=0;
	cin>>s;
	int len=s.length();
		 if(len<3	||	(len==3 && s<=_byte))	cout<<"byte";
	else if(len<5	||	(len==5 && s<=_short))	cout<<"short";
	else if(len<10	||	(len==10 && s<=_int))	cout<<"int";
	else if(len<19	||	(len==19 && s<=_long))	cout<<"long";
	else cout<<"BigInteger";
	return 0;
}
```












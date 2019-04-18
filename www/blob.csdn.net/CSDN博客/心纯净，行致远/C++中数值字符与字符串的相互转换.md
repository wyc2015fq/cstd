# C++中数值字符与字符串的相互转换 - 心纯净，行致远 - CSDN博客





2018年12月01日 09:54:16[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：148
个人分类：[【C++】																[【PAT总结】](https://blog.csdn.net/zhanshen112/article/category/8405927)](https://blog.csdn.net/zhanshen112/article/category/7639351)








C++处理字符串和数值时，经常需要相互转换。C++11及以上的<string>就提供了很多类似的函数。
- 
# 字符串转化为数值


Convert from strings

[**stoi**](http://www.cplusplus.com/reference/string/stoi/)    Convert string to integer (function template )

[**stol**](http://www.cplusplus.com/reference/string/stol/)    Convert string to long int (function template )

[**stoul**](http://www.cplusplus.com/reference/string/stoul/)   Convert string to unsigned integer (function template )

[**stoll    **](http://www.cplusplus.com/reference/string/stoll/)Convert string to long long (function template )

[**stoull**](http://www.cplusplus.com/reference/string/stoull/)    Convert string to unsigned long long (function template )

[**stof**](http://www.cplusplus.com/reference/string/stof/)       Convert string to float (function template )

[**stod**](http://www.cplusplus.com/reference/string/stod/)      Convert string to double (function template )

[**stold**](http://www.cplusplus.com/reference/string/stold/)     Convert string to long double (function template )

一般的字符串转化为数值，也有很多是自己写的。核心思想是让字符串的每一位-‘0’。
- 
# 数值转化为字符串std::to_string


string to_string (int val);
string to_string (long val);
string to_string (long long val);
string to_string (unsigned val);
string to_string (unsigned long val);
string to_string (unsigned long long val);
string to_string (float val);
string to_string (double val);
string to_string (long double val);
Convert numerical value to string

Returns a [string](http://www.cplusplus.com/string) with the representation of *val*.


The format used is the same that [printf](http://www.cplusplus.com/printf) would print for the corresponding type:
|type of *val*|printf equivalent|description|
|----|----|----|
|int|"%d"|Decimal-base representation of *val*.			The representations of negative values are preceded with a minus sign (-).|
|long|"%ld| |
|long long|"%lld| |
|unsigned|"%u"|Decimal-base representation of *val*.|
|unsigned long|"%lu| |
|unsigned long long|"%llu| |
|float|"%f"|As many digits are written as needed to represent the integral part, followed by the decimal-point character and six decimal digits.			inf (or infinity) is used to represent *infinity*.			nan (followed by an optional sequence of characters) to represent NaNs (*Not-a-Number*).			The representations of negative values are preceded with a minus sign (-).|
|double|"%f| |
|long double|"%Lf| |



### Parameters

val

Numerical value.

### Return Value

A [string](http://www.cplusplus.com/string) object containing the representation of *val* as a sequence of characters.





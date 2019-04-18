# 字符串转整数 C++编程 - sinat_32043495的博客 - CSDN博客





2017年11月24日 11:45:24[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：95标签：[C++剑指offer](https://so.csdn.net/so/search/s.do?q=C++剑指offer&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/sinat_32043495/article/category/7307292)









                
//首先考虑空的字符串指针

//正负号

//非数字字符串

//考虑溢出

#include<string>

#include<iostream>

using namespace std;

int sting_to_int(const string &str)throw(bad_cast)

{
int value = 0;
int sign = -1;
if (str.empty())
{
cout << "str is empty"<<endl;
return 0;
}
int i = 0;
if (str[0] == '-')
{
if (str.length() > 1)
{
sign = -1;
i = 1;
}
else
{
cout << "no number"<<endl;
return 0;
}

}
if (str[0] == '+')
{
if (str.length() > 1)
{
sign = 1;
i = 1;
}
else
{
cout << "no number" <<endl;
return 0;
}
}
size_t length = str.length();
for (; i < length; i++)
{
if ('0' <= str[i] && str[i] <= '9')
{
value = value * 10 + str[i] - '0';
if (((sign==1)&& value > 0x7FFFFFFF)
|| ((sign==-1) && value < 0x80000000))
{
cout << "str is out of range" <<endl;
return 0;
}
}
else
{
cout << "str has nonnumeric character" << endl;
return 0;
}
}
return value*sign;

}



int main()

{
/*sting_to_int(NULL);


sting_to_int("");*/


/*cout << sting_to_int("123")<<endl;


cout << sting_to_int("+123")<<endl;


cout << sting_to_int("-123")<<endl;


sting_to_int("1a33");


sting_to_int("+0");


sting_to_int("-0");*/


//有效的最大正整数, 0x7FFFFFFF
cout << sting_to_int("+2147483647") << endl;


cout << sting_to_int("-2147483647") << endl;


cout << sting_to_int("+2147483648") << endl;


//有效的最小负整数, 0x80000000
cout << sting_to_int("-2147483648") << endl;


cout << sting_to_int("+2147483649") << endl;


cout << sting_to_int("-2147483649") << endl;


/*sting_to_int("+");


sting_to_int("-");*/
system("pause");
return 0;

}
            



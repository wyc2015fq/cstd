# C++中int与string的相互转换 - Likes的博客 - CSDN博客
2018年10月30日 14:55:47[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：19标签：[C  																[字符串																[转换																[int																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=int&t=blog)](https://so.csdn.net/so/search/s.do?q=转换&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=C  &t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
# 一、int转string
### 1.c++11标准增加了全局函数std::to_string:
```cpp
string to_string (int val);
string to_string (long val);
string to_string (long long val);
string to_string (unsigned val);
string to_string (unsigned long val);
string to_string (unsigned long long val);
string to_string (float val);
string to_string (double val);
string to_string (long double val);
```
**Example:**
```cpp
// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
int main ()
{
  std::string pi = "pi is " + std::to_string(3.1415926);
  std::string perfect = std::to_string(1+2+4+7+14) + " is a perfect number";
  std::cout << pi << '\n';
  std::cout << perfect << '\n';
  return 0;
}
```
**Output:**
```cpp
pi is 3.141593
28 is a perfect number
```
### 2.采用sstream中定义的字符串流对象来实现
```cpp
ostringstream os; //构造一个输出字符串流，流内容为空 
int i = 12; 
os << i; //向输出字符串流中输出int整数i的内容 
cout << os.str() << endl; //利用字符串流的str函数获取流中的内容
```
# 二、string转int
### 1.可以使用std::stoi/stol/stoll等等函数
**Example:**
```cpp
// stoi example
#include <iostream>   // std::cout
#include <string>     // std::string, std::stoi
int main ()
{
  std::string str_dec = "2001, A Space Odyssey";
  std::string str_hex = "40c3";
  std::string str_bin = "-10010110001";
  std::string str_auto = "0x7f";
  std::string::size_type sz;   // alias of size_t
  int i_dec = std::stoi (str_dec,&sz);//&sz是传入的指针，将会记录转换成int的字符的个数，预期是4
  int i_hex = std::stoi (str_hex,nullptr,16);
  int i_bin = std::stoi (str_bin,nullptr,2);
  int i_auto = std::stoi (str_auto,nullptr,0);
  std::cout << str_dec << ": " << i_dec << " and [" << str_dec.substr(sz) << "]\n";//substr()取子字符串，从sz偏移截取到尾部
  std::cout << str_hex << ": " << i_hex << '\n';
  std::cout << str_bin << ": " << i_bin << '\n';
  std::cout << str_auto << ": " << i_auto << '\n';
  return 0;
}
```
**Output:**
```cpp
2001, A Space Odyssey: 2001 and [, A Space Odyssey]
40c3:  16579
-10010110001: -1201
0x7f: 127
```
### 2.采用标准库中atoi函数
对于其他类型也都有相应的标准库函数，比如浮点型atof(),long型atol()等等
```cpp
string s = "12";
int a = atoi(s.c_str());//c_str()取C++中string对象中封装的char*
```
### 3.采用sstream头文件中定义的字符串流对象来实现转换
```cpp
istringstream is("12"); //构造输入字符串流，流的内容初始化为“12”的字符串 
int i; 
is >> i; //从is流中读入一个int整数存入i中
```

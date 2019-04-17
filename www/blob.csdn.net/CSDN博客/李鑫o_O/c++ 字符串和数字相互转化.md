# c++ 字符串和数字相互转化 - 李鑫o_O - CSDN博客





2016年03月07日 16:24:53[hustlx](https://me.csdn.net/HUSTLX)阅读数：829








       在C++11中string 类中有stoi、stod、stof、stol和stoll等函数分别是把字符串转化为int、double、float、long和long、long型。

       而数字转字符串则用to_string

       用法举例如下：



```cpp
#include <iostream>   // std::cout
#include <string>     // std::string, std::stod

int main ()
{
  std::string str_bin = "2456"
  int i_bin = std::stoi (str_bin);
  std::string s=std::to_string(i_bin);

}
```


另外，stoi还可以指定向2、8、10、16进制转化：



```cpp
// stoi example
#include <iostream>   // std::cout
#include <string>     // std::string, std::stoi

int main ()
{
  std::string str_dec = "2001, A Space Odyssey";
  std::string str_hex = "40c3";
  std::string str_bin = "-10010110001";
  std::string str_auto = "0x7f";

  std::string::size_type sz;   // alias of size_t

  int i_dec = std::stoi (str_dec,&sz);
  int i_hex = std::stoi (str_hex,nullptr,16);
  int i_bin = std::stoi (str_bin,nullptr,2);
  int i_auto = std::stoi (str_auto,nullptr,0);

  std::cout << str_dec << ": " << i_dec << " and [" << str_dec.substr(sz) << "]\n";
  std::cout << str_hex << ": " << i_hex << '\n';
  std::cout << str_bin << ": " << i_bin << '\n';
  std::cout << str_auto << ": " << i_auto << '\n';

  return 0;
}
```










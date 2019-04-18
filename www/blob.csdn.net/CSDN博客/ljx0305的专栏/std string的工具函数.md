# std::string的工具函数 - ljx0305的专栏 - CSDN博客
2013年06月08日 17:37:21[ljx0305](https://me.csdn.net/ljx0305)阅读数：3645
一般来说，在处理字符串的时候通常会用到如下一些函数／方法：length、substring、find、charAt、toLowerCase、toUpperCase、trim、equalsIgnoreCase、startsWith、endsWith、parseInt、toString、split等。
　　如果使用STL中的std::string，它已经提供了如下一些比较有用的方法：
> 
length()，取得字符串的长度。
substr()，从字符串中取出一个子串。
at()/operator []，取得字符串中指定位置的字符。
find/rfind()，从前往后／从后往前在字符串中查找一个子串的位置。
find_first_of()，在字符串中找到第一个在指定字符集中的字符位置。
find_first_not_of()，在字符串中找到第一次人不在指定字符集中的字符位置。
find_last_of()，在字符串中找到最后一个在指定字符集中的字符位置。
find_last_not_of()，在字符串中找到最后一个不在字符集中的字符位置。
　　关于std::string的其它方法，请参阅它的文档（在MSDN中可以找到）。
　　很容易发现，std::string并没有提供所有需要方法。所以，需要用STL提供了算法库、字符串流以及现存的std::string的方法来实现它们。
**※ 将字符串转换为大写／小写**
std::transform(str.begin(), str.end(), str.begin(), tolower);
std::transform(str.begin(), str.end(), str.begin(), toupper);
**※ 去掉字符串两端的空格**
1) 去掉左边的空格 
str.erase(0, str.find_first_not_of(" /t/n/r"));
2) 去掉右边的空格
str.erase(str.find_last_not_of(" /t/n/r") +1);
3) 去掉两边的空格
str.erase(0, str.find_first_not_of(" /t/n/r")).erase(str.find_last_not_of(" /t/n/r") +1);
**※ 忽略大小写比较字符串**
　　这一功能的实现比较简单，只需要先将用于比较的两个字符串各自拷贝一个复本，并将这两个复本转换为小写，然后比较转换为小写之后的两个字符串即可。
**※ StartsWith和EndsWith**
1) StartsWith
str.find(substr) ==0;
如果返回值为true，则str是以substr开始的
2) EndsWith
str.rfind(substr) == (str.length() - substr.length());
如果返回值为true，则str是以substr结束的
　　还有另一个方法可以实现这两个函数。就是将str从头／尾截取substr长度的子串，再将这个子串也substr进行比较。不过这种方法需要判断str的长度是否足够，所以建议用find和rfind来实现。
**※ 从字符串解析出int和bool等类型的值**
　　说到将字符串解析成int，首先想到的一定是atoi、atol等C函数。如果用C++来完成这些工具函数，那就要用到std::istringstream。
　　除了解析bool值之外，下面这个函数可以解析大部分的类型的数值：
template<class T> parseString(const std::string& str) {
    T value;
    std::istringstream iss(str);
    iss >> value;
return value;
}
　　上面这个模板可以将0解析成bool值false，将非0解析成treu。但它不能将字符串"false"解析成false，将"true"解析成true。因此要用一个特别的函数来解析bool型的值：
template<bool>
bool parseString(const std::string& str) {
bool value;
    std::istringstream iss(str);
    iss >> boolalpha >> value;
return value;
}
　　上面的函数中，向输入流传入一个std::boolalpha标记，输入流就能认识字符形式的"true"和"false"了。
　　使用与之类似的办法解析十六进制字符串，需要传入的标记是std::hex：
template<classT> parseHexString(const std::string& str) {
    T value;
    std::istringstream iss(str);
    iss >> hex >> value;
return value;
}
**※ 将各种数值类型转换成字符串（toString）**
　　与解析字符串类似，使用std::ostringstream来将各种数值类型的数值转换成字符串，与上面对应的3个函数如下： 
template<class T> std::string toString(const T& value) {
    std::ostringstream oss;
    oss << value;
return oss.str();
}
string toString(constbool& value) {
    ostringstream oss;
    oss << boolalpha << value;
return oss.str();
}
template<class T> std::string toHexString(const T& value, int width) {
    std::ostringstream oss;
    oss << hex;
if (width >0) {
        oss << setw(width) << setfill('0');
    }
    oss << value;
return oss.str();
}
　　注意到上面函数中用到的setw和setfill没有？它们也是一种标记，使用的时候需要一个参数。std::setw规定了向流输出的内容占用的宽度，如果输出内容的宽度不够，默认就用空格填位。std::setfill则是用来设置占位符。如果还需要控制输出内容的对齐方式，可以使用std::left和std::right来实现。
**※ 拆分字符串和Tokenizer**
　　拆分字符串恐怕得用Tokenizer来实现。C提供了strtok来实现Tokenizer，在STL中，用std::string的find_first_of和find_first_not_of来实现。下面就是Tokenizer类的nextToken方法：
bool Tokenizer::nextToken(const std::string& delimiters) {
// find the start character of the next token.
    size_t i = m_String.find_first_not_of(delimiters, m_Offset);
if (i ==string::npos) {
        m_Offset = m_String.length();
returnfalse;
    }
// find the end of the token.
    size_t j = m_String.find_first_of(delimiters, i);
if (j ==string::npos) {
        m_Token = m_String.substr(i);
        m_Offset = m_String.length();
returntrue;
    }
// to intercept the token and save current position
    m_Token = m_String.substr(i, j - i);
    m_Offset = j;
returntrue;
}
**※ 源代码**
　　最后，关于上述的一些方法，都已经实现在strutil.h和strutil.cpp中，所以现在附上这两个文件的内容：
√Header file: strutil.h
////////////////////////////////////////////////////////////////////////////////
//
// Utilities for std::string
// defined in namespace strutil
// by James Fancy
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
// declaration
namespace strutil {
    std::string trimLeft(const std::string& str);
    std::string trimRight(const std::string& str);
    std::string trim(const std::string& str);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
bool startsWith(const std::string& str, const std::string& substr);
bool endsWith(const std::string& str, const std::string& substr);
bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
    template<class T> T parseString(const std::string& str);
    template<class T> T parseHexString(const std::string& str);
    template<bool>bool parseString(const std::string& str);
    template<class T> std::string toString(const T& value);
    template<class T> std::string toHexString(const T& value, int width =0);
    std::string toString(constbool& value);
    std::vector<std::string> split(const std::string& str, const std::string& delimiters);
}
// Tokenizer class
namespace strutil {
class Tokenizer
    {
public:
staticconst std::string DEFAULT_DELIMITERS;
        Tokenizer(const std::string& str);
        Tokenizer(const std::string& str, const std::string& delimiters);
bool nextToken();
bool nextToken(const std::string& delimiters);
const std::string getToken() const;
/**
        * to reset the tokenizer. After reset it, the tokenizer can get
        * the tokens from the first token.
*/
void reset();
protected:
        size_t m_Offset;
const std::string m_String;
        std::string m_Token;
        std::string m_Delimiters;
    };
}
// implementation of template functions
namespace strutil {
    template<class T> T parseString(const std::string& str) {
        T value;
        std::istringstream iss(str);
        iss >> value;
return value;
    }
    template<class T> T parseHexString(const std::string& str) {
        T value;
        std::istringstream iss(str);
        iss >> hex >> value;
return value;
    }
    template<class T> std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
return oss.str();
    }
    template<class T> std::string toHexString(const T& value, int width) {
        std::ostringstream oss;
        oss << hex;
if (width >0) {
            oss << setw(width) << setfill('0');
        }
        oss << value;
return oss.str();
    }
}
√Source file: strutil.cpp 
////////////////////////////////////////////////////////////////////////////////
//
// Utilities for std::string
// defined in namespace strutil
// by James Fancy
//
////////////////////////////////////////////////////////////////////////////////
#include "strutil.h"
#include <algorithm>
namespace strutil {
usingnamespace std;
string trimLeft(conststring& str) {
string t = str;
        t.erase(0, t.find_first_not_of(" /t/n/r"));
return t;
    }
string trimRight(conststring& str) {
string t = str;
        t.erase(t.find_last_not_of(" /t/n/r") +1);
return t;
    }
string trim(conststring& str) {
string t = str;
        t.erase(0, t.find_first_not_of(" /t/n/r"));
        t.erase(t.find_last_not_of(" /t/n/r") +1);
return t;
    }
string toLower(conststring& str) {
string t = str;
        transform(t.begin(), t.end(), t.begin(), tolower);
return t;
    }
string toUpper(conststring& str) {
string t = str;
        transform(t.begin(), t.end(), t.begin(), toupper);
return t;
    }
bool startsWith(conststring& str, conststring& substr) {
return str.find(substr) ==0;
    }
bool endsWith(conststring& str, conststring& substr) {
return str.rfind(substr) == (str.length() - substr.length());
    }
bool equalsIgnoreCase(conststring& str1, conststring& str2) {
return toLower(str1) == toLower(str2);
    }
    template<bool>
bool parseString(const std::string& str) {
bool value;
        std::istringstream iss(str);
        iss >> boolalpha >> value;
return value;
    }
string toString(constbool& value) {
        ostringstream oss;
        oss << boolalpha << value;
return oss.str();
    }
    vector<string> split(conststring& str, conststring& delimiters) {
        vector<string> ss;
        Tokenizer tokenizer(str, delimiters);
while (tokenizer.nextToken()) {
            ss.push_back(tokenizer.getToken());
        }
return ss;
    }
}
namespace strutil {
conststring Tokenizer::DEFAULT_DELIMITERS("");
    Tokenizer::Tokenizer(const std::string& str)
        : m_String(str), m_Offset(0), m_Delimiters(DEFAULT_DELIMITERS) {}
    Tokenizer::Tokenizer(const std::string& str, const std::string& delimiters)
        : m_String(str), m_Offset(0), m_Delimiters(delimiters) {}
bool Tokenizer::nextToken() {
return nextToken(m_Delimiters);
    }
bool Tokenizer::nextToken(const std::string& delimiters) {
// find the start charater of the next token.
        size_t i = m_String.find_first_not_of(delimiters, m_Offset);
if (i ==string::npos) {
            m_Offset = m_String.length();
returnfalse;
        }
// find the end of the token.
        size_t j = m_String.find_first_of(delimiters, i);
if (j ==string::npos) {
            m_Token = m_String.substr(i);
            m_Offset = m_String.length();
returntrue;
        }
// to intercept the token and save current position
        m_Token = m_String.substr(i, j - i);
        m_Offset = j;
returntrue;
    }
conststring Tokenizer::getToken() const {
return m_Token;
    }
void Tokenizer::reset() {
        m_Offset =0;
    }
}
引用:http://blog.csdn.net/jamesfancy/article/details/1543338

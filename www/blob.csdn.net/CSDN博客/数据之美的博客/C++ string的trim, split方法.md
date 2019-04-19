# C++ string的trim, split方法 - 数据之美的博客 - CSDN博客
2017年04月07日 16:07:47[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：7178
很多其他语言的libary都会有去除string类的首尾空格的库函数，但是标准C++的库却不提供这个功能。但是C++string也提供很强大的功能，实现trim这种功能也不难。下面是几种方法：
    1.使用string的find_first_not_of,和find_last_not_of方法
**[cpp]**[view
 plain](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)[copy](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)
- /* 
- Filename : StringTrim1.cpp
- Compiler : Visual C++ 8.0
- Description : Demo how to trim string by find_first_not_of & find_last_not_of
- Release : 11/17/2006
-  */
- #include <iostream>
- #include <string>
- 
- std::string& trim(std::string &);  
- 
- int main()   
- {  
-     std::string s = " Hello World!! ";  
-     std::cout << s << " size:" << s.size() << std::endl;  
-     std::cout << trim(s) << " size:" << trim(s).size() << std::endl;  
- 
- return 0;  
- }  
- 
- std::string& trim(std::string &s)   
- {  
- if (s.empty())   
-     {  
- return s;  
-     }  
- 
-     s.erase(0,s.find_first_not_of(" "));  
-     s.erase(s.find_last_not_of(" ") + 1);  
- return s;  
- }  
2.使用boost库中的trim,boost库对提供很多C++标准库没有但是又非常常用和好用的库函数，例如正则表达式，线程库等等。
**[cpp]**[view
 plain](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)[copy](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)
- /* 
- Filename : boostStringTrim.cpp
- Compiler : Visual C++ 8.0 / ISO C++ (boost)
- Description : Demo how to boost to trim string
- Release : 02/22/2007 1.0
- */
- #include <iostream>
- #include <string>
- #include <boost/algorithm/string.hpp>
- 
- usingnamespace std;  
- usingnamespace boost;  
- 
- int main() {  
-   string s = " hello boost!! ";  
-   trim(s);  
-   cout << s << endl;  
- }  
3.使用template（我用GCC编译不通过，用VS2005却可以）
**[cpp]**[view
 plain](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)[copy](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)
- /* 
- Filename : stringTrim1.cpp
- Compiler : Visual C++ 8.0
- Description : Demo how to trim string by other method.
- Release : 11/18/2006
- */
- #include <string>
- #include <iostream>
- #include <cwctype>
- 
- template <class T>  
- std::basic_string<T>& trim(std::basic_string<T>&);  
- 
- int main( )   
- {  
-     std::string s = " Hello World!! ";  
-     std::cout << s << " size:" << s.size() << std::endl;  
-     std::cout << trim(s) << " size:" << trim(s).size() << std::endl;  
- 
- return 0;  
- }  
- 
- template <class T>  
- std::basic_string<T>& trim(std::basic_string<T>& s)   
- {  
- if (s.empty()) {  
- return s;  
-   }  
- 
-     std::basic_string<T>::iterator c;  
- // Erase whitespace before the string
- 
- for (c = s.begin(); c != s.end() && iswspace(*c++);); s.erase(s.begin(), --c);  
- 
- // Erase whitespace after the string
- 
- for (c = s.end(); c != s.begin() && iswspace(*--c);); s.erase(++c, s.end());  
- 
- return s;  
- }  
## split方法
**[cpp]**[view
 plain](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)[copy](http://blog.csdn.net/butterfly_dreaming/article/details/10142443#)
- //注意：当字符串为空时，也会返回一个空字符串
- void split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
- {  
- size_t last = 0;  
- size_t index=s.find_first_of(delim,last);  
- while (index!=std::string::npos)  
-     {  
-         ret->push_back(s.substr(last,index-last));  
-         last=index+1;  
-         index=s.find_first_of(delim,last);  
-     }  
- if (index-last>0)  
-     {  
-         ret->push_back(s.substr(last,index-last));  
-     }  
- }  

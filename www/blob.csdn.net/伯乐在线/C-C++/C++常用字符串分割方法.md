# C++常用字符串分割方法 - 文章 - 伯乐在线
原文出处： [~小小鸟~](http://www.cnblogs.com/happykoukou/p/5427268.html)
**1、用strtok函数进行字符串分割**
原型： char *strtok(char *str, const char *delim);
功能：分解字符串为一组字符串。
参数说明：str为要分解的字符串，delim为分隔符字符串。
返回值：从str开头开始的一个个被分割的串。当没有被分割的串时则返回NULL。
其它：strtok函数线程不安全，可以使用strtok_r替代。
示例：


```
//借助strtok实现split
#include <string.h>
#include <stdio.h>
 
int main()
{
    char s[] = "Golden Global   View,disk * desk";
    const char *d = " ,*";
    char *p;
    p = strtok(s,d);
    while(p)
    {
        printf("%s\n",p);
        p=strtok(NULL,d);
    }
 
    return 0;
}
```
**2、substr函数**
原型：string substr ( size_t pos = 0, size_t n = npos ) const;
功能：获得子字符串。
参数说明：pos为起始位置（默认为0），n为结束位置（默认为npos）
返回值：子字符串


```
#include <iostream>
#include <string>
#include <vector>
 
//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  str+=pattern;//扩展字符串以方便操作
  int size=str.size();
 
  for(int i=0; i<size; i++)
  {
    pos=str.find(pattern,i);
    if(pos<size)
    {
      std::string s=str.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
  return result;
}
 
int main()
{
  std::string str;
  std::cout<<"Please input str:"<<std::endl;
  //std::cin>>str;
  getline(std::cin,str);
  std::string pattern;
  std::cout<<"Please input pattern:"<<std::endl;
  //std::cin>>pattern;
  getline(std::cin,pattern);//用于获取含空格的字符串
  std::vector<std::string> result=split(str,pattern);
  std::cout<<"The result:"<<std::endl;
  for(int i=0; i<result.size(); i++)
  {
    std::cout<<result[i]<<std::endl;
  }
 
  std::cin.get();
  std::cin.get();
  return 0;
}
```
**3、find_first_not_of**


```
#include<string>
#include<vector>
#include<iostream>
using namespace std;
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);
  while (string::npos != pos || string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
int main(int argc, char *argv[])
{
  string str("====aaa==bbb=ccc=ddd====");
  vector<string>tokens;
  Tokenize(str, tokens, "=");
  for( int i = 0; i < tokens.size() ; i++ )
  {
    cout << tokens[i] << endl;
  }
  return 0;
}
```
**4、用Boost进行字符串的分割**
用boost库的正则表达式实现字符串分割


```
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "boost/regex.hpp"
 
std::vector<std::string> split(std::string str,std::string s)
{
    boost::regex reg(s.c_str());
    std::vector<std::string> vec;
    boost::sregex_token_iterator it(str.begin(),str.end(),reg,-1);
    boost::sregex_token_iterator end;
    while(it!=end)
    {
        vec.push_back(*it++);
    }
    return vec;
}
int main()
{
    std::string str,s;
    str="sss/ddd/ggg/hh";
    s="/";
    std::vector<std::string> vec=split(str,s);
    for(int i=0,size=vec.size();i<size;i++)
    {
        std::cout<<vec[i]<<std::endl;
    }
    std::cin.get();
    std::cin.get();
    return 0;
}
```
5、最近发现boost里面有自带的split的函数，如果用boost的话，还是直接用split的好，代码如下：


```
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;
int main()
{
  string s = "sss/ddd,ggg";
  vector<string> vStr;
  boost::split( vStr, s, boost::is_any_of( ",/" ), boost::token_compress_on );
  for( vector<string>::iterator it = vStr.begin(); it != vStr.end(); ++ it )
  {
    cout << *it << endl;
  }
  return 0;
}
```

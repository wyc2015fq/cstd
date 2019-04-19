# C++ format 格式化字符串实现方式 - u013366022的专栏 - CSDN博客
2018年04月23日 10:18:03[slitaz](https://me.csdn.net/u013366022)阅读数：5410
### 1.
[http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf](http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf)
```
You can't do it directly, because you don't have write access to the underlying buffer (until C++11; see Dietrich Epp's comment). You'll have to do it first in a c-string, then copy it into a std::string:
  char buff[100];
  snprintf(buff, sizeof(buff), "%s", "Hello");
  std::string buffAsStdStr = buff;
But I'm not sure why you wouldn't just use a string stream? I'm assuming you have specific reasons to not just do this:
  std::ostringstream stringStream;
  stringStream << "Hello";
  std::string copyOfStr = stringStream.str();
```
### 2.
[https://www.zhihu.com/question/35967887/answer/125238385](https://www.zhihu.com/question/35967887/answer/125238385)
### 3.
boost::format
### 4.
[http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf](http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf)
```
Emphasising C++11 std::snprintf, this becomes a pretty easy and safe task. I see a lot of answers on this question that were apparently written before the time of C++11 which use fixed buffer lengths and vargs, something I would not recommend for safety, efficiency and clarity reasons.
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std; //Don't if you're in a header-file
template<typename ... Args>
string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
Line by line explanation:
Aim: Write to a char* by using  std::snprintf and then convert that to a std::string.
First, we determine the desired length of the char array.
From cppreference.com:
Return value
[...] If the resulting string gets truncated due to buf_size limit, function returns the total number of characters (not including the terminating null-byte) which would have been written, if the limit was not imposed.
This means that the desired size is the number of characters plus one, so that the null-terminator will sit after all other characters and that it can be cut off by the string constructor again. This issue was explained by @alexk7 in the comments.
Then, we allocate a new character array and assign it to a std::unique_ptr. This is generally advised, as you won't have to manually delete it again.
Note that this is not a safe way to allocate a unique_ptr with user-defined types as you can not deallocate the memory if the constructor throws an exception!
After that, we can of course just use snprintf for its intended use and write the formatted string to the char[] and afterwards create and return a new std::string from that.
You can see an example in action here.
Additional information for Visual Studio users:
As explained in this answer, Microsoft renamed std::snprintf to _snprintf (yes, without std::). MS further set it as deprecated and advises to use _snprintf_s instead, however _snprintf_s won't accept the buffer to be zero or smaller than the formatted output and will not calculate the outputs length if that occurs. So in order to get rid of the deprecation warnings during compilation, you can insert the following line at the top of the file which contains the use of _snprintf:
#pragma warning(disable : 4996)
```
### 5.
[https://github.com/fmtlib/fmt#compile-time-and-code-bloat](https://github.com/fmtlib/fmt#compile-time-and-code-bloat)
fmt is an open-source formatting library for C++. It can be used as a safe alternative to printf or as a fast alternative to IOStreams.
### 6.
[https://github.com/c42f/tinyformat](https://github.com/c42f/tinyformat)
tinyformat.h is a type safe printf replacement library in a single C++ header file. If you've ever wanted printf("%s", s) to just work regardless of the type of s, tinyformat might be for you. Design goals include:
Type safety and extensibility for user defined types.
C99 printf() compatibility, to the extent possible using std::ostream
Simplicity and minimalism. A single header file to include and distribute with your projects.
Augment rather than replace the standard stream formatting mechanism
C++98 support, with optional C++11 niceties

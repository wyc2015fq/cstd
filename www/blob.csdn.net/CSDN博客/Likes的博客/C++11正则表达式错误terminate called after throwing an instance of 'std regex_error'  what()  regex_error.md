# C++11正则表达式错误terminate called after throwing an instance of 'std::regex_error'   what():  regex_error - Likes的博客 - CSDN博客
2019年01月14日 12:00:04[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：49
原因：
C++11支持的正则表达式有限
解决方案：
- 使用boost库中的正则表达式
- 使用C++14

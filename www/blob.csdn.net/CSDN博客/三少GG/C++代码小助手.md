# C++代码小助手 - 三少GG - CSDN博客
2014年01月19日 16:39:41[三少GG](https://me.csdn.net/scut1135)阅读数：1439

C++ 常用小代码
[char* string互相转换] 
**string to char ***
char *p=string.c_str();
**char * to string**
string str(char*);
stringstream
字符串string转换为其它数据类型
temp="123456";
 1）短整型(int)
 i = atoi(temp);
 2）长整型(long)
 l = atol(temp);
 3）浮点(double)
 d = atof(temp);
[字符串处理]
std::string
[find_last_of](http://www.cplusplus.com/reference/string/string/find_last_of/)
```
unsigned found = str.find_last_of("/\\");
  std::cout << " path: " << str.substr(0,found) << '\n';
  std::cout << " file: " << str.substr(found+1) << '\n';
```

[读取、写入文件]
 [C语言如何获得精确到毫秒的时间]
 http://blog.jeoygin.org/2012/03/c-get-time-millisecond.html
    //计时结束
   struct timeval tm_end;
   gettimeofday(&tm_end, NULL);
   uint32_t proc_delay = (tm_end.tv_sec - tm_start.tv_sec) * 1000000 + (tm_end.tv_usec - tm_start.tv_usec);
   LOG_INFO("write time = %u us", proc_delay);
[ 时间戳]
[http://www.cnblogs.com/Wiseman/archive/2005/10/24/260576.html](http://www.cnblogs.com/Wiseman/archive/2005/10/24/260576.html)

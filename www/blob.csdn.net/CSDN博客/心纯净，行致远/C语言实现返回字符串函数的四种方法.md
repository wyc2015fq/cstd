# C语言实现返回字符串函数的四种方法 - 心纯净，行致远 - CSDN博客





2018年04月15日 17:14:42[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：844








**前言**

C语言返回字符串函数共有四种方式，分别如下：
-       使用堆空间，返回申请的堆地址，注意释放
-       函数参数传递指针，返回该指针
-       返回函数内定义的静态变量（共享）
-       返回全局变量


**下面来看看详细的介绍**

其实就是要返回一个有效的指针，尾部变量退出后就无效了。

使用分配的内存，地址是有效 



```cpp
char *fun() 
{ 
  char* s = (char*)calloc(100, sizeof(char*) ); 
  if (s) 
    strcpy ( s , "abc " ); 
  return s; 
}
```


但这种方式需要注意，必须由使用将将返回的地址free掉

将地址由入参传入 



```cpp
char* fun(char*s) 
{ 
  if (s) 
    strcpy(s, "abc "); 
  return s; 
}
```


这种方式调用都要注意给s分配的大小是足够。 


可以这样： 



```cpp
char* fun(char*s, int len) 
{ 
  if (s) 
  { 
    strncpy(s, "abc ", len-1); 
    s[len-1] = 0; 
  } 
  return s; 
}
```


或才使用局部静态变量 



```cpp
char* fun() 
{ 
  static char s[100]; 
  strcpy(s, "abc "); 
  return s; 
}
```


这种方式需要注意，不要修改返回的这个字符串，由于是共享地址，对它的修改会反应到每个调用者的。可以这样： 



```cpp
const char* fun() 
{ 
  static char s[100]; 
  strcpy(s, "abc "); 
  return s; 
}
```


还有一种是使用全局变量 



```cpp
char g_s[100]; 
char* fun() 
{ 
  strcpy(g_s, "abc "); 
  return s; 
}
```


同样的，也要注意这个变量可存储的最大空间。




出处：http://www.jb51.net/article/99824.htm





# C++ 中的sprintf和snprintf 函数的区别 - youbingchen的博客 - CSDN博客





2016年07月21日 10:59:31[youbingchen](https://me.csdn.net/youbingchen)阅读数：3741








> 
在编程过程中经常有字符串转数字和数字转字符串，相应的函数大家最经常见到可能是atoi和itoa。但是itoa不是标准库里面的函数，所以接下来介绍下面两个函数sprintf和snprintf函数，并将它们比较一番。最近写的博客目前就是一些自己在实现基于 Nginx磁盘缓存的一些知识总结


# sprintf()函数

### 用法

```cpp
int sprintf(char *string,char *format,arg_list);
```

### 说明

数sprintf()的用法和printf()函数一样，只是sprintf()函数给出第一个参数string(一般为字符数组），然后再调用 outtextxy()函数将串里的字符显示在屏幕上。arg_list为参数表，可有不定个数。通常在绘图方式下输出数字时可调用sprintf()函 数将所要输出的格式送到第一个参数，然后显示输出。

# snprintf函数

snprintf函数是sprintf的限制字符数量的一个表达。

### 用法

```cpp
int snprintf(char *str, size_t size, const char *format, ...)
```

### snprintf函数的返回值

> 
sprintf函数返回的是实际输出到字符串缓冲中的字符个数，包括null结束符。而snprintf函数返回的是应该输出到字符串缓冲的字符个数，所以snprintf的返回值可能大于给定的可用缓冲大小以及最终得到的字符串长度


# 总结

sprintf可能导致缓冲区溢出问题而不被推荐使用，所以在项目中我一直优先选择使用snprintf函数，虽然会稍微麻烦那么一点点。这里就是sprintf和snprintf最主要的区别：snprintf通过提供缓冲区的可用大小传入参数来保证缓冲区的不溢出，如果超出缓冲区大小则进行截断。但是对于snprintf函数，还有一些细微的差别需要注意。




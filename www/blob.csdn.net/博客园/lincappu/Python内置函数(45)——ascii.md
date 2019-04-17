# Python内置函数(45)——ascii - lincappu - 博客园







# [Python内置函数(45)——ascii](https://www.cnblogs.com/lincappu/p/8145067.html)





**英文文档：**

`ascii`(*object*)

    As `repr()`, return a string containing a printable representation of an object, but escape the non-ASCII characters in the string returned by `repr()` using `\x`, `\u` or `\U` escapes. This generates a string similar to that returned by `repr()` in Python 2.

 　　返回对象的可打印表字符串表现方式

**说明：**

    1. 返回一个可打印的对象字符串方式表示，如果是非ascii字符就会输出\x，\u或\U等字符来表示。与python2版本里的repr()是等效的函数。

```
>>> ascii(1)
'1'
>>> ascii('&')
"'&'"
>>> ascii(9000000)
'9000000'
>>> ascii('中文') #非ascii字符
"'\\u4e2d\\u6587'"
```













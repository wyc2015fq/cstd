# Python内置函数(58)——input - lincappu - 博客园







# [Python内置函数(58)——input](https://www.cnblogs.com/lincappu/p/8145306.html)





**英文文档：**

`input`([*prompt*])

If the *prompt* argument is present, it is written to standard output without a trailing newline. The function then reads a line from input, converts it to a string (stripping a trailing newline), and returns that. When EOF is read, `EOFError` is raised



**说明：**

　　1. 如果提供了promat参数，首先将参数值输出到标准的输出，并且不换行。函数读取用户输入的值，将其转换成字符串。

```
>>> s = input('please input your name:')
please input your name:Ain
>>> s
'Ain'
```













# Python3代码兼容Python2（两者的一些区别） - 浅梦的博客 - CSDN博客





2017年02月27日 12:20:25[浅梦s](https://me.csdn.net/u012151283)阅读数：6952








# 模块名处理

Python3中将一些Python2的模块名称做了修改，需要我们做一些处理来保证代码在不同Python版本中能够正常运行。本文主要记录自己遇得到的一些不同。 

我现在是通过异常处理的方式解决模块名称的问题。

```python
try:  #python3
    from unittest.mock import MagicMock
except ImportError:  #python3
    from mock import MagicMock
```
|Python3|Python2|
|----|----|
|input()|raw.input(),sys.readline()|
|整除//|/|

# 内置函数
|Python3|Python2|
|----|----|
|unittest.mock.MagicMock|mock.MagicMock|
|urllib.request.urlretrieve|urllib.urlretrieve|
|queue|Queue|
|functools.reduce|reduce|

# 使用future特性

一般在Python3代码引入模块代码的首行添加以下代码：

```python
from __future__ import print_function
from __future__ import division
```

# 列表推导式的变量作用域

在Python2中，列表推导式中的索引变量的是可以泄露到其封闭作用域中的。而Python3中则不可以。 

下面举一个简单的例子说明这个问题，如我们在一个for循环内部使用列表推导式

```
for i in range(2):
    a = [i for i in range(5)]
    print(i)
```

在Python3中，上面的代码输出为

```python
>>0
>>1
```

而在Python2中，上面的代码输出为

```python
>>4
>>4
```

这是由于Python2中的列表推导式中的索引变量i的作用域泄露到了for循环中。 

一种较好的解决方式是在列表推导式中使用与外层循环变量不同的索引变量名。

# 使用脚本将py2代码转为py3

Python安装目录下的`Scripts/2to3.exe`可以将Pyhon2代码转换为Python3。 

使用命令`2to3 -w file.py`可以直接在原地修改文件。其他命令可以使用`2to3 --help`查看。

参考资料

> 
[https://www.cnblogs.com/kungfupanda/archive/2016/06/01/5548303.html](https://www.cnblogs.com/kungfupanda/archive/2016/06/01/5548303.html)





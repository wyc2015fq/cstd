
# 4.2 Python模块的导入 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:40:36[seven-soft](https://me.csdn.net/softn)阅读数：172


Python使用 import 语句导入模块，它的语法如下所示：
import module1
import module2
:
import moduleN
也可以在一行内导入多个模块, 像这样
import module1, module2, ..., moduleN
但是这样的代码可读性不如多行的导入语句。 而且在性能上和生成 Python 字节代码时这两种做法没有什么不同。 所以一般情况下，我们使用第一种格式。
## import 语句的模块顺序
我们推荐所有的模块在 Python 模块的开头部分导入。 而且最好按照这样的顺序：Python 标准库模块；
Python 第三方模块；
应用程序自定义模块。
然后使用一个空行分割这三类模块的导入语句。 这将确保模块使用固定的习惯导入，有助于减少每个模块需要的 import 语句数目。
## from ... import语句
如果你想直接导入argv变量到程序中(为了避免每次为它键入sys.)，那么您可以使用from sys import argv语句。
一般来说，你应该避免使用这个语句，而应该使用import语句，因为你的程序将避免名称冲突，将更具可读性。
例如：frommathimportsqrt
print("16的平方根是",sqrt(16))



# python的format函数 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 15:34:24[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：369标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
在Python 3.0中，%操作符通过一个更强的格式化方法format()进行了增强。对str.format()的支持已经被反向移植到了Python 2.6
在2.6中，8-bit字符串和Unicode字符串都有一个format()方法，这个方法会把字符串当作一个模版，通过传入的参数进行格式化。这个用来格式化的模版使用大括号({,})作为特殊字符。
# Substitute positional argument 0 into the string.
"User ID: {0}".format("root") -> "User ID: root"
# Use the named keyword arguments
 'User ID: {uid} Last seen: {last_login}'.format(
  uid='root',
  last_login = '5 Mar 2008 07:20') ->
 'User ID: root Last seen: 5 Mar 2008 07:20'
大括号可以写两遍来转义。
format("Empty dict: {{}}") -> "Empty dict: {}"
字段名字可以为整数，表示参数的位置。像{0}, {1}等。也可以是参数的名字。你以可以使用字段的组合来读取属性或者字典的key值。
import sys
'Platform: {0.platform}\nPython version: {0.version}'.format(sys) ->
 'Platform: darwin\n
 Python version: 2.6a1+ (trunk:61261M, Mar 5 2008, 20:29:41) \n
 [GCC 4.0.1 (Apple Computer, Inc. build 5367)]'
import mimetypes
 'Content-type: {0[.mp4]}'.format(mimetypes.types_map) ->
 'Content-type: video/mp4'
注意，当使用字典类型的表示方法，如[.mp4]时，你不需要引号将字符串(.mp4)引起来；它会查找用.mp4作为主键的值。以整数开头的字符串会被转换成一个整数。你不能在被格式化的字符串中写复杂的表达式。
到此，我们已经演示了怎样替换指定的字段。我们还可以通过在格式化指示符后面添加一个冒号来进行精确格式化。例如：
# Field 0: left justify, pad to 15 characters
# Field 1: right justify, pad to 6 characters
fmt = '{0:15} ${1:>6}'
fmt.format('Registration', 35) ->
 'Registration $ 35'
fmt.format('Tutorial', 50) ->
 'Tutorial $ 50'
fmt.format('Banquet', 125) ->
 'Banquet $ 125'
格式化指示符可以通过嵌套进行引用。
fmt = '{0:{1}}'
width = 15
fmt.format('Invoice #1234', width) ->
 'Invoice #1234 '
width = 35
fmt.format('Invoice #1234', width) ->
 'Invoice #1234 '
可以指定所需长度的字符串的对齐方式。
效果字符：
< （默认）左对齐
> 右对齐
^ 中间对齐
= （只用于数字）在小数点后进行补齐
格式化指示符可以包含一个展示类型来控制格式。例如，浮点数可以被格式化为一般格式或用幂来表示。
>>> '{0:g}'.format(3.75)
 '3.75'
>>> '{0:e}'.format(3.75)
 '3.750000e+00'
展示类型有很多。2.6的文档里有完整的列表。这里列出一些示例。
'b' - 二进制。将数字以2为基数进行输出。
'c' - 字符。在打印之前将整数转换成对应的Unicode字符串。
'd' - 十进制整数。将数字以10为基数进行输出。
'o' - 八进制。将数字以8为基数进行输出。
'x' - 十六进制。将数字以16为基数进行输出，9以上的位数用小写字母。
'e' - 幂符号。用科学计数法打印数字。用'e'表示幂。
'g' - 一般格式。将数值以fixed-point格式输出。当数值特别大的时候，用幂形式打印。
'n' - 数字。当值为整数时和'd'相同，值为浮点数时和'g'相同。不同的是它会根据区域设置插入数字分隔符。
'%' - 百分数。将数值乘以100然后以fixed-point('f')格式打印，值后面会有一个百分号。
类和类型可以定义一个__format__()方法来控制怎样格式化自己。它会接受一个格式化指示符作为参数：
def __format__(self, format_spec):
 if isinstance(format_spec, unicode):
  return unicode(str(self))
 else:
  return str(self)
还有一个内置的format()方法可以用来格式化一个值。它会调用类型的__format__()方法，并将格式化指示符作为参数传进去。
>>> format(75.6564, '.2f')
 '75.66'
参考
格式化字符串的语法 - 格式化字段的参考文档
PEP 3101 - Advanced String Formatting
PEP由Talin编写，由Eric Smith实现。
from http://www.onlypython.com/post/1926/

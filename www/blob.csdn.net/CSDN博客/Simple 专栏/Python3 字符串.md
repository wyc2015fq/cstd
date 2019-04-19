# Python3  字符串 - Simple 专栏 - CSDN博客
2018年10月05日 19:24:49[Simple_Zz](https://me.csdn.net/love284969214)阅读数：107
- [访问字符串中的值](#Python%20%E8%AE%BF%E9%97%AE%E5%AD%97%E7%AC%A6%E4%B8%B2%E4%B8%AD%E7%9A%84%E5%80%BC)
- [字符串更新](#Python%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%9B%B4%E6%96%B0)
- [转义字符](#Python%E8%BD%AC%E4%B9%89%E5%AD%97%E7%AC%A6)
- [字符串运算符](#Python%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%BF%90%E7%AE%97%E7%AC%A6)
- [字符串格式化](#Python%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%A0%BC%E5%BC%8F%E5%8C%96)
- [三引号](#Python%E4%B8%89%E5%BC%95%E5%8F%B7)
- [Unicode 字符串](#Unicode%20%E5%AD%97%E7%AC%A6%E4%B8%B2)
字符串是 Python 中最常用的数据类型。我们可以使用引号  '  或  "  来创建字符串。
创建字符串很简单，只要为变量分配一个值即可。例如：
```python
var1 = 'Hello World!'
var2 = "Runoob"
```
## 访问字符串中的值
Python 不支持单字符类型，单字符在 Python 中也是作为一个字符串使用。
Python 访问子字符串，可以使用方括号来截取字符串，如下实例：
```python
#!/usr/bin/python3
 
var1 = 'Hello World!'
var2 = "Runoob"
 
print ("var1[0]: ", var1[0])
print ("var2[1:5]: ", var2[1:5])
```
以上实例执行结果：
```python
var1[0]:  H
var2[1:5]:  unoo
```
## 字符串更新
你可以截取字符串的一部分并与其他字段拼接，如下实例：
```python
#!/usr/bin/python3
 
var1 = 'Hello World!'
 
print ("已更新字符串 : ", var1[:6] + 'Runoob!')
```
以上实例执行结果：
```python
已更新字符串 :  Hello Runoob!
```
## 转义字符
在需要在字符中使用特殊字符时，python用反斜杠  \  转义字符。如下表：
|转义字符|描述|
|----|----|
|\\|反斜杠符号|
|\'|单引号|
|\"|双引号|
|\a|响铃|
|\b|退格(Backspace)|
|\e|转义|
|\000|空|
|\n|换行|
|\v|纵向制表符|
|\t|横向制表符|
|\r|回车|
|\f|换页|
|\oyy|八进制数，yy代表的字符，例如：\o12代表换行|
|\xyy|十六进制数，yy代表的字符，例如：\x0a代表换行|
|\other|其它的字符以普通格式输出|
## 字符串运算符
下表实例变量a值为字符串 "Hello"，b变量值为 "Python"：
|操作符|描述|实例|
|----|----|----|
|+|字符串连接|a + b 输出结果： HelloPython|
|*|重复输出字符串|a*2 输出结果：HelloHello|
|[]|通过索引获取字符串中字符|a[1] 输出结果 **e**|
|[ : ]|截取字符串中的一部分，遵循**左闭右开**原则，str[0,2] 是不包含第 3 个字符的。|a[1:4] 输出结果 **ell**|
|in|成员运算符 - 如果字符串中包含给定的字符返回 True|**'H' in a** 输出结果 True|
|not in|成员运算符 - 如果字符串中不包含给定的字符返回 True|**'M' not in a** 输出结果 True|
|%|格式字符串|请看下一节内容。|
|r/R|原始字符串 - 原始字符串：所有的字符串都是直接按照字面的意思来使用，没有转义特殊或不能打印的字符。 原始字符串除在字符串的第一个引号前加上字母 **r**（可以大小写）以外，与普通字符串有着几乎完全相同的语法。|print( r'\n' )			print( R'\n' )|
```python
#!/usr/bin/python3
 
a = "Hello"
b = "Python"
 
print("a + b 输出结果：", a + b)
print("a * 2 输出结果：", a * 2)
print("a[1] 输出结果：", a[1])
print("a[1:4] 输出结果：", a[1:4])
 
if( "H" in a) :
    print("H 在变量 a 中")
else :
    print("H 不在变量 a 中")
 
if( "M" not in a) :
    print("M 不在变量 a 中")
else :
    print("M 在变量 a 中")
 
print (r'\n')
print (R'\n')
```
以上实例输出结果为：
```python
a + b 输出结果： HelloPython
a * 2 输出结果： HelloHello
a[1] 输出结果： e
a[1:4] 输出结果： ell
H 在变量 a 中
M 不在变量 a 中
\n
\n
```
## 字符串格式化
Python 支持格式化字符串的输出 。尽管这样可能会用到非常复杂的表达式，但最基本的用法是将一个值插入到一个有字符串格式符 %s 的字符串中。
在 Python 中，字符串格式化使用与 C 中 sprintf 函数一样的语法。
```python
#!/usr/bin/python3
 
print ("我叫 %s 今年 %d 岁!" % ('小明', 10))
```
以上实例输出结果：
```python
我叫 小明 今年 10 岁!
```
python字符串格式化符号:
|符号|描述|
|----|----|
|%c|格式化字符及其ASCII码|
|%s|格式化字符串|
|%d|格式化整数|
|%u|格式化无符号整型|
|%o|格式化无符号八进制数|
|%x|格式化无符号十六进制数|
|%X|格式化无符号十六进制数（大写）|
|%f|格式化浮点数字，可指定小数点后的精度|
|%e|用科学计数法格式化浮点数|
|%E|作用同%e，用科学计数法格式化浮点数|
|%g|%f和%e的简写|
|%G|%f 和 %E 的简写|
|%p|用十六进制数格式化变量的地址|
格式化操作符辅助指令:
|符号|功能|
|----|----|
|*|定义宽度或者小数点精度|
|-|用做左对齐|
|+|在正数前面显示加号( + )|
|<sp>|在正数前面显示空格|
|#|在八进制数前面显示零('0')，在十六进制前面显示'0x'或者'0X'(取决于用的是'x'还是'X')|
|0|显示的数字前面填充'0'而不是默认的空格|
|%|'%%'输出一个单一的'%'|
|(var)|映射变量(字典参数)|
|m.n.|m 是显示的最小总宽度,n 是小数点后的位数(如果可用的话)|
## 三引号
python三引号允许一个字符串跨多行，字符串中可以包含换行符、制表符以及其他特殊字符。
实例如下：
```python
#!/usr/bin/python3
 
para_str = """这是一个多行字符串的实例
多行字符串可以使用制表符
TAB ( \t )。
也可以使用换行符 [ \n ]。
"""
print (para_str)
```
以上实例执行结果为：
```python
这是一个多行字符串的实例
多行字符串可以使用制表符
TAB (    )。
也可以使用换行符 [ 
 ]。
```
三引号让程序员从引号和特殊字符串的泥潭里面解脱出来，自始至终保持一小块字符串的格式是所谓的WYSIWYG（所见即所得）格式的。
一个典型的用例是，当你需要一块HTML或者SQL时，这时用字符串组合，特殊字符串转义将会非常的繁琐。
```python
errHTML = '''
<HTML><HEAD><TITLE>
Friends CGI Demo</TITLE></HEAD>
<BODY><H3>ERROR</H3>
<B>%s</B><P>
<FORM><INPUT TYPE=button VALUE=Back
ONCLICK="window.history.back()"></FORM>
</BODY></HTML>
'''
cursor.execute('''
CREATE TABLE users (  
login VARCHAR(8), 
uid INTEGER,
prid INTEGER)
''')
```
## Unicode 字符串
在Python2中，普通字符串是以8位ASCII码进行存储的，而Unicode字符串则存储为16位unicode字符串，这样能够表示更多的字符集。使用的语法是在字符串前面加上前缀 **u**。
在Python3中，所有的字符串都是Unicode字符串。

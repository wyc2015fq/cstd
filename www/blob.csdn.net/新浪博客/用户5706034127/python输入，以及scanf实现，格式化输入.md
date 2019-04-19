# python输入，以及scanf实现，格式化输入_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
n, m = [int(i) for i in temp.split(' ')]
n, m = map(int,raw_input().split(' '))
import sys
for line in sys.stdin:
for data in line.split(' '):
print data
import sys
arr = []
for line in sys.stdin:
arr.append([int(i) for i in line.split(' ')])
import sys
arr = []
for line in sys.stdin:
arr.append( set( line.lower.split(' ') ) )
while True:
    try:
        (x, y) = (int(x) for x in raw_input().split())
        print x + y
    except EOFError:
        break
printf：把各种类型变为字符串，这算个N->1的过程，这个只要有__str__/__repr__就是可以的，这是Explicit的scanf：把字符串变为各种类型，这是1->N的过程，这个就不Explicit了，各种异常输入都可能导致意外的事情发生，不同人对不同的异常期望不同的结果，比如字符串“1.0”转int，有人期望1，有人期望异常抛出，1.0后如果期望字符串，那如果取了1，“.0”算不算后面字符串里面的？理论上来说，输入的多样性是异常处理不过来的，如果提供，那就会有很多un-obvious的处理在里面，这个时候不验证，python的函数调用的时候又不限类型，那参数后续使用时，使用参数的函数是不是都得自己对类型负责而不能相信调用者？所以，如果要提供，只能强制格式约束，那还不如字符串split之后自己去类型转换校验，对吧>>>
import this挨条看看理解下，看这个scanf违背了多少python的方法，你就明白python了
你这么纠结不如用ctypes调用C函数吧
[https://docs.python.org/2/library/ctypes.html](https://docs.python.org/2/library/ctypes.html)
**Windows下：**
from ctypes import *
msvcrt = cdll.msvcrt
msg = "Hello world!\n"
msvcrt.printf("Testing: %s", msg)
**Linux下：**
from ctypes import *
libc = CDLL("libc.so.6")
msg = "Hello, world!\n"
libc.printf("Testing: %s", msg)
使用 Python 创建一个 C数据类型很简单，可以很容易的使用由C或者C++些的组件。 下面这张图很好的表示了映射关系。
在Python里，没有与scanf（）直接等同的功能函数，因此需要格式化输入，就需要使用正则表达式的功能来实现，并且正则表达式的功能比scanf（）更加灵活，功能更加强大，下面就来列出一些等同的表达：
|scanf()格式字符串|正则表达式|
|----|----|
|%c|.|
|\|.{5}|
|%d|[-+]?/d+|
|%e，%E，%f，%g|[-+]?(/d+(/.d*)?|/./d+)([eE][-+]?/d+)?|
|%i|[-+]?(0[xX][/dA-Fa-f]+|0[0-7]*|/d+)|
|%o|[-+]?[0-7]+|
|%s|/S+|
|%u|/d+|
|%x，%X|[-+]?(0[xX])?[/dA-Fa-f]+|
输入一个字符串的例子：
/usr/sbin/sendmail - 0 errors, 4 warnings
对于上面格式的字符串，如果使用C函数scanf（）来输入，需要使用下面的格式来实现：
%s - %d errors, %d warnings
如果我们使用正则表达式来表示，如下：
(/S+) - (/d+) errors, (/d+) warnings
例子：
print('scanf()')
pattern = re.compile(r"(/S+) - (/d+) errors, (/d+) warnings")
match = pattern.match('/usr/sbin/sendmail - 0 errors, 4 warnings')
if match:
print(match.groups())
结果输出如下：
scanf()
('/usr/sbin/sendmail', '0', '4')
%c的例子：
print('scanf() %c')
pattern = re.compile(r".")
match = pattern.match('this is for test/n')
if match:
print(match.group())
结果输出如下：
scanf() %c
t
\的例子：
print('scanf() \')
pattern = re.compile(r".{5}")
match = pattern.match('this is for test/n')
if match:
print(match.group())
结果输出如下：
scanf() \
this 
%e, %E, %f, %g的例子：
print('scanf() %e, %E, %f, %g')
pattern = re.compile(r"[-+]?(/d+(/./d*)?|/./d+)([eE][-+]?/d+)?")
match = pattern.match('+200.3721/n')
if match:
print(match.group())
match = pattern.match('x9876/n')
if match:
print(match.group())#不匹配没有输出
结果输出如下：
scanf() %e, %E, %f, %g
+200.3721
%i的例子：
print('scanf() %i')
pattern = re.compile(r"[-+]?(0[xX][/dA-Fa-f]+|0[0-7]*|/d+)")
match = pattern.match('0xAA55/n')
if match:
print(match.group())
match = pattern.match('234.56/n')
if match:
print(match.group())
结果输出如下：
scanf() %i
0xAA55
234
八进制的%o的例子：
print('scanf() %o')
pattern = re.compile(r"[-+]?[0-7]+")
match = pattern.match('0756/n')
if match:
print(match.group())
match = pattern.match('898/n')
if match:
print(match.group())#不匹配没有输出
结果输出如下：
scanf() %o
0756
字符串%s的例子：
print('scanf() %s')
pattern = re.compile(r"/S+")
match = pattern.match('深圳是一个小渔村/n')
if match:
print(match.group())
match = pattern.match('898/n')
if match:
print(match.group())
结果输出如下：
scanf() %s
深圳是一个小渔村
898
%u的例子：
print('scanf() %u')
pattern = re.compile(r"/d+")
match = pattern.match('756/n')
if match:
print(match.group())
match = pattern.match('-898/n')
if match:
print(match.group())#不匹配没有输出
结果输出如下：
scanf() %u
756
十六进制%x, %X的例子：
print('scanf() %x %X')
pattern = re.compile(r"[-+]?(0[xX])[/dA-Fa-f]+")
match = pattern.match('0x756/n')
if match:
print(match.group())
match = pattern.match('-898/n')
if match:
print(match.group())#不匹配没有输出
结果输出如下：
scanf() %x %X
0x756

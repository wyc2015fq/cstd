# python 变量作用域，sys.stdin重定向_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
python 变量作用域，sys.stdin，

##--------two--------
#coding:utf-8
def foo():
print raw_input()
print raw_input()
def bar():
 import sys
tmp_sys_stdin = sys.stdin
sys.stdin = open('in.txt')
while 1:
 try:
 foo()
raw_input()  # 各个样例之间有一行空格
 except:
break
sys.stdin = open('in.txt')
 sys.stdin.close()
sys.stdin = tmp_sys_stdin
bar()
try:
 print sys
except Exception,a:
 print Exception,a
##--------three--------
import two
print raw_input()
c里面是按照代码块来分的，严格按照向前绑定！！for while等都是代码块。
python的作用域是按照函数体，模块，类体来分的。
在two的bar函数中import sys，相当于将sys变量引入到bar函数体内，
在bar函数外部是引用不了的。所以print sys会抛出异常。但是修改sys.stdin会影响这个系统，也就是会影响，three里面的raw_input函数所以，无论是哪里修改了sys.stdin一定要修改回来！！！！但是你sys = 123就不会有什么影响，因为sys只是指向了sys模块而已，修改sys只是sys变量指向了新的对象。
# hi.py
Q = 'whoosh'
def tzl():   print Q
# test.py
from hi import *
Q = 'go'
tzl() # 打印的还是'whoosh'，因为就像闭包一样，tzl中的Q在定义的时候被绑定，到hi.Q上。而不是test.Q
|NameError|尝试访问一个没有申明的变量|
|----|----|
|ZeroDivisionError|除数为0|
|SyntaxError|语法错误|
|IndexError|索引超出序列范围|
|KeyError|请求一个不存在的字典关键字|
|IOError|输入输出错误（比如你要读的文件不存在）|
|AttributeError|尝试访问未知的对象属性|
|ValueError|传给函数的参数类型不正确，比如给int()函数传入字符串形|

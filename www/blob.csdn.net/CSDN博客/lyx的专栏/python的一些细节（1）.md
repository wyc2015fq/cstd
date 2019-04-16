# python的一些细节（1） - lyx的专栏 - CSDN博客





2016年04月21日 20:39:26[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：600
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









#### 定义函数时候的默认值



```python
def  cube(x=5,y)
```


如此，则参数x的默认值是5，当条用的时候没有定义x的值，程序就会默认为5.



#### python的格式化字符



```python
‘1%c1%c%d’%（‘+’，‘=’，2）
```


很类似与c语言中printf，不过python中就更加简化了。



#### 以大写字母R或小写字母r开头的

如

```python
r‘e:\book’
```


叫做原始字符串



#### 读取文件的三个方法

read（）整个文件读到字符串中

readline（）一行行读到字符串

readlines（）整个文件读入到列表中

#### 和c语言中一样，python中也可以使用全局关键字 global

#### lambda匿名函数

lanmbda的使用格式是这样的：lambda 参数列表：表达式

例如



```python
fun = lambda x：x*x-x
```
则fun（3）的值是6.

lambda的函数表达式中也可以包含别的函数，但是不能有判断语句之类的，也就是只能是单一表达式。

#### .py与.pyc的区别:

前者是脚本文件，后者是字节码形式的文件










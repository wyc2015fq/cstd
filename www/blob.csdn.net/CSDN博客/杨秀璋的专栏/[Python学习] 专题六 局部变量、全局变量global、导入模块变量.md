
# [Python学习] 专题六.局部变量、全局变量global、导入模块变量 - 杨秀璋的专栏 - CSDN博客

2015年09月27日 16:21:06[Eastmount](https://me.csdn.net/Eastmount)阅读数：16851标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[全局变量																](https://so.csdn.net/so/search/s.do?q=全局变量&t=blog)[局部变量																](https://so.csdn.net/so/search/s.do?q=局部变量&t=blog)[global																](https://so.csdn.net/so/search/s.do?q=global&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=局部变量&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=全局变量&t=blog)个人分类：[Python基础知识																](https://blog.csdn.net/Eastmount/article/category/2547623)
[
																					](https://so.csdn.net/so/search/s.do?q=全局变量&t=blog)所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)[
							](https://so.csdn.net/so/search/s.do?q=全局变量&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=python&t=blog)


定义在函数内的变量有局部作用域，在一个模块中最高级别的变量有全局作用域。本文主要讲述全局变量、局部变量和导入模块变量的方法。
参考：《Python核心编程 (第二版)》
## 一. 局部变量
声明适用的程序的范围被称为了声明的作用域。在一个过程中，如果名字在过程的声明之内，它的出现即为过程的局部变量；否则出现即为非局部。例：

```python
def foo(x):
    print 'x = ',x
    x = 200
    print 'Changed in foo(), x = ',x
    
x = 100
foo(x)
print 'x = ',x
```
输出结果如下：
```python
>>> 
x =  100
Changed in foo(), x =  200
x =  100
```
在主块中定义x=100，Python使用函数声明的形参传递x至foo()函数。foo()中把x赋值为200，x是函数的局部变量；所以在函数内改变x的值，主块中定义的x不受影响。
核心笔记：
当搜索一个标识符时，Python先从局部作用域开始搜索。如果在局部作用域内没有找到那个名字，那么一定会在全局域找到这个变量，否则会被抛出NameError异常。
作用域的概念和用于找到变量的名称空间搜索顺序相关。当一个函数执行时，所有在局部命名空间的名字都在局部作用域内；当查找一个变量时，第一个被搜索的名称空间，如果没有找到那个变量，那么就可能找到同名的局部变量。

## 二. 全局变量
全局变量的一个特征是除非删除掉，否则它们存活到脚本运行结束，且对于所有的函数，它们的值都是可以被访问的。然而局部变量，就像它们存放的栈，暂时地存在，仅仅只依赖于定义它们的函数现阶段是否处于活动。当一个函数调用出现时，其局部变量就进入声明它们的作用域。在那一刻，一个新的局部变量名为那个对象创建了，一旦函数完成，框架被释放，变量将会离开作用域。
```python
X = 100
def foo():
    global X
    print 'foo() x = ',X
    X = X + 5
    print 'Changed in foo(), x = ',X
def fun():
    global X
    print 'fun() x = ',X
    X = X + 1
    print 'Changed in fun(), x = ',X
    
if __name__ == '__main__':
    foo()
    fun()
    print 'Result x = ',X
```
输出结果如下：
```python
>>> 
foo() x =  100
Changed in foo(), x =  105
fun() x =  105
Changed in fun(), x =  106
Result x =  106
```
核心笔记：
使用global语句定义全局变量。当使用全局变量同名的局部变量时要小心，如果将全局变量的名字声明在一个函数体内，全局变量的名字能被局部变量给覆盖掉。所以，你应该尽量添加global语句，否则会使得程序的读者不清楚这个变量在哪里定义的。
你可以使用同一个global语句指定多个全局变量。例如global x, y, z。
当我在制作Python爬虫时，需要想函数中传递url，循环爬取每个url页面的InfoBox，此时的文件写入操作就可以有两种方法实现：1.通过传递参数file；2.通过定义全局变量file。
```python
SOURCE = open("F:\\test.txt",'w')
def writeInfo(i):
    global SOURCE
    SOURCE.write('number'+str(i)+'\n')
def main():
    i=0
    while i<50:
        writeInfo(i)
        print i
        i=i+1
    else:
        print 'End'
    SOURCE.close()
main()
```
**PS：**在此种用法中，如果我们在函数writeInfo()中不使用global 声明全局变量SOURCE，其实也可以使用，但是此时应该是作为一个内部变量使用，由于没有初始值，因此报错。Python查找变量是顺序是：先局部变量，再全局变量。
```python
UnboundLocalError: local variable 'SOURCE' referenced before assignment
```

## 三. 模块导入变量
主要方法是通过在py文件中模块定义好变量，然后通过import导入全局变量并使用。例：

```python
import global_abc
def foo():
    print global_abc.GLOBAL_A
    print global_abc.GLOBAL_B
    print global_abc.GLOBAL_C
    global_abc.GLOBAL_C = global_abc.GLOBAL_C + 200
    print global_abc.GLOBAL_C
    
if __name__ == '__main__':
    foo()
    print global_abc.GLOBAL_A + ' ' + global_abc.GLOBAL_B
    print global_abc.GLOBAL_C
```
输出如下所示，全局变量结构是可以改变的。
```python
>>> 
hello
world
300
500
hello world
500
```
截图如下所示：
![](https://img-blog.csdn.net/20150927161648858)
**PS：**应该尽量避免使用全局变量。不同的模块都可以自由的访问全局变量，可能会导致全局变量的不可预知性。对全局变量，如果程序员甲修改了_a的值，程序员乙同时也要使用_a，这时可能导致程序中的错误。这种错误是很难发现和更正的。同时，全局变量降低了函数或模块之间的通用性，不同的函数或模块都要依赖于全局变量。同样，全局变量降低了代码的可读性，阅读者可能并不知道调用的某个变量是全局变量，但某些情况不可避免的需要使用它。
最后关于闭包和Lambda(相当于函数)就不再介绍，希望文章对你有所帮助~同时今天也是中秋节，祝所有程序猿和读者中秋节快乐。
(By:Eastmount 2015-9-27 下午4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



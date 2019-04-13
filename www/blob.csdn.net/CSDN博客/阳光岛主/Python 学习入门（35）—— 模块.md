
# Python 学习入门（35）—— 模块 - 阳光岛主 - CSDN博客

2013年12月22日 01:26:11[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：23685所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**模块简介**
python是由一系列的模块组成的，每个模块就是一个py为后缀的文件，同时模块也是一个命名空间，从而避免了变量名称冲突的问题。模块我们就可以理解为lib库，如果需要使用某个模块中的函数或对象，则要导入这个模块才可以使用，除了系统默认的模块（[内置函数](http://blog.csdn.net/ithomer/article/details/17417569)）不需要导入外。
导入直接使用如下语法：**import**模块名称(不要.py后缀)
这样导入成功后，就可以在另外一个模块中使用被导入模块的功能函数或对象。
举例如下，比如在F:\Pro-files\workspace\myPython目录下建立com.homer.python模块：module_1.py：

```python
# -*- coding:utf-8 -*-
'''module_1.py'''
name = "ithomer"
url = "http://blog.ithomer.net"
```
上面第一行是指定编码格式，因为python默认是按照ascii编码来处理的，因此就无法处理非英文语言，通过指定编码就可以实现国际化效果，即容许中文注释。第二行是注释信息，使用”’进行注释。然后，我们通过F:\Pro-files\workspace\myPython\com\homer目录进入到python的命令行模式，这样就可以将当前目录作为工作目录，从而也就可以顺利的找到了module_1这个模块了，如下：

```python
:\Pro-files\workspace\myPython\com\homer>python
Python 3.3.3 (v3.3.3:c3896275c0f6, Nov 18 2013, 21:19:30) [MSC v.1600 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
```
此时如果我们直接输入print(url)，系统则会报错，url变量未定义：

```python
>>> print(url)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'url' is not defined
>>>
```
所以我们需要先导入module_1的内容才算定义了url变量，不过要注意的是导入的模块的变量并不是定义在顶层命名空间，而是在模块的命名空间中，因此使用如下方式导入后的打印变量如下：

```python
>>> import module_1
>>> print(module_1.url)
http://blog.ithomer.net
```
如果直接使用print(url)，仍然还是会报url未定义的错误，原因就是上面说到的命名空间的问题。如果想要直接在顶层命名空间中使用该变量，可以使用如下导入方式：

```python
>>> from module_1 import url
>>> print(url)
http://blog.ithomer.net
```
这样就可以将module_1模块中的url变量导入到顶层命名空间了，直接使用变量也就不会报错了。当然我们也可以为导入的变量重命名，如下把url重命名为myurl：

```python
>>> from module_1 import url as myurl
>>> print(myurl)
http://blog.ithomer.net
```
这样就可以将url的值赋给myurl这个变量了，因为是使用了from…import语句，因此是将变量绑定在顶层命名空间，我们也就可以直接使用变量名了。
需要注意的是无论是使用import还是使用from..import的方式导入模块，其实都是在告诉python解释器要加载指定的模块，并执行模块中的所有语句，因此如果模块中有类似print的语句时，我们在导入的过程中也会看到这些语句的输出。
对于每个模块的导入，python解释器只会导入一次，即使重复使用import和from…import语句，也只有在PVM检测到该模块没有被导入时才执行导入动作。即使后来你修改了模块的源代码，但没有重启PVM，python解释器仍然是使用之前导入的内容在处理。如果需要重新载入修改后的源码，一是退出python的交互模式后再进入，二是直接使用reload语句，如下：

```python
>>> from imp import reload
>>> reload(module_1)
<module 'module_1' from '.\\module_1.py'>
```
我们可以看到系统提示重新加载了module_1模块的源文件，我们修改内容后使用该方法重新导入后再执行就可以看到修改后的内容。
**模块导入和执行**

模块作为python语言中的基本单元，可以用来编写公用库函数或对象以便重复使用。同时模块还可以作为独立文件进行运行，之前也已经提到，只要是导入了模块文件，那么PVM就会依次执行模块文件中的所有语句。本篇主要介绍模块使用的一些进阶，首先定义一个模块module_2.py，内容如下：

```python
# -*- encoding:utf-8 -*-
'''module_2.py模块的内容'''
 
print(__name__)
 
def sum(a,b):
    return a+b
 
if __name__ == "__main__":
    import sys
    print(sys.argv[0])
    a = int(sys.argv[1])
    b = int(sys.argv[2])
    print(sum(a,b))
```
上面的这段代码基本上就包含了本篇文章要说的内容了：
### __name__变量说明
__name__是一个全局变量，在模块内部是用来标识模块的名称的。上面的例子中有一个打印__name__变量的语句，比如我们在python的交互模式下执行导入可以看到如下结果：
>>>import module_2
module_2

可以看到，通过使用import的方式导入模块后，打印的__name__值就是我们刚才说的模块的名称。另外如果是通过python解释器直接执行模块，则__name__会被设置为__main__这个字符串值，如下：

```python
F:\Pro-files\workspace\myPython\com\homer>python module_2.py 2 3
__main__
module_2.py
5
```
我们通过windows命令行直接执行module_2这个模块文件，可以看到的结果输入如上，打印的__name__值为__main__。通过这个特性我们可以将一个模块文件既当做普通的lib库供其他模块使用，又可以当做一个顶层执行文件进行执行，只是使用方式不一样而已。
1、当成lib库使用时，只需要在其他模块中使用import导入该模块即可（module_2）
2、当成执行模块时，通过python解释器直接运行该模块，然后在模块文件中最后写上上面例子中的if判断语句段即可（__main__）
通过__name__变量来区分是执行模块还是导入模块其实就非常类似java中的main函数了，不同的是java中是约定的方法名称，而python中约定是变量名称，异曲同工之效。

### 参数传递
参数传递主要是指作为执行模块时需要传递的参数，通过python解释器执行某一个执行模块传递参数的使用如下：
F:\Pro-files\workspace\myPython\com\homer>python 模块名称(包含.py后缀)参数1  参数2  参数3 ....
对于上面给定的例子执行就是：

```python
F:\Pro-files\workspace\myPython\com\homer>python module_2.py 2 3
__main__
module_2.py
5
```
python解释器会将所有传递的参数存储在sys.argv这个列表中，所有的参数都被当成字符串进行处理。同时即使不传递任何参数，也会有一个默认的参数sys.argv[0]标识当前模块的名称（如module_2.py），所以我们自己使用的参数都是从sys.argv[1]开始的，下标1表示第一个传递的参数，依次类推（如 2 3）。
**模块包和搜索路径**
一个python文件就是一个模块，使用独立的命名空间，但实际使用过程中单单用模块来定义python功能显然还不够。因为一个大型的系统几千上万个模块是很正常的事情，如果都聚集在一起显然不好管理并且有命名冲突的可能，因此python中也出现了一个包的概念。
**包**（package），通过使用“点模块名称”创建Python模块命名空间的一种方法。例如，模块名称 A.B 表示一个在名为 A的包下的名为B的子模块。就像使用模块让不同模块的作者无需担心彼此全局变量名称（冲突）一样，点模块名称让多模块包的作者无需担心彼此的模块名称（冲突）。模块包在文件系统中的表示就是一系列目录的集合，通过目录的层级结构形成模块包的层级结构，最终的模块文件就位于最后的目录中。比如定义一个简单的模块在包pkg下，那么执行如下步骤：
1、在F:\Pro-files\workspace\目录下建立一个目录，名称是myPython
2、在myPython下建立包com.homer，并在项目目录myPython/com/homer下com和com.homer下分别创建__init__.py的模块，内容为空即可
3、在myPython/com/homer目录下建立一个python模块module_3.py，内容如下：

```python
# -*- encoding:utf-8 -*-
'''pkg.module_3.py模块的内容'''
 
print("Hello World")
 
def func1():
    print("This is funciton one")
```
注意第2步，必须要在每一个包目录下建立一个__init__.py的模块，这个是python的规定，用来告诉python解释器将该目录当成一个内容包，即该目录是一个包，里面包含了python模块的。这个是必须的，如果不指定，则我们在python的交互模式下导入module_3.py这个模块时会报如下错误：

```python
>>> from com.homer import module_3
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: No module named pkg.module_3
```
因此，包目录下的__init__.py模块是必须的，但内容是可选的，可以为空内容，也可以写一些代码或作其他用途。PVM在导入某个包下的模块时会先导入这个包下的__init__.py模块，比如我们在__init__.py模块里添加内容：
print("This is __init__ module")
然后，在交互模式下重新导入这个包，则输出效果如下：

```python
>>> from com.homer import module_3
This is __init__ module
Hello World
>>>
```
可见，PVM首先加载的是__init__.py模块，然后才是找该目录下的其他模块并进行加载。

### python中的模块搜索路径
在一个模块被导入时，PVM会在后台从一系列路径中搜索该模块，其搜索过程如下：
1、在当前目录下搜索该模块；
2、在环境变量PYTHONPATH中指定的路径列表中依次搜索；
3、在python安装路径中搜索
事实上，PVM通过变量sys.path中包含的路径来搜索，这个变量里面包含的路径列表就是上面提到的这些路径信息，我们可以打印看下sys.pth都包含些哪些路径：

```python
>>> import sys
>>> print(sys.path)
['', 'C:\\Windows\\system32\\python33.zip', 'F:\\Pro-files\\Python33\\DLLs', 'F:\\Pro-files\\Python33\\lib', 'F:\\Pro-files\\Python33', 'F:\\Pro-files
\\Python33\\lib\\site-packages']
>>>
```
不同的机器上显示的路径信息可能不一样，但至少都包含上面提到的3点。知道了这个路径搜索规律后，我们就可以很方便的将某些目录动态的增加到搜索路径中去，比如在E盘下建立一个python模块module_4.py，内容如下：

```python
# -*- coding:utf-8 -*-
'''pkg.module_4.py'''
 
print("Hello world")
print(2 ** 2)
```
然后在交互模式下执行导入：

```python
>>> import module_4
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: No module named module_4
```
报错是预料中的，因为F盘并不在python模块的搜索路径中，然后我们动态的增加这个路径到搜索模块中，再执行导入：

```python
>>> import sys
>>> sys.path.append("F:\Pro-files\workspace\myPython")
>>> print(sys.path)
['', 'C:\\Windows\\system32\\python33.zip', 'F:\\Pro-files\\Python33\\DLLs', 'F:\\Pro-files\\Python33\\lib', 'F:\\Pro-files\\Python33', 'F:\\Pro-files
\\Python33\\lib\\site-packages', 'F:\\Pro-files\\workspace\\myPython']
>>> from com.homer import module_4
This is __init__ module
Hello world
4
>>>
```
首先是在sys.path中增加了F盘根目录作为搜索路径sys.path.append("F:\Pro-files\workspace\myPython")，随后的打印中可以看到确实已经被添加到sys.path中去了，然后再执行导入就会正常导入模块并执行模块中的语句了。当然，我们通过交互模式新增加的搜索路径也仅仅是在当前交互模式下有效，一旦退出了那么就就失效了。因此，我们可以根据搜索路径规则的第2步中说的来设置PYTHONPATH环境变量就可以满足不同使用情况下都可以找到模块了。


**参考推荐：**
[Python学习笔记-模块介绍](http://www.mzone.cc/article/434.html)
[Python 学习入门——常用类库](http://blog.csdn.net/ithomer/article/details/13022087)




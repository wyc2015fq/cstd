# 增加python引用库 sys.path.append() - YZXnuaa的博客 - CSDN博客
2018年07月11日 14:59:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：385
记录下：python 的 sys.path.append()
当我们导入一个模块时：import  xxx，默认情况下python解析器会搜索当前目录、已安装的内置模块和第三方模块，搜索路径存放在sys模块的path中：
```python
```
- 
>>> import sys
- 
>>> sys.path
- 
['', 'C:\\Python352\\Lib\\idlelib', 'C:\\Python352\\python35.zip', 'C:\\Python352\\DLLs', 'C:\\Python352\\lib', 'C:\\Python352', 'C:\\Python352\\lib\\site-packages', 'C:\\Python352\\lib\\site-packages\\setuptools-28.6.1-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\pip-8.1.2-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\requests-2.11.1-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\xlutils-2.0.0-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\xlwt-1.1.2-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\pymongo-3.3.1-py3.5-win-amd64.egg', 'C:\\Python352\\lib\\site-packages\\pytz-2016.7-py3.5.egg', 'C:\\Python352\\lib\\site-packages\\zope.interface-4.3.3-py3.5-win-amd64.egg']
sys.path 返回的是一个列表！
该路径已经添加到系统的环境变量了，当我们要添加自己的搜索目录时，可以通过列表的append()方法；
对于模块和自己写的脚本不在同一个目录下，在脚本开头加sys.path.append('xxx')：
```python
```
- 
import sys
- 
sys.path.append(’引用模块的地址')
这种方法是运行时修改，脚本运行后就会失效的。
另外一种方法是：
把路径添加到系统的环境变量，或把该路径的文件夹放进已经添加到系统环境变量的路径内。环境变量的内容会自动添加到模块搜索路径中。
原文：http://www.tuicool.com/articles/EB3miqr
sys模块包含了与python解释器和它的环境有关的函数，这个你可以通过dir(sys)来查看他里面的方法和成员属性。
下面的两个方法可以将模块路径加到当前模块扫描的路径里：
sys.path.append('你的模块的名称')。
sys.path.insert(0,'模块的名称')
永久添加路径到sys.path中，方式有三，如下：
1）将写好的py文件放到 已经添加到系统环境变量的 目录下 ；
2) 在 /usr/lib/python2.6/site-packages 下面新建一个.pth 文件(以pth作为后缀名) 
将模块的路径写进去，一行一个路径，如： vim pythonmodule.pth
/home/liu/shell/config
/home/liu/shell/base 
3) 使用PYTHONPATH环境变量
export PYTHONPATH=$PYTHONPATH:/home/liu/shell/config

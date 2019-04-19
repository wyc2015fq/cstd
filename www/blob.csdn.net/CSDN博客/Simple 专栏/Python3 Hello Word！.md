# Python3  Hello Word！ - Simple 专栏 - CSDN博客
2018年10月04日 20:20:36[Simple_Zz](https://me.csdn.net/love284969214)阅读数：194
**Python语言有许多C和Java的相似之处。不过，语言之间也有一些明显的差异。**
交互模式编程
```bash
[root@hadoop1 /]#python3
Python 3.7.0 (default, Oct  4 2018, 19:53:56) 
[GCC 4.4.7 20120313 (Red Hat 4.4.7-23)] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> print("Hello word!")
Hello word!
>>>
```
脚本编程模式
```bash
[root@hadoop1 /]#echo 'print("Hello,Word!")' > pythonTest.py
[root@hadoop1 /]#python pythonTest.py 
Hello,Word!
[root@hadoop1 /]#
```
shell脚本编程
在Linux系统中，你可以在脚本顶部添加以下命令让Python脚本可以像SHELL脚本一样可直接执行：
```python
#! /usr/bin/env python3
```
然后修改脚本权限，使其有执行权限，命令如下：
```python
chmod +x pythonTest.py
```
执行以下命令：
```python
./pythonTest.py
```
输出结果为：
```python
Hello, Python!
```

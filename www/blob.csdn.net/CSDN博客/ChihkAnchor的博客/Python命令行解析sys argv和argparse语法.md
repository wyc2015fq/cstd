# Python命令行解析sys.argv和argparse语法 - ChihkAnchor的博客 - CSDN博客





2019年01月09日 15:44:53[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：544








python中的命令行解析最简单最原始的方法是使用sys.argv来实现，Sys.argv[ ]

创建一个名为1.py的文件，并写入如下内容：

```python
import sys #导入sys模块
print(sys.argv[0]) # 打印sys.argv的第0个参数
```

在控制台下运行这个文件

```bash
$ python 1.py b c d e f 
1.py
```

 由此，sys.argv 返回脚本本身的名字及给定脚本的参数列表，里边的项为用户输入的参数。了解shell脚本编程的可以这样理解这些参数,sys.argv[0] 就是shell 脚本中的basename ,而sys.argv[1]....就是shell中的$1,$2，再如：

```python
import sys #导入sys模块
print(sys.argv[1:5:2])
print(sys.argv[-1])
print(sys.argv[::-1])
```

```bash
$ python 1.py a b c d e f 
 ['a', 'c']
 f
 ['f', 'e', 'd', 'c', 'b', 'a', '1.py']
```

###  argparse

更高级的可以使用argparse这个模块。argparse从python 2.7开始被加入到标准库中，所以如果你的python版本还在2.7以下，那么需要先手动安装。

**1、导入argparse模块**

**2、创建解析器对象ArgumentParser，可以添加参数。**

description：描述程序

parser=argparse.ArgumentParser(description="This is a example program ")

add_help：默认是True，可以设置False禁用

**3、add_argument()方法，用来指定程序需要接受的命令参数**

定位参数：

parser.add_argument("echo",help="echo the string")

可选参数：

parser.add_argument("--verbosity", help="increase output verbosity")

在执行程序的时候，定位参数必选，可选参数可选。

add_argument()常用的参数：

dest：如果提供dest，例如dest="a"，那么可以通过args.a访问该参数

default：设置参数的默认值

action：参数出发的动作

store：保存参数，默认

store_const：保存一个被定义为参数规格一部分的值（常量），而不是一个来自参数解析而来的值。

store_ture/store_false：保存相应的布尔值

append：将值保存在一个列表中。

append_const：将一个定义在参数规格中的值（常量）保存在一个列表中。

count：参数出现的次数

        parser.add_argument("-v", "--verbosity", action="count", default=0, help="increase output verbosity")

version：打印程序版本信息

type：把从命令行输入的结果转成设置的类型

choice：允许的参数值

        parser.add_argument("-v", "--verbosity", type=int, choices=[0, 1, 2], help="increase output verbosity")

help：参数命令的介绍

创建名为1.py的文件并输入如下内容：

```python
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("square", help="display a square of a given number")
args = parser.parse_args()
print(int(args.square)**2)
```

```bash
$ python 1.py 4
16
```

创建名为2.py的文件并输入如下内容 ：

```python
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbosity", help="increase output verbosity")
args = parser.parse_args()
if args.verbosity:
    print "verbosity turned on"
```

```bash
$ python 2.py -v 1
verbosity turned on
$ python 2.py -v
usage: 2.py [-h] [-v VERBOSITY]
2.py: error: argument -v/--verbosity: expected one argument
$ python 2.py --verbosity 1
verbosity turned on
```

 如果用省略方法 -v 赋值的方法，最终会到达 --verbosity 值会传递到这个完整的参数中去。这里值得一提的是，如果没有后面的 --verbosity 只有 -v 的话，那么值会可以通过 args.v 得到




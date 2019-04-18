# python：argparse - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:45[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：211

argparse是python的命令行解析工具，或者说可以在python代码中调用shell的一些命令，从而简化和系统命令之间的交互。tensorflow的一些例子中用argparse来定义一些默认命令，通常是全局变量，也是用作和系统命令之间交互的全局设置。具体编程时，argparse包的使用方法如下
### step 1、导入argparse模块
import argparse
### step 2、创建解析器对象ArgumentParser，可以添加参数。
description：描述程序
parser=argparse.ArgumentParser(description="This is a example program ")
add_help：默认是True，可以设置False禁用
#### step 3、add_argument()方法，用来指定程序需要接受的命令参数
- 定位参数：
parser.add_argument("echo",help="echo the string")
- 可选参数：
parser.add_argument("--verbosity", help="increase output verbosity")
在执行程序的时候，定位参数必选，可选参数可选。
add_argument()常用的参数：
dest：如果提供dest，例如dest="a"，那么可以通过args.a访问该参数
default：设置参数的默认值
action：参数触发的动作
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
### example: argparse_example.py
```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--echo", type=str，help="echo the string you use here")
parser.add_argument("--square", type=int, help="display a square of a given number")
args = parser.parse_args()
print(args.echo)
print(args.square**2)
```
这里第一个参数调用了系统的echo工具，将函数名称后的字符串打印在控制台显示。第二个参数做了平方运算。运行：
`python argparse_example.py --echo ‘hello!’ --square 4`
返回
hello!
16
**注意：parser.add_argument函数中第一个参数"--echo"和“echo”很不一样，前者是可选参数，后者是定位参数，也就是说，如果是"--echo"需要在调用时指定参数，“echo”不需要指定参数，按照先后顺序读入，分别赋值。这也是“定位”和“可选”参数最大的区别**
参考：[http://www.cnblogs.com/zknublx/p/6106343.html](http://www.cnblogs.com/zknublx/p/6106343.html)
官方文档：[https://docs.python.org/3.6/library/argparse.html#argparse.ArgumentParser](https://docs.python.org/3.6/library/argparse.html#argparse.ArgumentParser)
多个值的情况：[http://blog.csdn.net/guojuxia/article/details/44462807](http://blog.csdn.net/guojuxia/article/details/44462807)

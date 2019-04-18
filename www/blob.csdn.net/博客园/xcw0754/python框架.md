# python框架 - xcw0754 - 博客园
# [python框架](https://www.cnblogs.com/xcw0754/p/8723334.html)
### Argparse
[Tutorial](https://docs.python.org/2/howto/argparse.html)
argparse是参数解析工具，它能识别并解析命令行传来的参数，也能在参数不正确的时候输出帮助信息。
先来了解几个概念：
`positional` 表示位置参数，也就是必须按顺序给出的，比如mv命令就需要两个这样的参数，且规定了顺序的。
`optional` 表示可选参数，就是可有可无的都不影响程序运行的。
使用方法：
```
import argparse
parser = argparse.ArgumentParser() # 取得一个对象
parser.parse_args() # 开始分析参数
```
不过上面的代码并不能很好的工作，需要指明你的程序需要哪些参数，参数的类型，再定义些帮助信息以反馈给程序使用者。下面定义一个`positional`参数echo:
```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("echo") # here
args = parser.parse_args()
print args.echo # here
```
这段代码告诉parser我们需要一个位置参数`echo`，取的时候就是`args.echo`就可以了。打印帮助信息的结果是这样的：
```
$./test.py -h
usage: test.py [-h] echo
positional arguments:
  echo
optional arguments:
  -h, --help  show this help message and exit
```
这里echo参数是没有提示信息的，这次为它加点提示信息，可以这样：
`parser.add_argument("echo", help='echo the string you use here')`
在没有为echo指定类型的情况下，它就是string型，要指定类型可以这样：
`parser.add_argument("echo", help='echo the string you use here', type=int)`
常见的参数有带`-`的，这是简写，如`-h`。也有带`--`的，这是全拼，如`--help`。
可选参数一般可以这样：
`parser.add_argument("--verbose", help="increase output verbosity", action="store_true")`
也可以是这样：
`parser.add_argument("-t", help="specify timeout")`
还可以是这样：
`parser.add_argument("-t", "--timeout", help="specify timeout")`
第一种是只要提供`--verbose`，不需要再在后面跟true或false，提供此参数就是`true`，没有此参数就是`false`。可以这样取`args.verbose`的值。
第二种没有带`action`选项，那就需要在`-t`后面带一个参数，如果不需要指定timeout，那就`args.t`会被赋予`None`。
第三种集合了`-t`和`--timeout`，它们都表示同个参数。
有时需要限定参数的值为几个中一个，可以类似这样：
`parser.add_argument("-v", "--verbosity", type=int, choices=[0, 1, 2], help="increase output verbosity")`
有时需要知道程序执行时提供了几个同样的参数，比如tcpdump的-n和-nn，可以这样：
`parser.add_argument("-v", "--verbosity", action="count", help="increase output verbosity")`
这样的话`args.verbosity`取到的就是所提供的v或verbosity的个数，如`-vv`，则为2，`-v`则为1，`--verbosity --verbosity`则为2。
很常见的是可选参数不提供时需要有一个默认的值，而不是None，可以这样：
`parser.add_argument("-v", "--verbosity", action="count", default=0, help="increase output verbosity")`
还有其他的功能没有写出来，不是很常用，小程序没那么复杂，就暂时只记下这些。


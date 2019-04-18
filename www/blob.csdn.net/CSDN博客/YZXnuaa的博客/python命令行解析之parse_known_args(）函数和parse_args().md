# python命令行解析之parse_known_args(）函数和parse_args() - YZXnuaa的博客 - CSDN博客
2018年06月01日 13:25:11[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：234
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
在python中，命令行解析的很好用，
首先导入命令行解析模块
import argparse
import sys
然后创建对象
parse=argparse.ArgumentParser()
然后增加命令行
parse.add_argument("--learning_rate",type=float,default=0.01,help="initial learining rate")
parse.add_argument("--max_steps",type=int,default=2000,help="max")
parse.add_argument("--hidden1",type=int,default=100,help="hidden1")
对于函数add_argumen()第一个是选项，第二个是数据类型，第三个默认值，第四个是help命令时的说明
然后用arg=parse.parse_args(sys.argv[1:])
其中参数sys.argv[1:]是命令行语句中从第一个到最后。如在ubuntu下输入命令行python gg.py --learning_rate 20 --max_steps 10
则sys.argv[1:0]=--learning_rate 20 --max_steps 10
输出的arg为namespace空间，结果是Namespace(hidden1=100, learning_rate=20.0, max_steps=10)
但是parse_known_args（）函数输出结果为
te 20 --max_steps 10
20.0
10
100
程序：
import argparse
import sys
parse=argparse.ArgumentParser()
parse.add_argument("--learning_rate",type=float,default=0.01,help="initial learining rate")
parse.add_argument("--max_steps",type=int,default=2000,help="max")
parse.add_argument("--hidden1",type=int,default=100,help="hidden1")
flags,unparsed=parse.parse_known_args(sys.argv[1:])
print flags.learning_rate
print flags.max_steps
print flags.hidden1
print unparsed
输出结果：
20.0
10
100
[]
程序：
20.0
10
100
这两个函数的功能差不多            

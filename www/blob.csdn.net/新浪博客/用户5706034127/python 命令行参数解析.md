# python 命令行参数解析_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
知识点：sys.argv   getopt
本篇将介绍python中sys, getopt模块处理命令行参数
如果想对python脚本传参数，python中对应的argc, argv(c语言的命令行参数)是什么呢？
需要模块：sys
参数个数：len(sys.argv)
```
脚本名：  
 sys.argv[0]
参数1：  
  sys.argv[1]
参数2：    
sys.argv[2]
```
test.py
|```1```|```import````sys`|
|```2```|```print````"脚本名："````,sys.argv[````0``]`|
|```3```|```for````i ``in``range``(``1``, ``len``(sys.argv)):`|
|```4```|```````print``"参数"````,i, sys.argv[i]```|
>>>python test.py hello world
脚本名：test.py
参数 1 hello
参数 2 world
python中使用命令行选项：
例如我们需要一个convert.py脚本。它的作用是处理一个文件,并将处理后的结果输出到另一个文件中。
要求该脚本满足以下条件：
1.通过-i -o选项来区别参数是输入文件还是输出文件.
>>> python convert.py -i inputfile -o outputfile
2.当不知道convert.py需要哪些参数时，用-h打印出帮助信息
>>> python convert.py -h
getopt函数原形:
getopt.getopt(args,
options[, long_options])
convert.py
```
import
```
```
sys,
getopt
```
|```02```||
|```03```|```opts, args````=``getopt.getopt(sys.argv[``1``:], ``"hi:o:"``)`|
|```04```|```input_file````=``""`|
|```05```|```output_file````=``""`|
|```06```||
|```07```|```for``````op,value````in``opts:`|
|```08```|```````if``op ``=``=``"-i"``:`|
|```09```|```````input_file ``=``value`|
|```10```|```````elif``op ``=``=``"-o"``:`|
|```11```|```````output_file ``=``value`|
|```12```|```````elif``op ``=``=``"-h"``:`|
|```13```|```````usage()`|
|```14```|```````sys.exit()`|
```
代码解释：
a) sys.argv[1:]为要处理的参数列表，sys.argv[0]为脚本名，所以用sys.argv[1:]过滤掉脚本名。
b) "hi:o:":
当一个选项只是表示开关状态时，即后面不带附加参数时，在分析串中写入选项字符。当选项后面是带一个附加参数时，在分析串中写入选项字符同时后面加一个":"号。所以"hi:o:"就表示"h"是一个开关选项；"i:"和"o:"则表示后面应该带一个参数。
c) 调用getopt函数。函数返回两个列表：opts和args。opts为分析出的格式信息。args为不属于格式信息的剩余的命令行参数。opts是一个两元组的列表。每个元素为：(选项串,附加参数)。如果没有附加参数则为空串''。
```
```
getopt函数的第三个参数[,
long_options]为可选的长选项参数，上面例子中的都为短选项(如-i -o)
长选项格式举例:
--version
--file=error.txt
让一个脚本同时支持短选项和长选项
getopt.getopt(sys.argv[1:], "hi:o:", ["version",
"file="])
```

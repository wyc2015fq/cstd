# os, sys模块及其他内容 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 16:12:29[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：350标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
**sys模块**
sys模块包含系统对应的功能。我们已经学习了sys.argv列表，它包含命令行参数。
命令行参数
``
#!/usr/bin/python
# Filename: cat.py
**import****sys**
**def** readfile(filename):
'''Print a file to the standard output.'''
    f =file(filename)
**while****True**:
        line = f.readline()
**if****len**(line) ==0:
**break**
**print** line, # notice comma
    f.close()
# Script starts from here
**if len**(**sys**.argv)< 2:
**print**'No action specified.'
**sys**.exit()
**if****sys**.argv[1].startswith('--'):
    option =**sys**.argv[1][2:]
# fetch sys.argv[1] but without the first two characters
**if** option =='version':
**print**'Version 1.2'
**elif** option =='help':
**print**'''\
This program prints files to the standard output.
Any number of files can be specified.
Options include:
  --version : Prints the version number
  --help    : Display this help'''
**else**:
**print**'Unknown option.'
**sys**.exit()
**else**:
**for** filename **in****sys**.argv[1:]:
        readfile(filename)
输出
$ python cat.py
No action specified.
$ python cat.py --help
This program prints files to the standard output.
Any number of files can be specified.
Options include:
  --version : Prints the version number
  --help    : Display this help
$ python cat.py --version
Version 1.2
$ python cat.py --nonsense
Unknown option.
$ python cat.py poem.txt
Programming is fun
When the work is done
if you wanna make your work also fun:
        use Python!
函数：sys.argv[][], sys.exit()
**os模块**
**这个模块包含普遍的操作系统功能**。
如果你希望你的程序能够与平台无关的话，这个模块是尤为重要的。即它允许一个程序在编写后不需要任何改动，也不会发生任何问题，就可以在Linux和Windows下运行。一个例子就是使用os.sep可以取代操作系统特定的路径分割符。
os.name字符串指示你正在使用的平台。比如对于Windows，它是'nt'，而对于Linux/Unix用户，它是'posix'。
os.getcwd()函数得到当前工作目录，即当前Python脚本工作的目录路径。
os.getenv()和os.putenv()函数分别用来读取和设置环境变量。
//os.getenv(argv)   参数为 常用环境变量名 ‘PATH’'HOME'    可参考 "环境变量作用 "
os.listdir()返回指定目录下的所有文件和目录名。
os.remove()函数用来删除一个文件。
os.system()函数用来运行shell命令。
os.linesep字符串给出当前平台使用的行终止符。例如，Windows使用'\r\n'，Linux使用'\n'而Mac使用'\r'。
os.path.split()函数返回一个路径的目录名和文件名。
>>> os.path.split('/home/swaroop/byte/code/poem.txt')
('/home/swaroop/byte/code', 'poem.txt')
os.path.isfile()和os.path.isdir()函数分别检验给出的路径是一个文件还是目录。类似地，os.path.existe()函数用来检验给出的路径是否真地存在。
更多用法
在函数中接收元组和列表
当要使函数接收元组或字典形式的参数的时候，有一种特殊的方法，它分别使用*和**前缀。这种方法在函数需要获取可变数量的参数的时候特别有用。
``
#!/usr/bin/env python
#filename:rec_tuple.py
**def** powersum(power, *args):
    total =0
**for** i **in** args:
        total +=**pow**(i, power)
**return** total
**print** powersum(2,3, 4)
**print** powersum(2,10)
输出：
25
100
由于在args变量前有*前缀，所有多余的函数参数都会作为一个元组存储在args中。如果使用的
是**前缀，多余的参数则会被认为是一个字典的键/值对。
**lambda形式**
lambda语句被用来创建新的函数对象，并且在运行时返回它们。
``
#!/usr/bin/python
# Filename: lambda.py
**def** make_repeater(n):
**return****lambda** s: s*n
twice = make_repeater(2)
**print** twice('word')
**print** twice(5)
输出
$ python lambda.py
wordword
10
分析：使用了make_repeater函数在运行时创建新的函数对象，并且返回它。lambda语句用来创建函数对象。本质上，lambda需要一个参数(本例为 s)，后面仅跟单个表达式作为函数体 (本例为 s*n），而表达式的值被这个新建的函数返回。
**exec和eval语句**
exec语句用来执行储存在字符串中的Python语句。
exerfile()用来执行储存在文件中的Python语句。
例如  可以在运行时生成一个包含Python代码的字符串，然后使用exec语句执行这些语句。
eval语句用来计算存储在字符串中的有效Python表达式。
``
#!/usr/bin/env python
#filename:exec.py
**print**'exec:'
**exec**'print "hello world"'
**print**'execfile:'
**execfile**(r'lambda.py')
**print**'eval:'
**print****eval** ('2*3')
输出：
exec:
hello world
execfile:
wordword
10
9
81
eval：
6
**assert语句**
assert语句用来声明某个条件是真的。例如，如果你非常确信某个你使用的列表中至少有一个元素，而你想要检验这一点，并且在它非真的时候引发一个错误，那么assert语句是应用在这种情形下的理想语句。当assert语句失败的时候，会引发一个AssertionError。
>>> mylist = ['item']
>>> assert len(mylist) >= 1
>>> mylist.pop()
'item'
>>> assert len(mylist) >= 1
Traceback (most recent call last):
  File "<stdin>", line 1, in ?
AssertionError
总结完毕，欢迎各位多多交流

# python下编译py成pyc和pyo - 宇宙浪子的专栏 - CSDN博客
2015年01月06日 11:09:43[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：400标签：[python																[pyc																[pyo](https://so.csdn.net/so/search/s.do?q=pyo&t=blog)](https://so.csdn.net/so/search/s.do?q=pyc&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
# [](http://www.cnblogs.com/dkblog/archive/2009/04/16/1980757.html)
# [python下编译py成pyc和pyo](http://www.cnblogs.com/dkblog/archive/2009/04/16/1980757.html)
其实很简单，
用
python -m py_compile file.py
python -m py_compile /root/src/{file1,file2}.py
编译成pyc文件。
也可以写份脚本来做这事：
Code:
import py_compile 
py_compile.compile('path') //path是包括.py文件名的路径
用
python -O -m py_compile file.py
编译成pyo文件。
1.其中的 -m 相当于脚本中的import，这里的-m py_compile 相当于上面的 import py_compile 
2.-O 如果改成 -OO 则是删除相应的 pyo文件，具体帮助可以在控制台输入 python -h 查看
========================
from:[http://blogold.chinaunix.net/u3/93255/showart_1944929.html](http://blogold.chinaunix.net/u3/93255/showart_1944929.html)
### 什么是pyc文件
pyc是一种二进制文件，是由py文件经过编译后，生成的文件，是一种byte code，py文件变成pyc文件后，加载的速度有所提高，而且pyc是一种跨平台的字节码，是由python的虚拟机来执行的，这个是类似于JAVA或者.NET的虚拟机的概念。pyc的内容，是跟python的版本相关的，不同版本编译后的pyc文件是不同的，2.5编译的pyc文件，2.4版本的 python是无法执行的。
### 什么是pyo文件
pyo是优化编译后的程序 python -O 源文件即可将源程序编译为pyo文件 
### 什么是pyd文件
pyd是python的动态链接库。
### 为什么需要pyc文件
这个需求太明显了，因为py文件是可以直接看到源码的，如果你是开发商业软件的话，不可能把源码也泄漏出去吧？所以就需要编译为pyc后，再发布出去。当然，pyc文件也是可以反编译的，不同版本编译后的pyc文件是不同的，根据python源码中提供的opcode，可以根据pyc文件反编译出 py文件源码，网上可以找到一个反编译python2.3版本的pyc文件的工具，不过该工具从python2.4开始就要收费了，如果需要反编译出新版本的pyc文件的话，就需要自己动手了（俺暂时还没这能力^--^）,不过你可以自己修改python的源代码中的opcode文件，重新编译
 python，从而防止不法分子的破解。
### 生成单个pyc文件
python就是个好东西，它提供了内置的类库来实现把py文件编译为pyc文件，这个模块就是 py_compile 模块。
使用方法非常简单，如下所示，直接在idle中，就可以把一个py文件编译为pyc文件了。(假设在windows环境下)
import py_compile
py_compile.compile(r'H:\game\test.py')
compile函数原型：
compile(file[, cfile[, dfile[, doraise]]])
file 表示需要编译的py文件的路径
cfile 表示编译后的pyc文件名称和路径，默认为直接在file文件名后加c 或者 o，o表示优化的字节码
dfile 这个参数英文看不明白，请各位大大赐教。(鄙视下自己)原文：it is used as the name of the source file in error messages instead of file
doraise 可以是两个值，True或者False，如果为True，则会引发一个PyCompileError，否则如果编译文件出错，则会有一个错误，默认显示在sys.stderr中，而不会引发异常
(来自python2.5文档)
### 批量生成pyc文件
一般来说，我们的工程都是在一个目录下的，一般不会说仅仅编译一个py文件而已，而是需要把整个文件夹下的py文件都编译为pyc文件，python又为了我们提供了另一个模块：compileall 。使用方法如下：
import compileall
compileall.compile_dir(r'H:\game')
也可以直接用命令行编译一个目录下的文件，如：# python -m compileall
 /root/src/
这样就把game目录，以及其子目录下的py文件编译为pyc文件了。嘿嘿，够方便吧。来看下compile_dir函数的说明：
compile_dir(dir[, maxlevels[, ddir[, force[, rx[, quiet]]]]])
dir 表示需要编译的文件夹位置
maxlevels 表示需要递归编译的子目录的层数，默认是10层，即默认会把10层子目录中的py文件编译为pyc
ddir 英文没明白，原文：it is used as the base path from which the filenames used in error messages will be generated。
force 如果为True，则会强制编译为pyc，即使现在的pyc文件是最新的，还会强制编译一次，pyc文件中包含有时间戳，python编译器会根据时间来决定，是否需要重新生成一次pyc文件
rx 表示一个正则表达式，比如可以排除掉不想要的目录，或者只有符合条件的目录才进行编译
quiet 如果为True，则编译后，不会在标准输出中，打印出信息
(来自python2.5文档)
### 总结
通过上面的方法，可以方便的把py文件编译为pyc文件了，从而可以实现部分的源码隐藏，保证了python做商业化软件时，保证了部分的安全性吧，继续学习下，看怎么修改opcode。

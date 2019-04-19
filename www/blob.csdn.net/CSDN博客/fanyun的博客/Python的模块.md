# Python的模块 - fanyun的博客 - CSDN博客
2017年04月04日 19:35:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：425标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/fanyun_01/article/category/6838309)
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
       模块就是一个包含了所有你定义的函数和变量的文件，模块必须以.py为扩展名。模块可以从其他程序中‘输入’(import)以便利用它的功能。
　　在python程序中导入其他模块使用'import', 所导入的模块必须在sys.path所列的目录中，因为sys.path第一个字符串是空串''即当前目录，所以程序中可导入当前目录的模块。
1. 字节编译的.pyc文件
　　　　导入模块比较费时，python做了优化，以便导入模块更快些。一种方法是创建字节编译的文件，这些文件以.pyc为扩展名。
　　　　pyc是一种二进制文件，是py文件经编译后产生的一种byte code，而且是跨平台的（平台无关）字节码，是有python虚拟机执行的，类似于java或.net虚拟机的概念。pyc的内容，是跟python的版本相关的，不同版本编译后的pyc文件是不同的。
2. from .. import
　　　　如果想直接使用其他模块的变量或其他，而不加'模块名+.'前缀，可以使用from .. import。
　　　　例如想直接使用sys的argv，from sys import argv 或 from sys import *
3. 模块的__name__
　　　　每个模块都有一个名称，py文件对应模块名默认为py文件名，也可在py文件中为__name__赋值；如果是__name__，说明这个模块被用户
　　单独运行。
4. dir()函数
　　　　dir(sys)返回sys模块的名称列表；如果不提供参数，即dir()，则返回当前模块中定义名称列表。
　　　　del -> 删除一个变量/名称，del之后，该变量就不能再使用。

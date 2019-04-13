
# 4.7 Python包的导入 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:49:33[seven-soft](https://me.csdn.net/softn)阅读数：228


假定我们的包的例子有如下的目录结构：
Phone/
__init__.py
common_util.py
Voicedta/
__init__.py
Pots.py
Isdn.py
Fax/
__init__.py
G3.py
Mobile/
__init__.py
Analog.py
igital.py
Pager/
__init__.py
Numeric.py
Phone 是最顶层的包，Voicedta 等是它的子包。 我们可以这样导入子包：importPhone.Mobile.Analog
Phone.Mobile.Analog.dial()

你也可使用 from-import 实现不同需求的导入。
第一种方法是只导入顶层的子包，然后使用属性/点操作符向下引用子包树：fromPhoneimportMobile
Mobile.Analog.dial('555-1212')

此外，我们可以还引用更多的子包：fromPhone.MobileimportAnalog
Analog.dial('555-1212')

事实上，你可以一直沿子包的树状结构导入：fromPhone.Mobile.Analogimportdial
dial('555-1212')

在我们上边的目录结构中，我们可以发现很多的 __init__.py 文件。这些是初始化模块，from-import 语句导入子包时需要用到它。 如果没有用到，他们可以是空文件。 程序员经常忘记为它们的包目录加入 __init__.py 文件，所以从 Python 2.5 开始，这将会导致一个 ImportWarning 信息。
不过，除非给解释器传递了 -Wd 选项，否则它会被简单地忽略。
包同样支持 from-import all 语句：frompackage.moduleimport*

然而，这样的语句会导入哪些文件取决于操作系统的文件系统。所以我们在__init__.py 中加入 __all__ 变量。该变量包含执行这样的语句时应该导入的模块的名字。它由一个模块名字符串列表组成.。


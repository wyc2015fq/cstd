# Python-GoogleCourse 学习 Day1 - fighting！！！ - CSDN博客
2019年02月21日 19:53:08[dujiahei](https://me.csdn.net/dujiahei)阅读数：80
**目录**
[参考：](#%E5%8F%82%E8%80%83%EF%BC%9A)
[知识点](#%E7%9F%A5%E8%AF%86%E7%82%B9)
[代码练习](#%E4%BB%A3%E7%A0%81%E7%BB%83%E4%B9%A0)
## 参考：
[http://cs231n.github.io/ipython-tutorial/](http://cs231n.github.io/ipython-tutorial/)
[https://www.jianshu.com/p/9070e7377eb6](https://www.jianshu.com/p/9070e7377eb6)
[https://www.jianshu.com/p/f5ae570fdc18](https://www.jianshu.com/p/f5ae570fdc18)
[https://www.python.org/dev/peps/pep-0008/](https://www.python.org/dev/peps/pep-0008/)
[https://www.cnblogs.com/lfri/p/10397271.html](https://www.cnblogs.com/lfri/p/10397271.html)
[https://blog.csdn.net/ratsniper/article/details/78954852](https://blog.csdn.net/ratsniper/article/details/78954852)
[https://lelglin.iteye.com/blog/1870243](https://lelglin.iteye.com/blog/1870243)
[https://blog.csdn.net/xiaotao_1/article/details/78760473](https://blog.csdn.net/xiaotao_1/article/details/78760473)
[http://www.runoob.com/python3/python3-data-type.html](http://www.runoob.com/python3/python3-data-type.html)
[http://www.runoob.com/python/python-operators.html](http://www.runoob.com/python/python-operators.html) （多看）
[https://blog.csdn.net/cc1770374/article/details/83591444](https://blog.csdn.net/cc1770374/article/details/83591444)
[https://www.cnblogs.com/pjsdly-NLP/p/9961929.html](https://www.cnblogs.com/pjsdly-NLP/p/9961929.html)
[https://github.com/YZHANG1270/Girls-In-AI/blob/master/machine_learning_diary/base/data_type/type_trans.md](https://github.com/YZHANG1270/Girls-In-AI/blob/master/machine_learning_diary/base/data_type/type_trans.md)
[https://www.jianshu.com/p/86117613b7a6](https://www.jianshu.com/p/86117613b7a6)
## 知识点
- 环境搭建
	- anaconda环境配置
- 解释器
	- 熟悉解释器的概念
		当我们编写Python代码时，我们得到的是一个包含Python代码的以.py为扩展名的文本文件。要运行代码，就需要Python解释器去执行.py文件。
		由于整个Python语言从规范到解释器都是开源的，所以理论上，只要水平够高，任何人都可以编写Python解释器来执行Python代码（当然难度很大）。事实上，确实存在多种Python解释器。
- CPython,IPython两种解释器
		CPython当我们从Python官方网站下载并安装好Python 3.x后，我们就直接获得了一个官方版本的解释器：CPython。这个解释器是用C语言开发的，所以叫CPython。在命令行下运行python就是启动CPython解释器。
		CPython是使用最广的Python解释器。教程的所有代码也都在CPython下执行。
		IPythonIPython是基于CPython之上的一个交互式解释器，也就是说，IPython只是在交互方式上有所增强，但是执行Python代码的功能和CPython是完全一样的。好比很多国产浏览器虽然外观不同，但内核其实都是调用了IE。
		CPython用>>>作为提示符，而IPython用In [序号]:作为提示符。
- 小结
		Python的解释器很多，但使用最广泛的还是CPython。如果要和Java或.Net平台交互，最好的办法不是用Jython或IronPython，而是通过网络调用来交互，确保各程序之间的独立性。
		本教程的所有代码只确保在CPython 3.x版本下运行。请务必在本地安装CPython（也就是从Python官方网站下载的安装程序）。
- jupyetr notebook
	- 操作代码单元格
		- 运行的三种方式
			首先，运行以下代码单元格。就像之前我所提到的，你可以通过选中并点击运行（run cell）按钮来运行代码单元格。另外，你也可以通过快捷键 Shift + Enter 来运行。使用快捷键的好处是你的双手不需要离开键盘。Shift + Enter 运行单元格之后会自动选中下一个单元格，或者根据需要创建新的单元格。你也可以通过 Control + Enter 在运行之后仍然选中当前单元格。
			        单元格中的代码运行的结果将会显现在单元格下方。它和正常的 Python shell 一样打印出代码的运行结果，但是只会打印最后一个运行结果。如果你想打印所有结果，你需要使用 print() 。
			所有在单元格创建的变量，函数和类都可以在当前 Notebook 中的其它单元格访问。​
- 代码补全
		当你在写代码的时候，你将发现代码补全可以大大节省你使用变量或者函数的时间，因为你只需要键入名称的一部分，然后按 tab 。
- 工具使用建议
		你看到有一个函数叫作 random.gauss 但如何使用它呢？你可以点击 文档，或者直接在 notebook 中查看。
- 快捷键使用
		- 编辑模式与命令模式切换
			首先，在编辑模式和命令模式间相互切换。在编辑模式中你可以键入单元格，在命令模式你可以敲击键盘来执行命令，例如创建一个新的单元格和打开一个命令面板。当你选择一个单元格时，你可以通过单元格框线的颜色来分辨出你当前正在使用的模式。在编辑模式中，左边粗框线是绿色的。在命令模式中，左边粗框线是蓝色的。在编辑模式中，你能在单元格中看到一个光标。
			        默认情况下，当你能够创建一个新的单元格或者可以移动到下一行，那么你就在命令模式。要进入编辑模式，按Enter或者Return。要从编辑模式返回到命令模式，按Escape。
- 命令模式中的帮助命令
			如果你需要查找一个命令，你可以通过在命令模式中按H调出快捷方式列表。上面的帮助列表也提供了键盘快捷键。
- 创建一个新的单元格
			最常见的命令之一是创建一个新的单元格。你可以通过在命令模式按 A 在当前单元格上方创建一个单元格。按 B 在当前选定的单元格下方创建一个单元格。
			通过使用键盘快捷键，可以很快很简单地在Markdown和代码单元格之间相互切换。从Markdown切换到单元格，按 Y 。从代码切换到Markdown，按 M。​
- 行号
			很多时候出于调试的目的在代码中对代码进行编号是很有用的。你可以在命令模式中的代码单元格按 L 打开数字。
- 删除单元格
			 在一行连续按两次 D 可以删除单元格。这是为了防止出现意外的删除，所以你必须连续按两次键。
- 复制、剪切、粘贴单元格
- 保存笔记本
			笔记本每隔一段时间会自动保存，但是如果你想在这些时间内保存你的工作，按 S 去保存笔记本。如此简单！
- 命令面板
			通过按Shift + Control或者Command + P，你可以很轻松地访问命令面板。
			        注意： 很不幸，在火狐浏览器和Internet Explorer浏览器中命令面板不能工作，在这些浏览器中已经有一些功能赋值给这些快捷键。在谷歌浏览器和Safari浏览器中可以正常打开命令面板。
			        你可以在弹出的命令面板中搜索不能用键盘快捷键实现的命令。例如，工作栏上有按钮可以实现上下移动单元格的命令（上下箭头），但是没有相对应的键盘快捷键。要把一个单元格向下移动，你可以打开命令面板键入"move"就会出现移动命令。
- 设置当前运行环境
- 从本地文件夹打开 jupyter notebook
- 运行py文件
- python初体验
	- input() and print()
		
![](https://img.mubu.com/document_image/36361574-033f-448f-bc07-19d86cecaba0-2534569.jpg)
![](https://img.mubu.com/document_image/3c51f819-6aa9-4074-834b-46b50e0d4446-2534569.jpg)
		用print()在括号中加上字符串，就可以向屏幕上输出指定的文字。
- 数据类型和变量
		- 数据类型
			整数、浮点数、字符串（注意转义）、布尔、空值、列表list、字典dict、元组tuple、自定义数据类型
- python变量特性+命名规则
			
![](https://img.mubu.com/document_image/cb558a33-0ccf-4a6e-aacd-092597074b4b-2534569.jpg)
![](https://img.mubu.com/document_image/ca826bc8-4442-4ecb-9efe-507b03ad8844-2534569.jpg)
			变量在程序中就是用一个变量名表示了，变量名必须是大小写英文、数字和_的组合，且不能用数字开头。在Python中，等号=是赋值语句，可以把任意数据类型赋值给变量，同一个变量可以反复赋值，而且可以是不同类型的变量。这种变量本身类型不固定的语言称之为动态语言，与之对应的是静态语言。静态语言在定义变量时必须指定变量类型，如果赋值的时候类型不匹配，就会报错。例如Java是静态语言。
- 常量
			所谓常量就是不能变的变量，比如常用的数学常数π就是一个常量。在Python中，通常用全部大写的变量名表示常量：PI。但事实上PI仍然是一个变量，Python根本没有任何机制保证PI不会被改变，所以，用全部大写的变量名表示常量只是一个习惯上的用法，如果你一定要改变变量PI的值，也没人能拦住你。
- 两种除法
			
![](https://img.mubu.com/document_image/351c0395-e075-40e0-8435-88c27d76f5cc-2534569.jpg)
- 小结
			Python支持多种数据类型，在计算机内部，可以把任何数据都看成一个“对象”，而变量就是在程序中用来指向这些数据对象的，对变量赋值就是把数据和变量给关联起来。
			对变量赋值x = y是把变量x指向真正的对象，该对象是变量y所指向的。随后对变量y的赋值不影响变量x的指向。
			注意：Python的整数没有大小限制，而某些语言的整数根据其存储长度是有大小限制的，例如Java对32位整数的范围限制在-2147483648-2147483647。
			Python的浮点数也没有大小限制，但是超出一定范围就直接表示为inf（无限大）。
- 学会使用dir()及和help( )
- pep8基础掌握--代码规范
		- 代码布局
			- 缩进
				对于每一次缩进使用4个空格。使用括号、中括号、大括号进行垂直对齐，或者缩进对齐。
				​缩进与换行，每级缩进使用4个空格，而不是tab。
- 行最大长度与换行
				限制所有行的最大长度为79个字符。使用反斜杠来分行是一个很好的选择。我们应当选择在二元操作符之后进行分行，而不是之前。​
- 空行
				使用两行空行来分隔顶层函数和类定义。
				使用单行空行来分隔类方法定义。
				在函数中使用空行来表示不同的逻辑块。
- import导入
				​导入通常应当使用单独的行，而不是几个导入写在一起。导入总是位于文件的顶部，在模块注释和文档字符串之后，在模块的全局变量与常量之前。
				​导入应该按照以下的顺序分组，且每组导入之间使用空行隔开： 
				​standard library imports 标准库导入 
				​related third party imports 相关第三方导入
				​local application/library specific imports 本地应用程序/库的特定导入 。
				使用绝对包路径导入。​
				​每组导入之间使用空行隔开。 从一个包含类的模块中导入类时，常常这么写： from myclass import MyClass 。
				每一个导入通常应当使用单独的行。
				导入应当位于文件顶部，在模块注释和文档字符串之后，在全局变量和常量之前。​
- 编码
				Python核心发行代码里面优先使用ASCII码或Latin-1编码。3.0后UTF-8编码优先于Latin-1。
- 注释
			误导的注释不如没有注释
			注释应当为完整的句子，且句号结尾的句子后面应当有2个空格。如果注释很短，那么结尾的句号可以忽略。
			- 块注释
				块注释应当和代码缩进保持一致。每行注释开头应以#开头，然后紧跟一个空格。
- 行内注释
				行注释至少和语句间隔2个空格。同样的注释应当以#开头，然后紧跟一个空格。
- 文档字符串
				对于所有的公有模块、类、函数和方法都需要编写文档字符串。
				""" 作为多行的文档字符串的结束，应该单独一行，并且之前有一个空行。
				对于只有一行的文档字符串来说，结尾的 """ 在同一行。
				更详细的文档字符串规范见 PEP 257。
- 命名规范
			目前Python库的命名规范尚未达成一致，但有一些推荐的标准。
			在Python里面，有一些具有特定意义的下划线前缀或者后继的特殊格式。如：
			1、_single_leading_underscore：（单下划线开始）弱"内部使用"指示器。例如：from M import * 不会导入以下划线开始的对象。
			2、single_trailing_underscore_：（单下划线结束）规定使用其来避免与Python关键字冲突，例如：
			Tkinter.Toplevel(master, class_='ClassName') 在参数class后面加单下划线，避免与关键字class冲突3、__double_leading_underscore：（双下划线开始）命名一个类的属性时，调用"name mangling"（类FooBar中，__boo 变为了 _FooBar__boo; 见下文）
			4、__double_leading_and_trailing_underscore__：（双下划线开始和结束）存活在用户控制命名空间的"magic"对象或属性。 例如__init__, __import__ 或 __file__。永远不要起这样的名字。
			- 避免使用的命名
				不要使用小写的L、大写的O、以及大写的I作为单字符变量名。
- 包与模块名称
				模块应当使用简短、全小写的名字，也可使用下划线连接来提高可读性。
				包也应当使用简短、全小写的名字，但不要使用下划线。
				这是由于模块名与文件名关联，而在某些文件系统中大小写不敏感，且会截断过长的名字。
				当使用C/C++来编写一个扩展模块时，应当使用下划线作为模块名的前缀。
- 类名
				类名应当使用驼峰式（CapWords）。内部使用的类名应当加下划线前缀。
- 异常名
				异常也是一个类，所以需要遵循类名规则。但如果你的异常确实是个错误的话，请使用Error前缀。
- 全局变量名
				遵循函数规则。
- 函数名
				函数名全小写，可以使用下划线分隔来提高可读性。
- 函数与方法参数
				使用self作为实例方法的第一个参数。
				使用cls作为类方法的第一个参数。
				当函数的参数名与保留字冲突时，使用下划线后缀（第二次提醒）。
- 方法名与实例变量
				遵循函数规则。
				在私有方法和实例变量前用单下划线前缀。
				使用双下划线前缀来调用"name mangling"来避免与子类命名冲突。
				如果类Foo有个属性叫__a，那么它不能使用Foo.__a读取，但仍然可以Foo._Foo__a来读取。
- 常量
				常量通常在模块级别定义，使用全大写和下划线分隔的形式。
- 继承设计
				在设计类的方法或实例变量时，应当觉得其实公有的还是非公有的。当不能确定时，设计为私有的。
				在Python中没有真正的私有属性。
				公有属性不应当使用下划线开始。
				当公有属性与保留字冲突时，在名称后面加下划线后缀（第三次提醒）。
				对于简单的公有属性，最好直接访问其属性名，而非get/set方法。
- python数值基本知识
	- 数值类型：int，float，bool，e记法等
- 算数运算符
		
![](https://img.mubu.com/document_image/6c0ac888-16e3-44ab-b58a-239d7c831319-2534569.jpg)
- 逻辑运算符
		
![](https://img.mubu.com/document_image/3cacc92a-112c-455d-9b54-3c9dabcb0d1a-2534569.jpg)
- 运算符优先级
		
![](https://img.mubu.com/document_image/103f1af2-8cf6-4e17-bd01-b9ece77d1ead-2534569.jpg)
- 比较（关系）运算符
		
![](https://img.mubu.com/document_image/24e29108-05a0-4c06-a0a6-f6f6e8bee70c-2534569.jpg)
- 赋值运算符
		
![](https://img.mubu.com/document_image/f777a3f5-c3f9-4ebc-a7f0-130abeb44d79-2534569.jpg)
- 位运算符
		
![](https://img.mubu.com/document_image/5e50e4ae-a504-4ec4-b41e-d4ccfbb3c5f6-2534569.jpg)
- 成员运算符
		
![](https://img.mubu.com/document_image/f51aefe2-bf55-4daf-8cfe-f7861084fdf1-2534569.jpg)
- 身份运算符
		
![](https://img.mubu.com/document_image/9f7c1e9b-8b70-4cc4-a2f5-511afc1451fb-2534569.jpg)
## 代码练习
```python
name,gender,age=input("请输入您的姓名，性别，年龄，并以空格分割输入信息，例如：张三 男 18.   ").split()
birthY=2019-int(age)
print("*******************************************************")
print("您的姓名是：%s, 您的性别是：%s, 您是%d年生的。" %(name,gender,birthY))
print("*************************************************************************")
```
运行结果：![](https://img-blog.csdnimg.cn/20190221202857274.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)

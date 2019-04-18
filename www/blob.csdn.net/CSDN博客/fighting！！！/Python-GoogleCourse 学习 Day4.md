# Python-GoogleCourse 学习 Day4 - fighting！！！ - CSDN博客
2019年02月24日 22:29:45[dujiahei](https://me.csdn.net/dujiahei)阅读数：11
## 参考：[https://blog.csdn.net/alvinpanda/article/details/76254427](https://blog.csdn.net/alvinpanda/article/details/76254427)
[https://www.cnblogs.com/baomanji/p/6701981.html](https://www.cnblogs.com/baomanji/p/6701981.html)
[https://www.cnblogs.com/hackerl/p/5985102.html](https://www.cnblogs.com/hackerl/p/5985102.html)
[https://www.cnblogs.com/guobaoyuan/p/6756763.html](https://www.cnblogs.com/guobaoyuan/p/6756763.html)
[http://www.runoob.com/python3/python3-function.html](http://www.runoob.com/python3/python3-function.html)
[https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431752945034eb82ac80a3e64b9bb4929b16eeed1eb9000](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431752945034eb82ac80a3e64b9bb4929b16eeed1eb9000)
[http://www.runoob.com/python3/python3-func-filter.html](http://www.runoob.com/python3/python3-func-filter.html)
[http://www.runoob.com/python/python-func-reduce.html](http://www.runoob.com/python/python-func-reduce.html)
## 知识点
- 函数的定义
	- 概念
		函数是组织好的，可重复使用的，用来实现单一，或相关联功能的代码段。函数能提高应用的模块性，和代码的重复利用率。你已经知道Python提供了许多内建函数，比如print()。但你也可以自己创建函数，这被叫做用户自定义函数。
- 定义规则
		函数代码块以 def 关键词开头，后接函数标识符名称和圆括号 ()。
		任何传入参数和自变量必须放在圆括号中间，圆括号之间可以用于定义参数。
		函数的第一行语句可以选择性地使用文档字符串—用于存放函数说明。
		函数内容以冒号起始，并且缩进。
		return [表达式] 结束函数，选择性地返回一个值给调用方。不带表达式的return相当于返回 None。
- 语法
		def 函数名（参数列表）:
		函数体
		默认情况下，参数值和参数名称是按函数声明中定义的顺序匹配起来的。​​
- 函数的调用
	定义一个函数：给了函数一个名称，指定了函数里包含的参数，和代码块结构。
	这个函数的基本结构完成以后，你可以通过另一个函数调用执行，也可以直接从 Python 命令提示符执行。
	​要调用一个函数，需要知道函数的名称和参数，比如求绝对值的函数abs，只有一个参数。可以直接从Python的官方网站查看文档：
[http://docs.python.org/3/library/functions.html#abs](http://docs.python.org/3/library/functions.html#abs)
	也可以在交互式命令行通过help(abs)查看abs函数的帮助信息。
	Python内置的常用函数还包括数据类型转换函数，比如int()函数可以把其他数据类型转换为整数：int('123')
	​函数名其实就是指向一个函数对象的引用，完全可以把函数名赋给一个变量，相当于给这个函数起了一个“别名”：a = abs # 变量a指向abs函数 
	​ a(-1) # 所以也可以通过a调用abs函数
- 参数传递
	
![](https://img.mubu.com/document_image/105e0392-0bc2-4f06-82d8-908bfeda8489-2329912.jpg)
	python 中一切都是对象，严格意义我们不能说值传递还是引用传递，我们应该说传不可变对象和传可变对象。
	- 传不可变对象
		类似 c++ 的值传递，如 整数、字符串、元组。如fun（a），传递的只是a的值，没有影响a对象本身。比如在 fun（a）内部修改 a 的值，只是修改另一个复制的对象，不会影响 a 本身。
- 传可变对象
		类似 c++ 的引用传递，如 列表，字典。如 fun（la），则是将 la 真正的传过去，修改后fun外部的la也会受影响
- 函数参数
	- 普通参数/必需参数
		必需参数须以正确的顺序传入函数。调用时的数量必须和声明时的一样。
		调用printme()函数，你必须传入一个参数，不然会出现语法错误：
- 关键字参数
		关键字参数和函数调用关系紧密，函数调用使用关键字参数来确定传入的参数值。
		使用关键字参数允许函数调用时参数的顺序与声明时不一致，因为 Python 解释器能够用参数名匹配参数值。
- 默认参数（定义了默认值的参数）
		调用函数时，如果没有传递参数，则会使用默认参数。
- 可选参数/不定长参数
		你可能需要一个函数能处理比当初声明时更多的参数。这些参数叫做不定长参数，和上述 2 种参数不同，声明时不会命名。基本语法如下：
		​def functionname([formal_args,] *var_args_tuple ):
		"函数_文档字符串"
		function_suite
		return [expression]
		- 一个*号
			加了星号 * 的参数会以元组(tuple)的形式导入，存放所有未命名的变量参数。如果在函数调用时没有指定参数，它就是一个空元组。我们也可以不向函数传递未命名的变量。
- 两个*号
			def functionname([formal_args,] **var_args_dict ):
			"函数_文档字符串"
			function_suite
			return [expression]
			​加了两个星号 ** 的参数会以字典的形式导入。
			声明函数时，参数中星号 * 可以单独出现；​如果单独出现星号 * 后的参数必须用关键字传入。
- 函数返回值
	return [表达式] 语句用于退出函数，选择性地向调用方返回一个表达式。不带参数值的return语句返回None。
- 函数变量作用域
	Python 中，程序的变量并不是在哪个位置都可以访问的，访问权限决定于这个变量是在哪里赋值的。变量的作用域决定了在哪一部分程序可以访问哪个特定的变量名称。
	Python的作用域一共有4种，分别是：
	L （Local） 局部作用域
	E （Enclosing） 闭包函数外的函数中
	G （Global） 全局作用域
	B （Built-in） 内置作用域（内置函数所在模块的范围）
	以 L –> E –> G –>B 的规则查找，即：在局部找不到，便会去局部外的局部找（例如闭包），再找不到就会去全局找，再者去内置中找。​
	- 全局变量 和 局部变量
		定义在函数内部的变量拥有一个局部作用域，定义在函数外的拥有全局作用域。
		局部变量只能在其被声明的函数内部访问，而全局变量可以在整个程序范围内访问。调用函数时，所有在函数内声明的变量名称都将被加入到作用域中。
- global 和 nonlocal关键字
		当内部作用域想修改外部作用域的变量时，就要用到global和nonlocal关键字了。
		如果要修改嵌套作用域（enclosing 作用域，外层非全局作用域）中的变量则需要 nonlocal 关键字了。​
- 内嵌函数及闭包
	- 内嵌函数
		Python的函数定义可以嵌套的，也就是允许在函数内部创建另一个函数，这样的函数叫做内嵌函数或者内部函数。
- 闭包
		闭包（closure）是函数式编程的一个重要语法结构，函数式编程是一种编程范式，著名的函数编程语言就是LISP语言（主要用于绘图，AI）
		不同的编程语言实现闭包的方式不同，Python中闭包从表现上定义为：如果在一个内部函数里，对在外部作用域（但不是在全局作用域）的变量进行引用，那么内部函数就被认为是闭包（closure）。
		内部函数的代码包含对外部作用域的引用，但一定不是对全局作用域的引用，闭包函数一定有__closure__方法。​
- lambda表达式--匿名函数
	python 使用 lambda 来创建匿名函数。
	​所谓匿名，意即不再使用 def 语句这样标准的形式定义一个函数。
	lambda 只是一个表达式，函数体比 def 简单很多。
	lambda的主体是一个表达式，而不是一个代码块。仅仅能在lambda表达式中封装有限的逻辑进去。
	lambda 函数拥有自己的命名空间，且不能访问自己参数列表之外或全局命名空间里的参数。
	虽然lambda函数看起来只能写一行，却不等同于C或C++的内联函数，后者的目的是调用小函数时不占用栈内存从而增加运行效率。
	lambda 函数的语法只包含一个语句，如下：
	lambda [arg1 [,arg2,.....argn]]:expression
	​
- 重要的BIF(built-in functions, 内建函数) 
	通过help()命令可以看到关于BIF的介绍及用法。
	- filter（）
		- 描述
			filter() 函数用于过滤序列，过滤掉不符合条件的元素，返回一个迭代器对象，如果要转换为列表，可以使用 list() 来转换。
			该接收两个参数，第一个为函数，第二个为序列，序列的每个元素作为参数传递给函数进行判，然后返回 True 或 False，最后将返回 True 的元素放到新列表中。
- 语法
			filter(function, iterable)
- 参数
			function -- 判断函数。
			iterable -- 可迭代对象。
- 返回值
			返回一个迭代器对象
- map（）
		- 描述
			map() 会根据提供的函数对指定序列做映射。
			第一个参数 function 以参数序列中的每一个元素调用 function 函数，返回包含每次 function 函数返回值的新列表。
- 语法
			map(function, iterable, ...)
- 参数
			function -- 函数
			iterable -- 一个或多个序列
- 返回值
			Python 2.x 返回列表。
			Python 3.x 返回迭代器。
- reduce（）--在 Python3 中，reduce() 函数已经被从全局名字空间里移除了，它现在被放置在 functools 模块里，如果想要使用它，则需要通过引入 functools 模块来调用 reduce() 函数： from functools import reduce
	- 描述
		reduce() 函数会对参数序列中元素进行累积。
		函数将一个数据集合（链表，元组等）中的所有数据进行下列操作：用传给 reduce 中的函数 function（有两个参数）先对集合中的第 1、2 个元素进行操作，得到的结果再与第三个数据用 function 函数运算，最后得到一个结果。
- 语法
		reduce(function, iterable[, initializer])
- 参数
		function -- 函数，有两个参数
		iterable -- 可迭代对象
		initializer -- 可选，初始参数
- 返回值
		返回函数计算结果。

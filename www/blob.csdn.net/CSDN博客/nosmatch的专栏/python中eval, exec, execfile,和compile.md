# python中eval, exec, execfile,和compile - nosmatch的专栏 - CSDN博客
2011年08月17日 11:01:51[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：715标签：[python																[lambda																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本](https://blog.csdn.net/HDUTigerkin/article/category/845589)

python中eval, exec, execfile,和compile2008-08-03 16:49eval(str [,globals [,locals ]])函数将字符串str当成有效Python表
达式来求值，并返回计算结果。
同样地, exec语句将字符串str当成有效Python代码来执行.提供给exec的代码的名称空间和exec语句的名称空间相同.
最后，execfile(filename [,globals [,locals ]])函数可以用来执行一个文件,看下面的例子:
>>> eval('3+4')
        7
        >>> exec 'a=100'
        >>> a
        100
        >>> execfile(r'c:\test.py')
        hello,world!
        >>>
默认的，eval(),exec,execfile()所运行的代码都位于当前的名字空间中. eval(), exec,和 execfile()函数也可以接受一个或两个
可选字典参数作为代码执行的全局名字空间和局部名字空间. 例如:
   1 globals = {'x': 7,
   2            'y': 10,
   3            'birds': ['Parrot', 'Swallow', 'Albatross']
   4           }
   5 locals = { }
   6 
   7 # 将上边的字典作为全局和局部名称空间
   8 a = eval("3*x + 4*y", globals, locals)
   9 exec "for b in birds: print b" in globals, locals   # 注意这里的语法
10 execfile("foo.py", globals, locals)
如果你省略了一个或者两个名称空间参数,那么当前的全局和局部名称空间就被使用.如果一个函数体内嵌嵌套函数或lambda匿名函数时,同时又在函数主体中使用exec或execfile()函数时， 由于牵到嵌套作用域，会引发一个SyntaxError异常.（在Python2.4中俺未发现可以引起异常
注意例子中exec语句的用法和eval(), execfile()是不一样的. exec是一个语句(就象print或while), 而eval()和execfile()则是内建函数.
exec(str) 这种形式也被接受，但是它没有返回值。 当一个字符串被exec,eval(),或execfile()执行时,解释器会先将它们编译为字节代码，然后再执行.这个过程比较耗时,所以如果需要对某段代码执行很多次时,最好还是对该代码先进行预编译,这样就不需要每次都编译一遍代码，可以有效提高程序的执行效率。
compile(str ,filename ,kind )函数将一个字符串编译为字节代码, str是将要被编译的字符串, filename是定义该字符串变量的文
件，kind参数指定了代码被编译的类型-- 'single'指单个语句, 'exec'指多个语句, 'eval'指一个表达式. cmpile()函数返回一个代
码对象，该对象当然也可以被传递给eval()函数和exec语句来执行,例如:
   1 str = "for i in range(0,10): print i"
   2 c = compile(str,'','exec')      # 编译为字节代码对象
   3 exec c                          # 执行
   4 
   5 str2 = "3*x + 4*y"
   6 c2 = compile(str2, '', 'eval') # 编译为表达式
   7 result = eval(c2)               # 执行

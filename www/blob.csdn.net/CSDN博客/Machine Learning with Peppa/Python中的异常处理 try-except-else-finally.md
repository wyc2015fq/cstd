# Python中的异常处理: try-except-else-finally - Machine Learning with Peppa - CSDN博客





2018年02月05日 22:14:32[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：571
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)










在Python的异常处理中，作为异常类的标准处理方式，try语句块为我们提供了便捷的判断与执行入口。不同于默认的traceback Error，当我们在执行程序中，难免会碰到因为一些原因如输入输出导致致命性错误产生的情况（如因为输入的文件名错误而导致无法运行相关的代码。此时不希望程序直接挂掉，而是通过显示一些信息，使其平稳的结束。此时，就可以使用try，except和else这三个关键字来组成一个包容性很好的程序。尽管try/except和try/finally的作用不同，但是在编程实践中通常可以把它们组合在一起使用try/except/else/finally的形式来实现稳定性和灵活性更好的设计。


 与其他语言相同，在python中，try/except语句主要是用于处理程序正常执行过程中出现的一些异常情况，如语法错误（python作为脚本语言没有编译的环节，在执行过程中对语法进行检测，出错后发出异常消息）、数据除零错误、从未定义的变量上取值等；而try/finally语句则主要用于在无论是否发生异常情况，都需要执行一些清理工作的场合。


默认情况下，在程序段的执行过程中，如果没有提供try/except的处理，脚本文件执行过程中所产生的异常消息会自动发送给程序调用端，如python shell，而python shell对异常消息的默认处理则是终止程序的执行并打印具体的出错信息。这也是在python shell中执行程序错误后所出现的出错打印信息的由来。





分别解释三个关键字：


try：执行可能会出错的试探性语句，即这里面的语句是可以导致致命性错误使得程序无法继续执行下去


except：如果try里面的语句无法正确执行，那么就执行except里面的语句，这里面可以是错误信息或者其他的可执行语句


else：如果try里面的语句可以正常执行，那么就执行else里面的语句（相当于程序没有碰到致命性错误）







python中try/except/else/finally语句的完整格式如下所示：


try:


     Normal execution block


except A:

     Exception A handle


except B:

     Exception B handle

except:

     Other exception handle

else:

     if no exception,get here


finally:

     print("finally")   





参考文章：https://www.cnblogs.com/lordage/p/5722086.html




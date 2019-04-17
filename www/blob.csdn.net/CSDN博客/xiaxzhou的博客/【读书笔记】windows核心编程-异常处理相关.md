# 【读书笔记】windows核心编程 - 异常处理相关 - xiaxzhou的博客 - CSDN博客





2017年08月19日 15:22:19[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：165








### _try 和       _finally的使用：

终止处理程序：

不管try段代码是如何退出的——无论是return、goto、break、continue（除非是exitprocess、exitthread、terminateprocess、terminatethread），finally段的代码都能执行。

finally段的return会覆盖try段的return。

### _try  和  _except  的使用：
- 异常处理程序：

不能同时有_finally 和 _except段，也不能有多个 _finally  或  _except段。 

不过可以将try-finally块嵌套到try-except段中。反过来也可以。

> 
EXCEPTION_EXECUTE_HANDLER：


异常被识别，执行 __ except代码块，__except代码块执行完毕后，继续执行 __except代码之后的代码。

全局展开：

当异常过滤程序返回EXCEPTION_EXECUTE_HANDLER时，如果当前指令是在try-finally代码的try中，则离开try之前，会先执行finally中的代码。

如果一个异常发生在try-finally块中，并在其上层又没有try-except块（同时返回EXCEPTION_EXECUTE_HANDLER），进程就会立刻终止。不会全局展开执行finally代码。

> 
EXCEPTION_CONTINUE_EXECUTION：


异常被忽略，控制流将在异常出现的点之后，继续恢复运行

> 
EXCEPTION_CONTINUE_SEARCH (0) 


异常不被识别，即当前的这个 __except 模块不是这个异常错误所对应的正确的异常处理模块。系统将继续到上一层的try-except域中继续查找一个恰当的 __except 模块。




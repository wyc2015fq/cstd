# 如何在Python中实现尾递归优化 - Machine Learning with Peppa - CSDN博客





2018年04月27日 20:07:22[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1164








#### 一般递归
























|123456|def normal_recursion(n):ifn==1:return1else:returnn+normal_recursion(n-1)|
|----|----|

执行：
























|12345678910|normal_recursion(5)5+normal_recursion(4)5+4+normal_recursion(3)5+4+3+normal_recursion(2)5+4+3+2+normal_recursion(1)5+4+3+35+4+65+1015|
|----|----|

可以看到, 一般递归, 每一级递归都需要调用函数, 会创建新的栈,


随着递归深度的增加, 创建的栈越来越多, 造成爆栈



#### 尾递归

[尾递归基于函数的尾调用](https://zh.wikipedia.org/wiki/%E5%B0%BE%E8%B0%83%E7%94%A8), 每一级调用直接返回函数的返回值更新调用栈,而不用创建新的调用栈, 类似迭代的实现, 时间和空间上均优化了一般递归!
























|123456|def tail_recursion(n,total=0):ifn==0:returntotalelse:returntail_recursion(n-1,total+n)|
|----|----|

执行：
























|12345678|tail_recursion(5)tail_recursion(4,5)tail_recursion(3,9)tail_recursion(2,12)tail_recursion(1,14)tail_recursion(0,15)15|
|----|----|

可以看到, 每一级递归的函数调用变成”线性”的形式.

## 深入理解尾递归

呃, 所以呢? 是不是感觉还不够过瘾… 谁说尾递归调用就不用创建新的栈呢?
还是让我们去底层一探究竟吧
























|123456789101112131415|inttail_recursion(intn,inttotal){if(n==0){returntotal;}else{returntail_recursion(n-1,total+n);}}intmain(void){inttotal=0,n=4;tail_recursion(n,total);return0;}|
|----|----|

反汇编
- `$ gcc -S tail_recursion.c -o normal_recursion.S`
- `$ gcc -S -O2 tail_recursion.c -o tail_recursion.S` gcc开启尾递归优化

对比反汇编代码如下(AT&T语法)

![](http://jbcdn2.b0.upaiyun.com/2016/12/a6d33ee84a3e4300f2efe4f4e0121371.png)

可以看到, 开启尾递归优化前, 使用call调用函数, 创建了新的调用栈(LBB0_3);
而开启尾递归优化后, 就没有新的调用栈生成了, 而是直接pop
bp指向的`_tail_recursion`函数的地址(pushq %rbp)然后返回,

![](http://jbcdn2.b0.upaiyun.com/2016/12/81e9afc597b93feda540cf7c41b17b3b.png)

仍旧用的是同一个调用栈!

## 存在的问题

虽然尾递归优化很好, 但python 不支持尾递归，递归深度超过1000时会报错
























|12|RuntimeError:maximum recursion depth exceeded|
|----|----|



## 一个牛人想出的解决办法

#### 实现一个 tail_call_optimized 装饰器


























|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253|#!/usr/bin/env python2.4# This program shows off a python decorator(# which implements tail call optimization. It# does this by throwing an exception if it is# it's own grandparent, and catching such# exceptions to recall the stack.import sysclassTailRecurseException:def __init__(self,args,kwargs):self.args=argsself.kwargs=kwargsdef tail_call_optimized(g):"""    This function decorates a function with tail call    optimization. It does this by throwing an exception    if it is it's own grandparent, and catching such    exceptions to fake the tail call optimization.    This function fails if the decorated    function recurses in a non-tail context.    """def func(*args,**kwargs):f=sys._getframe()# 为什么是grandparent, 函数默认的第一层递归是父调用,# 对于尾递归, 不希望产生新的函数调用(即:祖父调用),# 所以这里抛出异常, 拿到参数, 退出被修饰函数的递归调用栈!(后面有动图分析)iff.f_back andf.f_back.f_back\andf.f_back.f_back.f_code==f.f_code:# 抛出异常raise TailRecurseException(args,kwargs)else:while1:try:returng(*args,**kwargs)except TailRecurseException,e:# 捕获异常, 拿到参数, 退出被修饰函数的递归调用栈args=e.argskwargs=e.kwargsfunc.__doc__=g.__doc__returnfunc@tail_call_optimizeddef factorial(n,acc=1):"calculate a factorial"ifn==0:returnaccreturnfactorial(n-1,n*acc)print factorial(10000)|
|----|----|

为了更清晰的展示开启尾递归优化前、后调用栈的变化和tail_call_optimized装饰器抛异常退出递归调用栈的作用, 我这里利用[pudb调试工具](https://github.com/inducer/pudb)做了动图

开启尾递归优化前的调用栈

![](http://jbcdn2.b0.upaiyun.com/2016/12/5aa2ad30f260211c82e510715322a81b.gif)

开启尾递归优化后(tail_call_optimized装饰器)的调用栈

![](http://jbcdn2.b0.upaiyun.com/2016/12/11ed841df4b1555a5f6084d8346d7fd4.gif)

通过pudb右边栏的stack, 可以很清晰的看到调用栈的变化.

因为尾递归没有调用栈的嵌套, 所以Python也不会报`RuntimeError: maximum recursion depth exceeded`错误了!

这里解释一下 sys._getframe() 函数:




























|12345678910111213141516|sys._getframe([depth]):Returnaframe objectfrom the call stack.Ifoptional integerdepth isgiven,returnthe frame objectthat many calls below the top of the stack.Ifthat isdeeper than the call stack,ValueEfror israised.The defaultfordepth iszero,returning the frame at the top of the call stack.即返回depth深度调用的栈帧对象.import sysdef get_cur_info():print sys._getframe().f_code.co_filename# 当前文件名print sys._getframe().f_code.co_name# 当前函数名print sys._getframe().f_lineno# 当前行号print sys._getframe().f_back# 调用者的帧|
|----|----|

更多关于`sys._getframe`请看[Frame Hacks](http://feihonghsu.com/secrets/framehack/index.html#crazy-monkey-patching-lxml)





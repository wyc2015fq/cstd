
# Python的定时器 - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:37[saltriver](https://me.csdn.net/saltriver)阅读数：70723个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)



在实际应用中，我们经常需要使用定时器去触发一些事件。Python中通过线程实现定时器timer,其使用非常简单。看示例：
importthreading
deffun_timer():
print('Hello Timer!')
timer = threading.Timer(1,fun_timer)
timer.start()输出结果：
Hello Timer!
Process finished with exit code 0

注意，只输出了一次，程序就结束了，显然不是我们想要的结果。看Timer类中的解释性描述："""Call a function after a specified number of seconds"""一段时间后调用一个函数，但并没有说要循环调用该函数。因此，修改如下：deffun_timer():
print('Hello Timer!')
globaltimer
timer = threading.Timer(5.5,fun_timer)
timer.start()
timer = threading.Timer(1,fun_timer)
timer.start()输出结果：
Hello Timer!
Hello Timer!
Hello Timer!
Hello Timer!
............
定时器工作正常。

**在使用Python定时器时需要注意如下4个方面：**
（1）定时器构造函数主要有2个参数，第一个参数为时间，第二个参数为函数名，第一个参数表示多长时间后调用后面第二个参数指明的函数。第二个参数注意是函数对象，进行参数传递，用函数名(如fun_timer)表示该对象，不能写成函数执行语句fun_timer()，不然会报错。用type查看下，可以看出两者的区别。
print(type(fun_timer()))
print(type(fun_timer))输出结果：
Hello Timer!
<class 'NoneType'>
<class 'function'>
（2）必须在定时器执行函数内部重复构造定时器，因为定时器构造后只执行1次，必须循环调用。
（3）定时器间隔单位是秒，可以是浮点数，如5.5，0.02等，在执行函数fun_timer内部和外部中给的值可以不同。如上例中第一次执行fun_timer是1秒后，后面的都是5.5秒后执行。
（4）可以使用cancel停止定时器的工作，如下例：
\# -*- coding: utf-8 -*-importthreadingimporttime
deffun_timer():
print('Hello Timer!')
globaltimer
timer = threading.Timer(5.5,fun_timer)
timer.start()
timer = threading.Timer(1,fun_timer)
timer.start()
time.sleep(15) \# 15秒后停止定时器
timer.cancel()输出结果：
Hello Timer!
Hello Timer!
Hello Timer!
Process finished with exit code 0





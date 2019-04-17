# 深入浅出带你了解Python2与Python3的异同 - Machine Learning with Peppa - CSDN博客





2018年01月09日 21:35:08[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：299
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)










最近C语言的学习告一段落，为了新的算法学习开始重新捡起python。和两年前不同，这次我选择了最新的3.6而不是2.7。当然，入手的教材都是3的语言描述。为了简单的了解2和3的区别，我在网上翻阅了不少资料，分享给大家。

尽管版本在不断跌进，但是人们总是喜欢怀旧。比如现在还有人在用windows server 2002，尽管已经被微软停止更新，再比如还有人在用Python 2.7版本，尽管Python 3.6版本已经正式释出。Python 3.x引入了一些与Python 2.7不兼容的关键字和特性，在Python 2.7中，使用者其实可以通过内置的__future__模块导入这些新内容。当然，你也可以同时使用2.7和3.6几个版本。


Python3.x引入了一些Python2不兼容的关键字和特性，在Python2中可以通过内建的__future__模块导入。如果你计划对你的代码提供Python3.x支持，推荐使用__future__模块。举例，如果我们想要在Python2中有Python3.x的整数除法，我们可以通过如下导入：

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488355817346.PNG)


__future__模块的更多功能在下表中列出：


特性


可选


强制


作用


nested_scopes


2.1.0b1 2.2 PEP 227:静态嵌套的范围


generators 2.2.0a1 2.3 PEP 255:简单生成器


division 2.2.0a2 3.0 PEP 238:修改除法运算符


absolute_import 2.5.0a1 3.0 PEP 328:导入：多行和绝对/相对


with_statement 2.5.0a1 2.6 PEP 343:“with” 语句


print_function 2.6.0a2 3.0 PEP 3105:打印函数


unicode_literals 2.6.0a2 3.0 PEP 3112:Python 3000字节文本


（源：[https://docs.python.org/2/library/__future__.html](https://docs.python.org/2/library/__future__.html#module-__future__)）

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488363942898.PNG)

print函数


啰嗦了点，print语法的变化可能是最广为人知的更改，但仍值得一提：print语句被print函数取代，表示我们必须用圆括号将想打印的对象引起来。


Python2不需要圆括号，相比之下，Python3以Python2没有圆括号的方式调用print函数会抛出SyntaxError异常。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488378624713.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488402213932.PNG)


注意：


上面Python2打印"Hello, World"的例子看起来很“正常”。但是，如果括号中有多个对象，我们会创建一个元组，因为Python2中的print是语句，不是函数调用。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488437567679.PNG)

整数除法


如果你正在移植代码或在Python2上执行Python3的代码，这种变化特别危险，因为整数除法行为的变化常常不能引起人们的注意（它不抛出SyntaxError异常）。


所以，在我的Python3脚本中，我仍然倾向于使用float(3)/2或3/2.0而不是3/2，这样可以为Python2的小伙伴们省去一些麻烦（反之亦然，我建议在Python2的脚本中使用from __future__ import division）。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488450969986.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488481127756.PNG)

Unicode


Python2有ASCII str()字符串类型，独立的unicode类型，但是没有byte字节类型。


现在，Python3中，我们终于有Unicode(utf-8)字符串和2字节类︰byte和bytearrays。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488506139836.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488529511940.JPEG)

xrange


xrange()的用法在Python2.x中非常流行，用于创建一个迭代对象，例如，在一个for循环或列表/字典解析。


这个行为非常类似于生成器（即“惰性计算”），但在这里的xrange-iterable是不可穷举的——意味着，你可以无限地迭代下去。


由于“惰性计算”，如果你只遍历一次的话（例如，在一个for循环中）xrange()通常是更快的。然而，相比于1次迭代，不建议重复迭代多次，因为每一次迭代都会从头开始。


在Python3中，range()实现了类似xrange()的功能，因此专门的xrange()函数不存在了（在Python3中使用xrange()会抛出一个NameError异常。）

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488542198482.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488560032272.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488584467069.PNG)

Python3中range 对象的__contains__方法


另一个值得一提的是：Python3.x中，range有一个新的__contains__方法（多谢Yuchen Ying指出来）。__contains__方法可以显著地加快Python3.x对整型和布尔类型的查找。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488649011003.PNG)


根据上面的timeit变量结果可以看出，整数类型的查找比浮点数类型的查找快60,000倍。然而，在Python2.x中range和xrange没有__contains__方法，对整型和浮点类型的查找差异不会太大。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488692191635.PNG)


下面__contain__方法的“证明”目前还没有加入到Python2.x中。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488725038935.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488746037864.PNG)

关于Python2和Python3速度差异的说明


一些人指出了Python3 range()和Python2 xrange()的速度差异。因为它们实现了相同的方式，就会有相同的速度。然而，有这样一个事实：Python3通常慢于Python2。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488767624568.PNG)

引发异常


Python2接受“新”、“旧”两种语法格式，而Python3会堵塞（会引发SyntaxError异常），如果我们不将异常参数括在括号里：

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488803942111.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488832982608.PNG)


在Python3中引发异常的正确方式：

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488885738656.PNG)

处理异常


此外在Python3中对异常的处理略有改变。在Python3中，现在我们必须使用"as"关键字。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488907911282.PNG)

next()函数和.next()方法


next()（.next()）是一种常用的函数（方法），这是另一种语法更改（或者说在执行时更改）值得一提：在Python2.7.5中你可以使用next()函数和.next()方法，在Python3只保留了next()函数（调用.next()方法会引发AttributeError异常）。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488928306127.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488949355063.PNG)

For循环变量和全局命名空间泄漏


好消息是：在Python3.x for循环中的变量不会影响全局命名空间了！


这里回头看下Python3.x中的更改和新特性描述，如下所述：


“列表解析不再支持的语法 [... for var in item1, item2, ...]。使用[... for var in (item1, item2, ...)]语法代替。此外请注意，列表解析有不同的语义：他们更接近于list()构造函数内部生成器表达式的语法糖，特别是循环控制的变量不再影响周围范围。”

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660488973964259.PNG)

比较unorderable类型


在Python3中另一个不错的改变是：如果我们尝试比较unorderable类型，会引发TypeError异常。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660489003155954.PNG)

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660489025572256.PNG)

通过input()解析用户输入


幸运的是，input()函数在Python3中被修复了，因此它将用户的输入存储成字符串对象。为了避免在Python2中读取非字符串类型的输入，我们必须使用raw_input()函数。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660489060887353.JPEG)

返回可迭代对象而非列表


正如我们已经在xrange章节看到的，在Python3中，一些函数和方法返回可迭代对象，而不是Python2中的列表。


因为我们通常只遍历一次，我认为这种变化非常合理的节省内存。然而，相比于生成器，如果需要的话遍历多次也是可能的，那么这种情况下就不那么有效了。


对于那些确定需要列表对象的情况，我们可以通过list()函数简单地将可迭代对象转换成列表。

![](http://n1.itc.cn/img8/wb/recom/2016/06/22/146660489076632104.PNG)


一些更常用的函数和方法在Python3中不再返回列表了：


zip()


map()


filter()


字典的.keys()方法


字典的.values()方法


字典的.items()方法


关于Python2和Python3更多文章


这里是一些我推荐的关于Python2和Python3的好文章供后续学习。


// 移植到Python3


Should I use Python 2 or Python 3 for my development activity?


What’s New In Python 3.0


Porting to Python 3


Porting Python 2 Code to Python 3


How keep Python 3 moving forward


// 支持和反对Python3


10 awesome features of Python that you can’t use because you refuse to upgrade to Python 3


Everything you did not want to know about Unicode in Python 3


Python 3 is killing Python


Python 3 can revive Python


Python 3 is fine


英文原文：http://sebastianraschka.com/Articles/2014_python_2_3_key_diff.html


译者：leisants




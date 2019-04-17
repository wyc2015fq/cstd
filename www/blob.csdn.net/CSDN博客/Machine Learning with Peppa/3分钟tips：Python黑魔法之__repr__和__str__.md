# 3分钟tips：Python黑魔法之__repr__和__str__ - Machine Learning with Peppa - CSDN博客





2018年04月01日 21:02:59[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：583标签：[Python																[面向对象																[字符串																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[编程之美：Python](https://blog.csdn.net/qq_39521554/article/category/7392111)





                __repr__和__str__这两个方法都是用于显示的，__str__是面向用户的，而__repr__面向程序员。

打印操作会首先尝试__str__和str内置函数(print运行的内部等价形式)，它通常应该返回一个友好的显示。__repr__用于所有其他的环境中：用于交互模式下提示回应以及repr函数，如果没有使用__str__，会使用print和str。它通常应该返回一个编码字符串，可以用来重新创建对象，或者给开发者详细的显示。


当我们想所有环境下都统一显示的话，可以重构__repr__方法；当我们想在不同环境下支持不同的显示，例如终端用户显示使用__str__，而程序员在开发期间则使用底层的__repr__来显示，实际上__str__只是覆盖了__repr__以得到更友好的用户显示。

下面来看一个例子：
class Person(object):
    def __init__(self, name, gender):
        self.name = name
        self.gender = gender
    def __str__(self):

        return '(Person: %s, %s)' % (self.name, self.gender)

现在，交互式命令行下用 print 试试：
>>> p = Person('Bob', 'male')
>>> print p
(Person: Bob, male)
但是，如果直接敲变量 p：

>>> p
<main.Person object at 0x10c941890>
似乎__str__() 不会被调用。

因为 Python 定义了__str__()和__repr__()两种方法，__str__()用于显示给用户，而__repr__()用于显示给开发人员。


有一个偷懒的定义__repr__的方法：

class Person(object):
    def __init__(self, name, gender):
        self.name = name
        self.gender = gender
    def __str__(self):
        return '(Person: %s, %s)' % (self.name, self.gender)
    __repr__ = __str__](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)





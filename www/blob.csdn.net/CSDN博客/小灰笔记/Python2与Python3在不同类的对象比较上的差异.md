# Python2与Python3在不同类的对象比较上的差异 - 小灰笔记 - CSDN博客





2017年08月16日 23:10:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1394








       之前写了那么多的脚本，没有发现Python2与Python3太大的差异。接触面向对象编程，学习的过程中发现各种不同的差异。

       接下来看一下Python2与Python3在不同类的对象对比上的差异。

       先看一段示范代码：

class myClass:

       def __init__(self,value):

              self.value = value

       def demo(self):

              pass



myObj =myClass('abc')

myIntValue = 4

print(myObj ==myIntValue)

print(myObj >=myIntValue)



a = 'abc'

b = 123

print(a > b)

       最初我用的Python2，我也期待着上面的代码在执行的时候报错。不过，执行的结果却让我觉得有意思，运行居然一切顺利！下面是运行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python compare_different_class.py

False

True

True

       切换到Python3，运行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>pythoncompare_different_class.py

False

Traceback (mostrecent call last):

  File "compare_different_class.py",line 10, in <module>

    print(myObj >= myIntValue)

TypeError: '>='not supported between instances of 'myClass' and 'int'

       从上面的结果可以看出：并不支持不同类的对象之间的比较。这就有点意思了，难不成Python3比Python2的检测要严苛一些？

       再次修改一下代码，依然是一个比较功能的演示：

       其中，Python2中的运行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python compare_different_class.py

False

False

False

       Python3中的运行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>pythoncompare_different_class.py

False

False

False

       这又是一个很有意思的现象，在比较是否相等的时候居然不同类的对象之间能够顺利比较！针对这个现象，我查了一下相关的资料。具体的解释为：如果Python遇到比较的两个类其中的一个没有实现比较方法的时候，Python会尝试交换两个比较参数。这样，只要其中的一个参数具有相应的方法实现就能够进行比较。如果两个都有实现，那么就得进行同类型的比较了。上述情况的原因其实是因为Python2中少了部分实现而导致的。下面看一下两个Python版本中类具有的方法：

       其中，Python2中的信息如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python compare_different_class.py

False

False

False

method for myClass

['__doc__','__init__', '__module__', 'demo']

method for int:

['__abs__','__add__', '__and__', '__class__', '__cmp__', '__coerce__', '__delattr__','__div__', '__divmod__', '__doc__', '__float__', '__floordiv__', '__format__','__getattribute__', '__getnewargs__', '__hash__', '__hex__', '__index__','__init__', '__int__',
 '__invert__', '__long__', '__lshift__', '__mod__','__mul__', '__neg__', '__new__', '__nonzero__', '__oct__', '__or__', '__pos__','__pow__', '__radd__', '__rand__', '__rdiv__', '__rdivmod__', '__reduce__','__reduce_ex__', '__repr__', '__rfloordiv__', '__rlshift__',
 '__rmod__','__rmul__', '__ror__', '__rpow__', '__rrshift__', '__rshift__', '__rsub__','__rtruediv__', '__rxor__', '__setattr__', '__sizeof__', '__str__', '__sub__','__subclasshook__', '__truediv__', '__trunc__', '__xor__', 'bit_length','conjugate', 'denominator',
 'imag', 'numerator', 'real']

       Python3中的信息如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>pythoncompare_different_class.py

False

False

False

method for myClass

['__class__','__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__','__ge__', '__getattribute__', '__gt__', '__hash__', '__init__','__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__','__reduce__', '__reduce_ex__',
 '__repr__', '__setattr__', '__sizeof__','__str__', '__subclasshook__', '__weakref__', 'demo']

method for int:

['__abs__','__add__', '__and__', '__bool__', '__ceil__', '__class__', '__delattr__','__dir__', '__divmod__', '__doc__', '__eq__', '__float__', '__floor__','__floordiv__', '__format__', '__ge__', '__getattribute__', '__getnewargs__','__gt__', '__hash__',
 '__index__', '__init__', '__init_subclass__', '__int__','__invert__', '__le__', '__lshift__', '__lt__', '__mod__', '__mul__', '__ne__','__neg__', '__new__', '__or__', '__pos__', '__pow__', '__radd__', '__rand__','__rdivmod__', '__reduce__', '__reduce_ex__',
 '__repr__', '__rfloordiv__','__rlshift__', '__rmod__', '__rmul__', '__ror__', '__round__', '__rpow__','__rrshift__', '__rshift__', '__rsub__', '__rtruediv__', '__rxor__','__setattr__', '__sizeof__', '__str__', '__sub__', '__subclasshook__','__truediv__', '__trunc__',
 '__xor__', 'bit_length', 'conjugate','denominator', 'from_bytes', 'imag', 'numerator', 'real', 'to_bytes']




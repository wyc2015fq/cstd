# Python对象属性的自动更新 - 小灰笔记 - CSDN博客





2017年09月02日 11:41:32[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1307








            在软件设计中会遇到这样的问题：有些属性之间有相互关联。这样，其中的一个属性变化的时候其他的属性也应该跟随变化。

            先看一段示例代码：

#!/usr/bin/python



class DemoClss:

            def__init__(self,val1,val2):

                        self.val1= val1

                        self.val2= val2

                        self.sum= self.val1 + self.val2



obj = DemoClss(123,234)

print(obj.sum)

obj.val1 = 321

print(obj.sum)

            代码的执行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\09>pythonattr_auto_update.py

357

357

            两个结果一致！其实，这种类似的设计中我们大多时候是希望有一个sum属性跟随者两个数值能够自动变化。但是，上面的这种方式显然是不行的，因为这种设计的生效只能够是在初始化的时候生效。

            那么，如果想要实现一个随时求和的方法我们该怎么做呢？其实，我们可以通过方法来实现！代码的改进如下：

#!/usr/bin/python



class DemoClss:

            def__init__(self,val1,val2):

                        self.val1= val1

                        self.val2= val2

            defsum(self):

                        returnself.val1 + self.val2



obj = DemoClss(123,234)

print(obj.sum())

obj.val1 = 321

print(obj.sum())

            程序的执行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\09>pythonattr_auto_update_1.py

357

555

            这样的改进方式是可行的，但是却总觉得有点不方便。因为总体上的实现并没有体现出OOP的什么优势，说穿了这其实还是函数调用而已。那么，能不能改进一下呢？我们期望的又是一种什么样的效果呢？其实，我们需要的功能应该是一个初始化功能而方法功能的一个结合体，而这种类似功能的结合体是存在的，那就是特性。特性是一种看上去像方法的属性，每次引用都会执行相关的方法。如此，代码改进如下：

#!/usr/bin/python



class DemoClss:

            def__init__(self,val1,val2):

                        self.val1= val1

                        self.val2= val2

            @property

            defsum(self):

                        returnself.val1 + self.val2



obj = DemoClss(123,234)

print(obj.sum)

obj.val1 = 321

print(obj.sum)

            程序执行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\09>pythonattr_auto_update_2.py

357

555

            这样就实现了关联属性的自动更新，看上去更加符合人们的正常思维了。




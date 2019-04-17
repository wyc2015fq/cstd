# Python2与Python3中__bool__方法的差异 - 小灰笔记 - CSDN博客





2017年08月16日 22:24:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1810








       学习Python面向对象编程的时候，遇到了一个很有意思的小问题。Python的__bool__方法不起作用的问题。

       我反复读了我手中的教程，确认了我写的代码应该管用。可是在测试的时候却一直不通过，后来发现我实现的__bool__方法似乎并不是Python本身的接口。

       代码如下：

class Demo():

       def __init__(self,value = 0):

              self.value = value

       def __bool__(self):

              return bool(self.value > 5)



obj = Demo()

obj.value = 0

if obj:

       print("yes")

else:

       print("no")

print(bool(obj))

       这几天使用Win10下面的bash用着比较顺手，我就一直在这个环境中进行程序的调试。测试的结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python bool_demo.py

yes

True

       这多少有点奇怪，居然跟我预期的不一样。尝试直接调用bool方法发现根本不存在之后，我猜测这或许是py2与py3的一点差异。Win10的Linux子系统中python版本是2.X，具体如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python -V

Python 2.7.6

       我在win10上安装的Python版本是3.X，切换CMD只有测试如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>pythonbool_demo.py

no

False



E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>python-V

Python 3.6.0

       最终结果符合预期！

       看起来，这应该是Python3与Python2之间的一点接口上的差异。如果从好用的角度考虑，自然是具有这种接口的Python3更加人性化一些。这会给程序员更大的自由度，也能够实现与Python本身集成度更高的代码。




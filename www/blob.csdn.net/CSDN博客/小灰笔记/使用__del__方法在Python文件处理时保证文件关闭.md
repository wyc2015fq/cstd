# 使用__del__方法在Python文件处理时保证文件关闭 - 小灰笔记 - CSDN博客





2017年08月18日 02:07:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：311
个人分类：[Python																[OOP](https://blog.csdn.net/grey_csdn/article/category/6926551)](https://blog.csdn.net/grey_csdn/article/category/6639459)








       __del__方法的一个很好的用途就是用于文件处理中关闭文件，通过与文件关闭功能的关联让关闭成为一种默认的处理方式。

       在文件处理的程序中通常会含有__del__= close来保证__del__()方法与close()方法的一致。

       这种设计考虑的思想很容易理解，但是到底在什么场景中使用确实是一个比较难考虑的问题。毕竟，这个方法的实现应该是在类中，而相应的功能映射应该也是在这个阶段。

       如果真是用在文件中，似乎不会有这么一个内置的标识吧？为了验证这个猜测，交互界面中测试如下：

In [2]:print(__del__)

---------------------------------------------------------------------------

NameError                                 Traceback(most recent call last)

<ipython-input-2-bfc22615882a>in <module>()

----> 1print(__del__)



NameError: name'__del__' is not defined

       这么看的话，这个东西应该不能够直接在文件脚本中使用，如果纯粹的脚本中使用最多也就是定义了一个相应的句柄量而已。再次进行测试：

In [2]: f =open('demo.c','r')



In [3]: __del__ =f.close



In [4]:print(__del__)

<built-inmethod close of _io.TextIOWrapper object at 0x0000026431CDD120>

       从上面的结果看出，猜测的这些基础的常识性东西也是猜测对的。至于这个方法到底怎么用，看看以后是否能够接触到相关的代码吧！





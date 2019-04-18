# Python中定制类的比较运算方法 - 小灰笔记 - CSDN博客





2017年08月16日 22:39:58[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1214








       Python中的比较运算有几种：小于、小于等于、等于、大于等于、大于、不等于等。如果我们的数据对象具有明确的物理含义，比如说数值是带有数字与物理单位的字符串组合，那么进行大小比较的时候就可以做此定制。

     下面针对6种比较方法中的一种来进行定制示范，选择小于判断来做尝试。写如下示范代码：

class myClass:

       def __init__(self,value):

              self.value = value

       def __lt__(self,other):

              return int(self.value[:-1]) <int(other.value[:-1])





a = myClass('12a')

b = myClass('20a')



print(a < b)

       代码执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python compare.py

True

       从上面看，myClass类的比较方法定制有效。上面例子中，对象的数值是带有单位的，其实是一个数值加单位的字符串。其中，单位就是’a’。如果进行小大比较，可以把单位剥离出去只进行数字的比较。于是，构建出上面的方法。

       其实，其他的几种比较方法定制也与此类似。虽说不进行定制也能够完成我们要的功能，但是如果进行类似的定制我们可以获得可读性以及可维护性更好的软件。




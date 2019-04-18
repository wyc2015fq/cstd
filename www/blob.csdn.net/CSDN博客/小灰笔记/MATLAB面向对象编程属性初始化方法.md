# MATLAB面向对象编程属性初始化方法 - 小灰笔记 - CSDN博客





2017年05月19日 00:38:46[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1799








      在MATLAB面向对象编程的类构建中，属性的初始化一般有两种方法：一种是使用构造函数，另一种则是在定义属性的时候初始化。

      使用构造函数的例子：

classdef DemoClass

properties

        age;

        hight;

end

methods

function obj = DemoClass(a,h)

            obj.age = a;

            obj.hight = h;

end

end

end

      创建对象：

>> D = DemoClass(1,2)



D = 



  DemoClass withproperties:



      age: 1

hight: 2

对象的两个属性数值根据对象创建时传入的参数而进行相应的数值初始化。

      在属性创建时初始化例子：

classdef DemoClass

properties

        age = 123;

        hight = 456;

end

end

      对象创建操作如下：

>> D = DemoClass()



D = 



  DemoClass withproperties:



      age: 123

hight: 456

通过这种方法实现的类进行实例化创建对象的时候，不需要指定属性的初始化数值。属性的初始化数值会在对象创建的同时自动按照属性中的数值产生。




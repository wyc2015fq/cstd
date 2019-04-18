# Python面向对象编程中属性的基本操作 - 小灰笔记 - CSDN博客





2017年08月19日 11:15:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：211








    在面向对象编程中，属性与方法的使用十分频繁。接下来对属性的基础操作进行简单的学习小结。

    在Python OOP中，属性的操作一般有如下几种：

        1，添加属性

        2，修改属性的值

        3，获取属性的值

        4，删除属性

    通过一段简单的示范代码来说明一下：

class DemoClass:

    pass



obj = DemoClass()



# add new attribute

obj.value = 123



# get the value of attribute

print(obj.value)



# modify the value of attribute

obj.value = 456

print(obj.value)



# delete attribute

del obj.value



try:

    print(obj.value)

except:

    print("error")

    在上面的代码中，分别通过注释说明了相应的操作是什么含义。在上面的代码中，简单展示了属性的添加、修改、获取值以及删除几种操作。其中，删除后的属性肯定是不能够访问的，这样通过一个try语句避开了上面可能抛出的错误。

    程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python attr_demo.py

123

456

Error

    从上面的结果，大致做如下小结：

    1，直接给对象加一个属性的赋值操作，如果属性不存在则会创建新的属性；

    2，对于对象的属性赋值操作，如果属性已经存在则会进行修改操作；

    3，属性的数值获取，类似于C语言中的数据结构；

    4，属性删除之后，相应的信息不能够继续引用。




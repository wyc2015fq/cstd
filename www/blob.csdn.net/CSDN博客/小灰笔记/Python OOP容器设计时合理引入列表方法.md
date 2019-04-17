# Python OOP容器设计时合理引入列表方法 - 小灰笔记 - CSDN博客





2017年08月09日 23:25:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：108
个人分类：[OOP																[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)](https://blog.csdn.net/grey_csdn/article/category/6926551)








       面向对象编程的一个最大的优点就在于继承，通过继承可以重用很多已经设计好的功能。接下来，通过简单的一个代码模拟说明一下继承的优点。

       首先，写一段代码如下：

import random



class Demo1():

       def __init__(self):

              self.nums = [i for i in range(10)]

              random.shuffle(self.nums)



       def Pop(self):

              return self.nums.pop()



nums1 = Demo1()

print(nums1.nums)

print(nums1.Pop())

print(nums1.nums)

       在这段代码中，模拟了一个随机数组生成的功能。同时，又给Demo1这个类创建了一个类似列表的Pop功能。程序的执行结果如下：

E:\01_workSpace\02_programme_language\03_python\OOP\2017\08\09>pythonoopListMethod.py

[3, 4, 9, 7, 1, 5,0, 6, 2, 8]

8

[3, 4, 9, 7, 1, 5,0, 6, 2]

       当然，这段代码其实并不复杂。如果进一步简化或者修改也并不能一下子看出在代码构建上的优越性，但是也已经能够窥豹一斑了。

       代码修改如下：

import random



class Demo1(list):

       def __init__(self):

              self.extend([i for i inrange(10)])

              random.shuffle(self)



nums1 = Demo1()

print(nums1)

print(nums1.pop())

print(nums1)

       程序执行结果如下：

E:\01_workSpace\02_programme_language\03_python\OOP\2017\08\09>pythonoopListMethod1.py

[3, 9, 6, 1, 5, 4,7, 2, 8, 0]

0

[3, 9, 6, 1, 5, 4,7, 2, 8]

       从结果上看，这段代码与前一段代码具有相同的效果。但是这段代码更为简单。除此之外，这段代码构建的类不仅仅具有pop这一个功能，还具有append、extend等不同的功能。相比之下，第一种代码构建方式需要类似的功能的时候还得继续做功能的扩充开发。从这个角度上讲，在构建类的时候选择一个合理的基类是很重要的。





# Python中list的extend方法 - 小灰笔记 - CSDN博客





2017年08月09日 22:47:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2103








       List的extend方法是一个扩充列表元素内容的方法，在一定程度上其行为有点像append。只是在接受的参数以及最终的效果上有些差异。

       写如下示范代码：

#!/usr/bin/python



list1 = [1,2,4]

list2 = [12,6]



list1.extend(list2)

print(list1)

       程序执行结果：

E:\01_workSpace\02_programme_language\03_python\OOP>python1_7_3_extend.py

[1, 2, 4, 12, 6]

       从上面的结果可以看出，list1通过extend扩充增加了list2的元素。继续修改代码如下：

#!/usr/bin/python



list1 = [1,2,4]

list2 =[[12,6],123]



list1.extend(list2)

print(list1)

       程序执行结果：

E:\01_workSpace\02_programme_language\03_python\OOP>python1_7_3_extend.py

[1, 2, 4, [12, 6],123]

       如同上面的演示，常见的列表都可以通过这种方式进行扩充。

       修改代码，秀改成一个append用法的脚本：

#!/usr/bin/python



list1 = [1,2,4]

list2 =[[12,6],123]



list1.append(list2)

print(list1)

       程序的执行结果：

E:\01_workSpace\02_programme_language\03_python\OOP>python1_7_3_append.py

[1, 2, 4, [[12,6], 123]]

       从上面的结果可以看出，两者的行为效果有着本质的区别。Extend是把每个元素都作为一个独立的元素扩充到原来的列表，而append则是把整个扩充列表作为一个元素追加到列表最后。

       在看一下两种方法所能够接受的参数，写测试代码如下：

#!/usr/bin/python



list1 = [1,2,3]

list2 = [1,2,3]



list1.extend(3)

       代码执行结果：

E:\01_workSpace\02_programme_language\03_python\OOP>pythonappend_extend.py

Traceback (mostrecent call last):

  File "append_extend.py", line 6, in<module>

    list1.extend(3)

TypeError: 'int'object is not iterable

       从上面的提示可以看出，extend能够接受的参数必须是一个可迭代对象。相比之下，比较熟悉的append则可以接受一个简单的元素，类型任意。

       修改代码如下：

#!/usr/bin/python



list1 = [1,2,3]

list2 = [1,2,3]



list1.extend((3,))

print(list1)

       程序执行结果：

E:\01_workSpace\02_programme_language\03_python\OOP>pythonappend_extend.py

[1, 2, 3, 3]

       从上面的结果可以看出，如果传入的参数是一个一个元素的元组的时候，extend也能够进行列表成员的扩充。参数为列表的时候应该也是如此。




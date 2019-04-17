# python生成器迭代器区别，range与xrange区别，yield - 别说话写代码的博客 - CSDN博客





2019年01月10日 00:36:23[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：48








迭代器：迭代就是循环。**迭代器是可以被next() 函数调用并不断返回下一个值的对象称为迭代器。**

生成器：一边循环一边计算的机制， 优点是节约内存，迭代到下次调用使用的参数使上一次保留的

可于for循环的（统称可迭代对象Iterable,可以用isinstance({}，Iterable)判断，），只能往前不能后退：

一类是集合数据类型，如list,tuple,dict,set,str等

一类是generator，包括生成器(比如a=(x for x in range(10)，括号表示)和带yield的generator function

 生成器不但可以用for，还可以用next





range直接生成一个可迭代的list，xrange 生成器，省内存 



yield生成函数。每次生成一个，从上次生成的位置继续生成

参考：

[https://www.cnblogs.com/tianyiliang/p/7811041.html](https://www.cnblogs.com/tianyiliang/p/7811041.html)

[https://www.cnblogs.com/wj-1314/p/8490822.html](https://www.cnblogs.com/wj-1314/p/8490822.html)




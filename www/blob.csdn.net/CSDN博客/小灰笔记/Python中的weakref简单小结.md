# Python中的weakref简单小结 - 小灰笔记 - CSDN博客





2017年08月18日 01:39:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2909








       当对一个对象的引用只有弱引用的时候，垃圾回收机制可以把孩子个对象在不用了的时候销毁，把内存用于其他地方。

       弱引用其中一个用途是用于实现大对象的缓存或者映射，由于是缓存火映射，对象不需要独立存在。Python weakref中提供WeakKeyDictionary和WeakValueDictionary两个类用于构建非长久驻留内存的对象。当最后一次引用之后，垃圾回收机制会回收其内存。而相应的映射关系也会被删除。这两个类在实现的时候使用了弱引用，并设计了垃圾回收弱引用字典时的回调通知函数。

       并不是所有的对象都可以进行弱引用。

       许多内置的类型，例如列表和字典不能够直接支持弱引用，但是可以通过子类支持。其他的内置类型诸如元组和long，即使通过子类的方式也不能够进行弱引用。

       为了展示简单的weakref功能，写如下代码：

import weakref



class MyClass:

       pass



myObj = MyClass()

myRef =weakref.ref(myObj)

myRefObj = myRef()



print(type(myObj))

print(type(myRef))



print(myRefObj ismyObj)

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python weakref_demo.py

<type'instance'>

<type'weakref'>

True

       从上面看，两个实例对象一致。修改代码如下：

import weakref



class MyClass:

       pass



myObj = MyClass()

myRef =weakref.ref(myObj)

myRefObj = myRef()



print(type(myObj))

print(type(myRef))



print(myRefObj ismyObj)



del myObj,myRefObj

print(myRef())

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python weakref_demo.py

<type'instance'>

<type'weakref'>

True

None

       从上面的结果可以看出，在被引用的对象全都使用完之后，weakref对象也变不再存在。




# 描述符（property）的原理简述 - miner_zhu的博客 - CSDN博客





2018年07月17日 10:03:53[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：130








描述符就是将某种特殊类型的类的实例指派给另一个类的属性。

而这种特殊类型的类就是实现了__get__，__set__,__delete__的新式类(即继承object)。

    __get__(self,instance,owner)用于访问属性，并返回值

    __set__(self,instance,value)将在属性分配操作中调用，不返回值

    __delete__(self,instance)控制删除操作，不返回任何值






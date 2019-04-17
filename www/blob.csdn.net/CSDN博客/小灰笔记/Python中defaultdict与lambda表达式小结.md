# Python中defaultdict与lambda表达式小结 - 小灰笔记 - CSDN博客





2017年07月11日 00:07:57[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3526








       从教程中看到defaultdict是一个类，在一台装有Python2.7.6的电脑上使用发现不存在。在文档中搜索了一下也没有找到，想当然以为这或许是Python 3.X专有的。因为教程就是基于Python 3.X实现的。后来换了一台装有Python 3.X的电脑依然出问题。

       求助于网络，发现这个类其实是collections模块中的一个类。看来，学习很难摆脱网络环境啊！

       这个类是dict的一个子类，重写了一个方法并增加了一个事件变量。在实例化的时候，第一个参数提供给default_factory的初始化函数。这个参数可以是一个类型或者函数，至于是类型这不难理解，其实类型基本上都是工厂函数。但是，有时候我们想用此方法传入一个常量，这个时候就需要自己单独设计一个常量函数或者直接使用lambda表达式。

先看如下的示范：

>>> fromcollections import defaultdict

>>> c1 =defaultdict(int)

>>>c1.get(123)

>>>c1.get('abc')

>>> defConst():

       return 23



>>> c2 =defaultdict(Const)

>>>c2.get(123)

>>> c2

defaultdict(<functionConst at 0x000001D7E26F58C8>, {})

>>>c2[123]

23

>>>c2['abc']

23

>>>c1[123]

0

       从上面可以看出，这种方式可以为一个字典对象不存在的key自动给出一个默认的value。用上面的这种方式自然是可以实现value为某个常量，但是使用lambda可以使得代码更为简洁：

>>> c3 =defaultdict(lambda :123)

>>>c3[12]

123

       在最近遇到的使用场景中，这种方法能够让代码变得简洁不少，而习惯后的代码可读性也没有任何影响。




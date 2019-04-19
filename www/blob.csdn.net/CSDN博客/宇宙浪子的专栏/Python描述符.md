# Python描述符 - 宇宙浪子的专栏 - CSDN博客
2017年12月28日 23:48:38[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：151
转自：[http://python.jobbole.com/81211/](http://python.jobbole.com/81211/)
描述符（Descriptors）是Python语言中一个深奥但却重要的一部分。它们广泛应用于Python语言的内核，熟练掌握描述符将会为Python程序员的工具箱添加一个额外的技巧。为了给接下来对描述符的讨论做一些铺垫，我将描述一些程序员可能会在日常编程活动中遇到的场景，然后我将解释描述符是什么，以及它们如何为这些场景提供优雅的解决方案。在这篇总结中，我会使用新样式类来指代Python版本。
1、假设一个程序中，我们需要对一个对象属性执行严格的类型检查。然而，Python是一种动态语言，所以并不支持类型检查，但是这并不妨碍我们实现自己版本，且较为初级的类型检查。对象属性类型检查的传统方法可能采用下面的方式：

Python
```
```
|123456789|def__init__(self,name,age):ifisinstance(str,name):self.name=nameelse:raiseTypeError("Must be a string")ifisinstance(int,age):self.age=ageelse:raiseTypeError("Must be an int")|
上面是执行这种类型检查的一种方法，但是参数数量增加时它将变得比较繁琐。另外，在赋值之前，我们可以创建一个在__init__中调用的type_check(type, val)函数，但是当我们想在其他地方设置属性值时，该如何简单地实现这种检查呢。我想到的一个快速解决方案是Java中的getters和setters，但是这并不符合Python风格，并且比较麻烦。
2、假设在一个程序中，我们想创建一些在运行时立刻初始化然后变成只读的属性。有人也能想到利用Python中的特殊方法来实现，但这种实现方法仍旧是笨拙和繁琐的。
3、最后，设想一个程序中，我们希望以某种方式自定义对象属性的访问。例如需要记录这种属性的访问。同样的，还是可以想到一个解决方法，即使这种解决方案可能比较笨重并且不可复用。
上述问题因都与属性引用相关而全部联系在了一起。下面，我们将尝试自定义属性的访问方法。
## Python描述符
针对上面所列的问题，描述符提供了优雅、简洁、健壮和可重用的解决方案。简而言之，一个描述符就是一个对象，该对象代表了一个属性的值。这就意味着如果一个账户对象有一个属性“name”，那么描述符就是另一个能够用来代表属性“name”持有值的对象。描述符协议中“定义了__get__”、“__set__”或”__delete__” 这些特殊方法，描述符是实现其中一个或多个方法的对象。这些方法中每一种方法的签名如下所示：python descr.get(self,obj,type=None)->value。

Python
``
|123|descr.__set__(self,obj,value)-->Nonedescr.__delete__(self,obj)-->None|
实现__get__方法的对象是非数据描述符，意味着在初始化之后它们只能被读取。而同时实现__get__和__set__的对象是数据描述符，意味着这种属性是可写的。
为了更好地理解描述符，我们给出针对上述问题基于描述符的解决方法。使用Python描述符实现对象属性的类型检查将是一个非常简单的任务。装饰器实现这种类型检查的代码如下所示：

Python
```
```
|1234567891011121314151617181920212223242526272829303132|classTypedProperty(object):def__init__(self,name,type,default=None):self.name="_"+nameself.type=typeself.default=default ifdefault elsetype()def__get__(self,instance,cls):returngetattr(instance,self.name,self.default)def__set__(self,instance,value):ifnotisinstance(value,self.type):raiseTypeError("Must be a %s"%self.type)setattr(instance,self.name,value)def__delete__(self,instance):raiseAttributeError("Can't delete attribute")classFoo(object):name=TypedProperty("name",str)num=TypedProperty("num",int,42)>>acct=Foo()>>acct.name="obi">>acct.num=1234>>printacct.num1234>>printacct.name obi# trying to assign a string to number fails>>acct.num='1234'TypeError:Must bea<type'int'>|
在这个例子中，我们实现了一个描述符TypedProperty，并且这个描述符类会对它所代表的类的任何属性执行类型检查。注意到这一点很重要，即描述符只能在类级别进行合法定义，而不能在实例级别定义。例如，在上面例子中的__init__方法里。
当访问类Foo实例的任何属性时，描述符会调用它的__get__方法。需要注意的是，__get__方法的第一个参数是描述符代表的属性被引用的源对象。当属性被分配时，描述符会调用它的__set__方法。为了理解为什么可以使用描述符代表对象属性，我们需要理解Python中属性引用解析的执行方式。对于对象来说，属性解析机制在object.__getattribute__()中。该方法将b.x转换成type(b).__dict__[‘x’].__get__(b, type(b))。然后，解析机制使用优先级链搜索属性，在优先级链中，类字典中发现的数据描述符的优先级高于实例变量，实例变量优先级高于非数据描述符，如果提供了getattr()，优先级链会为getattr()分配最低优先级。对于一个给定的对象类，可以通过自定义__getattribute__方法来重写优先级链。
深刻理解优先级链之后，就很容易想出针对前面提出的第二个和第三个问题的优雅解决方案了。那就是，利用描述符实现一个只读属性将变成实现数据描述符这个简单的情况了，即不带__set__方法的描述符。尽管在本例中不重要，定义访问方式的问题只需要在__get__和__set__方法中增加所需的功能即可。
## 类属性
每次我们想使用描述符的时候都不得不定义描述符类，这样看起来非常繁琐。Python特性提供了一种简洁的方式用来向属性增加数据描述符。一个属性签名如下所示：

Python
``
|1|property(fget=None,fset=None,fdel=None,doc=None)->propertyattribute|
fget、fset和fdel分别是类的getter、setter和deleter方法。我们通过下面的一个示例来说明如何创建属性：

Python
```
```
|1234567891011121314|classAccout(object):def__init__(self):self._acct_num=Nonedefget_acct_num(self):returnself._acct_numdefset_acct_num(self,value):self._acct_num=valuedefdel_acct_num(self):delself._acct_numacct_num=property(get_acct_num,set_acct_num,del_acct_num,"Account number property.")|
如果acct是Account的一个实例，acct.acct_num将会调用getter，acct.acct_num = value将调用setter，del acct_num.acct_num将调用deleter。
在Python中，属性对象和功能可以像《[描述符指南](https://docs.python.org/2/howto/descriptor.html)》中说明的那样使用描述符协议来实现，如下所示：

Python
``
|123456789101112131415161718192021222324252627282930313233343536|classProperty(object):"Emulate PyProperty_Type() in Objects/descrobject.c"def__init__(self,fget=None,fset=None,fdel=None,doc=None):self.fget=fgetself.fset=fsetself.fdel=fdelifdocisNoneandfgetisnotNone:doc=fget.__doc__self.__doc__=docdef__get__(self,obj,objtype=None):ifobjisNone:returnselfifself.fgetisNone:raiseAttributeError("unreadable attribute")returnself.fget(obj)def__set__(self,obj,value):ifself.fsetisNone:raiseAttributeError("can't set attribute")self.fset(obj,value)def__delete__(self,obj):ifself.fdelisNone:raiseAttributeError("can't delete attribute")self.fdel(obj)defgetter(self,fget):returntype(self)(fget,self.fset,self.fdel,self.__doc__)defsetter(self,fset):returntype(self)(self.fget,fset,self.fdel,self.__doc__)defdeleter(self,fdel):returntype(self)(self.fget,self.fset,fdel,self.__doc__)|
Python也提供了@ property装饰器，可以用它来创建只读属性。一个属性对象拥有getter、setter和deleter装饰器方法，可以使用它们通过对应的被装饰函数的accessor函数创建属性的拷贝。下面的例子最好地解释了这一点：

Python
```
```
|1234567891011121314151617|classC(object):def__init__(self):self._x=None@property# the x property. the decorator creates a read-only propertydefx(self):returnself._x@x.setter# the x property setter makes the property writeabledefx(self,value):self._x=value@x.deleterdefx(self):delself._x|
如果我们想让属性只读，那么我们可以去掉setter方法。
在Python语言中，描述符有着广泛的应用。Python函数、类方法、静态方法都是非数据描述符的例子。针对列举的Python对象是如何使用描述符实现的问题，《描述符指南》给出了一个基本的描述。
## 扩展阅读
1. [描述符指南](https://docs.python.org/2/howto/descriptor.html)。
2. Python参考手册，第四版，David Beazley
3. [python中使用描述符和装饰器实现缓存](http://www.gghh.name/dibtp/?p=489)
4. [深入新样式类](http://python-history.blogspot.co.uk/2010/06/inside-story-on-new-style-classes.html?m=1)

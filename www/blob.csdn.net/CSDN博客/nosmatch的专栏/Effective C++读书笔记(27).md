# Effective C++读书笔记(27) - nosmatch的专栏 - CSDN博客
2013年07月16日 23:34:54[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：583
条款27：尽量少做转型动作
> 
在C++中尽量的少用类型的转化，只要有类型转化的地方就是有可能会出现问题的地方，在C++中有四种强制类型转化，
如下：
1、static_cast
用法：static_cast<type-id>(expression)
该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
①用于类层次结构中基类和子类之间指针或引用的转换。进行上行转换（把子类的指针或引用转换成基类表示）是安全的；进行下
行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
②用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
③把空指针转换成目标类型的空指针。
④把任何类型的表达式转换成void类型。
注意：static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
2、const_cast
用法：const_cast<type_id>(expression)
该运算符用来修改类型的const或volatile属性。除了const或volatile修饰之外，type_id和expression的类型是一样的。常量指针被转化
成非常量指针，并且仍然指向原来的对象；常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对
象。
3、dynamic_cast
用法：dynamic_cast<type-id>(expression)
该运算符把expression转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void*；如果type-id是类指针类型，那么
expression也必须是一个指针，如果type-id是一个引用，那么expression也必须是一个引用。
dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。在类层次间进行上行转化时，
dynamic_cast和static_cast的效果是一样的；在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。
4、reinterpret_cast
用法：reinterpret_cast<type-id>(expression)
reinterpret_cast是C++里的强制类型转换符。操作符修改了操作数类型，但仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换。
使用注意：
1、static_cast转换的类型有限制，例如不能把int转成指针等。static_cast不能从表达式中去除const、volatile等属性。进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
2、const_cast只用来修改类型的const或volatile属性。
3、dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。有类型检查，失败的转换将返回空指针（当对指针进行类型转换时）或者抛出异常（当对引用进行类型转换时）。
4、reinterpret_cast不进行任何意义的二进制转换。reinterpret_cast只能在指针之间转换。
> 
上面主要的介绍了C++中的四种强制类型转化，下面我们简单的介绍下如果应用强制类型转化会出现什么问题：
很多人可能认为类型转化只是告诉编译器将一个对象类型视为另外一种对象类型，没有做别的处理，其实不然，考虑下面
两种情况：
> 
1.对于基本类型的转化：
```cpp
int x,y;
...
double d = static_cast<double>(x)*y;
```
此时对变量x做了强制类型转化，这个在编译器中肯定是做了一定的处理的因为int和double在底层肯定是不同的实现方式
2.对于继承体系中的类型转化
```cpp
class Base{...};
class Derived:public Base{...};
Derived d;
Base* b = dynamic_cast<Base*>(&d);
```
这里使得父类的只能指向了子类的地址，在很多编译器中会在子类指针身上会有一个偏移量，由于在Derived的指针上面
取得父类的地址！换句话可以这么认为一个对象可能拥有两个地址，一个是Base*指向的地址，一个是Derive*指向的地址！
> 
在C++中我们尽量的通过多种方法来避免类型转化的进行，例如通过virtual函数或者在子类中通过Base:function来直接调
用父类的函数！还有dynamic_cast类型转化的效率是超级的慢的！
> 
C++中的类型转化比C/Java/C#等有更多的坑，因此应用的时候一定要慎重，能不用尽量不要用！
请记住：
- 如果可以尽量的不要使用类型的转化，特别是在注重效率的时候更加要慎重的选择dynamic_cast类型转化符
- 如果必须要使用类型转化，尽量的使用C++上面提到的类型转化，这样如果识别出来类型的转化
- ps:让个人感觉这个条款写的好啰嗦！


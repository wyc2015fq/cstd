
# Python 学习入门（30）—— 多态 - 阳光岛主 - CSDN博客

2013年12月21日 21:55:50[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4080所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



多态性允许将父对象设置成为和一个或多个它的子对象相等的技术，比如Parent~Child，多态性使得能够利用同一类（基类）类型的指针来引用不同类（子类）的对象，以及根据所引用对象的不同，以不同的方式执行相同的操作
**c++中多态更容易理解的概念**
允许父类指针或名称来引用子类对象或对象方法，而实际调用的方法为对象的类类型方法。

python不支持多态，随着对python理解得加深，对python中得多态又有了一些看法。
首先python不支持多态，也不用支持多态，python是一种多态语言，崇尚鸭子类型。以下是维基百科中对鸭子类型得论述：
在程序设计中，鸭子类型（英语：duck typing）是动态类型的一种风格。在这种风格中，一个对象有效的语义，不是由继承自特定的类或实现特定的接口，而是由当前方法和属性的集合决定。这个概念的名字来源于由James Whitcomb Riley提出的鸭子测试，“鸭子测试”可以这样表述：
“当看到一只鸟走起来像鸭子、游泳起来像鸭子、叫起来也像鸭子，那么这只鸟就可以被称为鸭子。”
在鸭子类型中，关注的不是对象的类型本身，而是它是如何使用的。例如，在不使用鸭子类型的语言中，我们可以编写一个函数，它接受一个类型为鸭的对象，并调用它的走和叫方法。在使用鸭子类型的语言中，这样的一个函数可以接受一个任意类型的对象，并调用它的走和叫方法。如果这些需要被调用的方法不存在，那么将引发一个运行时错误。任何拥有这样的正确的走和叫方法的对象都可被函数接受的这种行为引出了以上表述，这种决定类型的方式因此得名。
鸭子类型通常得益于不测试方法和函数中参数的类型，而是依赖文档、清晰的代码和测试来确保正确使用。从静态类型语言转向动态类型语言的用户通常试图添加一些静态的（在运行之前的）类型检查，从而影响了鸭子类型的益处和可伸缩性，并约束了语言的动态特性。
毫无疑问在python中对象也是一块内存，内存中除了包含属性、方法之外，还包含了对象得类型，我们通过引用来访问对象，比如a＝A()，首先python创建一个对象A，然后声明一个变量a，再将变量a与对象A联系起来。变量a是没有类型得，它的类型取决于其关联的对象。a=A()时，a是一个A类型的引用，我们可以说a是A类型的，如果再将a赋值3，a=3，此时a就是一个整型的引用，但python并不是弱类型语言，在python中'2'+3会报错，而在php中'2'+3会得到5。可以这么理解，在python中变量类似与c中的指针，和c不同的是python中的变量可以指向任何类型，虽然这么说不太准确，但是理解起来容易点。
因此，在python运行过程中，参数被传递过来之前并不知道参数的类型，虽然python中的方法也是后期绑定，但是和java中多态的后期绑定却是不同的，java中的后期绑定至少知道对象的类型，而python中就不知道参数的类型。
还引用上次的例子：

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-
#
# ithomer.net
#
class A:  
    def pout(self):  
        print "A"  
  
class B(A):  
    def pout(self):  
        print "B"  
          
class C(A):  
    def pout(self):  
        print "C"  
         
class D(A):  
    pass  
  
class E:  
    def pout(self):  
        print "E"  
  
class F:  
    pass  
  
def test(arg):  
    arg.pout()  
  
a = A()  
b = B()  
c = C()  
d = D()  
e = E()  
f = F()  
  
test(a)  
test(b)  
test(c)  
test(d)  
test(e)  
test(f)
```
输出结果：

```python
A
B
C
A
E
Traceback (most recent call last):
  File "/home/homer/workspace/myPython/com/homer/example.py", line 44, in <module>
    test(f)  
  File "/home/homer/workspace/myPython/com/homer/example.py", line 30, in test
    arg.pout()  
AttributeError: F instance has no attribute 'pout'
```
a，b，c，d都是A类型的变量，所以可以得到预期的效果（从java角度的预期）
e并不是A类型的变量但是根据鸭子类型，走起来像鸭子、游泳起来像鸭子、叫起来也像鸭子，那么这只鸟就可以被称为鸭子，e有pout方法，所以在test方法中e就是一个A类型的变量，f没有pout方法，所以f不是A类型的变量。
以上是从java的角度分析的，其实上边都是一派胡言，只是为了说明python中的运行方法。没有谁规定test方法是接收的参数是什么类型的。test方法只规定，接收一个参数，调用这个参数的pout方法。在运行的时候如果这个参数有pout方法，python就执行，如果没有，python就报错，因为abcde都有pout方法，而f没有，所以得到了上边得结果，这就是python的运行方式。



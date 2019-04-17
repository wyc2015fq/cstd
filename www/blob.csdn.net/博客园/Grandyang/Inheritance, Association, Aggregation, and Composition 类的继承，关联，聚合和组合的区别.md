# Inheritance, Association, Aggregation, and Composition 类的继承，关联，聚合和组合的区别 - Grandyang - 博客园







# [Inheritance, Association, Aggregation, and Composition 类的继承，关联，聚合和组合的区别](https://www.cnblogs.com/grandyang/p/5117658.html)







在C++中，类与类之间的关系大概有四种，分别为继承，关联，聚合，和组合。其中继承我们大家应该都比较熟悉，因为是C++的三大特性继承Inheritance，封装Encapsulation，和多态Polymorphism之一。

继承Inheritance：是指一个类(子类)来继承另一个类(基类)，并增加自己的功能，可以通过重写基类中的函数来实现。可以将继承理解成“IS A”的关系，比如A cat "IS A" animal, or A car "IS A" vehicle.



关联Association：是类之间很弱的一种联系，在C++中表现为一个类作为另一个类方法的参数，比如：

```
class A;
class B;
A::Function1(B &b);
A::Function2(B b);
A::Function3(B *b);
```



聚合Aggregation：指的是整体和部分的关系，通过从别的类中拿来函数来组成一个新类，或在新类中添加其他代码的一个常用的接口Interface。可以将聚合理解成“HAS A”, 比如 A car "HAS A" engine, or A person "HAS A" name.

Inheritance v.s. Aggregation: 我们有时候会犹豫何时该用继承，何时该用聚合，通常来说，当一个接口Interface的部分不需要使用或是应该被修改以避免不合逻辑的情况时，我们需要用聚合。假如我们需要用几乎所有的函数而并不需要太大的改变时，使用继承。



组合Composition：和聚合类似，也是表示整体和部分的关系，但不同的是，整体和部分具有统一的生存期，一旦整体不存在了，部分也就没有了。可以将组合理解成“CONTAIN A", 比如地球和人的关系，假如地球毁灭了，那么人类也就玩完了，这是组合的关系。那么聚合的关系就好比主机和其外设，即使主机挂了，外设也可以连到其他机子上使用。在C++中，比如：

```
class A;
class B { A *a; ... };
class C { A a; ... };
```

如上所示，类A和类B是聚合关系，因为B包含了A的指针，即使B销毁了，A还会在。而类A和类C是组合关系，C包含了A的对象，C一旦销毁，A也就不复存在了。



参考资料：

[http://blog.csdn.net/daxiamit/article/details/6433678](http://blog.csdn.net/daxiamit/article/details/6433678)

[http://stackoverflow.com/questions/269496/inheritance-vs-aggregation](http://stackoverflow.com/questions/269496/inheritance-vs-aggregation)

[http://stackoverflow.com/questions/5543288/inheritance-vs-aggregation-and-has-a-vs-is-a](http://stackoverflow.com/questions/5543288/inheritance-vs-aggregation-and-has-a-vs-is-a)















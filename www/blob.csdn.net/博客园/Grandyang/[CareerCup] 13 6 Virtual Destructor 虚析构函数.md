# [CareerCup] 13.6 Virtual Destructor 虚析构函数 - Grandyang - 博客园







# [[CareerCup] 13.6 Virtual Destructor 虚析构函数](https://www.cnblogs.com/grandyang/p/4924771.html)







13.6 Why does a destructor in base class need to be declared virtual?



这道题问我们为啥基类中的析构函数要定义为虚函数。首先来看下面这段代码：

```
class Foo {
public:
    void f();
};

class Bar: public Foo {
public:
    void f();
};

Foo *p = new Bar();
p->f();
```

调用p->f()会调用基类中的f()，这是因为f()不是虚函数。为了调用派生类中的f()，我们需要在基类中定义其为虚函数。

析构函数是用来清除内存和资源的，如果基类的析构函数不是虚函数，那么当p是派生类的对象时，也会只调用基类的析构函数而无法调用派生类的析构函数。这就是我们要定义析构函数为虚函数的原因，我们要保证每次都调用最新派生类的析构函数。













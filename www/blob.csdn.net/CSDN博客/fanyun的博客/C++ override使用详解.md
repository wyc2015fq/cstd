# C++ override使用详解 - fanyun的博客 - CSDN博客
2018年01月21日 19:15:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4445
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
       C++ override从字面意思上，是覆盖的意思，实际上在C++中它是覆盖了一个方法并且对其重写，从而达到不同的作用。在我们C++编程过程中，最熟悉的就是对接口方法的实现，在接口中一般只是对方法进行了声明，而我们在实现时，就需要实现接口声明的所有方法。还有一个典型应用就是在继承中也可能会在子类覆盖父类的方法。
**      公有继承包含两部分：一是“接口”(interface)，二是 "实现" (implementation)。**
      例如Person类的几种成员函数的继承方式：
```cpp
class Person{
public:
    virtual void Eat() const = 0;    // 1) 纯虚函数
    virtual void Say(const std::string& msg);  // 2) 普通虚函数
    int Name() const;  // 3) 非虚函数
};
class Student: public Person{ ... };
class Teahcer: public Person{ ... };
```
**1.纯虚函数**
纯虚函数，继承的是基类成员函数的接口，必须在派生类中重写该函数的实现：
```cpp
Person *s1 = new Student;
s1->Eat(); // calls Student::Eat
Person *t1 = new Ellipse;
t1->Eat(); // calls Teacher::Eat
```
若想调用基类的 Eat()，须加上 类作用域操作符 ::
```cpp
s1->Person::Eat(); // calls Person::Eat
```
2.普通虚函数
      普通虚函数，对应在基类中定义一个缺省的实现 (default implementation)，表示继承的是基类成员函数的接口和缺省的实现，由派生类自行选择是否重写该函数。
      实际上，允许普通虚函数同时继承接口和缺省实现是危险的。 如下, CarA 和 CarB 是 Car的两种类型，且二者的运行方式完全相同。
```cpp
class Car{
public:
    virtual void Run(const Car& destination);
};
class CarA: public Car{ ... };
class CarB: public Car{ ... };
```
这是典型的面向对象设计，两个类共享一个特性 -- Run，则 Run可在基类中实现，并由两个派生类继承。
  现增加一个新的飞机型号 CarC，其飞行方式与 CarA，CarB 并不相同，假如不小心忘了在 CarC 中重写新的 Fly 函数
```cpp
class CarC: public Car{
    ... // no fly function is declared
};
```
 则调用 CarC 中的 Run 函数，就是调用 Car::Run，但是 CarC的运行方式和缺省的并不相同
```cpp
Car *pa = new CarC;
pa->Run(Beijing); // calls Car::Run!
```
 这就是前面所说的，普通虚函数同时继承接口和缺省实现是危险的，最好是基类中实现缺省行为 (behavior)，但只有在派生类要求时才提供该缺省行为.
        方法一：
       一种方法是 纯虚函数 + 缺省实现，因为是纯虚函数，所以只有接口被继承，其缺省的实现不会被继承。派生类要想使用该缺省的实现，必须显式的调用:
```cpp
class Car{
public:
    virtual void Run(const Run& destination) = 0;
};
void Car::Run(const Airport& destination)
{ 
    // a pure virtual function default code for Run an Car to the given destination
}
class CarA: public Car{
public:
    virtual void Run(const Car& destination) { Car::Run(destination); }
};
```
这样在派生类 CarC 中，即使一不小心忘记重写 Run函数，也不会调用 Car的缺省实现
```cpp
class CarC: public CAr{
public:
    virtual void Run(const Car& destination);
};
void CarC::Run(const Car& destination)
{
    // code for Run a CarC Car to the given destination
}
```
方法二：
可以看到，上面问题的关键就在于，一不小心在派生类 CarC中忘记重写 Run函数，C++11 中使用关键字 override，可以避免这样的“一不小心”。
非虚函数：
非虚成员函数没有virtual关键字，表示派生类不但继承了接口，而且继承了一个强制实现（mandatory implementation），既然继承了一个强制的实现，
则在派生类中，无须重新定义继承自基类的成员函数，如下：
使用指针调用 Name 函数，则都是调用的 Person::Name()
```cpp
Student s1; // s1 is an object of type Student
Person *p= &s1; // get pointer to s1
p->Name(); // call Name() through pointer
Student *s= &s1; // get pointer to s1
s->Name(); // call Name() through pointer
```
  如果在派生类中重新定义了继承自基类的成员函数 Name 呢？
```cpp
class Student : public Person{
public:
    int Name() const; // hides Person::Name
};
p->Name(); // calls Person::Name()
s->Name(); // calls Student::Name()
```
      此时，派生类中重新定义的成员函数会 “隐藏” (hide) 继承自基类的成员函数
      这是因为非虚函数是 “静态绑定” 的，p被声明的是 Person* 类型的指针，则通过 p调用的非虚函数都是基类中的，既使 p指向的是派生类。
      与“静态绑定”相对的是虚函数的“动态绑定”，即无论 p被声明为 Person* 还是 Student* 类型，其调用的虚函数取决于 p实际指向的对象类型
**重写 (override)**
      在程序中加override 关键字，可以避免派生类中忘记重写虚函数的错误
     下面以重写虚函数时，容易犯的四个错误为例，详细阐述之
```cpp
class Base {
public:
    virtual void fun1() const;
    virtual void fun2(int x);
    virtual void fun3() &;
    void fun4() const;    // is not declared virtual in Base
};
class Derived: public Base {
public:
    virtual void fun1();        // declared const in Base, but not in Derived.
    virtual void fun2(unsigned int x);    // takes an int in Base, but an unsigned int in Derived
    virtual void fun3() &&;    // is lvalue-qualified in Base, but rvalue-qualified in Derived.
    void fun4() const;        
};
```
 在派生类中，重写 (override) 继承自基类成员函数的实现 (implementation) 时，要满足如下条件：
  一虚：基类中，成员函数声明为虚拟的 (virtual)
  二容：基类和派生类中，成员函数的返回类型和异常规格 (exception specification) 必须兼容
  四同：基类和派生类中，成员函数名、形参类型、常量属性 (constness) 和 引用限定符 (reference qualifier) 必须完全相同
  如此多的限制条件，导致了虚函数重写如上述代码，极容易因为一个不小心而出错
  C++11 中的 override 关键字，可以显式的在派生类中声明，哪些成员函数需要被重写，如果没被重写，则编译器会报错。
```cpp
class Derived: public Base {
public:
    virtual void fun1() override;
    virtual void fun2(unsigned int x) override;
    virtual void fun3() && override;
    virtual void fun4() const override;
};
```
```cpp
class Derived: public Base {
public:
    virtual void fun1() const override;  // adding "virtual" is OK, but not necessary
    virtual void fun2(int x) override;
    void fun3() & override;
    void fun4() const override; 
};
```
1)  公有继承
　　纯虚函数      => 继承的是：接口 (interface)
　　普通虚函数   => 继承的是：接口 + 缺省实现 (default implementation)
　　非虚成员函数 =>继承的是：接口 + 强制实现 (mandatory implementation)
2)  不要重新定义一个继承自基类的非虚函数 (never redefine an inherited non-virtual function
3)  在声明需要重写的函数后，加关键字 override
这样，即使不小心漏写了虚函数重写的某个苛刻条件，也可以通过编译器的报错，快速改正错误。
        在使用中需要注意以下几点：
(1).覆盖的方法的标志必须要和被覆盖的方法的标志完全匹配，才能达到覆盖的效果；
(2).覆盖的方法的返回值必须和被覆盖的方法的返回一致；
(3).覆盖的方法所抛出的异常必须和被覆盖方法的所抛出的异常一致，或者是其子类；
(4).被覆盖的方法不能为private，否则在其子类中只是新定义了一个方法，并没有对其进行覆盖。

# C++：继承和多态 - guoziqing506的博客 - CSDN博客





2018年07月10日 14:26:03[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：294








C++中类之间的关系，一共分为以下三种：
- has-a：包含关系。表示类A的一个成员是类B，举例来说，比如类`Student`中，有数据成员`score`，而`score`是属于类`Score`的；
- use-a：使用关系。表示类B会用到类A的成员，比如类`State`会用到类`Region`中的成员函数`getArea()`，我们可以通过定义友元类来实现快捷编程；
- is-a：继承关系。表示类B是类A的一个子集，比如类`Student`是类`Person`的一个子集，我们就说类`Student`继承了类`Person`；

本文，我要说的就是is-a这种关系。也叫继承

## 继承的概念

如果类`B`是继承于类`A`的，我们就把类`A`叫做“基类”（也叫父类），而把类`B`叫做“派生类”（也叫“子类”）。一个子类继承了它的父类所有可访问的数据成员和函数，这样，我们在编程时，不需要“白手起家”了，即不需要重头开始定义一个已经定义了的类的子类，大大节省了开发时间。

继承的特点有如下4条：
- 子类拥有父类所有课访问的数据和方法（除了构造函数和析构函数）；
- 子类可以拥有父类没有的属性和方法；
- 子类是本质上也是一种特殊的父类；
- 子类对象可以当做父类对象使用；

其实关于继承的理解，只需要把握一点：子类是父类的特例，父类有的特性，子类都应该有。

## 三种继承的方式

C++中，定义子类的语法形式如下：
`class DeriveClassName : access-lable BaseClassName`
其中，`access-lable`为继承的方式，一共有三种：
- 
公有继承（public）：子类继承了父类的公有成员和保护成员，并作为公有成员。而父类的私有成员仍然是私有的，不能被子类访问。

- 
私有继承（private）：子类继承了父类的公有成员和保护成员，并作为私有成员。而父类的私有成员仍然是私有的，不能被子类访问。

- 
保护继承（protected）：子类继承了父类的公有成员和保护成员，并作为保护成员。而父类的私有成员仍然是私有的，不能被子类访问。


这里公有成员和私有成员就不解释了，稍微说一下保护成员：保护成员用关键字`protected`定义，它的访问权限介于`public`和`private`之间，它可以被类内访问，也可以被友元类或者子类访问。

举个公有方式继承的例子：

```
// Person类是基类
class Person {
public:
...
Private:
...
};

// Student类是派生类，按公有方式继承
class Student：public Person {
public:
...
Private:
...
};
```

从上面说的3种继承方式可以看出，基类的私有成员都不能被派生类访问，此外，3种继承方式的差别在于对于基类的公有成员和保护成员在派生类中的访问模式不同（公有、私有或者保护）。

## 构造函数和析构函数

### 调用构造函数

上面说了，不能继承基类的构造函数和析构函数。但是有些时候，我们是需要在派生类中调用基类的构造函数或者析构函数的。具体的例子可以参照我下面写的代码。基类`Person`的拥有构造函数：`Person(string id, string name)`，因为派生类`Student`也需要设置这两个参数，我们当然希望在派生类`Student`中直接调用这个函数，而不是费劲地在派生类中写设置函数来设置。

怎么做呢？一句话概括，就是在派生类自己的构造函数中调用基类的构造函数。语法形式如下：

```
DeriveClassName(argu_1, argu_2, argu_3) : BaseClassName(argu_1, argu_2) {
    ...
}
```

下面举个例子，大家理解起来更容易。首先写一个头文件，包含基类`Person`和其派生类`Student`的定义：

```
class Person {
public:

    // 默认构造函数，作为基类是必不可少的
    Person() {
        id = "";
        name = "";
    }

    // 自定义构造函数，直接建立Person类的对象时有用
    Person(string id, string name) {
        this -> id = id;
        this -> name = name;
        this -> age = 20;
        this -> gender = 'F';
    }

    // 以下是2个设置函数
    void setAge(int newAge) {
        age = newAge;
    }

    void setGender(char newGender) {
        gender = newGender;
    }

    // 以下是4个访问函数
    string getName() {
        return name;
    }

    string getId() {
        return id;
    }

    int getAge() {
        return age;
    }

    char getGender() {
        return gender;
    };

private:
    string id;
    string name;
    int age;
    char gender;
};

class Student: public Person {
public:

    // 派生类的构造函数，其中调用了基类的构造函数
    Student(string id, string name, int *newScore): Person(id, name) {
        id = id;
        name = name;
        score = newScore;
    };

    // 派生类特有的函数
    double average(int *score, int len) {
        double sum = 0;
        for (int i = 0; i < len; i++) {
            sum += score[i];
        }
        return double(sum / len);
    }

private:
    int *score;
};
```

需要注意：
- 第11行，作为基类，默认构造函数是必不可少的，否则派生类找不到，会报错；
- 第51行表达`Student`是由基类`Person`由公开继承的方式继承。一般情况下，我们都用公开的方式继承，`Private`和`Protected`用的很少；
- 第55行在派生类的构造函数中用了基类的构造函数，而基类的构造函数`Person(id, name)`，这个构造函数直接可以用`id, name`初始化对象；

看看应用的效果：

```cpp
int main() {

    // 生成Person类的对象
    Person p1("001", "Alice");

    // 调用基类的函数，访问并更改私有数据域
    p1.setAge(23);
    cout << p1.getAge() << endl;

    // 生成Student类的对象
    int score[3] = {90, 90, 90};
    Student s1("002", "Bob", score);

    // 派生类对象调用派生类特有的函数
    s1.average(score, 3);

    // 直接调用基类的公开函数
    cout << s1.getGender() << "***" << s1.getAge() << endl;

    return 0;
}
```

需要注意的是，第18行表面的效果是派生类访问了基类的私有数据域，但实际上是用基类公开的访问函数访问的，派生类还是不能直接访问基类的私有数据。

### 构造函数链和析构函数链

这是两个简单的概念，告诉我们派生类对象的构建/销毁对其基类构造/析构函数的调用顺序。
- 当构建一个派生类对象时，其基类的构造函数从“老”到“小”依次被调用；
- 当销毁一个派生类对象时，其基类的析构函数从“小”到“老”依次被调用；

显然，第二条是因为函数被压入了栈中，而栈是“先进后出”的结构。

## 泛型程序设计

泛型程序设计总结起来就是一句话：当程序中需要一个基类对象时，向其提供一个派生类对象时允许的。

还是上面`Person`和`Student`的例子。我现在写一个函数`printName(Person p)`，该函数的形参为`Person`型的对象：

```cpp
void printName(Person p) {
    cout << p.getName();
}
```

现在尝试带入`Student`型的对象`s1`，发现也是可以正常运行的：
`printName(s1); // "Bob"`
## 函数的重定义

有时候，我们需要在派生类中重新定义从基类继承而来的函数，比如上面的例子中，我在定义`Student`时，希望`getId()`函数返回的结果更详细，那么直接可以在`Student`中这样定义：

```
string getId() {
    return "the student id is " + Person::getId();
}
```

注意，如果还要使用基类中这个函数，那么需要声明清楚现在调用的是基类函数，比如我在上面写的`Person::getId()`，告诉计算机此时要用的是`Person`类的`getId()`函数。

## 多态和虚函数

多态在类的继承当中发挥着非常重要的作用。在了解其定义之前，先来看这样一个例子：

我定义了3个类，如下。可以看到类`DeriveClass1`是由`BaseClass`派生的，类`DeriveClass2`是由`DeriveClass1`派生的。这3个类有一个同名的函数`void printID()`。根据上面我说的，可以知道`DeriveClass1`和`DeriveClass2`中的这个函数都是经过重定义的。

```cpp
class BaseClass {
public:
    void printID() {
        cout << "this is BaseClass" << endl;
    }
};

class DeriveClass1: public BaseClass {
    void printID() {
        cout << "this is DeriveClass1" << endl;
    }
};

class DeriveClass2: public DeriveClass1 {
    void printID() {
        cout << "this is DeriveClass2" << endl;
    }
};
```

那么实现的时候可能会产生这样的问题，比如下面的代码：

```
void display(BaseClass o) {
    o.printID();
}


int main() {

    BaseClass o1 = BaseClass();
    DeriveClass1 o2 = DeriveClass1();
    DeriveClass2 o3 = DeriveClass2();

    display(o1); // this is BaseClass
    display(o2); // this is BaseClass
    display(o3); // this is BaseClass

    return 0;
}
```

我们当然是希望第12-14行输出的是3个对象分别对应的类的函数，但是不幸的是这样做都执行的是基类的函数，因为`display()`定义的时候形参就是`BaseClass`型的。那怎样达到这种效果呢？就要用到虚函数了。

虚函数用关键字`virtual`定义，一般定义在基类中，这样，当为一个形参为基类对象指针的函数传入派生类对象的指针时，会自动调用派生类的相关的重定义函数。说的话有点绕，直接看代码吧。先将`BaseClass`如下定义：

```
class BaseClass {
public:
    virtual void printID() {
        cout << "this is BaseClass" << endl;
    }
};
```

然后将`display()`函数这样定义：

```
void display(BaseClass *o) {
    o -> printID();
}
```

再跑一遍主函数，效果就不一样了：

```
int main() {

    BaseClass o1 = BaseClass();
    DeriveClass1 o2 = DeriveClass1();
    DeriveClass2 o3 = DeriveClass2();

    display(&o1); // this is BaseClass
    display(&o2); // this is DeriveClass1
    display(&o3); // this is DeriveClass2

    return 0;
}
```

实际上，当派生类对象的指针传给函数时，函数会动态地确定到底应该使用哪个类（基类或者某个派生类）的重定义的函数。这种动态的使用方式也叫做“多态”。而在派生类中重定义一个虚函数的操作也叫做“函数覆盖”（或者函数重写）。

说道这里，多谈两句关于函数的重载、重定义和重写的区别：
- 重载：同一个函数名，不同形参。调用时根据传入的实参判断到底是哪个函数。需要注意的是，重载函数必须有不同的参数列表（可以是参数的个数，次序或类型不同），而不能仅仅依赖于函数的不同返回类型来重载函数；
- 重定义：在派生类中重新定义从基类继承而来的函数，当使用派生类对象调用函数时，执行这个重定义的函数，而非基类函数；
- 重写：在派生类中重定义一个基类的虚函数，当传入派生类对象的指针，C++会动态判断调用哪个函数；




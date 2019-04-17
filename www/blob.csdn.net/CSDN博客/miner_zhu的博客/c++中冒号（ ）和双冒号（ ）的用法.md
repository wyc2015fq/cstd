# c++中冒号（:）和双冒号（::）的用法 - miner_zhu的博客 - CSDN博客





2018年11月07日 14:03:24[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：25








### 1.冒号（:）用法

（1）表示机构内位域的定义（即该变量占几个bit空间）

```
typedef struct _XXX{

unsigned char a:4;

unsigned char c;

} ; XXX
```

（2）构造函数后面的冒号起分割作用，是类给成员变量赋值的方法，初始化列表，更适用于成员变量的常量const型。

```
struct _XXX{

_XXX() : y(0xc0) {}

};
```

（3） public:和private:后面的冒号，表示后面定义的所有成员都是公有或私有的，直到下一个"public:”或"private:”出现为止。"private:"为默认处理。

（4）类名冒号后面的是用来定义类的继承。

```
class 派生类名 : 继承方式 基类名

{

派生类的成员

};

继承方式：public、private和protected，默认处理是public。
```

### 2．双冒号（::）用法

（1）表示“域操作符”

例：声明了一个类A，类A里声明了一个成员函数void f()，但没有在类的声明里给出f的定义，那么在类外定义f时，

就要写成void A::f()，表示这个f()函数是类A的成员函数。

（2）直接用在全局函数前，表示是全局函数

例：在VC里，你可以在调用API 函数里，在API函数名前加：：

（3）表示引用成员函数及变量，作用域成员运算符

例：System::Math::Sqrt() 相当于System.Math.Sqrt()



原文链接：[http://fengqing888.blog.163.com/blog/static/3301141620100623933512/](http://fengqing888.blog.163.com/blog/static/3301141620100623933512/)




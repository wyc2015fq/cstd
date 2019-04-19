# C++学习（七） C++构造函数与析构函数（二） - HJ - CSDN博客
2017年12月04日 13:00:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1205
个人分类：[C++](https://blog.csdn.net/feizaoSYUACM/article/category/7287457)
所属专栏：[C++学习](https://blog.csdn.net/column/details/18257.html)
构造函数 ，是一种特殊的方法 。主要用来在创建对象时初始化对象， 即为对象成员变量赋初始值，总与new运算符一起使用在创建对象的语句中 。特别的一个类可以有多个构造函数 ，可根据其参数个数的不同或参数类型的不同来区分它们 即构造函数的重载。（摘自百度百科 构造函数 ）。
一、最基本的构造函数
```
class Base{
public:    
    Base(int var) : m_Var(var)
    {
    }
private:
    int m_Var;
};
```
以上构造函数的执行过程：
1）传参   2）给类数据成员开辟空间     3）执行冒号语法给数据成员初始化    4）执行构造函数括号里面的内容
这里需要说明的是：冒号语法后面的内容相当于int a = 10;（初始化），而构造函数括号里面则是相当于是int a; a = 10;（赋初值）
二、拷贝构造函数
```
class Base{
public:
    Base(int var) : m_Var(var)
    {
    }
    //拷贝构造函数
    Base(Base &ref) : m_Var(ref.m_Var)
    {
    }
private:
    int m_Var;
};
```
为什么拷贝构造函数的参数只能用引用呢？
这就要从拷贝构造函数式数码时候触发开始说起了，以下几种情况都会自动调用拷贝构造函数：
1）用一个已有的对象初始化一个新对象的时候
2）将一个对象以值传递的方式传给形参的时候
3）函数返回一个对象的时候
所以当一个对象以传递值的方式传一个函数的时候，拷贝构造函数自动的被调用来生成函数中的对象。如果一个对象是被传入自己的拷贝构造函数，它的拷贝构造函数将会被调用来拷贝这个对象这样复制才可以传入它自己的拷贝构造函数，这会导致无限循环直至栈溢出除了当对象传入函数的时候被隐式调用以外，拷贝构造函数在对象被函数返回的时候也同样的被调用。（摘自百度百科 拷贝构造函数 ）。
拷贝构造函数，一般不需要自己编写，系统默认的拷贝构造函数就能抗住了，但是有些情况需要在构造的时候开辟空间，这时候就需要拷贝构造函数了，如下代码是摘自 林锐 博士的 高质量C++编程指南 一文。
```cpp
class String{
public:
    String(const char *str = NULL); // 普通构造函数
    String(const String &other);    // 拷贝构造函数
    ~ String(void);                 // 析构函数
private:
    char *m_data; // 用于保存字符串
};
// String 的析构函数
String::~String(void) 
{
    delete [] m_data;
    // 由于m_data 是内部数据类型，也可以写成 delete m_data;
}
// String 的普通构造函数
String::String(const char *str) 
{
    if(str==NULL)
    {
        m_data = new char[1]; // 若能加 NULL 判断则更好
        *m_data = '\0';
    }
    else
    {
        int length = strlen(str);
        m_data = new char[length+1]; // 若能加 NULL 判断则更好
        strcpy(m_data, str);
    }
}
// 拷贝构造函数
String::String(const String &other) 
{
    int length = strlen(other.m_data);
    m_data = new char[length+1]; // 若能加 NULL 判断则更好
    strcpy(m_data, other.m_data);
}
```
三、普通派生类构造函数的写法
定义派生类对象的时候，会按如下步骤执行构造操作：
1）传参     2）根据继承时的声明顺序构造基类    3）给类数据成员开辟空间    4）执行冒号语法后面的语句    5）执行构造函数函数体语句
```
class Base{
public:
    Base(int b) : m_b(b)
    {
    }
private:
    int m_b;
};
class Derived : public Base{
public:
    //普通派生类构造函数的写法
    Derived(int b, int d) : Base(b), m_d(d)
    {
    }
private:
    int m_d;
};
```
再写一个多继承的示例：
```
class Base1{
public:
     Base1(int b1) : m_b1(b1)
    {
    }
private:
    int m_b1;
};
class Base2{
public:
    Base2(int b2) : m_b2(b2)
    {
    }
private:
    int m_b2;
};
class Derived : public Base1, public Base2{
public:
    Derived(int b1, int b2, int d) : Base1(b1), Base2(b2), m_d(d)
    { //注意冒号语法后面的顺序无所谓，创造基类是按照上面的继承声明顺序来进行的...
    }
private:
    int m_d;
};
```
四、含有虚继承的派生类构造函数的写法
为何要用到虚继承？
虚继承主要是针对多继承时，出现二义性问题而提出的。比如，如下代码就需要用到虚继承，否则的话Derived类继承时，Base类就会不明确。
虚继承构造函数的执行按照如下步骤：
1）传参 2）创建基类，注意这时候需要显示创建所有“有参构造函数”的基类，包括直接基类，间接基类。 3）给类数据成员开辟空间  4）执行冒号语法  5）执行构造函数函数体
注：你可能会疑惑，如下代码不是将Base间接基类创建了3次吗？其实不是这样的，编译器是这样处理的，当最远的派生类Derived创建了基类Base之后，其直接基类创建Base类的语句将会被忽略掉。
```
class Base{
public:
    Base(int b) : m_b(b)
    {
    }
private:
    int m_b;
};
class Base1 : virtual public Base{
public:
    Base1(int b, int b1) : Base(b), m_b1(b1)
    {
    }
private:
    int m_b1;
};
class Base2 : virtual public Base{
public:
    Base2(int b, int b2) : Base(b), m_b2(b2)
    {
    }
private:
    int m_b2;
};
//虚继承，避免二义性
class Derived : public Base1, public Base2{
public:
    Derived(int b, int b1, int b2, int d) : Base(b), Base1(b, b1), Base2(b, b2), m_d(d)
    { //注意冒号语法后面的顺序无所谓，创造基类是按照上面的继承声明顺序来进行的...
    }
private:
    int m_d;
};
```
五、关于虚析构
虚析构一般伴随着多态而产生，多态主要方式就是用基类的指针或引用指向或引用派生类，而形成多态。
但是这样就会存在一个问题，当我们析构的时候，由于是基类的指针，就会调用的是基类的构造函数，从而造成派生内存溢出。为了解决这个问题，引入了虚析构的概念。将基类的构造函数声明为虚，从而使其在调用析构函数的时候能够准确的调用派生类的析构函数。
如下代码必须用到虚析构才能准确的析构派生类，并释放其占有内存。
```
class Base{
public:
   Base(int b) : m_b(b)
   {
   }
   //虚析构，使基类指针能准确的释放所指向的派生类里面的内容
   virtual ~Base()
   { 
   }
private:
   int m_b;
 };
class Derived : public Base{
public:
   Derived(int b, char *pStr) : Base(b)
   { 
       m_pStr = new char[strlen(pStr)+1];
       strcpy(m_pStr,pStr);
   }
   ~Derived()
   {
       delete m_pStr;
       m_pStr = NULL;
    }
private:
    char *m_pStr;
};
int main(void)
{
    char *pStr = "abcdefg";
    Base *b = new Derived(1,pStr);
    delete b; 
    return 0;
}
```

# c++知识点汇总 - writeeee的专栏 - CSDN博客
2016年08月26日 22:19:34[writeeee](https://me.csdn.net/writeeee)阅读数：328
## 1.static member function(静态成员函数)后不可以加const, 原因是什么？
成员函数后加const 代表不可以修改成员变量 
static　TestClass::TestFunc()  const进过编译器会经过３步转化，转化成非成员函数。  
第一步就是扩张额外参数（即this 指针)，其中加const　的扩张形式如下。 
TestClass::TestFunc(const TestClass * const  this) //**指向常量对象的指针常量。**
扩张过程用用到了this ，静态方法是不可以用this的，因此静态成员后不可以加const
顺便写一下普通函数扩张形式，noconst nostatic 的memeber function 扩张是 
    TestClass::TestFunc( TestClass * const  this) //**指向普通对象的指针常量**
## 2. 多态基类的析构函数为什么要为虚函数。
因为如果baseclass a=new childclass(); 
如果不是虚析构，析构时调用base 的析构函数，导致childclass 的成员不能成功的被析构，造成内存泄露
## 3. 什么时候类成员必须用初始化?
TestClass{ 
        int x; 
        int y; 
    TestClass(int tmpx,int tmpy):x(tmpx),y(tmpy){**//这里就是在初始化**
```
}
```
}
//下边这种是赋值而不是初始化， 
TestClass{ 
        int x; 
        int y; 
    TestClass(int tmpx,int tmpy){//**这里就是在赋值而非初始化**
        x=tmpx; 
        y=tmpy; 
    } 
}
如果为const 或　引用时必须初始化，因为不允许再进行赋值。
## ４. 多态继承类中，必须是先调用基类的构造函数，再构造子类，而析构的过程必须是先析构子类再析构基类
先构造基类，是为了保证继承的成员都能正常访问到 
先析构子类是因为，如果析构了基类，那么子类在析构函数中访问了基类成员就会报错，是不安全的。
## ５．为什么virtual 函数不能是static 函数
因为虚函数的实现是进行晚绑定，需要在构造函数中初始化指向虚函数表的指针，因此必须先执行构造函数，才能调用虚函数。

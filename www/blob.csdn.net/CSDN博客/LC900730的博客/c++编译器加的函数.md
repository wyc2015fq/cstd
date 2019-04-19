# c++编译器加的函数 - LC900730的博客 - CSDN博客
2017年09月27日 10:08:25[lc900730](https://me.csdn.net/LC900730)阅读数：109
## C++默认编写并调用哪些函数
一个empty class经过c++处理之后，编译器会为其声明(编译器版本的)一个copy构造函数、一个copy assignment操作符和一个析构函数。此外，如果没有声明任何构造函数，编译器会声明一个default构造函数。所有这些函数是public 和inline。
`class Empty{}`
等价于
```
class Empty{
    public :
        Empty(){...}
        Empty(const Empty &rhs){...}
        ~Empty(){...}
        Empty& operator=(const Empty &rhs){...}
}
```
只有当这些函数被需要(被调用)，它们才会被编译器创建出来。 
下面代码造成每一个函数被编译产出。
```php
Empty e1;           //default构造函数
Empty e2(e1);       //copy构造函数
e2=e1;              //copy assignment操作符
```
编译器创建的版本只是单纯将来源对象的每一个non-static成员变量拷贝到目标对象。
## 析构函数吐出异常
```
class Widget{
public:
    ...
    ~Widget(){...} 
};
void doSomething(){
    std::vector<Widget>v;
    ...
}
    //v在这里被自动销毁
}
```
当vector被销毁，它有责任销毁其内含有的所有widgets。加入v内含有10个widgets.在析构第一个元素期间，有个异常抛出，其他9个还是Widgets还是应该被销毁。因此v应该调用他们的各个析构函数。
执行析构函数可能出现异常
```
class DBConnection{
    public:
        static DBConnection create();
        void close();
}
//定义一个class来管理DBConnection资源，在析构函数中调用close
class DBConn{
    public :
        ~DBConn(){
            db.close();
        }
    }
    private:
        DBConnection db;
};
```
## 连锁赋值
赋值采用右结合律。 
x=y=z=15；   //赋值连锁形式 
x=（y=（z=15））；//15先被赋给z，然后更新后的z在被赋值给y，然后更新后的y被赋值给x。 
为了实现“连锁赋值”，赋值操作符必须返回一个reference指向操作符号的左侧实参。 
如：
```
class Widget{
public:
    ...
    Widget &operator=(const Widget &rhs){
            ...
            return *this;   //返回左侧对象
    }
    ...
}
```

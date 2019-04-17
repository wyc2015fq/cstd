# C++ 智能指针shared_ptr/unique_ptr自定义删除器 - DumpDoctorWang的博客 - CSDN博客





2019年03月16日 16:42:56[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：19








默认情况下，智能指针使用delete释放其管理的资源，有时候，可能要修改默认使用delete释放资源的行为。本文将列出我所知道的所有自定义删除器的方法。



### 目录
- [零、引例](#_3)
- [一、使用函数](#_41)
- [二、使用可调用类](#_70)
- [三、使用lambda表达式](#lambda_99)
- [四、使用std::function](#stdfunction_113)




# 零、引例

Connection是一个管理连接类，在释放Connection之前，我们需要调用close函数来关闭连接。观察如下代码：

```
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Connection{
public:
    explicit Connection(string name):_name(name){
    }
    string get_name() const {
        return _name;
    }
private:
    string _name;
};

void close(Connection* connection){
    cout << string("关闭")+connection->get_name()+"管理的连接中..." << endl;
    //关闭连接的代码
    // .....
    cout << "关闭完成。" << endl;
}

int main(){
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(new Connection);
    unique_ptr<Connection> up(new Connection);
}
```

执行上述代码，发现并没有办法调用close函数，因为控制权完全在shared_ptr/unique_ptr中。你可能会说，在退出作用域之前我调用`close(sp.get())`先关闭连接，这样不就可以了嘛？实际上，这种做法对于shared_ptr并不安全，手动close之后，不能确保sp管理的Connection只有一份拷贝（即sp中的计数器多于1）。因此，需要使用自定义的删除器。

**为了节省篇幅，后面代码中不在贴出公共的代码。**

# 一、使用函数

删除函数定义类似于：

```
void Deleter(T *val){
    // 其他代码
    // 释放val的内存
    delete val;
    // 或者(如果val是数组)
    delete[] val;
}
```

T是shared_ptr/unique_ptr管理的类型，val是指针，可以指向一个实例，也可以是数组的首地址（参考：[使用shared_ptr/unique_ptr管理数组](https://blog.csdn.net/DumpDoctorWang/article/details/88600780)），取决于shared_ptr/unique_ptr管理的具体内容。

原理是：当删除器的指针`Deleter`传给shared_ptr/unique_ptr时，shared_ptr/unique_ptr不会使用默认的`delete val`来释放其管理的资源，而是使用`Deleter(val)`来释放资源，这样就调用了Deleter来释放管理的资源。后面的各种方式的原理也是如此。

```
// 函数式删除器
void Deleter(Connection *connection){
    close(connection);
    delete connection;
}

int main(){
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(new Connection("shared_ptr"), Deleter);
    unique_ptr<Connection, decltype(Deleter)*> up(new Connection("unique_ptr"), Deleter);
}
```

shared_ptr在使用的时候，只需要把函数式删除器的指针传给构造函数就行；而unique_ptr还用增加一个模板参数`decltype(Deleter)*`，这是shared_ptr和shared_ptr的不同点之一（注意：**unique_ptr的第二个模板参数是指针**）。

# 二、使用可调用类

可调用类是指重载了调用运算符的类。可调用的对象的好处是它也是一个类，可以用来保存一些状态。

```
class DeleterClass{
public:
    DeleterClass():_count(0){}
    /**
     * 重载调用运算符。
     * 这里要定义成模板函数，才可以在unique_ptr中使用。
     */
    template <typename T>
    void operator ()(T *connection){
        close(connection);
        delete connection;
    }
private:
    int _count;
};

int main(){
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(new Connection("shared_ptr"), DeleterClass());
    DeleterClass dc;
    unique_ptr<Connection, DeleterClass> up(new Connection("unique_ptr"), dc);
    unique_ptr<Connection, DeleterClass> up1(new Connection("unique_ptr2"), up.get_deleter());
}
```

**unique_ptr的第二个模板参数是类类型，构造函数的第二个参数是可调用类的实例。**

# 三、使用lambda表达式

目前最简单的自定义删除器（好爽啊，还有如此简洁的代码）。

```
int main(){
    auto DeleterLambda=[](Connection *connection){
        close(connection);
        delete connection;
    };
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(new Connection("shared_ptr"), DeleterLambda);
    unique_ptr<Connection, decltype(DeleterLambda)> up(new Connection("unique_ptr"), DeleterLambda);
}
```

# 四、使用std::function

使用这种形式讲真没有第一种简单，优势在于std::function的特点（比如参数绑定等等）。

```
void Deleter(Connection *connection){
    close(connection);
    delete connection;
}

int main(){
    std::function<void (Connection*)> deleter(Deleter);
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(new Connection("shared_ptr"), deleter);
    unique_ptr<Connection, decltype(deleter)> up(new Connection("unique_ptr"), deleter);
}
```




# C++ 使用智能指针shared_ptr/unique_ptr管理数组 - DumpDoctorWang的博客 - CSDN博客





2019年03月16日 16:41:46[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：32
个人分类：[C++](https://blog.csdn.net/DumpDoctorWang/article/category/7156241)












### 目录
- [零、要管理的类](#_3)
- [一、使用shared_ptr管理数组](#shared_ptr_30)
- [二、使用unique_ptr管理数组](#unique_ptr_61)
- [1、第一种方式](#1_62)
- [2、第二种方式](#2_75)



关于shared_ptr/unique_ptr的基础，我不在本篇博客中赘述。本篇博客主要关注如何安全地使用智能指针来管数组。


# 零、要管理的类

Connection是一个管理连接的类。

```
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Connection{
public:
    Connection():_name(""){}
    Connection(string name):_name(name){
    }
    string get_name() const {
        return _name;
    }
    ~Connection(){
        cout << string("关闭")+get_name()+"管理的连接中..." << endl;
        //关闭连接的代码
        // .....
        cout << "关闭完成。" << endl;
    }
private:
    string _name;
};
```

# 一、使用shared_ptr管理数组

给出一个例子：

```
int main(){
    Connection* c1 = new Connection[2]{string("s1"), string("s2")};
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(c1);
}
```

运行输出：

```bash
关闭s1管理的连接中...
munmap_chunk(): invalid pointer
关闭完成。

Process finished with exit code 134 (interrupted by signal 6: SIGABRT)
```

可以发现，只有s1被正确释放了，s2没有被成功释放。因为shared_ptr默认是使用delete来释放管理的资源，delete只会调用第一个元素的析构函数，所以只有s1被正确析构了（delete[ ] 会依次调用数组元素的析构函数）。要使用shared_ptr来管理数组，就需要需要自定义删除器（[参考链接](https://blog.csdn.net/DumpDoctorWang/article/details/88598015)）。

```
int main(){
    auto Deleter=[](Connection *connection){
        delete[] connection;
    };
    Connection* c1 = new Connection[2]{string("s1"), string("s2")};
    // 新建管理连接Connection的智能指针
    shared_ptr<Connection> sp(c1, Deleter);
}
```

# 二、使用unique_ptr管理数组

## 1、第一种方式

unique_ptr可以像shared_ptr一样使用删除器来释放管理的数组。

```
int main(){
    auto Deleter=[](Connection *connection){
        delete[] connection;
    };
    Connection* c1 = new Connection[2]{string("c1"), string("c2")};
    // 新建管理连接Connection的智能指针
    unique_ptr<Connection, decltype(Deleter)> up(c1, Deleter);
}
```

## 2、第二种方式

unique_ptr重载了管理数组的版本，使用方法如下：

```
int main(){
    Connection* c1 = new Connection[2]{string("c1"), string("c2")};
    // 新建管理连接Connection的智能指针
    unique_ptr<Connection[]> up(c1);
}
```

**模板参数“Connection”后面还有“[]”。** unique_ptr就会自动使用delete[]来释放c1。




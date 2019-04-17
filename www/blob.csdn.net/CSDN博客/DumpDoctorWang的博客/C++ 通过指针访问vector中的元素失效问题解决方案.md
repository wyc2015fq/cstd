# C++ 通过指针访问vector中的元素失效问题解决方案 - DumpDoctorWang的博客 - CSDN博客





2018年04月16日 22:21:09[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：118








本篇博客主要针对vector容器的使用事项做讨论，以如何在多线程中使用vector。

## 一、vector基础

1. [C++vector的使用总结及常用vector操作](https://blog.csdn.net/fanyun_01/article/details/56842637)

2. [C++ 中vector的使用方法](https://blog.csdn.net/duan19920101/article/details/50617190/)

## 二、vector使用注意事项

不能在后面还会改变vector容量的情况下(添加元素、删除元素)，使用指针访问vector里面的元素。 这里原因是当元素的容量增加时，vector的数据存储区容量不够，就会重新申请更大的数据存储区，把原来的数据复制到新的数据存储区，然后释放掉旧的数据存储区，此时，如果有指针指向旧的数据区的数据，这些指针将指向不确定的内容(非法指针)，访问这些指针时就会出错，而且这些错误非常值隐蔽，难以发现。

典型的场景就是就是多线程的应用中将vector作为容器。主线程不断地像vector中添加数据，多个子线程从vector得到指向某个数据的指针，以方便处理完这个数据之后通过指针更新数据。在这种情况下，由于主线程不断的向vector中添加数据，vector就会重新获取新的数据缓冲区，子线程中获取到的指针就指向不确定内容，程序运行就会出错。

观察如下代码：

```cpp
#include <iostream>
#include <vector>

void test() {
    std::vector<int> arr;
    for(int i=0;i<10;i++){
        arr.push_back(i);
    }

    // 获取指向第二个元素的指针
    auto *p = &arr[1]; // int *p = &arr[1];

    //通过vector修改第二个元素
    arr[1] *= 10;
    std::cout << "arr[1]的值为："<< arr[1] << std::endl;
    std::cout << "*p的值为："<< *p << std::endl;
}


int main(){
    test();
}
```

运行上面的代码，输出为：

```cpp
arr[1]的值为：10
*p的值为：10
```

可以看到*p的值和arr[1]的值是一样的。再观察如下代码：

```cpp
#include <iostream>
#include <vector>

void test() {
    std::vector<int> arr;
    for(int i=0;i<10;i++){
        arr.push_back(i);
    }

    // 获取指向第二个元素的指针
    auto *p = &arr[1]; // int *p = &arr[1];
    std::cout << "*p的值为："<<*p << std::endl;

    //增加元素
    for(int i=0;i<20;i++){
        arr.push_back(i);
    }
    std::cout << "*p的值为："<< *p << std::endl;

    //通过vector修改第二个元素
    arr[1] *= 10;
    std::cout << "arr[1]的值为："<< arr[1] << std::endl;
    std::cout << "*p的值为："<< *p << std::endl;
}


int main(){
    test();
}
```

运行上面的代码，输出为：

```cpp
*p的值为：1
*p的值为：0
arr[1]的值为：10
*p的值为：0
```

可以发现，在vector后push了一些元素之后，p已经不是指向arr中第二个元素了，如果仅仅是读取p所指的那块内存区域的值，程序是不会报错的。

要想使用指针访问一个容量会变的vector，安全的方法是使用使用智能指针。可以简单地把智能指针理解为管理指针的一个类，会自动帮你释放指针的内存。智能指针包含std::shared_ptr和std::unique_ptr；std::shared_ptr是可共享的智能指针，可拷贝，也可以作为返回值。std::unique_ptr独占指针，不能拷贝，但也可以作为返回值。在本篇博客中不对智能指针做过多的概述。

为了能通过指针访问vector中的元素，我们可以把容器的类型重新定义为std::vector<std::shared_ptr<int>> arr，然后就可以使用智能指针来访问修改vector中的元素。代码如下：

```cpp
#include <iostream>
#include <vector>
#include <memory>

void test() {
    std::vector<std::shared_ptr<int>> arr;
    for(int i=0;i<10;i++){
        arr.push_back(std::make_shared<int>(i));
    }

    // 获取vector中的第二个元素，是个智能指针。
    auto p = arr[1]; // std::shared_ptr<int> p = arr[1];
    std::cout << "*p的值为："<<*p << std::endl;

    //增加元素
    for(int i=0;i<20;i++){
        arr.push_back(std::make_shared<int>(i));
    }
    std::cout << "*p的值为："<< *p << std::endl;

    //通过vector修改第二个元素
    *arr[1] *= 10;
    std::cout << "arr[1]的值为："<< *arr[1] << std::endl;
    std::cout << "*p的值为："<< *p << std::endl;
}


int main(){
    test();
}
```

运行结果如下：

```cpp
*p的值为：1
*p的值为：1
arr[1]的值为：10
*p的值为：10
```

完全与预期的结果相同。原因是在vector容量发生变化时，拷贝的是shared_ptr的对象，不管shared_ptr被拷贝多少次，所有shared_ptr管理的指针都指向同一个内存地址，故结果是正确的。








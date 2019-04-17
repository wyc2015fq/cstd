# C++类的函数指针成员（有错误，待编辑） - DumpDoctorWang的博客 - CSDN博客





2018年04月17日 12:24:06[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：68
个人分类：[C++](https://blog.csdn.net/DumpDoctorWang/article/category/7156241)









## 一、函数指针基础

1.[C/C++ 函数指针使用总结](http://www.cnblogs.com/lvchaoshun/p/7806248.html)

2.[C++ 函数指针取地址与取值](http://www.cnblogs.com/hellscream-yi/p/7943848.html)

## 二、类的函数指针成员

前面两篇文章中提到，函数指针也是一种指针变量，因此将函数指针指向参数和返回值（也就是函数签名）相同、但实现不同的函数，通过函数指针来调用函数时，就能表现出不同的行为。

通常来将，类的函数指针成员可以指向这三类函数：

1. 普通函数，直接赋值，''函数指针''="函数名"。

2. 类的友元函数，直接赋值，''函数指针''="函数名"。类的友元函数可以访问类的私有变量。

3. 类的静态成员函数，赋值，''函数指针''="类名::函数名".

需要指出的是，类的函数指针无法指向非静态成员。

## 三、例子

这部分给出函数指针指向类的静态函数的例子，为了能在静态函数中访问类的成员，在静态函数中增加一个参数来指向一个实例。



```
/**
 * 1.Array是一个模板类,作用类似于一个vector，但是只实现了部分函数。
 * 最大的特点是在多线程环境下通过指针访问数组的元素的是安全的，不会
 * 像vector一样出现非法指针，详见博客：
 * https://blog.csdn.net/DumpDoctorWang/article/details/79966945
 *
 * 2.设计思想是，每当向Array数组的末尾push一个元素时，不是直接将数
 * 据本身放进数据容器，而是new一块空间，把数据的值放进去，由于new
 * 的空间需要手动释放才会失效，因此通过指针访问数组的元素时，除非手
 * 动释放了空间，都不会出现异常。然后将新分配空间的指针存储在vector
 * 里，这样就可以通过指针访问数组元素。这部分详见push()的实现。
 *
 * 3.这个类实现了一种可以在参数不同的情况下，调用同一个成员函数表现出
 * 不同的行为的代码。具体思想是：
 * 首先定义一个函数指针f，在构造函数中根据参数的不同将f指向本类的参数
 * 相同、实现不同的静态函数。在调用的时候，就通过指针f来调用函数。这样
 * ，在不同参数下，通过调用指针f就表现出不同的行为。一些说明：
 * 为什么定义函数指针f？函数指针可以指向参数和返回值都相同的，但实现不
 * 同的函数，因此，通过指针f调用函数时，就可以表现出不同的行为。
 * 为什么使用静态函数？因为函数指针不能指向非静态成员函数，编译器会报错。
 */

#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <vector>
#include <stdexcept>
#include <mutex>

template<typename T>
class Array {
public:
    /**
     * 构造函数，根据参数的不同使函数指针指向不同的友元函数
     * @param singleThread 为true时指向单线程版本，为false时指向多线程版本
     */
    explicit Array(bool singleThread){
        if(singleThread) {//使函数指针指向单线程版本的函数
            push_agent = Array<T>::push_s;
            ptr_agent = Array<T>::ptr_s;
            get_agent = Array<T>::get_s;
            set_agent = Array<T>::set_s;
        } else{//使函数指针指向多线程版本的函数
            push_agent =  Array<T>::push_m;
            ptr_agent = Array<T>::ptr_m;
            get_agent = Array<T>::get_m;
            set_agent = Array<T>::set_m;
        }
    }
    virtual ~Array() {
        clear();
    }

    /**
     * 向函数末尾添加数据
     * @param elem 数据
     */
    inline void push(T &elem){
        push_agent(this,elem);//通过函数指针来调用函数，对外的接口是统一的
    }

    /**
     * 获取第index个元素的指针
     * @param index 索引
     * @return 指针
     */
    inline T* ptr(std::size_t index) {
        return ptr_agent(this,index);
    }

    /**
     * 获取第index个元素
     * @param index 索引
     * @return 元素
     */
    inline T get(std::size_t index) {
        return get_agent(this,index);
    }

    /**
     * 用elem更新第index个元素
     * @param index 索引
     * @param elem 元素
     */
    inline void set(std::size_t index, T elem) {
        set_agent(this,index,elem);
    }

    /**
     * 清空所有元素，并释放内存
     */
    inline void clear() {
        for (std::size_t i = 0; i < _pointer_vector.size(); i++) {
            delete _pointer_vector[i];
        }
        _pointer_vector.clear();
        _mutex.unlock();
    }

    /**
     * 返回元素个数
     * @return 大于等于0的数
     */
    inline size_t size() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _pointer_vector.size();
    }

private:
    //这部分是函数指针的定义
    void (*push_agent)(Array<T> *me, T& elem);
    T* (*ptr_agent)(Array<T> *me,std::size_t index);
    T (*get_agent)(Array<T> *me,std::size_t index);
    void (*set_agent)(Array<T> *me,std::size_t index, T elem);
private:
    //这部分是单线程版本(_s,single)和多线程版本(_m,multi)静态函数的声明
    static void push_s(Array<T> *me, T& elem);
    static void push_m(Array<T> *me, T& elem);
    static T* ptr_s(Array<T> *me,std::size_t index);
    static T* ptr_m(Array<T> *me,std::size_t index);
    static T get_s(Array<T> *me,std::size_t index);
    static T get_m(Array<T> *me,std::size_t index);
    static void set_s(Array<T> *me,std::size_t index,T elem);
    static void set_m(Array<T> *me,std::size_t index,T elem);
private:
    std::vector<T *> _pointer_vector;
    std::mutex _mutex;
};


template <typename T>
void Array<T>::push_s(Array<T> *me, T &elem){
    auto node = new T;  //分配空间
    (*node) = elem;     //赋值
    me->_pointer_vector.push_back(node);//存下指针
}

template <typename T>
void Array<T>::push_m(Array<T> *me, T &elem) {
    auto node = new T;  //分配空间
    (*node) = elem;     //赋值
    std::lock_guard<std::mutex> lock(me->_mutex);   //加锁
    me->_pointer_vector.push_back(node);            //存下指针
}

template <typename T>
T* Array<T>::ptr_s(Array<T> *me, std::size_t index) {
    if (index < me->_pointer_vector.size()) {
        return me->_pointer_vector[index];
    } else {
        me->clear();//释放内存
        throw std::out_of_range("Array::ptr():index超出范围");
    }
}

template <typename T>
T* Array<T>::ptr_m(Array<T> *me, std::size_t index) {
    std::lock_guard<std::mutex> lock(me->_mutex);//加锁
    return ptr_s(me,index);//调用单线程的程序
}

template <typename T>
T Array<T>::get_s(Array<T> *me, std::size_t index) {
    if (index < me->_pointer_vector.size()) {
        return *(me->_pointer_vector[index]);
    } else {
        me->clear();//释放内存
        throw std::out_of_range("Array::ptr():index超出范围");
    }
}

template <typename T>
T Array<T>::get_m(Array<T> *me, std::size_t index) {
    std::lock_guard<std::mutex> lock(me->_mutex);
    return get_s(me,index);
}

template <typename T>
void Array<T>::set_s(Array<T> *me, std::size_t index,T elem) {
    *(ptr_s(me,index))=elem;
}

template <typename T>
void Array<T>::set_m(Array<T> *me, std::size_t index,T elem) {
    *ptr_m(me,index)=elem;
}


#endif //MY_ARRAY_H
```












# shared_ptr总结 - Likes的博客 - CSDN博客
2018年11月29日 10:30:59[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：23
## 1、基本使用
```cpp
shared_ptr<A> p=make_shared<A>();
```
    上述方法可以满足大多数需求，同时也是最好、最安全的shared_ptr使用方式。
## 2、返回share_ptr形式的this指针
```cpp
class A : public enable_shared_from_this<A>
{
  public:
        shared_ptr<A> GetThis()
        {
            return shared_from_this();
        }
}
```
    原则就是尽量不要和原生指针打交道，所以这种方法返回this是推荐的。但是很多时候容易出抛出bad_weak_ptr的异常，在不明白shared_from_this内部机制的前提下，要解决起来也不是那么容易。
[https://blog.csdn.net/yockie/article/details/40213331](https://blog.csdn.net/yockie/article/details/40213331)
## 3、将原生指针包装成shared_ptr
```cpp
//使用shared_ptr包装this原生指针
shared_ptr<A> ptr(this);
```
   这种方法可以代替2中所述方法，但问题是两个shared_ptr指向了同一块内存，当一个shared_ptr的计数器为0释放以后，另一个shared_ptr悬空。或者导致两次内存释放。
## 4、share_ptr重置内部的原生指针
```cpp
shared_ptr<A> ptr=make_shared<A>();
ptr.reset(new A());
```
## 5、shared_ptr的复制
shared_ptr对象拷贝复制的内部机制就是包含的原生指针类型不变，计数器加1。所以在写包含有shared_ptr成员的class的时候并不用像包含原生指针的class那样创建专门的拷贝构造、移动构造函数。直接按对象浅拷贝share_ptr对象即可，换言之，使用默认的拷贝构造和赋值构造函数即可。

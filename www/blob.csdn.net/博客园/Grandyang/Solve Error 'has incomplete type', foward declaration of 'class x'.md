# Solve Error: 'has incomplete type', foward declaration of 'class x' - Grandyang - 博客园







# [Solve Error: 'has incomplete type', foward declaration of 'class x'](https://www.cnblogs.com/grandyang/p/4788361.html)







在C++的OOB编程中，有时候我们会遇到这样的错误Error: 'has incomplete type'，forward declaration of 'class x',那么是什么原因引起的这个问题呢，我们首先来看下面这段代码：



```
// Error: field '_a' has incomplete type 'A'
// forward declaration of 'class A'
class A;

class B {
public:
    B(A a): _a(a) {}
private:
    A _a;
};

class A {
public:
    A(B b): _b(b) {}
private:
    B _b;
};
```



在上面这段代码中，类A和类B互相含有对方作为自己的私有成员变量，那么不管谁写在前面，如果不事先声明另一个的话，都会报错找不到定义，那么我们事先声明A就没事了吗，也不是，像上面那样B中声明A的对象还是会出错，因为编译器不知道A的定义，无法生成类A的实例，所以会报错。一种改正方法是把对象实例变成对象指针，如下所示：



```
// Correct
class A;

class B {
public:
    B(A *a): _a(a) {}
private:
    A *_a;
};

class A {
public:
    A(B *b): _b(b) {}
private:
    B *_b;
};
```



还有一种解决方法是引入头文件，这样编译器就知道提前知道类A和类B的结构了，虽然不知道具体的实现，但是互相建对象实例是木有问题的~













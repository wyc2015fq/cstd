# [CareerCup] 13.3 Virtual Functions 虚函数 - Grandyang - 博客园







# [[CareerCup] 13.3 Virtual Functions 虚函数](https://www.cnblogs.com/grandyang/p/4922297.html)







13.3 How do virtual functions work in C++?



这道题问我们虚函数在C++中的工作原理。虚函数的工作机制主要依赖于虚表格vtable，即Virtual Table。当类中定义了虚函数时，一个虚表格就建立了用来保存该类的虚函数的地址。此时编译器Compiler也会在该类中增加一个虚指针vptr(Virtual Pointer)，用来指向虚表格。当一个虚函数在派生类中没有被重写时，派生类中的虚表格中仍然存的是基类的虚函数的地址。当虚函数被调用时，就要到虚表格中取找函数地址。C++中的动态绑定机制主要就是通过虚表格来实现的。

当我们将基类的指针指向一个派生类的实体时，虚指针vptr就指向派生类的虚表格，这样就保证了派生类中的虚函数能被调用，参见如下代码：



```
class Shape {
public:
    int edge_len;
    virtual int circumference() {
        cout << "Circumference of Base Class\n";
        return 0;
    }
};

class Triangle: public Shape {
public:
    int circumference() {
        cout << "Circumference of Triangle Class\n";
        return 3 * edge_len;
    }
};

int main() {

    Shape *x = new Shape(); 
    x->circumference(); // "Circumference of Base Class"
    Shape *y = new Triangle();
    y->circumference(); // "Circumference of Triangle Class"
    
    return 0;
}
```















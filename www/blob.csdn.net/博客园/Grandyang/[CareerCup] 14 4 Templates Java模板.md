# [CareerCup] 14.4 Templates Java模板 - Grandyang - 博客园







# [[CareerCup] 14.4 Templates Java模板](https://www.cnblogs.com/grandyang/p/4964173.html)







14.4 Explain the difference between templates in C++ and generics in Java.



在Java中，泛式编程Generic Programming的实现是通过一种就做类型擦除Type Erasure的机制来实现的。当源码转为Java虚拟机JVM的字节代码时擦除参数的类型，例如下面的例子：

```
Vector<String> vector = new Vector<String>();
vector.add(new String("hello"));
String str = vector.get(0);
```

在编译过程中，被重写为：

```
Vector vector = new Vector();
vector.add(new String("hello"));
String str = (String) vector.get(0);
```



这跟C++中有很大的不同。在C++中，模板是一个宏设置Macro Set，编译器对每一个类型的模板代码都创建一份拷贝。验证这一点可以通过以下事实： MyClass<Foo>的一个实例不会跟MyClass<Bar>共享一个静态变量，但是两个MyClass<Foo>之间会共享一个静态变量，参见如下代码：

```
template<class T>
class MyClass {
public:
    static int val;
    MyClass(int v) { val = v; }
};

template<typename T>
int MyClass<T>::val;

class Foo;
class Bar;

int main() {

    MyClass<Foo> *foo1 = new MyClass<Foo>(10);
    MyClass<Foo> *foo2 = new MyClass<Foo>(15);
    MyClass<Bar> *bar1 = new MyClass<Bar>(20);
    MyClass<Bar> *bar2 = new MyClass<Bar>(35);
    
    cout << foo1->val << endl; // will equal 15
    cout << foo2->val << endl; // will equal 15
    cout << bar1->val << endl; // will equal 35
    cout << bar2->val << endl; // will equal 35
    
    return 0;
}
```



而在Java中，静态变量会在所有的MyClass的实例中共享，不论其参数是否相同，参见下列代码：

```
public class Foo {}
public class Bar {}

public static class MyClass<T> {
    public static int val;
    public MyClass(int v) { val = v; }
}

public static void main (String[] args) {
    System.out.println("Hello World!");
    MyClass<Foo> foo1 = new MyClass<Foo>(10);
    MyClass<Foo> foo2 = new MyClass<Foo>(15);
    MyClass<Bar> bar1 = new MyClass<Bar>(20);
    MyClass<Bar> bar2 = new MyClass<Bar>(35);
    
    System.out.println(foo1.val); // will equal 35
    System.out.println(foo2.val); // will equal 35
    System.out.println(bar1.val); // will equal 35
    System.out.println(bar2.val); // will equal 35
}
```



由于架构的不同，Java的泛式编程和C++的模板还有许多不同：

- C++可以使用主要类型，例如int，而Java只能使用Integer

- Java中，你可以限制模板的参数类型为一个确定的类型。例如，你可能使用泛式编程来实现一个CardDeck，限定其参数类型必须从CardGame派生过来。

- C++中的参数类型可以被实例化，而Java中的不行。

- 在Java中，参数类型（例如MyClass<Foo>中的Foo）不能用于静态方法或变量，因为这些会被MyClass<Foo>和MyClass<Bar>所共享。而C++中这些类是不同的，所以可以用于静态方法或变量中。

- 在Java中，MyClass的所有实例，不管其参数类型是什么，都是同一个类型，其参数类型在运行时被擦除了。而C++中，拥有不同参数类型的实例是不同的类型。















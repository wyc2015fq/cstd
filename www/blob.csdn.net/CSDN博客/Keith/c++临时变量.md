# c++临时变量 - Keith - CSDN博客





2017年05月21日 19:42:57[ke1th](https://me.csdn.net/u012436149)阅读数：465








# C++临时变量

“有时候，在求表达式的期间，编译器必须创建临时变量(temporary object)。像其它任何对象一样，它们需要存储空间，并且必须能够构造和销毁。需要注意的是，编译器创建的这个临时变量为常量.” – Thinking in C++

## 什么情况下编译器会创建临时变量

看下面代码：

```
class A{
public:
  print(){
    cout<<"hello world"<<endl;
  }
}
A generateA(){
  A a = A();
  return a;
}

int main(){
  A a = generateA();
  generateA();
}
```

在`A a = generateA()`时，编译器不会创建临时变量，因为在`generateA()`返回之前，就已经把函数里面的`a`对象拷贝给了`main`函数中的`a`对象。

`generateA()`这句会使编译器创建一个临时对象，因为`generateA()`是有返回值的，但是在`main`函数中并没有对象来收留它，所以编译器会创建一个临时对象来收留它，为可能的后续操作做准备。例如：`generateA().print()`。




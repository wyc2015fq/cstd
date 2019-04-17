# [CareerCup] 13.5 Volatile Keyword 关键字volatile - Grandyang - 博客园







# [[CareerCup] 13.5 Volatile Keyword 关键字volatile](https://www.cnblogs.com/grandyang/p/4924765.html)







13.5 What is the significance of the keyword "volatile" in C



这道题考察我们对于关键字volatile的理解，顾名思义，volatile有易变的易挥发的意思，在C/C++里，表示告知编译器某个变量可能会由程序外部改变，比如操作系统，硬件或者其他的线程。由于变量会由无法预期的改变，所有编译器每次都需要从内存中读取变量值。我们可以如下定义一个整型变量为volatile：

```
int volatile x;
volatile int x;
```

当然我们也可以定义一个指针变量：

```
volatile int *x;
int volatile *x;
```

volatile 的变量不会被优化，这很有用，看下面的代码：

```
int opt = 1;
void Fn(void) {
    start:
        if (opt == 1) goto start;
        else break;
}
```

乍看上去，上面的程序是死循环的，编译器会自动进行如下优化：

```
void Fn(void) {
    start:
        int opt = 1;
        if (true) goto start;
        else break;
}
```

这样的确是个死循环，但是外部的操作想要给局部变量opt赋值0，这样可以跳出循环。

为了防止编译器进行上述的优化，我们想要从外部改变参数值，从而控制循环。我们通过加volatile关键字来做：

```
volatile int opt = 1;
void Fn(void) {
    start:
        if (opt == 1) goto start;
        else break;
}
```

votlatile变量在多线程程序中有全局变量且每个线程可以改变共享值时非常有用，我们不想这些共享变量被优化。















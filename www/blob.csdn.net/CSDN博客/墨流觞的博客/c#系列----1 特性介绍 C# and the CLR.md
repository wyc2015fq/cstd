# c#系列----1 特性介绍  C# and the CLR - 墨流觞的博客 - CSDN博客





2018年10月30日 21:40:52[墨氲](https://me.csdn.net/dss_dssssd)阅读数：27








encapsulation : 封装

inheritance: 继承

polymorphism: 多态性
- 
unified type system :

type: an encapsulated unit of data and functions（类似于c++的类对象）

in C#, all types ultimately share a commom base type , this means that all types share the same basic functionality.（所有的类型共同继承于一个基类，即对于基本的特性，所有的类型都相同。比如都具有toString方法）

- 
classes and interfaces

interface：  对于函数成员，只给出定义，对于该成员的功能实现，交付实现（implement）该接口的类完成。类似于c++的抽象类（abstract class）

C#不支持多继承， 因而需要类似多种继承功能的实现只能依靠接口（interface）实现。

- 
properties, methods, and events

methods: 是一种成员（member），包含属性和事件

property: 也是成员，封装对象的状态，比如按钮的颜色，文本的属性

events: 当对象状态改变时，该成员会响应


## c#支持函数式编程。

## Type Safety(类型安全)：

这意味着在C#中，类型的实例只能通过他们预定义的协议进行交互，。比如string类型的实例不能与integer类型的实例交互（做加减法，赋值等）。

c#更偏向于静态语言，和c++类似，变量名之前需要有类型声明`String a = 'dkjs'`,而与动态语言python有很大的不同；这在很大程度上能够通过预编译在程序运行之前消除大量错误。当然c通过关键字*dynami*c#也支持动态编写代码,但主要还是静态类型

与c++相同，C#是一种强类型语言（strong typed language），比如如果函数参数为整型，而传入的是浮点型（floating-point）,程序将报错。

## Memory Management

与c++不同的是，C# 在运行时会自动进行内存管理，通过垃圾收集器能够自动回收内存中不在使用的变量。这将不在需要程序员手动释放对象，python，java也都有类似的垃圾回收机制

注意：C# does not eliminate pointers: it merely makes them unnecessary for most programming

tasks。（大概的意思是，C#并不会释放指针指针仍然存在，会造成资源损耗），因而在对性能要求很高的程序中，可以通过视同unsafe代码块中直接操作内存指针

```
unsafe static void Main(string[] args){ 

unsafe
{
//代码块
}
```

C#，java，python严格上没有指针的概念，有的是C++引用的概念

## platform support(平台支持性)

主要运行于windows但是是跨平台的（linux， macos， android， ios）

## C# and the CLR

**关于C#和CLR的解释：[http://developer.51cto.com/art/200910/158022.htm](http://developer.51cto.com/art/200910/158022.htm)需要看一下**

C# 依赖于一个运行时（runtime），该运行时具有自动内存管理和异常处理（automatic memory management and exception handling）的特性，而在微软的.NET框架中，Common Language Runtime（CLR）提供了这些运行时特性（runtime features）。注意CLR是独立于语言之外的，支持在多种语言下进行应用开发（比如 C#,F#, VB, Managed C++）

C#会被编译为托管代码（Managed code）， 托管代码用中间语言（Intermediate Language ，IL）表示.CLR通常会在代码执行之前将中间语言转化编译为机器代码，以供操作系统执行，比如X86和X64，这称为即时编译（just-in-time compilation, JIT）.在资源受限的设备（ios）上，提前编译可以缩短启动时间。这中技术在java和python中也有使用。

托管代码的容器称为程序集或可移动可执行体（assembly or portable executable），程序集（assembly）可以使可执行文件（.exe）或库（.dll）,包含IL和类型信息（metadata， 元数据）；元数据的存在使的程序集可以不需要其他的文件而直接引用另一个程序集的类型。
**c#和c/c++编译生成的exe文件的区别:**
[https://blog.csdn.net/sjjbupt/article/details/8124438](https://blog.csdn.net/sjjbupt/article/details/8124438)




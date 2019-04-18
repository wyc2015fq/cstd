# 从汇编层面深度剖析 C++ 虚函数 - 文章 - 伯乐在线
原文出处： [海枫，2011-04-20](http://blog.csdn.net/linyt/article/details/6336762)
虚函数是C++语言实现运行时多态的唯一手段，因此掌握C++虚函数也成为C++程序员是否合格的试金石。csdn网友所发的一篇博文[《VC虚函数布局引发的问题》](http://blog.csdn.net/zhanglei8893/archive/2011/04/19/6333751.aspx) 从汇编角度分析了对象虚函数表的构，以及C++指针或者引用是如何利用这个表来实现运行时多态。
诚然，C++虚函数的结构会因编译器不同而异，但所使用的原理是一样的。为此，本文使用linux平台下的g++编译器，试图从汇编的层面上分析虚函数表的结构，以及如何利用它来实现运行时多态。
汇编语言是难读的，特别是对一些没有汇编基础的朋友，因此，本文将汇编翻译成相应的C语言，以方便读者分析问题。
**1. 代码**
为了方便表述问题，本文选取只有虚函数的两个类，当然，还有它的构造函数，如下：


```
class Base      
{     
 public:     
    virtual void f() { }     
    virtual void g() { }     
};     
class Derive : public Base     
{     
  public:  
    virtual void f() {}  
};     
int main()     
{     
  Derive d;     
  Base *pb;  
  pb = &d;   
  pb->f();  
  return 0;  
}
```
**2. 两个类的虚函数表(vtable)**
使用g++ –Wall –S test.cpp命令，可以将上述的C++代码生成它相应的汇编代码。


```
ZTV4Base:  
    .long   0    
    .long   _ZTI4Base  
    .long   _ZN4Base1fEv  
    .long   _ZN4Base1gEv  
    .weak   _ZTS6Derive  
    .section    .rodata._ZTS6Derive,"aG",@progbits,_ZTS6Derive,comdat  
    .type   _ZTS6Derive, <a href="http://www.jobbole.com/members/anduo1989">@object</a>  
    .size   _ZTS6Derive, 8
```
_ZTV4Base是一个数据符号，它的命名规则是根据g++的内部规则来命名的，如果你想查看它真正表示C++的符号名，可使用c++filt命令来转换，例如：


```
[lyt@t468 ~]$ c++filt _ZTV4Base 
vtable for Base
```
_ZTV4Base符号(或者变量)可看作为一个数组，它的第一项是0，第二项_ZIT4Base是关于Base的类型信息，这与typeid有关。为方便讨论，我们略去此二项数据。 因此Base类的vtable的结构，翻译成相应的C语言定义如下：


```
unsigned long Base_vtable[] = {  
    &Base::f(),  
    &Base::g(),  
};
```
而Derive的更是类似，只有稍为有点不同：


```
ZTV6Derive:  
    .long   0    
    .long   _ZTI6Derive  
    .long   _ZN6Derive1fEv  
    .long   _ZN4Base1gEv  
    .weak   _ZTV4Base  
    .section    .rodata._ZTV4Base,"aG",@progbits,_ZTV4Base,comdat  
    .align 8  
    .type   _ZTV4Base, <a href="http://www.jobbole.com/members/anduo1989">@object</a>  
    .size   _ZTV4Base, 16
```
相应的C语言定义如下：


```
unsigned long Derive_vtable[] = {  
    &Derive::f(),  
    &Base::g(),  
};
```
从上面两个类的vtable可以看到，Derive的vtable中的第一项重写了Base类vtable的第一项。只要子类重写了基类的虚函数，那么子类vtable相应的项就会更改父类的vtable表项。 这一过程是编译器自动处理的，并且每个的类的vtable内容都放在数据段里面。
**3. 谁让对象与 vtable 绑到一起**
上述代码只是定义了每个类的vtable的内容，但我们知道，带有虚函数的对象在它内部都有一个vtable指针，指向这个vtable，那么是何时指定的呢？ 只要看看构造函数的汇编代码，就一目了然了：


```
_ZN4BaseC1Ev:  
.LFB6:  
    .cfi_startproc  
    .cfi_personality 0x0,__gxx_personality_v0  
    pushl   %ebp  
    .cfi_def_cfa_offset 8  
    movl    %esp, %ebp  
    .cfi_offset 5, -8  
    .cfi_def_cfa_register 5  
    movl    8(%ebp), %eax  
    movl    $_ZTV4Base+8, (%eax)  
    popl    %ebp  
    ret  
    .cfi_endproc
```
Base::Base()函数的编译代码如下：
ZN4BaseC1Ev这个符号是C++函数Base::Base() 的内部符号名，可使用c++flit将它还原。C++里的class，可以定义数据成员，函数成员两种。但转化到汇编层面时，每个对象里面真正存放的是数据成员，以及虚函数表。
在上面的Base类中，由于没有数据成员，因此它只有一个vtable指针。故Base类的定义，可以写成如下相应的C代码：


```
struct Base {  
    unsigned long **vtable;  
}
```
构造函数中最关键的两句是：


```
movl    8(%ebp), %eax 
    movl    $_ZTV4Base+8, (%eax)
```
$_ZTV4Base+8 就是Base类的虚函数表的开始位置，因此，构造函数对应的C代码如下：


```
void Base::Base(struct Base *this)  
{  
    this->vtable = &Base_vtable;  
}
```
同样地，Derive类的构造函数如下：


```
struct Derive {  
    unsigned long **vtable;  
};  
void Derive::Derive(struct Derive *this)  
{  
    this->vtable = &Derive_vtable;  
}
```
**4. 实现运行时多态的最关键一步**
在造构函数里面设置好的vtable的值，显然，同一类型所有对象内的vtable值都是一样的，并且永远不会改变。下面是main函数生成的汇编代码，它展示了C++如何利用vtable来实现运行时多态。


```
.globl main  
    .type   main, @function  
main:  
.LFB3:  
    .cfi_startproc  
    .cfi_personality 0x0,__gxx_personality_v0  
    pushl   %ebp  
    .cfi_def_cfa_offset 8  
    movl    %esp, %ebp  
    .cfi_offset 5, -8  
    .cfi_def_cfa_register 5  
    andl    $-16, %esp  
    subl    $32, %esp  
    leal    24(%esp), %eax  
    movl    %eax, (%esp)  
    call    _ZN6DeriveC1Ev  
    leal    24(%esp), %eax  
    movl    %eax, 28(%esp)  
    movl    28(%esp), %eax  
    movl    (%eax), %eax  
    movl    (%eax), %edx  
    movl    28(%esp), %eax  
    movl    %eax, (%esp)  
    call    *%edx  
    movl    $0, %eax  
    leave  
    ret   
    .cfi_endproc
```

```
andl    $-16, %esp 
subl    $32, %esp
```
这两句是为局部变量d和bp在堆栈上分配空间，也即如下的语句：


```
Derive d;   
Base *pb;
leal    24(%esp), %eax 
movl    %eax, (%esp) 
call    _ZN6DeriveC1Ev
```
esp+24是变量d的首地址，先将它压到堆栈上，然后调用d的构造函数，相应翻译成C语言则如下：


```
Derive::Dervice(&d);
leal    24(%esp), %eax 
movl    %eax, 28(%esp)
```
这里其实是将&d的值赋给pb，也即：


```
pb = &d;
```
最关键的代码是下面这一段：


```
movl    28(%esp), %eax  
movl    (%eax), %eax  
movl    (%eax), %edx  
movl    28(%esp), %eax  
movl    %eax, (%esp)  
call    *%edx
```
翻译成C语言也就传神的那句：


```
pb->vtable[0](bp);
```
编译器会记住f虚函数放在vtable的第0项，这是编译时信息。
**5. 小结**
这里省略了很多关于编译器和C++的细枝未节，是出于讨论方便用的需要。从上面的编译代码可以看到以下信息：
1.每个类都有各有的vtable结构，编译会正确填写它们的虚函数表
2. 对象在构造函数时，设置vtable值为该类的虚函数表
3.在指针或者引用时调用虚函数，是通过object->vtable加上虚函数的offset来实现的。
当然这仅仅是g++的实现方式，它和VC++的略有不同，但原理是一样的。

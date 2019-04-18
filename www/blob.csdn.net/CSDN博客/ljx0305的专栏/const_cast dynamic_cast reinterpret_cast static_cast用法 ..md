# const_cast dynamic_cast reinterpret_cast static_cast用法 . - ljx0305的专栏 - CSDN博客
2012年06月20日 11:55:30[ljx0305](https://me.csdn.net/ljx0305)阅读数：537标签：[class																[编译器																[delete																[object																[float																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
# const_cast
**用法**：const_cast<type_id> (expression)
　　该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
　　常量指针被转化成非常量指针，并且仍然指向原来的对象；
　　常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
　　Voiatile和const类试。举如下一例：
　　class B{
　　public:
　　int m_iNum;
　　}
　　void foo(){
　　const B b1;
　　b1.m_iNum = 100; //comile error
　　B b2 = const_cast<B>(b1);
　　b2. m_iNum = 200; //fine
　　}
　　上面的代码编译时会报错，因为b1是一个常量对象，不能对它进行改变；
　　使用const_cast把它转换成一个常量对象，就可以对它的数据成员任意改变。注意：b1和b2是两个不同的对象。
# dynamic_cast
**用法**：dynamic_cast < type-id > ( expression )
　　该运算符把expression转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void *；
　　如果type-id是类指针类型，那么expression也必须是一个指针，如果type-id是一个引用，那么expression也必须是一个引用。
　　dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
　　在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
　　在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。
　　class B{
　　public:
　　int m_iNum;
　　virtual void foo();
　　};
　　class D:public B{
　　public:
　　char *m_szName[100];
　　};
　　void func(B *pb){
　　D *pd1 = static_cast<D *>(pb);
　　D *pd2 = dynamic_cast<D *>(pb);
　　}
　　在上面的代码段中，如果pb指向一个D类型的对象，pd1和pd2是一样的，并且对这两个指针执行D类型的任何操作都是安全的；
　　但是，如果pb指向的是一个B类型的对象，那么pd1将是一个指向该对象的指针，对它进行D类型的操作将是不安全的（如访问m_szName），
　　而pd2将是一个空指针。
　　另外要注意：B要有虚函数，否则会编译出错；static_cast则没有这个限制。
　　这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表（
　　关于虚函数表的概念，详细可见<Inside c++ object model>）中，只有定义了虚函数的类才有虚函数表，
　　没有定义虚函数的类是没有虚函数表的。
　　另外，dynamic_cast还支持交叉转换（cross cast）。如下代码所示。
　　class A{
　　public:
　　int m_iNum;
　　virtual void f(){}
　　};
　　class B:public A{
　　};
　　class D:public A{
　　};
　　void foo(){
　　B *pb = new B;
　　pb->m_iNum = 100;
　　D *pd1 = static_cast<D *>(pb); //compile error
　　D *pd2 = dynamic_cast<D *>(pb); //pd2 is NULL
　　delete pb;
　　}
　　在函数foo中，使用static_cast进行转换是不被允许的，将在编译时出错；而使用 dynamic_cast的转换则是允许的，结果是空指针。
# **reinterpret_cast**
**reinterpret_cast**是C++里的强制类型转换符。
　　操作符修改了操作数类型,但仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换。
　　例如：int *n= new int ;
　　double *d=**reinterpret_cast**<double*> (n);
　　在进行计算以后, d 包含无用值. 这是因为 **reinterpret_cast** 仅仅是复制 n 的比特位到 d, 没有进行必要的分析。
　　因此, 需要谨慎使用 **reinterpret_cast**.
　　并且：**reinterpret_cast** 只能在指针之间转换。
　　== ===========================================
　　== static_cast .vs. **reinterpret_cast**
　　== ================================================
**reinterpret_cast**是为了映射到一个完全不同类型的意思，这个关键词在我们需要把类型映射回原有类型时用到它。我们映射到的类型仅仅是为了故弄玄虚和其他目的，这是所有映射中最危险的。(这句话是C++编程思想中的原话)
　　static_cast 和 **reinterpret_cast** 操作符修改了操作数类型。它们不是互逆的； static_cast 在编译时使用类型信息执行转换，在转换执行必要的检测(诸如指针越界计算, 类型检查). 其操作数相对是安全的。另一方面；**reinterpret_cast**
 仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换， 例子如下：
　　int n=9; double d=static_cast < double > (n); 
　　上面的例子中, 我们将一个变量从 int 转换到 double。 这些类型的二进制表达式是不同的。 要将整数 9 转换到 双精度整数 9，static_cast 需要正确地为双精度整数 d 补足比特位。其结果为 9.0。而**reinterpret_cast** 的行为却不同:
　　int n=9; 
　　double d=**reinterpret_cast**<double & > (n);
　　这次, 结果有所不同. 在进行计算以后, d 包含无用值. 这是因为 **reinterpret_cast** 仅仅是复制 n 的比特位到 d, 没有进行必要的分析. 
　　因此, 你需要谨慎使用 **reinterpret_cast**.
# static_cast
**用法**：static_cast < type-id > ( expression )
　　该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种**用法**：
　　①用于类层次结构中基类和子类之间指针或引用的转换。
　　进行上行转换（把子类的指针或引用转换成基类表示）是安全的；
　　进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
　　②用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
　　③把空指针转换成目标类型的空指针。
　　④把任何类型的表达式转换成void类型。
　　注意：static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
　　C++中static_cast和**reinterpret_cast**的区别
　　C++primer第五章里写了编译器隐式执行任何类型转换都可由static_cast显示完成;**reinterpret_cast**通常为操作数的位模式提供较低层的重新解释
　　1、C++中的static_cast执行非多态的转换，用于代替C中通常的转换操作。因此，被做为隐式类型转换使用。比如： 
　　int i; 
　　float f = 166.7f; 
　　i = static_cast<int>(f); 
　　此时结果，i的值为166。 
　　2、C++中的**reinterpret_cast**主要是将数据从一种类型的转换为另一种类型。所谓“通常为操作数的位模式提供较低层的重新解释”也就是说将数据以二进制存在形式的重新解释。比如：
　　int i; 
　　char *p = "This is a example."; 
　　i = **reinterpret_cast**<int>(p);
　　此时结果，i与p的值是完全相同的。**reinterpret_cast**的作用是说将指针p的值以二进制（位模式）的方式被解释为整型，并赋给i，一个明显的现象是在转换前后没有数位损失。
引用:[http://blog.csdn.net/public/article/details/4944505](http://blog.csdn.net/public/article/details/4944505)

# Qt智能指针学习总结 - fanyun的博客 - CSDN博客
2017年06月30日 19:58:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：398
##    我们从堆中申请、释放内存时，必须使用指针。构建复杂的数据结构时，也离不开指针。但是在指针的使用时，我们可能不知道一个指针所指的对象已经被析构，从而错误地通过该指针去访问一个已经失效的对象。另外一个常见的错误是：我们使用new运算符创建一个对象并令一个指针指向该对象，由于种种原因，我们却没有对这个指针使用delete 析构该对象，造成内存泄露问题。
##    智能指针是为解决内存泄漏而产生的，使得我们在C++中对内存的泄露问题得到更好的解决和预防内存泄露问题，Qt提供的一组类模板，每个类模板对普通指针进行封装。每个类模板重载了与指针相关的运算符，如“*”，“->”等，使得每个类模板均具有普通指针的使用格式。同时，每个类模板提供了普通指针所不具备的功能，这些类模板被称为智能指针（Smart Pointer）。
     为了管理内存等资源，C++程序员通常采用RAII(Resource Acquisition Is Initialization)机制：在类的构造函数中申请资源，然后使用，最后在析构函数中释放资源。
如果没有智能指针，程序员必须保证new对象能在正确的时机delete，四处编写异常捕获代码以释放资源，而智能指针则可以在退出作用域时(不管是正常流程离开或是因异常离开)总调用delete来析构在堆上动态分配的对象。
我们看看Qt家族的智能指针：
|智能指针||引入|
|----|----|----|
|QPointer|Qt Object 模型的特性(之一)注意：析构时不会delete它管理的资源||
|QSharedPointer|带引用计数|Qt4.5|
|QWeakPointer||Qt4.5|
|QScopedPointer||Qt4.6|
|QScopedArrayPointer|QScopedPointer的派生类|Qt4.6|
|QSharedDataPointer|用来实现Qt的隐式共享(Implicit Sharing)|Qt4.0|
|QExplicitlySharedDataPointer|显式共享|Qt4.4|
||||
|std::auto_ptr|||
|std::shared_ptr|std::tr1::shared_ptr|C++0x|
|std::weak_ptr|std::tr1::weak_ptr|C++0x|
|std::unique_ptr|boost::scoped_ptr|C++0x|
注：
- MSVC2010 和 GCC g++ 4.3 支持 C++0x
- MSVC2008 sp1 及 GCC g++ 4.0 支持 tr1
有了这些东西，我们就可以很容易改造我们前面的例子了(只需改变一行)：
std::auto_ptr<QLabel> label(new QLabel("Hello Dbzhang800!"));
根据你所用的Qt的版本，以及C++编译器的支持程度，你可以选用：
- QScopedPointer
- std::unique_ptr
- QSharedPointer
- std::shared_ptr
- std::tr1::shared_ptr
## QPointer
- 
The QPointer class is a template class that provides  guarded pointers   to QObjects.
- 
使用：一个guarded指针，QPointer<T> ，行为和常规的指针 T * 类似
- 特点：当其指向的对象(T必须是QObject及其派生类)被销毁时，它会被自动置NULL.
- 注意：它本身析构时不会自动销毁所guarded的对象
- 用途：当你需要保存其他人所拥有的QObject对象的指针时，这点非常有用
一个例子
     QPointer<QLabel> label = new QLabel;
     label->setText("&Status:");
     ...
     if (label)
         label->show();
如果在...部分你将该对象delete掉了，label会自动置NULL，而不会是一个悬挂(dangling)的野指针。
QPointer 属于Qt Object模型的核心机制之一，请注意和其他智能指针的区别。
## std::auto_ptr
这个没多少要说的。
- 不能让多个auto_ptr 指向同一个对象！(auto_ptr被销毁时会自动删除它指向的对象，这样对象会被删除多次)
- 通过拷贝构造或赋值进行操作时，被拷贝的会自动变成NULL，复制所得的指针将获得资源的唯一所有权
- 智能指针不能指向数组(因为其实现中调用的是delete而非delete[])
- 智能指针不能作为容器类的元素。
在C++0x中，auto_ptr已经不建议使用，以后应该会被其他3个智能指针所取代。
## QScopedPointer 与 std::unique_ptr
它们概念上应该是是一样的。下面不再区分：
这是一个很类似auto_ptr的智能指针，它包装了new操作符在堆上分配的动态对象，能够保证动态创建的对象在任何时候都可以被正确地删除。但它的所有权更加严格，不能转让，一旦获取了对象的管理权，你就无法再从它那里取回来。
无论是QScopedPointer 还是 std::unique_ptr 都拥有一个很好的名字，它向代码的阅读者传递了明确的信息：这个智能指针只能在本作用域里使用，不希望被转让。因为它的拷贝构造和赋值操作都是私有的，这点我们可以对比QObject及其派生类的对象哈。
用法 (来自Qt的manual)：
考虑没有智能指针的情况，
 void myFunction(bool useSubClass)
 {
     MyClass *p = useSubClass ? new MyClass() : new MySubClass;
     QIODevice *device = handsOverOwnership();
     if (m_value > 3) {
         delete p;
         delete device;
         return;
     }
     try {
         process(device);
     }
     catch (...) {
         delete p;
         delete device;
         throw;
     }
     delete p;
     delete device;
 }
我们在异常处理语句中多次书写delete语句，稍有不慎就会导致资源泄露。采用智能指针后，我们就可以将这些异常处理语句简化了：
 void myFunction(bool useSubClass)
 {
     QScopedPointer<MyClass> p(useSubClass ? new MyClass() : new MySubClass);
     QScopedPointer<QIODevice> device(handsOverOwnership());
     if (m_value > 3)
         return;
     process(device);
 }
另，我们一开始的例子，也是使用这两个指针的最佳场合了(出main函数作用域就将其指向的对象销毁)。
注意：因为拷贝构造和赋值操作私有的，它也具有auto_ptr同样的“缺陷”——不能用作容器的元素。
## QSharedPointer 与 std::shared_ptr
QSharedPointer 与 std::shared_ptr 行为最接近原始指针，是最像指针的"智能指针"，应用范围比前面的提到的更广。
QSharedPointer 与 QScopedPointer 一样包装了new操作符在堆上分配的动态对象，但它实现的是引用计数型的智能指针 ，可以被自由地拷贝和赋值，在任意的地方共享它，当没有代码使用（引用计数为0）它时才删除被包装的动态分配的对象。shared_ptr也可以安全地放到标准容器中，并弥补了std::auto_ptr 和 QScopedPointer 因为转移语义而不能把指针作为容器元素的缺陷。
## QWeakPointer 与 std::weak_ptr
强引用类型的QSharedPointer已经非常好用，为什么还要有弱引用的 QWeakPointer？
QWeakPointer 是为配合 QSharedPointer 而引入的一种智能指针，它更像是 QSharedPointer 的一个助手(因为它不具有普通指针的行为，没有重载operator*和->)。它的最大作用在于协助 QSharedPointer 工作，像一个旁观者一样来观测资源的使用情况。
- weak_ptr 主要是为了避免强引用形成环状。摘自msdn中一段话：
- A cycle occurs when two or more resources controlled by shared_ptr objects hold mutually referencing shared_ptr objects. For example, a circular linked list with three elements has a head node N0;
 that node holds a shared_ptr object that owns the next node, N1; that node holds a shared_ptr object that owns the next node, N2; that node, in turn, holds a shared_ptr object that owns the head node, N0, closing the cycle. In this situation, none of the reference
 counts will ever become zero, and the nodes in the cycle will not be freed. To eliminate the cycle, the last node N2 should hold a weak_ptr object pointing to N0 instead of a shared_ptr object. Since the weak_ptr object does not own N0 it doesn't affect N0's
 reference count, and when the program's last reference to the head node is destroyed the nodes in the list will also be destroyed.
- 在Qt中，对于QObject及其派生类对象，QWeakPointer有特殊处理。它可以作为QPointer的替代品
- 这种情况下，不需要QSharedPointer的存在
- 效率比 QPointer 高
## QSharedDataPointer
这是为配合 QSharedData 实现隐式共享(写时复制 copy-on-write))而提供的便利工具。
Qt中众多的类都使用了隐式共享技术，比如QPixmap、QByteArray、QString、...。而我们为自己的类实现隐式共享也很简单，比如要实现一个 Employee类：
- 
定义一个只含有一个数据成员(QSharedDataPointer<EmployeeData>) 的 Employee 类
- 
我们需要的所有数据成员放置于 派生自QSharedData的 EmployeeData类中。
具体实现看 QSharedDataPointer 的Manual，此处略
## QExplicitlySharedDataPointer
这是为配合 QSharedData 实现显式共享而提供的便利工具。
QExplicitlySharedDataPointer 和 QSharedDataPointer 非常类似，但是它禁用了写时复制功能。这使得我们创建的对象更像一个指针。
一个例子，接前面的Employee：
 #include "employee.h"
 int main()
 {
     Employee e1(1001, "Albrecht Durer");
     Employee e2 = e1;
     e1.setName("Hans Holbein");
 }
写时复制技术导致：e1和e2有相同的工号，但有不同名字。与我们期待的不同，显式共享可以解决这个问题，这也使得Employee本身更像一个指针。
## 便看看google编码规范中对3个智能指针的建议：
- **scoped_ptr：**
Straightforward and risk-free. Use wherever appropriate.- **auto_ptr**：
Confusing and bug-prone ownership-transfer semantics. Do not use.- **shared_ptr：**
Safe with const referents (i.e. shared_ptr<const T> ).
 Reference-counted pointers with non-const referents can occasionally be the best design, but try to rewrite with single owners where possible.
参考博客：
**http://www.cnblogs.com/findumars/p/**

# Polymorphism & virtual function - 三少GG - CSDN博客
2012年04月25日 01:57:05[三少GG](https://me.csdn.net/scut1135)阅读数：955标签：[function																[destructor																[constructor																[binding																[编译器																[inheritance](https://so.csdn.net/so/search/s.do?q=inheritance&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=binding&t=blog)](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
Polymorphism & virtual function
2007-06-19 0:45
1. Inheritance的一个好处就是你可以将子类upcast到父类，然后将其当作父类来使用。但是在默认情况下，如果你将子类upcast到父类，然后调用某个函数时，调用的确只是父类的对应函数，而不是子类中的对应函数，那也就是说当你upcast之后，你也就完全失去了子类原有的信息。***但是如果我们希望****upcast之后，调用的函数仍然是子类中对应的函数，我们应该怎么办呢？*
    这也就是Polymorphism的用武之地了。这可以通过virtual function, 动态绑定来实现。
2. ***绑定是指的什么？它有什么作用？***
    所谓绑定就是将对函数的调用对应到该函数的定义上。绑定可分为两种，静态绑定和动态绑定。所谓静态绑定就是在编译时就确定了调用的函数所对应的函数体。而动态绑定指的是对调用函数的函数体的确定是在运行期间进行的。比如在第1个问题中提出的问题。在那里我们需要调用的函数体是在子类中定义的。而在编译时我们知道的该变量的类型却是父类的类型。但我们知道该变量原有的类型是子类的类型，而这个信息只能是在运行时获得的。所以这里我们需要在运行时对函数进行绑定。也就是动态绑定。
3.***在C++中我们如何实现动态绑定？***
    利用virtual函数。你只需要将基类中的对应函数声明为virtual就可以了。
4.***C++在底层是如何实现动态绑定的？***
    我们从第2个问题中应该可以看出，为了实现动态绑定我们需要知道某个变量的运行时类型，而不只是它在编译时的类型。因此我们需要在某个地方存储该类型信息。对于某个含有virtual函数的类，编译器会为该类对常见两类信息。一是一个用于存储所有virtual functions的表格，VTABLE。而一个就是在该类中新添加一个数据成员VPTR，该成员为一个指针。对于含有virtual函数的类，我们每创建一个该类的对象时，编译器会将该对象的数据成员VPTR的值设为该类对应的VTABLE的地址。那么当我们将该类upcast到它的父类时，该对象的VPTR指的位置仍然是子类的VTABLE的地址，这样我们调用virtual函数时，就会调用正确的函数了。
5.***C++中类型一般可以分为三类：对象，指针，引用。那么late binding是否对它们都适用？***
    这个答案的问题是否定的。在C++中，late binding只是当编译器在编译时不知道某个对象的具体类型时才使用的。也就是说late binding只对指针或引用适用，而对对象并不适用。因为对于对象而言，编译器是可以在编译时就确定他的具体类型的。并且，当你将一个子类的对象赋给父类的一个对象时（或者在传递参数时发生这种情况），会出现slicing问题。
6.**Virtual function并不是C++对函数的默认选择，为什么C++不像java一样将其设为默认的呢？**
    这是处于效率的考虑。因为为了实现virtual function,我们需要付出额外的时间，空间代价，比如VTABLE,VPER,以及调用virtual function是的开销。
7.有时我们需要创建一个类作为多个类的基类。我们创建该类的目的只是为了使用该类的接口，也就是说只是当我们需要将其他类upcast到该基类，而我们并不需要创建该类的对象。***那么我们应该如何防止创建该类的对象呢？***
    答案就是将该类定义为abstract class。在C++中是通过pure virtual function来实现的。也就是只要某个类中包含有pure virtual function，那么该类就是abstract
 class。并且我们不能创建该类的对象了，同时我们也不能将该对象通过by value的方式传递给函数了。也就是我们此时只能使用指针或引用。另外一点需要注意的是，对于父类中的pure virtual function，我们在子类中必须重新定义，否则的话该子类也会成为abstract class。
8. 对于Inheritance & Composition中的第4个问题，如果将基类中的对应函数为virtual，情况是否有所不同？
    基本上来说，没有什么差别。也就是说，如果你在子类中定义了不同signature但相同函数名的函数的话，该函数同样会覆盖基类中的对应函数，即使他是virtual。只有一点不同，那就是*你不能够在子类中定义一个与基类中的*virtual*函数相同函数名，相同参数列表，但不同返回值的函数*，编译器是不允许的。这一点也有一点是例外的。*如果父类中的*virtual*函数的返回值为某个类*A*的指针或引用的话，你在子类中可以将该*virtual*函数的返回值改为类*A*的某个子类的指针或引用。*
9. 一般来说，如果你在某个类的普通成员函数中调用了该类的某个virtual函数，该函数的调用将会是动态绑定的。也就是说在类的普通成员函数中对该类的virtual函数的调用可能调用的是该类的子类的virtual函数版本。但是，**如果你是在该类的构造函数中调用该类的virtual function的话，又会如何？**
    在构造函数中对virtual函数的调用调用的将是该函数的local版本。也就相当于静态绑定了。我觉得这是因为动态绑定是由VPTR来实现的。而VPTR的值是由构造函数来设定的（编译器会偷偷的在构造函数中加入一下代码）。在构造函数中，VPTR设定的值只能是当前类本身的VPTABLE的位置，它不可能将其设为该类的子类或其他值。那么你在构造函数中调用virtual函数时，你是根据VPTR来调用的。既然当前VPTR的值设的是当前类的VPTABLE的位置，那么对virtual函数的调用也就只能是当前类的版本了。
10. **是否所有的函数都可以声明为virtual的？**
    不是。constructor不能是virtual的。但是destructor可以是virtual的。并且如果你的类中定义了virtual函数的话，你应该立刻将该类的destructor设为virtual的。因为这可以防止内存泄露。比如A,
 B两个类,B 是A的子类。A* a = new B; delete a; 如果A中的构造函数不是virtual的话，delete a就只会调用A的destructor，而不会调用B的destructor。这样的话就可能会出问题。因为B中的某些部分并没有被析构。另外destructor还可以是pure virtual的。但是和一般的pure virtual函数不一样，你必须为pure virtual destructor提供函数体。只是因为当调用子类的destructor是，编译器会自动调用父类的destructor，如果父类的destructor没有函数体的话，那么如果调用该destructor呢？还有就是对于pure
 virtual destructor，你并不一定要在子类中override该destructor, 因为即使你不这样做，编译器也会自动给您生成一个。其实pure virtual destructor 和 virtual destructor并没有太大的差别，只是如果你将某个类的destructor设为pure virtual，该类就成为abstract class, 也就是你不能创建该类的对象了。
11. **在destructor调用该类的virtual function，是否是动态绑定？**
    和constructor相同，在destructor中对virtual function的调用也只是static
 binding，也就是说它调用的也只是local 版本而已。这只因为，destructor的调用顺序是从子类到父类的。如果你在父类的destructor中调用了某个virtual function，此时子类已经被析构了，所以显然你不能在调用子类中该virtual
 function的版本了。因此，对destructor中virtual function的调用也只能是local版本。但是这个原因和constructor中的early binding是不同的。在constructor中，我们是因为没有子类的充分信息所以不能调用该子类的virtual function版本，而在destructor中，我们是因为如果调用该子类的virtual function版本可能不可靠，因为此时子类的某些信息可能已经被析构了。

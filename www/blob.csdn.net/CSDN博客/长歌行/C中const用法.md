# C中const用法 - 长歌行 - CSDN博客





2012年02月06日 16:21:24[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：618








看来还是不够理解啊，一用到就会犹豫，然后反复尝试试出来结果。从网上摘点资料吧。

http://zhidao.baidu.com/question/345457720.html




## C中CONST的使用


　　const是一个C语言的关键字，它限定一个变量不允许被改变。使用const在一定程度上可以提高程序的安全性和可靠性，另外，在观看别人代码的时候，清晰理解const所起的作用，对理解对方的程序也有一些帮助。

　　虽然这听起来很简单，但实际上，const的使用也是c语言中一个比较微妙的地方，微妙在何处呢？请看下面几个问题。

　　问题：const变量 & 常量

　　为什么下面的例子在使用一个const变量来初始化数组，ANSI C的编译器会报告一个错误呢？

const int n = 5;

int a[n];

　　答案与分析:

　　1）、这个问题讨论的是“常量”与“只读变量”的区别。常量肯定是只读的，例如5， “abc”，等，肯定是只读的，因为因为常量是被编译器放在内存中的只读区域，当然也就不能够去修改它。而“只读变量”则是在内存中开辟一个地方来存放它的值，只不过这个值由编译器限定不允许被修改。C语言关键字const就是用来限定一个变量不允许被改变的修饰符（Qualifier）。上述代码中变量n被修饰为只读变量，可惜再怎么修饰也不是常量。而ANSI C规定数组定义时维度必须是“常量”，“只读变量”也是不可以的。

　　2)、注意：在ANSI C中，这种写法是错误的，因为数组的大小应该是个常量，而const int n,n只是一个变量（常量 != 不可变的变量，但在标准C++中，这样定义的是一个常量，这种写法是对的），实际上，根据编译过程及内存分配来看，这种用法本来就应该是合理的，只是 ANSI C对数组的规定限制了它。

　　3)、那么，在ANSI C 语言中用什么来定义常量呢？答案是enum类型和#define宏，这两个都可以用来定义常量。

　　问题：const变量 & const 限定的内容

　　下面的代码编译器会报一个错误，请问，哪一个语句是错误的呢？　

　　typedef char * pStr;

　　char string[4] = "abc";

　　const char *p1 = string;

　　const pStr p2 = string;

　　p1++;

　　p2++;

　　答案与分析：

　　问题出在p2++上。

　　1)、const使用的基本形式： const char m;

　　限定m不可变。

　　2)、替换1式中的m, const char *pm;

　　限定*pm不可变，当然pm是可变的，因此问题中p1++是对的。

　　3)、替换1式char, const newType m;

　　限定m不可变，问题中的charptr就是一种新类型，因此问题中p2不可变，p2++是错误的。

　　问题：const变量 & 字符串常量

　　请问下面的代码有什么问题？

　　char *p = "i'm hungry!";

　　p[0]= 'I';

　　答案与分析：

　　上面的代码可能会造成内存的非法写操作。分析如下， “i'm hungry”实质上是字符串常量，而常量往往被编译器放在只读的内存区，不可写。p初始指向这个只读的内存区，而p[0] = 'I'则企图去写这个地方，编译器当然不会答应。

　　问题：const变量 & 字符串常量2

　　请问char a[3] = "abc" 合法吗？使用它有什么隐患？

　　答案与分析：

　　在标准C中这是合法的，但是它的生存环境非常狭小；它定义一个大小为3的数组，初始化为“abc”，，注意，它没有通常的字符串终止符'\0'，因此这个数组只是看起来像C语言中的字符串，实质上却不是，因此所有对字符串进行处理的函数，比如strcpy、printf等，都不能够被使用在这个假字符串上。

　　问题5：const & 指针

　　类型声明中const用来修饰一个常量，有如下两种写法，那么，请问，下面分别用const限定不可变的内容是什么?

　　1)、const在前面

　　const int nValue； //nValue是const

　　const char *pContent; //*pContent是const, pContent可变

　　const (char *) pContent;//pContent是const,*pContent可变

　　char* const pContent; //pContent是const,*pContent可变

　　const char* const pContent; //pContent和*pContent都是const

　　2)、const在后面，与上面的声明对等

　　int const nValue； // nValue是const

　　char const * pContent;// *pContent是const, pContent可变

　　(char *) const pContent;//pContent是const,*pContent可变

　　char* const pContent;// pContent是const,*pContent可变

　　char const* const pContent;// pContent和*pContent都是const

　　答案与分析：

　　const和指针一起使用是C语言中一个很常见的困惑之处，在实际开发中，特别是在看别人代码的时候，常常会因为这样而不好判断作者的意图，下面讲一下我的判断原则：

　　沿着*号划一条线，如果const位于*的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；如果const位于*的右侧，const就是修饰指针本身，即指针本身是常量。你可以根据这个规则来看上面声明的实际意义，相信定会一目了然。

　　另外，需要注意：对于const (char *) ; 因为char *是一个整体，相当于一个类型(如 char)，因此，这是限定指针是const。

[编辑本段]C++中CONST

　　C中常用:“ #define 变量名 变量值”定义一个值替代,然而却有个致命缺点:缺乏类型检测机制,这样预处理在C++中成为可能引发错误的隐患,于是引入const.

　　const使用:

　　1. 用于指针的两种情况:const是一个左结合的类型修饰符.

　　int const *A; //A可变,*A不可变

　　int *const A; //A不可变,*A可变

　　2.限定函数的传递值参数:

　　void function(const int Var); //传递过来的参数在函数内不可以改变.

　　3.限定函数返回值型.

　　const int function(); //此时const无意义

　　const myclassname function(); //函数返回自定义类型myclassname.

　　4.限定函数类型.

　　void function()const; //常成员函数, Const成员函数不能改变对象的成员函数。

　　例如： 

　　int Point::GetY()

　　{

　　return yVal;

　　} 

　　这个函数被调用时，不改变Point对象，而下面的函数改变Point对象： 

　　void Point:: SetPt (int x, int y)

　　{

　　xVal=x;

　　yVal=y;

　　} 

　　为了使成员函数的意义更加清楚，我们可在不改变对象的成员函数的函数原型中加上const说明： 

　　class Point 

　　{ 

　　public:

　　int GetX() const;

　　int GetY() const;

　　void SetPt (int, int);

　　void OffsetPt (int, int);

　　private:

　　int xVal, yVal;

　　};

　　const成员函数应该在函数原型说明和函数定义中都增加const限定： 

　　int Point::GetY() const

　　{

　　return yVal;

　　}

　　class Set {

　　public:

　　Set (void){ card = 0; }

　　bool Member(const int) const;

　　void AddElem(const int);

　　//...

　　};

　　bool Set::Member (const int elem) const

　　{

　　//...

　　}

　　非常量成员函数不能被常量成员对象调用，因为它可能企图修改常量的数据成员： 

　　const Set s;

　　s.AddElem(10); // 非法: AddElem不是常量成员函数

　　s.Member(10); // 正确 

　　但构造函数和析构函数对这个规则例外，它们从不定义为常量成员，但可被常量对象调用（被自动调用）。它们也能给常量的数据成员赋值，除非数据成员本身是常量。 

　　为什么需要const成员函数？

　　我们定义的类的成员函数中，常常有一些成员函数不改变类的数据成员，也就是说，这些函数是"只读"函数，而有一些函数要修改类数据成员的值。如果把不改变数据成员的函数都加上const关键字进行标识，显然，可提高程序的可读性。其实，它还能提高程序的可靠性，已定义成const的成员函数，一旦企图修改数据成员的值，则编译器按错误处理。 

　　const成员函数和const对象

　　实际上，const成员函数还有另外一项作用，即常量对象相关。对于内置的数据类型，我们可以定义它们的常量，用户自定义的类也一样，可以定义它们的常量对象。例如，定义一个整型常量的方法为： 

　　const int i=1 ；

　　同样，也可以定义常量对象，假定有一个类classA，定义该类的常量对象的方法为：

　　const classA a(2)；

　　这里，a是类classA的一个const对象，"2"传给它的构造函数参数。const对象的数据成员在对象寿命期内不能改变。但是，如何保证该类的数据成员不被改变呢？

　　为了确保const对象的数据成员不会被改变，在C++中，const对象只能调用const成员函数。如果一个成员函数实际上没有对数据成员作任何形式的修改，但是它没有被const关键字限定的，也不能被常量对象调用。下面通过一个例子来说明这个问题： 

　　class C

　　{

　　int X;

　　public:

　　int GetX()

　　{

　　return X;

　　}

　　void SetX(int X) 

　　{

　　this->X = X;

　　}

　　}; 

　　void main()

　　{

　　const C constC;

　　cout<<constC.GetX();

　　}

　　如果我们编译上面的程序代码，编译器会出现错误提示：constC是个常量对象，它只能调用const成员函数。虽然GetX( )函数实际上并没有改变数据成员X，由于没有const关键字限定，所以仍旧不能被constC对象调用。如果我们将上述加粗的代码：

　　int GetX()

　　改写成：

　　int GetX()const

　　再重新编译，就没有问题了。

　　const成员函数的使用

　　const成员函数表示该成员函数只能读类数据成员，而不能修改类成员数据。定义const成员函数时，把const关键字放在函数的参数表和函数体之间。有人可能会问：为什么不将const放在函数声明前呢？因为这样做意味着函数的返回值是常量，意义完全不同。下面是定义const成员函数的一个实例：

　　class X

　　{

　　int i;

　　public:

　　int f() const;

　　}; 

　　关键字const必须用同样的方式重复出现在函数实现里，否则编译器会把它看成一个不同的函数：

　　int X::f() const 

　　{

　　return i;

　　} 

　　如果f( )试图用任何方式改变i或调用另一个非const成员函数，编译器将给出错误信息。任何不修改成员数据的函数都应该声明为const函数，这样有助于提高程序的可读性和可靠性。



C#.net中的const

　　const 关键字用于修改字段或局部变量的声明。它指定字段或局部变量的值是常数，不能被修改。例如：

　　const int x = 0;

　　public const double gravitationalConstant = 6.673e-11;

　　private const string productName = "Visual C#";

　　备注

　　常数声明的类型指定声明引入的成员类型。常数表达式必须产生具有目标类型或者可隐式转换为目标类型的类型的值。 

　　常数表达式是在编译时可被完全计算的表达式。因此，对于引用类型的常数，可能的值只能是 string 和 null。

　　常数声明可以声明多个常数，例如：

　　public const double x = 1.0, y = 2.0, z = 3.0;

　　不允许在常数声明中使用 static 修饰符。

　　常数可以参与常数表达式，如下所示：

　　public const int c1 = 5;

　　public const int c2 = c1 + 100;

　　注意 

　　readonly 关键字与 const 关键字不同。const 字段只能在该字段的声明中初始化。readonly 字段可以在声明或构造函数中初始化。因此，根据所使用的构造函数，readonly 字段可能具有不同的值。另外，const 字段是编译时常数，而 readonly 字段可用于运行时常数，如下面的代码行所示：public static readonly uint l1 = (uint)DateTime.Now.Ticks;








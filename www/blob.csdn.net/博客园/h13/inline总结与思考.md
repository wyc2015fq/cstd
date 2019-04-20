# inline总结与思考 - h13 - 博客园
1　C语言的inline函数
本文介绍了GCC和C99标准中inline使用上的不同之处。inline属性在使用的时候，要注意以下两点：inline关键字在GCC参考文档中仅有对其使用在函数定义（Definition）上的描述，而没有提到其是否能用于函数声明（Declare）。 
从inline的作用来看，其放置于函数声明中应当也是毫无作用的：inline只会影响函数在translation unit（可以简单理解为C源码文件）内的编译行为，只要超出了这个范围inline属性就没有任何作用了。所以inline关键字不应该出现在函数声明中，没有任何作用不说，有时还可能造成编译错误（在包含了sys/compiler.h的情况下，声明中出现inline关键字的部分通常无法编译通过）； 
inline关键字仅仅是建议编译器做内联展开处理，而不是强制。在gcc编译器中，如果编译优化设置为-O0，即使是inline函数也不会被内联展开，除非设置了强制内联（__attribute__((always_inline))）属性。
[1.1　GCC的inline](http://www.cnblogs.cc2/link.php?url=http://blog.chinaunix.net%2Fchinaunixeditor.php%3Fop%3Dblank%26amp%3Bcharset%3Dgbk%23_top)
gcc对C语言的inline做了自己的扩展，其行为与C99标准中的inline有较大的不同。 
1.1.1　static inline
GCC的static inline定义很容易理解：你可以把它认为是一个static的函数，加上了inline的属性。这个函数大部分表现和普通的static函数一样，只不过在调用这种函数的时候，gcc会在其调用处将其汇编码展开编译而不为这个函数生成独立的汇编码。除了以下几种情况外： 
函数的地址被使用的时候。如通过函数指针对函数进行了间接调用。这种情况下就不得不为static inline函数生成独立的汇编码，否则它没有自己的地址。 
其他一些无法展开的情况，比如函数本身有递归调用自身的行为等。
static inline函数和static函数一样，其定义的范围是local的，即可以在程序内有多个同名的定义（只要不位于同一个文件内即可）。 
注意  
gcc的static inline的表现行为和C99标准的static inline是一致的。所以这种定义可以放心使用而没有兼容性问题。 
要点： 
gcc的static inline相对于static函数来说只是在调用时建议编译器进行内联展开； 
gcc不会特意为static inline函数生成独立的汇编码，除非出现了必须生成不可的情况（如通过函数指针调用和递归调用）； 
gcc的static inline函数仅能作用于文件范围内。
[1.1.2　inline](http://www.cnblogs.cc2/link.php?url=http://blog.chinaunix.net%2Fchinaunixeditor.php%3Fop%3Dblank%26amp%3Bcharset%3Dgbk%23_top)
相对于C99的inline来说，GCC的inline更容易理解：可以认为它是一个普通全局函数加上了inline的属性。即在其定义所在文件内，它的表现和static inline一致：在能展开的时候会被内联展开编译。但是为了能够在文件外调用它，gcc一定会为它生成一份独立的汇编码，以便在外部进行调用。即从文件外部看来，它和一个普通的extern的函数无异。举个例子：
foo.c:
/* 这里定义了一个inline的函数foo() */
inline foo() {
    ...;   <- 编译器会像非inline函数一样为foo()生成独立的汇编码
}
void func1() {
    foo(); <- 同文件内foo()可能被编译器内联展开编译而不是直接call上面生成的汇编码
}而在另一个文件里调用foo()的时候，则直接call的是上面文件内生成的汇编码：
bar.c:
extern foo(); <- 声明foo()，注意不能在声明内带inline关键字
void func2() {
    foo();    <- 这里就是直接call在foo.c内为foo()函数生成的汇编码了
} 重要  
虽然gcc的inline函数的行为很好理解，但是它和C99的inline是有很大差别的。请注意看后面对C99 inline的描述（第 2.2 节 “inline”），以及如何以兼顾GCC和C99的方式使用inline函数。 
要点： 
gcc的inline函数相对于普通extern函数来说只是在同一个文件内调用时建议编译器进行内联展开； 
gcc一定会为inline函数生成一份独立的汇编码，以便其在本文件之外被调用。在别的文件内看来，这个inline函数和普通的extern函数无异； 
gcc的inline函数是全局性的：在文件内可以作为一个内联函数被内联展开，而在文件外可以调用它。
[1.1.3　extern inline](http://www.cnblogs.cc2/link.php?url=http://blog.chinaunix.net%2Fchinaunixeditor.php%3Fop%3Dblank%26amp%3Bcharset%3Dgbk%23_top)
GCC的static inline和inline都很好理解：看起来都像是对普通函数添加了可内联的属性。但是这个extern inline就千万不能想当然地理解成就是一个extern的函数+inline属性了。实际上gcc的extern inline十分古怪：一个extern inline的函数只会被内联进去，而绝对不会生成独立的汇编码！即使是通过指针应用或者是递归调用也不会让编译器为它生成汇编码，在这种时候对此函数的调用会被处理成一个外部引用。另外，extern inline的函数允许和外部函数重名，即在存在一个外部定义的全局库函数的情况下，再定义一个同名的extern inline函数也是合法的。以下用例子具体说明一下extern inline的特点：
foo.c:
extern inline
int foo(int a)
{
    return (-a);
}
void func1()
{
    ...;
    a = foo(a);   ①
    p_foo = foo;  ②
    b = p_foo(b); ③
}在这个文件内，gcc不会生成foo函数的汇编码。在func1中的调用点①，编译器会将上面定义的foo函数在这里内联展开编译，其表现类似于普通inline函数。因为这样的调用是能够进行内联处理的。而在②处，引用了foo函数的地址。但是注意：编译器是绝对不会为extern inline函数生成独立汇编码的！所以在这种非要个函数地址不可的情况下，编译器不得不将其处理为外部引用，在链接的时候链接到外部的foo函数去（填写外部函数的地址）。这时如果外部没有再定义全局的foo函数的话就会在链接时产生foo函数未定义的错误。 
假设在另一个文件里面也定义了一个全局函数foo：
foo2.c:
int foo(int a)
{
    return (a);
}那么在上面那个例子里面，后面一个对foo函数地址的引用就会在链接时被指到这个foo2.c中定义的foo函数去。也就是说：①调用foo函数的结果是a=-a，因为其内联了foo.c内的foo函数；而③调用的结果则是b=b，因为其实际上调用的是foo2.c里面的foo函数！ 
extern inline的用法很奇怪也很少见，但是还是有其实用价值的。第一：它可以表现得像宏一样，可以在文件内用extern inline版本的定义取代外部定义的库函数（前提是文件内对其的调用不能出现无法内联的情况）；第二：它可以让一个库函数在能够被内联的时候尽量被内联使用。举个例子： 
在一个库函数的c文件内，定义一个普通版本的库函数libfunc：
lib.c:
void libfunc()
{
    ...;
}然后再在其头文件内，定义（注意不是声明！）一个实现相同的exterin inline的版本：
lib.h:
extern inline libfunc()
{
    ...;
}那么在别的文件要使用这个库函数的时候，只要include了lib.h，在能内联展开的地方，编译器都会使用头文件内extern inline的版本来展开。而在无法展开的时候（函数指针引用等情况），编译器就会引用lib.c中的那个独立编译的普通版本。即看起来似乎是个可以在外部被内联的函数一样，所以这应该是gcc的extern inline意义的由来。 
但是注意这样的使用是有代价的：c文件中的全局函数的实现必须和头文件内extern inline版本的实现完全相同。否则就会出现前面所举例子中直接内联和间接调用时函数表现不一致的问题。 
重要  
gcc的extern inline函数的用法相当奇怪，使用的范围也非常狭窄：几乎没有什么情况会需要用它。 
在C99中，也没有关于extern inline这样的描述，所以不建议大家使用extern inline，除非你明确理解了这种用法的意义并且有充足的理由使用它！ 
要点： 
gcc绝对不会为extern inline的函数生成独立汇编码 
extern inline函数允许和全局函数重名，可以在文件范围内替代外部定义的全局函数 
extern inline函数的应用范围十分狭窄，而且行为比较奇怪，不建议使用
[1.2　C99的inline](http://www.cnblogs.cc2/link.php?url=http://blog.chinaunix.net%2Fchinaunixeditor.php%3Fop%3Dblank%26amp%3Bcharset%3Dgbk%23_top)
以下主要描述C99的inline与Gcc不同的部分。对于相同的部分请参考GCC inline的说明。 
1.2.1　static inline
同GCC的static inline（第 1.1 节 “static inline”）。
1.2.2　inline
C99的inline的使用相当令人费解。当一个定义为inline的函数没有被声明为extern的时候，其表现有点类似于gcc中extern inline那样（C99里面这段描述有点晦涩，原文如下）： 
If all of the file scope declarations for a function in a translation unit include the inline function specifier without extern, then the definition in that translation unit is an inline definition. An inline definition does not provide an external definition for the function, and does not forbid an external definition in another translation unit. An inline definition provides an alternative to an external definition, which a translator may use to implement any call to the function in the same translation unit. It is unspecified whether a call to the function uses the inline definition or the external definition.
即如果一个inline函数在文件范围内没有被声明为extern的话，这个函数在文件内的表现就和gcc的extern inline相似：在本文件内调用时允许编译器使用本文件内定义的这个内联版本，但同时也允许外部存在同名的全局函数。只是比较奇怪的是C99居然没有指定编译器是否必须在本文件内使用这个inline的版本而是让编译器厂家自己来决定，相当模糊的定义。 
如果在文件内把这个inline函数声明为extern，则这个inline函数的表现就和gcc的inline一致了：这个函数即成为一个“external definition”（可以简单理解为全局函数）：可以在外部被调用，并且在程序内仅能存在一个这样名字的定义。 
下面举例说明C99中inline的特性：
inline double fahr(double t)
{
    return (9.0 * t) / 5.0 + 32.0;
}
inline double cels(double t)
{
    return (5.0 * (t - 32.0)) / 9.0;
}
extern double fahr(double);   ①
double convert(int is_fahr, double temp)
{
    return is_fahr ? cels(temp) : fahr(temp);   ②
}在上面这个例子里，函数fahr是个全局函数：因为在①处将fahr声明为extern，因此在②处调用fahr的时候使用的一定是这个文件内所定义的版本（只不过编译器可以将这里的调用进行内联展开）。在文件外部也可以调用这个函数（说明像gcc的inline一样，编译器在这种情况下会为fahr生成独立的汇编码）。 
而cels函数因为没有在文件范围内被声明为extern，因此它就是前面所说的“inline definition”，这时候它实际上仅能作用于本文件范围（就像一个static的函数一样），外部也可能存在一个名字也为cels的同名全局函数。在②处调用cels的时候编译器可能选择用本文件内的inline版本，也有可能跑去调用外部定义的cels函数（C99没有规定此时的行为，不过编译器肯定都会尽量使用文件内定义的inline版本，要不然inline函数就没有存在的意义了）。从这里的表现上看C99中未被声明为extern的inline函数已经和gcc的extern inline十分相似了：本文件内的inline函数可以作为外部库函数的替代。 
重要  
C99标准中的inline函数行为定义的比较模糊，并且inline函数有没有在文件范围内被声明为extern的其表现有本质不同。如果和gcc的inline函数比较的话，一个被声明为extern的inline函数基本等价于GCC的普通inline函数；而一个没有被声明为extern的inline函数基本等价于GCC的extern inline函数。 
因为C99的inline函数如此古怪，所以在使用的时候，建议为所有的inline函数都在头文件中创建extern的声明：
foo.h:
extern foo();而在定义inline函数的c文件内include这个头文件：
foo.c:
#include "foo.h"
inline void foo()
{
    ...;
}这样无论是用gcc的inline规则还是C99的，都能得到完全相同的结果：foo函数会在foo.c文件内被内联使用，而在外部又可以像普通全局函数一样直接调用。
[1.2.3　extern inline](http://www.cnblogs.cc2/link.php?url=http://blog.chinaunix.net%2Fchinaunixeditor.php%3Fop%3Dblank%26amp%3Bcharset%3Dgbk%23_top)
C99没有见到extern inline的用法。

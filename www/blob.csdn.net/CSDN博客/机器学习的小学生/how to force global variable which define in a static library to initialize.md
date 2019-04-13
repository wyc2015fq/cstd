
# how to force global variable which define in a static library to initialize? - 机器学习的小学生 - CSDN博客


2017年03月31日 18:06:04[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：358



## 文献1：
answer1:
> 如果引用和全局变量同样文件中的某个函数，那么静态全局变量将被初始化。
answer2:
> 在头文件中定义为static，然而在源文件中使用：namespace::variable = value 的形式初始化。
question:
> 事实上，我们可以在头文件中定义一个全局的静态变量，然后在执行工程中通过include *.h 文件，可以初始化全局变量。但是我们是否可以通过link 选项，将静态库中的.h文件link到执行工程?而不是通过引用对象或者函数的形式。
answer3:
> 根据标准，没有执行此操作的正式方法，它取决于实现-定义的行为。（即每个编译器都允许做法不同）。这意味着，依靠静态库中的未引用的全局变量来去构建/连接是错误的。

> 为了可靠地让你的全局变量在任何C++编译器上连接到（也就是标准方式的做法），你实际上必须在你的程序内部使用全局变量，例如：

> //static_library.cpp
> Reflection g_myGlobal(
> "foo"
> ,
> "bar"
> );
Reflection** GetReflectionTable()
{
> static
> Reflection* table = { &g_myGlobal,
> NULL
> };
> return
> table;
}
> //main.cpp
> int
> main()
{
> // this forces g_myGlobal to be linked, because it's actually used by the program now.
> for
> ( Reflection** staticLibReflection = GetReflectionTable(); *staticLibReflection, ++staticLibReflection )
  {
> const
> Reflection& r = **staticLibReflection;
    r
> .foo
> ();
  }
}
> 文献4

> question:

> 我正在使用一个工厂，可以上里面添加类型。然而，如果类不在.exe中明显的初始化（编译时），那么该类型不能添加到工厂。这是由于静态调用有点不起作用。如何解决这个问题呢？下面是5个非常小的文件，我们将其放入到lib中，然而通过.exe调用lib.

> answer:

> 一般来说，一个应用程序不能包含来自一个库的静态的或者全局的变量，除非这些变量隐式的或者明显的被应用使用。有上百种方式实现，一个方法是将静态变放置到函数的内部，然后确保函数被调用。

> question:

> 有没有一种自动的登记方式，像我正在做的，可以运行的，有效的。

> answer:

> 没有100%的自动，但是如果在你的从应用程序中包含的任何代码或者变量引用这个静态变量，那么将有效。

> answer2:

> When you are linking with a static library, you are in fact extracting from it the object files which provide symbols which are currently used but not defined. In the pattern that you are using, there is probably no undefined symbols provided by the object file which contains the static variable which triggers registration.

> Solutions:

> use explicit registration

> have somehow an undefined symbol provided by the compilation uni

> – use the linker arguments to add your static variables as a undefined symbols

> – something useful, but this is often not natural

> – a dummy one, well it is not natural if it is provided by the main program, as a linker argument it main be easier than using the mangled name of the static variable

> use a linker argument stating that all the objects of a library have to be included

> dynamic libraries are fully imported, thus don’t have that problem

> 当您连接到静态库时，实际上是从静态库中抽取目标文件，目标文件提供了当前使用但未定义的符号。 在你使用的工厂模式中，包含静态变量（其触发注册操作）目标文件中可能没有未定义的符号。（感觉好别扭，可能有未定义的符号？）

> 解决方案：

> 使用显式注册

> 以某种方式由编译单元提供的未定义符号

> – 使用链接器参数将静态变量添加为未定义的符号

> – 有用的东西，但这往往是不自然的

> – 一个虚拟的，如果它是由主程序提供的，作为链接器参数，它不是自然的，它主要比使用静态变量的被破坏的名称更容易

> 使用一个链接器参数，指出一个库必须包括的所有对象

> 动态库是完全导入的，因此不存在这样的问题

> 参考文献：

> [https://www.gamedev.net/topic/622861-how-to-force-global-variable-which-define-in-a-static-library-to-initialize/](https://www.gamedev.net/topic/622861-how-to-force-global-variable-which-define-in-a-static-library-to-initialize/)

> [http://gamedev.stackexchange.com/questions/37813/variables-in-static-library-are-never-initialized-why](http://gamedev.stackexchange.com/questions/37813/variables-in-static-library-are-never-initialized-why)

> [https://social.msdn.microsoft.com/Forums/vstudio/en-US/5c5ee89d-2a38-483d-8550-484550901170/issues-with-static-variables-in-static-libraries?forum=vclanguage](https://social.msdn.microsoft.com/Forums/vstudio/en-US/5c5ee89d-2a38-483d-8550-484550901170/issues-with-static-variables-in-static-libraries?forum=vclanguage)

> [http://stackoverflow.com/questions/5202142/static-variable-initialization-over-a-library](http://stackoverflow.com/questions/5202142/static-variable-initialization-over-a-library)



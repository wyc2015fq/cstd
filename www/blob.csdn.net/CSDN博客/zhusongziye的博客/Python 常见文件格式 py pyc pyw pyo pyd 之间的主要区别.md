# Python 常见文件格式 .py .pyc .pyw .pyo .pyd 之间的主要区别 - zhusongziye的博客 - CSDN博客





2018年03月26日 20:35:53[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：66









Python是一种面向对象、解释型计算机程序设计语言。Python语法简洁、清晰，具有丰富和强大的类库。




Python源代码遵循 GPL (GNU General Public License) 协议，由 Guido van Rossum 于 1989 年底发明，第一个公开发行版发行于 1991 年。




Python 常被称为胶水语言，能把用其他语言编写的各模块 (尤其是 C/C++) 轻松地联结在一起。常见情形是，用 Python 快速生成程序原型 (有时甚至是程序最终界面)，然后对其中有特别要求的部分，用更合适的语言改写；譬如：3D 游戏中的图形渲染模块，性能要求特别高，就可用 C/C++ 重写，而后封装为 Python 可调用的扩展类库。需要注意的是，在您使用扩展类库时可能需要考虑平台问题，某些扩展类库可能不提供跨平台实现。




一般认为，Python 是一种解释性语言，Python 在执行时，会先将 .py 文件中的源代码编译成byte code (字节码)，然后再由 Python Virtual Machine 来执行这些编译 byte code。




这种机制的基本思想跟 Java、.NET 一致；但 Python Virtual Machine 与 Java 或 .NET 的 Virtual Machine 不同的是：Python 的 Virtual Machine 是一种更高级的 Virtual Machine。这里的高级并不是通常意义上的高级，不是说 Python 的 Virtual Machine 比 Java 或 .NET 的功能更强大，更拽，而是说和 Java 或 .NET 相比，Python 的 Virtual Machine 距真实机器的距离更远。或者可以这么说，Python 的 Virtual Machine 是一种抽象层次更高的 Virtual Machine。




Python 提供了一种中间编译结果保存机制，即 byte code，或更准确地说，保存 PyCodeObject。事实上，Python 确实提供了这样一种机制，在执行一个 .py 文件的源代码之后，Python 并不会自动生成与该 .py 文件对应的 .pyc 文件。Python 会根据需要自动触发、创建 .pyc 文件，原理很简单，就是利用 Python 的 import 机制。




在 Python 运行过程中如碰到 import abc 这样的语句，Python 将会在设定好的 path 中寻找 abc.pyc 或 abc.dll 文件。若没有这样的文件，而只是发现了 abc.py 文件，那么，Python 会首先将 abc.py 编译成相应的 PyCodeObject 中间结果，然后再创建 abc.pyc 文件，并将中间结果写入该文件。接下来，Python 才会对 abc.pyc 文件进行一个 import 动作，实际上也就是将 abc.pyc 文件中的 PyCodeObject 重新在内存中复制出来，加以运行。




由于 Python .pyc 文件的此种运行机制，所以，可随时删除所有中间过程 .pyc 文件，当 Python 再次运行时，还会重生成这些 .pyc 文件，对您所编辑程序的运行并无任何影响。当然，得有源代码才行。




视窗用 python.exe 运行 .py ，用 pythonw.exe 运行 .pyw。因为安装视窗版 Python 时，扩展名 .py 自动被登记为用 python.exe 运行， 而 .pyw 则被登记为用 pythonw.exe 运行。 




.py 文件

以 .py 作扩展名的文件是 Python 源代码文件，由 python.exe 解释，可在控制台下运行。




当然，也可用文本编辑器或其它专用 Python IDE (集成开发环境) 工具进行修改。




.pyc 文件

以 .pyc 作扩展名的文件是 python 编译文件。.pyc 文件是不能直接用文本编辑器进行编辑，其优点是 .pyc 文件的执行速度要远快于 .py 文件。




至于为什么要有 .pyc 文件，因为 .py 文件是可直接看到源码的。若是软件开发商，不可能把源码泄漏出去？所以，就需编译成 .pyc 后再发布。




但 .pyc 文件只是经简单编译，并未加密，因此，有些工具还能反编译它得出源代码。




.pyw 文件

.pyc 文件执行时，桌面会出现类似 CMD 命令的黑色 shell 窗口，十分难看，于是 .pyw 文件就应运而生了。.pyw 文件与 .pyc 文件的执行，本质上并没什么区别，只是 .pyw 文件执行的时候不会出现类似 CMD 命令的黑色 shell 窗口。




.pyw 文件格式主要是设计用来运行 Python 纯 GUI (图形用户界面) 程序的。纯 GUI (图形用户界面) 程序的用户不需要看到类似 CMD 命令的黑色 shell 控制台窗口。当 .pyw 文件运行时，所有 stdout、stderr 输出无效，所有原 stdin 的读取只会得到 EOF。




值得一提的是，开发纯 GUI (图形用户界面) 程序时，可暂时把 .pyw 改成 .py，以便运行时调出控制台窗口，看到所有错误信息，方便修改、调试。




注意：采用 PyQt、PySide 等 GUI (图形用户界面) 开发框架开发的程序，源文件仍可采用 .py，无需单独使用 .pyw。




.pyo 文件

"python -O 源文件" 即可将 Python 源程序编译成 .pyo 文件，但有时仍得采用 .pyc 作后缀，才能正常运行。




.pyo 文件是相对 .pyc 而言的，优化编译后的 Python 文件。 同样，.pyo 文件也不能直接用文本编辑器进行编辑。




.pyd 文件

.pyd 文件是非 Python，由其它编程语言 "编写-编译" 生成的 Python 扩展模块。




Python 要导入 .pyd 文件，实际上是在 .pyd 文件中封装了一个 module。在 python 中使用时，把它当成 module 来用就可以了，即："import 路径名.modulename" 即可，路径名为 .pyd 文件所在的路径。




基于 Qt/C++ 的 PyQt、PySide GUI (图形用户界面) 开发框架绑定，会为每个 Qt/C++ 库 .dll 文件生成一个中间 .pyd 文件。PyWin32 项目分发包中，也存在很多 MicroSoft Windows OS 相应 .dll 文件的中间 .pyd 文件。




Cython 可将个人基于 Python 语言编写的 Python 模块编译成具有 C 语言特性的 .pyd 文件。




.pyd 文件有时也是用 D 语言按照一定格式编写，编译生成的二进制文件。




那么什么是 "D 语言" 呢？它是 C/C++ 的综合进化版，不仅具有二者的全部优点，且整体性能更佳，但其抽象程度高。D 语言最初由 Digital Mars 公司就职的 Walter Bright 于 2001 年发布，意图改进 C++ 语言。目前最新 D 语言被简称为 D2。最主要的 D 语言实现是 DMD。




D 语言源自 C/C++，借鉴了众多编程语言的特色和现代编译器技术，融会贯通了设计者丰富的实践经验，使之具备了非凡的威力 ── 既有 C/C++ 语言的强大威力，又有 Python 和 Ruby 的开发效率。它集众多系统级编程所需的功能于一身，例如垃圾回收、手工内存操作、契约式设计、高级模板技术、内嵌汇编、内置单元测试、Mixin 风格多继承、类 Java 包管理机制、内置同步机制、内建基本运行时信息。


Python是一种面向对象、解释型计算机程序设计语言。Python语法简洁、清晰，具有丰富和强大的类库。




Python源代码遵循 GPL (GNU General Public License) 协议，由 Guido van Rossum 于 1989 年底发明，第一个公开发行版发行于 1991 年。




Python 常被称为胶水语言，能把用其他语言编写的各模块 (尤其是 C/C++) 轻松地联结在一起。常见情形是，用 Python 快速生成程序原型 (有时甚至是程序最终界面)，然后对其中有特别要求的部分，用更合适的语言改写；譬如：3D 游戏中的图形渲染模块，性能要求特别高，就可用 C/C++ 重写，而后封装为 Python 可调用的扩展类库。需要注意的是，在您使用扩展类库时可能需要考虑平台问题，某些扩展类库可能不提供跨平台实现。




一般认为，Python 是一种解释性语言，Python 在执行时，会先将 .py 文件中的源代码编译成byte code (字节码)，然后再由 Python Virtual Machine 来执行这些编译 byte code。




这种机制的基本思想跟 Java、.NET 一致；但 Python Virtual Machine 与 Java 或 .NET 的 Virtual Machine 不同的是：Python 的 Virtual Machine 是一种更高级的 Virtual Machine。这里的高级并不是通常意义上的高级，不是说 Python 的 Virtual Machine 比 Java 或 .NET 的功能更强大，更拽，而是说和 Java 或 .NET 相比，Python 的 Virtual Machine 距真实机器的距离更远。或者可以这么说，Python 的 Virtual Machine 是一种抽象层次更高的 Virtual Machine。




Python 提供了一种中间编译结果保存机制，即 byte code，或更准确地说，保存 PyCodeObject。事实上，Python 确实提供了这样一种机制，在执行一个 .py 文件的源代码之后，Python 并不会自动生成与该 .py 文件对应的 .pyc 文件。Python 会根据需要自动触发、创建 .pyc 文件，原理很简单，就是利用 Python 的 import 机制。




在 Python 运行过程中如碰到 import abc 这样的语句，Python 将会在设定好的 path 中寻找 abc.pyc 或 abc.dll 文件。若没有这样的文件，而只是发现了 abc.py 文件，那么，Python 会首先将 abc.py 编译成相应的 PyCodeObject 中间结果，然后再创建 abc.pyc 文件，并将中间结果写入该文件。接下来，Python 才会对 abc.pyc 文件进行一个 import 动作，实际上也就是将 abc.pyc 文件中的 PyCodeObject 重新在内存中复制出来，加以运行。




由于 Python .pyc 文件的此种运行机制，所以，可随时删除所有中间过程 .pyc 文件，当 Python 再次运行时，还会重生成这些 .pyc 文件，对您所编辑程序的运行并无任何影响。当然，得有源代码才行。




视窗用 python.exe 运行 .py ，用 pythonw.exe 运行 .pyw。因为安装视窗版 Python 时，扩展名 .py 自动被登记为用 python.exe 运行， 而 .pyw 则被登记为用 pythonw.exe 运行。 




**.py 文件**

以 .py 作扩展名的文件是 Python 源代码文件，由 python.exe 解释，可在控制台下运行。




当然，也可用文本编辑器或其它专用 Python IDE (集成开发环境) 工具进行修改。




**.pyc 文件**

以 .pyc 作扩展名的文件是 python 编译文件。.pyc 文件是不能直接用文本编辑器进行编辑，其优点是 .pyc 文件的执行速度要远快于 .py 文件。




至于为什么要有 .pyc 文件，因为 .py 文件是可直接看到源码的。若是软件开发商，不可能把源码泄漏出去？所以，就需编译成 .pyc 后再发布。




但 .pyc 文件只是经简单编译，并未加密，因此，有些工具还能反编译它得出源代码。




**.pyw 文件**

.pyc 文件执行时，桌面会出现类似 CMD 命令的黑色 shell 窗口，十分难看，于是 .pyw 文件就应运而生了。.pyw 文件与 .pyc 文件的执行，本质上并没什么区别，只是 .pyw 文件执行的时候不会出现类似 CMD 命令的黑色 shell 窗口。




.pyw 文件格式主要是设计用来运行 Python 纯 GUI (图形用户界面) 程序的。纯 GUI (图形用户界面) 程序的用户不需要看到类似 CMD 命令的黑色 shell 控制台窗口。当 .pyw 文件运行时，所有 stdout、stderr 输出无效，所有原 stdin 的读取只会得到 EOF。




值得一提的是，开发纯 GUI (图形用户界面) 程序时，可暂时把 .pyw 改成 .py，以便运行时调出控制台窗口，看到所有错误信息，方便修改、调试。




注意：采用 PyQt、PySide 等 GUI (图形用户界面) 开发框架开发的程序，源文件仍可采用 .py，无需单独使用 .pyw。




**.pyo 文件**

"python -O 源文件" 即可将 Python 源程序编译成 .pyo 文件，但有时仍得采用 .pyc 作后缀，才能正常运行。




.pyo 文件是相对 .pyc 而言的，优化编译后的 Python 文件。 同样，.pyo 文件也不能直接用文本编辑器进行编辑。




**.pyd 文件**

.pyd 文件是非 Python，由其它编程语言 "编写-编译" 生成的 Python 扩展模块。




Python 要导入 .pyd 文件，实际上是在 .pyd 文件中封装了一个 module。在 python 中使用时，把它当成 module 来用就可以了，即："import 路径名.modulename" 即可，路径名为 .pyd 文件所在的路径。




基于 Qt/C++ 的 PyQt、PySide GUI (图形用户界面) 开发框架绑定，会为每个 Qt/C++ 库 .dll 文件生成一个中间 .pyd 文件。PyWin32 项目分发包中，也存在很多 MicroSoft Windows OS 相应 .dll 文件的中间 .pyd 文件。




Cython 可将个人基于 Python 语言编写的 Python 模块编译成具有 C 语言特性的 .pyd 文件。




.pyd 文件有时也是用 D 语言按照一定格式编写，编译生成的二进制文件。




那么什么是 "D 语言" 呢？它是 C/C++ 的综合进化版，不仅具有二者的全部优点，且整体性能更佳，但其抽象程度高。D 语言最初由 Digital Mars 公司就职的 Walter Bright 于 2001 年发布，意图改进 C++ 语言。目前最新 D 语言被简称为 D2。最主要的 D 语言实现是 DMD。




D 语言源自 C/C++，借鉴了众多编程语言的特色和现代编译器技术，融会贯通了设计者丰富的实践经验，使之具备了非凡的威力 ── 既有 C/C++ 语言的强大威力，又有 Python 和 Ruby 的开发效率。它集众多系统级编程所需的功能于一身，例如垃圾回收、手工内存操作、契约式设计、高级模板技术、内嵌汇编、内置单元测试、Mixin 风格多继承、类 Java 包管理机制、内置同步机制、内建基本运行时信息。





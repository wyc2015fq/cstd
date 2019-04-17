# make与CMake的联系与区别 - 站在巨人的肩膀上coding - CSDN博客





2018年04月09日 20:37:38[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：137








下面是中文翻译部分： 
    编程人员已经使用CMake和Make很长一段时间了。当你加入一家大公司或者开始在一个具有大量代码的工程上开展工作时，你需要注意所有的构建。你需要看到处跳转的”CMakeLists.txt”文件。你应该会在终端使用”cmake”和”make”。很多人都是盲目的跟着操作说明而并不在意我们已何种方式去做我们需要做的事。构建的整个过程是什么？为什么要用这种方式去组织？Cmake和Make之间有什么区别？这有关系吗？他们可以互相转换吗？ 
    事实证明，它们之间有很多的不同。理解它们之间的不同很重要，这样你可以确保在使用的过程中不会陷入麻烦。在讲述它们之间的区别之前，我们首先来看看它们是什么。 
**Make**
    要设计一个软件系统，我们首先编写源码，然后通过编译器编译和创建可执行文件。可执行文件就是要实现最终功能的文件。“Make”是一个工具，它控制可执行程序和程序源文件中非源码文件的生成。 
    “Make”工具需要清楚的知道如何构建程序。 它通过一个叫做“makefile”的文件知晓如何构建你的程序。这个文件列出了所有的非源码文件以及如何由别的文件来计算它。当你编写了一个程序，你应该为它写一个makefile文件，这样才有可能通过使用“Make”来构建和安装你的程序。 很简单的事情。如果你不理解的话，多读几遍这一段文字，因为理解这一段文字对于接下来的篇幅很重要。 
为什么需要“Make” 
    需要”make”的一个原因是，它可以使得终端用户构建和安装你的应用包，而不用去详细的了解它具体是如何做到的。每一个工程都有它自己的规则和细微的差别，这会使得每次在复用的时候会变得很痛苦。这就是我们创建这个makefile文件的原因。 构建步骤精确的记录在你提供的这个makefile文件中。“Make” 当源码文件发生变化时自动的指出哪一个文件需要更新。 同时它也自动确定以适当的顺序进行更新文件，当一个非源码文件依赖的另一个非源码文件发生改变时。 
    每次当我们改变了系统中的一小部分源码的时候，重新编译整个程序的效率是很低的。因此，党我们改变了一小部分的源码文件的时候重新执行“Make”，它将不会重新编译整个程序。它仅仅更新那些直接或者间接依赖这些改变了的源码文件的非源码文件。很酷吧！“Make” 不局限于具体的语言。对于程序中的每一个非源码文件，makefile文件详细的说明了执行需要的shell命令。这些shell命令能够启动编译器产生目标文件，链接器产生可执行文件，ar更新库，镜像生成器格式化文档，等等。。。Make不仅仅局限于构建一个包。你也可以安装或者卸载一个包，生成索引表或者其他一些你经常做的值得你写下来怎么去做的事情。 
**CMake**
    CMake支持跨平台Make。 CMake 辨别使用那种编译器去编译给出的源码种类。如果你不知道使用何种编译器，你不能使用相同的编译器去编译所有不同种类的源码。你可以手动的指用何种编译器，但是这将变得繁琐和痛苦。CMake为每一种类型的目标平台按照正确的顺序调用命令。因此，将有很多非显式的命令，比如$(CC). 
    如果你是代码强迫症，请继续往下读。如果你不喜欢这一切，你可以跳过这一部分。一般的编译/链接标识处理头文件、库文件、以及重定位其他平台无关和构建系统独立命令。调试标识被包含，通过设置变量CMAKE_BUILD_TYPE 为“debug”，或者在调用程序的使用传递给CMake：cmake -DCMAKE——BUILD——TYPE：STRING=Debug。 
    CMake也提供平台无关的包含，通过‘-fPIC’标志（POSITION_INDEPENDENT_CODE属性）。因此，更多隐式的设置能够在CMake命令中实现，在makefile文件中也可以（通过使用COMPILE_FLAGS或者相关的属性）。当然，CMake在集成第三方库（像OpenGL）方面也变得更加轻便。 
**它们之间有什么不同点？**
    如果你要使用编译脚本，构建的过程中有一个步骤，也就是需要在命令行中输入”make”。 对于CMake，需要进行2步：第一，你需要配置你的编译环境（可以通过在你的编译目录中输入cmake ，也可以通过使用GUI客户端）。这将创建一个等效的，依赖你选择的编译环境的编译脚本或其他。编译系统可以传递给CMake一个参数。总之，CMake根据你的系统配置选择合理的的默认的选择。第二，你在你选择的编译系统中执行实际的构建。 
    你将进入GNU构建系统领域。 如果你不熟悉这个，这一段对于你来说就是碎碎念了。现在，说完了一些严肃的警告，往下走把！ 我们可以使用Autotool来比较CMake。当我们这样做的时候，我们会发现Make的缺点，而且这就是Autotool产生的理由了。我们可以看到CMake明显比Make优越的理由了。Autoconf 解决了一个重要的问题，也就是说与系统有关的构建和运行时信息的的可信赖发现。但是，这仅仅是轻便软件的开发中的一小部分。作为结尾，GNU工程已经开发了一系列集成的实用程序，用于完成Autoconf开始之后的工作：GNU构建系统中最重要的组件是Autoconf, Automake, and Libtool. 
    “Make”就不能那样做了，至少不修改任何东西是做不到的。你可以自己做所有的跨平台工作，但是这将花费很多时间。CMake解决了这个问题，但是与此同时，它比GNU构建系统更有优势：
- 用于编写CMakeLists.txt文件的语言具有可读性和很容易理解。 
- 不仅可以使用“Make” 来构建工程。
- 支持多种生产工具，比如Xcode, Eclipse, Visual Studio, etc.  
    CMake与Make对比具有以下优点：
- 自动发现跨平台系统库。
- 自动发现和管理的工具集
- 更容易将文件编译进共享库， 以一种平台无关的方式或者以比make更容易使用的的生成方式。 
    CMake不仅仅只“make”，所以它变得更复杂。从长远来看，最好能够学会使用它。如果你仅仅在一个平台上构建小的工程，“Make”更适合完成这部分工作。

下面是本文的英文原文部分： 
**CMake vs Make**
    Programmers have been using CMake and Make for a long time now. When you join a big company or start working on a project with a large codebase, there are all these builds that you need to take care of.You must have seen those “CMakeLists.txt” files floating around. You are supposed to run “cmake” and “make” commands on the terminal. A lot of people just follow the instructions blindly, not really caring about why we need to do things in a certain way. What is this whole build process and why is it structured this way? What are the differences between CMake and Make? Does it matter? Are they interchangeable? 
    As it turns out, they are quite different. It is important to understand the differences between them to make sure you don’t get yourself in trouble. Before getting into the differences, let’s first see what they are. 
**Make**
    The way in which we design a software system is that we first write code, then the compiler compiles it and creates executable files. These executable files are the ones that carry out the actual task. “Make” is a tool that controls the generation of executables and other non-source files of a program from the program’s source files. 
    The “Make” tool needs to know how to build your program.It gets its knowledge of how to build your program from a file called the “makefile”. This makefile lists each of the non-source files and how to compute it from other files. When you write a program, you should write a makefile for it, so that it is possible to use “Make” to build and install the program.Simple stuff! If you didn’t understand it, go back and read the paragraph again because it’s important for the next part. 
**Why do we need “Make”?**
    The reason we need “Make” is because it enables the end user to build and install your package without knowing the details of how it’s done. Every project comes with its own rules and nuances, and it can get quite painful every time you have a new collaborator. That’s the reason we have this makefile.The details of the build process are actually recorded in the makefile that you supply. “Make” figures out automatically which files it needs to update, based on which source files have changed.It also automatically determines the proper order for updating the files, in case one non-source file depends on another non-source file. 
    Recompiling the entire program every time we change a small part of the system would be inefficient.Hence, if you change a few source files and then run “Make”, it doesn’t recompile the whole thing. It updates only those non-source files that depend directly or indirectly on the source files that you changed.Pretty neat!“Make” is not limited to any particular language. For each non-source file in the program, the makefile specifies the shell commands to compute it. These shell commands can run a compiler to produce an object file, the linker to produce an executable, ar to update a library, Makeinfo to format documentation, etc.“Make” is not limited to just building a package either. You can also use “Make” to control installing or uninstalling a package, generate tags tables for it, or anything else you want to do often enough to make it worth while writing down how to do it. 
**CMake**
    CMake stands for Cross-platform Make. CMake recognizes which compilers to use for a given kind of source.In case you didn’t know, you can’t use the same compiler to build all the different kinds of sources. You can do this manually every time you want to build your project, but it would be tedious and painful. CMake invokes the right sequence of commands for each type of target. Therefore, there is no explicit specification of commands like $(CC). 
    For coding junkies who really want the gory details, read on. If you are not into all that, you can skip to the next section. All the usual compiler/linker flags dealing with the inclusion of header files, libraries, etc are replaced by platform independent and build system independent commands.Debugging flags are included by either setting the variable CMAKE_BUILD_TYPE to “Debug”, or by passing it to CMake when invoking the program:cmake -DCMAKE_BUILD_TYPE:STRING=Debug. 
    CMake also offers the platform independent inclusion of the ‘-fPIC’ flag (via the POSITION_INDEPENDENT_CODE property) and many others. Still, more obscure settings can be implemented by hand in CMake just as well as in a Makefile (by using COMPILE_FLAGS and similar properties). Of course CMake really starts to shine when third party libraries (like OpenGL) are included in a portable manner. 
**What is the difference?**
    The build process has one step if you use a Makefile, namely typing “make” at the command line.For CMake, there are two steps: First, you need to setup your build environment (either by typing cmake  in your build directory or by running some GUI client).This creates a makefile or something equivalent, depending on the build system of your choice (e.g. Make on *nix, VC++ or MinGW on Windows, etc). The build system can be passed to CMake as a parameter. However, CMake makes reasonable default choices depending on your system configuration. Second, you perform the actual build in the selected build system. 
    We are going to jump into the GNU build system territory here.If you are not familiar with that, this paragraph might look like jibber-jabber to you. Alright, now that I have given the statutory warning, let’s move on!We can compare CMake with Autotools. When we do that, we can see the shortcomings of Make, and they form the reason for the creation of Autotools. We can also see the obvious advantages of CMake over Make. Autoconf solves an important problem i.e. reliable discovery of system-specific build and runtime information. But this is only a small part in the development of portable software. To this end, the GNU project has developed a suite of integrated utilities to finish the job Autoconf started: the GNU build system, whose most important components are Autoconf, Automake, and Libtool. 
    “Make” can’t do that, at least not without modifying it anyway!You can make it do all that stuff but it would take a lot of time maintaining it across platforms.  CMake solves the same problem, but at the same time, it has a few advantages over the GNU Build System:
- The language used to write CMakeLists.txt files is readable and easier to understand.
- It doesn’t only rely on “Make” to build the project. 
- It supports multiple generators like Xcode, Eclipse, Visual Studio, etc. 

    When comparing CMake with Make, there are several advantages of using Cmake:
- Cross platform discovery of system libraries. 
- Easier to compile your files into a shared library in a platform agnostic way, and in general easier to use than make.
- Automatic discovery and configuration of the toolchain. 

    CMake does more than just “make”, so it can be more complex. In the long run, it’s better to learn how to use it. If you have just a small project on only one platform, then maybe “Make” can do a better job.




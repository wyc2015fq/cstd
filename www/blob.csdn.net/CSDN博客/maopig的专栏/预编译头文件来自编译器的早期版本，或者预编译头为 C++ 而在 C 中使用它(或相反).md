# 预编译头文件来自编译器的早期版本，或者预编译头为 C++ 而在 C 中使用它(或相反) - maopig的专栏 - CSDN博客
2011年09月07日 10:46:42[maopig](https://me.csdn.net/maopig)阅读数：629标签：[编译器																[c																[c++																[header																[properties																[compiler](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
当 Visual C++ 项目启用了预编译头 (Precompiled header) 功能时，如果项目中同时混合有 .c 和 .cpp 源文件，则可能收到 C1853 编译器错误：fatal error C1853: 'pjtname.pch' precompiled header file is from a previous version of the compiler, or
 the precompiled header is C++ and you are using it from C (or vice versa)（致命错误C1853: “filename.pch”预编译头文件来自编译器的早期版本，或者预编译头为C++ 而在C 中使用它(或相反)）。
该错误是因为当项目中混合了 .cpp 和 .c 文件时，编译器会对它们采取不同的编译方式（主要是因为对函数声明的处理方式不同），因而不能共用一个预编译头文件。在 VC++ 中，默认的预编译头文件是针对 C++ 的 (stdafx.h 和 stdafx.cpp），当然也可以创建针对 C 的预编译头。有趣的是，在旧版的 VC++ 中，这个错误的提示很具有误导性：fatal error C1853:
 'xxx.pch' is not a precompiled header file created with this compiler. 常常让人摸不着头脑。应该说，在新版中的这个提示是有所改进的。不过在网上搜索一番，对这个问题往往都是建议对整个项目取消预编译头的设置。这显然不是一个好的解决方案。对于一个比较大的工程来说，使用预编译头可以使总的编译时间大大减少。因而保留预编译头的设置才是比较好的解决方案。搜索
 MSDN，针对不同的情况，可以有不同的解决方案：
本人是用方法已解决的
方案1：适用于绝大多数文件是 .cpp 或绝大多数文件是.c的情况。在这种情况下，将少数的不同类文件设为不使用预编译头是比较平衡的做法，方法是：对于 VC++6.0，在 FileView 里对要取消预编译头的 .c (或 .cpp) 文件点右键，选择 settings，在弹出的对话框右边选择
 category 为 precompiled headers，再设置选项为 not using ...；（对于 
VS2005，则在 solution explorer 中对相应文件点右键选择 properties，在 precompiled headers 项下设置 not using... 即可。如果需要设置多个文件，则可以按住 Ctrl 键再同时选中这些文件并设置）PS：解释如下点击项目 点击属性 然后选择C/C++   预编译头 创建使用头文件 不使用预编译头文件
方案2：影响的文件比较多，则把它们都设置禁止预编译头的话仍然会使项目总体的编译速度大大降低，得不偿失。这时考虑可以为这组文件建立专用的预编译头。在 VC++ 极早期版本（1.5及以前版本）中是支持单个工程中建立分别针对 .c 和 .cpp 的预编译头的，但之后的版本中只支持单独的预编译头。在这种情况下，我们可以在workspace（或 solution）中建立一个新的静态链接库 (Static Library)
 工程，将所有的 .c 文件独立出来加入到该工程中单独编译，这样就可以在该静态链接库中针对 .c 文件创建预编译头。但是这样做在一定程度上需要被独立出来的代码在逻辑上是属于同一模块中的，这样才便于维护。不过从设计的角度来说，这个要求一般是满足的，否则就应考虑下项目的总体设计了:P 最后别忘了设置原项目的依赖项 (dependency) 为独立出来的这个静态库项目。

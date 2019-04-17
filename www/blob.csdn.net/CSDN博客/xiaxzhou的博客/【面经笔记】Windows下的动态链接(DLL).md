# 【面经笔记】Windows下的动态链接(DLL) - xiaxzhou的博客 - CSDN博客





2017年09月05日 11:59:50[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1295








## 使用DLL的优点

共享、模块化，可方便的组合，重用，升级

### 基地址和RVA

当一个PE文件装载时，其进程地址空间中的起始地址就是基地址，对于可执行文件exe，一般为0x400000，对于DLL文件一般为0x10000000。

若该地址被占用，则会选用其他空闲的地址，RVA就是一个地址相对于基地址的偏移。

### 声明

可以通过`_declspec(dllexport)`表示该符号是从dll导出的符号，`_declspec(dllimport)`表示符号从别的dll导入的符号。

在C++中，如果希望导入导出的符号符合C的规范，必须在这个符号的定义之前加上extern” C”，防止C++编译器进行符号修饰。

默认情况下，MSVC把C语言的函数当做_cdecl类型，这种情况下，它对该函数不进行任何符号修饰。

但一旦我们使用其他函数调用规范时，MSVC编译器就会对符号名进行修饰。比如_stdcall调用规范的函数会被修饰为以`"_"`开头，以`"@n"`结尾，**n表示函数调用时，参数所占堆栈空间大小**。

我们常看到windows的API使用“WINAPI”方式声明，其实“WINAPI”实际上是被定义为_stdcall的宏。WINDOWS 的API函数都是__stdcall类型的。 

但是WINDOWS 的API没有出现“_Add@16”这种被修饰了的命名方式。这是因为，它使用了.def文件将导出函数重命名。

#### 调用约定：

函数的调用约定，顾名思义就是对函数调用的一个约束和规定(规范)，描述了函数参数是怎么传递和由谁清除堆栈的。它决定以下内容：(1)函数参数的压栈顺序，(2)由调用者还是被调用者把参数弹出栈，(3)以及产生函数修饰名的方法。

1.__cdecl 是C Declaration的缩写（declaration，声明），表示C语言默认的函数调用方法：所有参数从右到左依次入栈，由调用者负责把参数压入栈，最后也是由调用者负责清除栈的内容，一般来说，这是 C/C++ 的默认调用函数的规则，MS VC 编译器采用的规则则是这种规则。 

2.__stdcall是StandardCall的缩写，是C++的标准调用方式：所有参数从右到左依次入栈，由调用者负责把参数压入栈，最后由被调用者负责清除栈的内容，Windows API 所采用的函数调用规则则是这种规则。

另外，采用 __cdecl 和 __stdcall 不同规则的函数所生成的修饰名也各为不同，相同点则是生成的函数修饰名前缀都带有下划线，不同的则是后缀部分，当然，这两者最大的不同点就在于恢复栈的方式不同，而且这点亦是最为重要的。

### Dll显示运行时链接

windows提供了**三个API**：
- LoadLibrary()：装载DLL到进程的地址空间
- GetProcAddress()：查找符号的地址
- FreeLibrary()：卸载模块

## DLL的入口函数是什么，什么时候调用此函数？

[http://blog.csdn.net/xiaxzhou/article/details/76589061](http://blog.csdn.net/xiaxzhou/article/details/76589061)

DllMain函数为入口函数，(实际入口函数为DllMainCRTStartUp)

被DLL用来执行一些与进程或线程有关的初始化和清理工作。

DllMain的第二个参数fdwReason指明了系统调用Dll的原因，它可能是: 
- DLL_PROCESS_ATTACH：

1、**当创建新进程时**，系统加载exe及所需DLL文件，然后，系统创建主线程，并用这个线程调用每个DLL的DLLMain函数，传入DLL_PROCESS_ATTACH。所有DLL处理完后，系统执行C/C++启动代码，然后进入main函数。

2、**显示加载DLL过程**，LoadLibrary的时候，系统加载DLL，并使用调用加载函数的线程执行DllMain函数，传入DLL_PROCESS_ATTACH。
- DLL_PROCESS_DETACH：

1、当将DLL从进程地址空间**撤销**时，传入DLL_PROCESS_DETACH。

2、当调用FreeLibrary时，如该线程的使用**计数为0时**，操作系统才会使用DLL_PROCESS_DETACH来调用DllMain。如使用计数大于0，则只是单纯的减少该DLL的计数。
- DLL_THREAD_ATTACH：

当进程**创建一个新线程**，则系统会检查当前已映射到该进程空间中的所有DLL映像，并用DLL_THREAD_ATTACH来调用每个DLL的DllMain。这告诉DLL需要执行与线程相关的初始化。且由新创建的线程负责执行DLLMain函数。

DLL_THREAD_DETACH：

**线程若要终止**，会调用ExitThread，但是系统不会立即终止线程，而是会让这个即将结束的线程用DLL_THREAD_DETACH来调用当前进程地址空间中的所有DLL镜像的DllMain函数，告诉DLL执行与线程相关的清理工作。  

当每个DLL的DllMain都处理完后，系统才会真正的结束线程。

### DLL的入口函数是必需的吗？

DLL入口函数实际上是_DllMainCRTStartup，而不是DllMain。

_DllMainCRTStartup 会对DLL_PROCESS_ATTACH通知进行处理：**初始化C/C++运行库，并完成全局、静态变量的构造**。然后调用DllMain函数。

当收到DLL_PROCESS_DETACH消息时，_DllMainCRTStartup 会先调用DLLMain函数，**DLLMain返回后**，再调用DLL中所有全局变量，静态变量的**析构函数**。

_DllMainCRTStartup 对线程消息不会做任何处理。

DLLMain函数在源码中不是必需的，连接DLL的时候，如果链接器无法在.obj文件中找到名为DLLMain的函数，那么**它会链接C/C++运行库提供的DLLMain函数。**

如果不提供自己的DllMain，那么C/C++运行库会认为我们不关心DLL_THREAD_ATTACH、DLL_THREAD_DETACH通知。

### 何为DLL的基地址重定位，重定位会有哪些缺点？如何优化？

DLL装载时，如果默认目标地址被占用，那么系统重新分配一块空间将DLL装载到这个地址。问题是**DLL代码段不是地址无关的**如：`MOV [0X00414540], 5`，DLL所涉及的绝对地址引用怎么办？

答案是对每个绝对地址引用都进行重定位。

当链接器在构建模块时，会将重定位段嵌入到生成的文件中，**这个段包含一个字节偏移量的列表，每个字节偏移量表示一条机器指令所使用的一个内存地址**。如果加载器能将模块载入到默认基地址，则系统不会访问模块的重定位段。

如果无法加载到默认基地址，系统会打开模块的重定位段，并遍历其中的所有条目，对每个条目，**加载器会先找到包含机器指令的那个存储页面，然后将模块的默认首选基地址与实际地址差值，加到机器指令当前正在使用的内存地址上。**

1、加载程序必须遍历重定位段**修改大量代码**。 

2、由于写时赋值机制，会强制这些模块的代码页面以系统的页交换文件为后备存储器。这意味着，**系统不能再抛弃模块的代码页面，直接载入模块在磁盘上的文件镜像。**

### DLL优化：

1、修改默认加载基地址 

2、模块绑定：每次程序运行时，都要重新进行符号的查找、解析、和重定位。如果将导出函数的地址写入到模块的导入表中，省去每次启动时的符号解析过程。




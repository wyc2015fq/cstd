# vc下DLL/lib的调用 - ljx0305的专栏 - CSDN博客
2009年11月04日 17:33:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：477标签：[dll																[windows																[library																[mfc																[api																[system](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
调用DLL，首先需要将DLL文件映像到用户进程的地址空间中，然后才能进行函数调用，这个函数和进程内部一般函数的调用方法相同。Windows提供了两种将DLL映像到进程地址空间的方法：
　　1、隐式的加载时链接
　　这种方法需要DLL工程经编译产生的LIB文件，此文件中包含了DLL允许应用程序调用的所有函数的列表，当链接器发现应用程序调用了LIB文件列出的某个函数，就会在应用程序的可执行文件的文件映像中加入一些信息，这些信息指出了包含这个函数的DLL文件的名字。当这个应用程序运行时，也就是它的可执行文件被操作系统产生映像文件时，系统会查看这个映像文件中关于DLL的信息，然后将这个DLL文件映像到进程的地址空间。
　　系统通过DLL文件的名称，试图加载这个文件到进程地址空间时，它寻找DLL 文件的路径按照先后顺序如下：
　　·程序运行时的目录，即可执行文件所在的目录；
　　·当前程序工作目录
　　·系统目录：对于Windows95/98来说，可以调用GetSystemDirectory函数来得到，对于WindowsNT/2000来说，指的是32位Windows的系统目录，也可以调用GetSystemDirectory函数来得到，得到的值为SYSTEM32。
　　·Windows目录
　　·列在PATH环境变量中的所有目录
　　VC中加载DLL的LIB文件的方法有以下三种：
　　①LIB文件直接加入到工程文件列表中
　　在VC中打开File View一页，选中工程名，单击鼠标右键，然后选中"Add Files to Project"菜单，在弹出的文件对话框中选中要加入DLL的LIB文件即可。
　　②设置工程的 Project Settings来加载DLL的LIB文件
　　打开工程的 Project Settings菜单，选中Link，然后在Object/library modules下的文本框中输入DLL的LIB文件。
　　③通过程序代码的方式
　　加入预编译指令#pragma comment (lib,"*.lib")，这种方法优点是可以利用条件预编译指令链接不同版本的LIB文件。因为，在Debug方式下，产生的LIB文件是Debug版本，如Regd.lib；在Release方式下，产生的LIB文件是Release版本，如Regr.lib。
　　当应用程序对DLL的LIB文件加载后，还需要把DLL对应的头文件（*.h）包含到其中，在这个头文件中给出了DLL中定义的函数原型，然后声明。
　　2、显式的运行时链接
　　隐式链接虽然实现较简单，但除了必须的*.dll文件外还需要DLL的*.h文件和*.lib文件，在那些只提供*.dll文件的场合就无法使用，而只能采用显式链接的方式。这种方式通过调用API函数来完成对DLL的加载与卸载，其能更加有效地使用内存，在编写大型应用程序时往往采用此方式。这种方法编程具体实现步骤如下：
　　①使用Windows API函数Load Library或者MFC提供的AfxLoadLibrary将DLL模块映像到进程的内存空间，对DLL模块进行动态加载。
　　②使用GetProcAddress函数得到要调用DLL中的函数的指针。
　　③不用DLL时，用Free Library函数或者AfxFreeLibrary函数从进程的地址空间显式卸载DLL。
引用:http://hi.baidu.com/kakakun1/blog/item/2a98dca79830279cd14358ad.html

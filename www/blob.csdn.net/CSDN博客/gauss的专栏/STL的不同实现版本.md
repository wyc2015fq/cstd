# STL的不同实现版本 - gauss的专栏 - CSDN博客
2013年01月08日 13:08:30[gauss](https://me.csdn.net/mathlmx)阅读数：393
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

 HP STL
HP STL是所有其它STL实现版本的根源。它是STL之父Alexander Stepanov在惠普的Palo Alto实验室工作时，和Meng Lee共同完成的，是第一个STL的实现版本（参见1.2节）。这个STL是开放源码的，所以它允许任何人免费使用、复制、修改、发布和销售该软件和相关文档，前提是必须在所有相关文件中加入HP STL的版本信息和授权信息。现在已经很少直接使用这个版本的STL了。
P.J. Plauger STL
P. J. Plauger STL属于个人作品，由P. J. Plauger本人实现，是HP STL的一个继承版本，因此在其所有头文件中都含有HP STL的相关声明，同时还有P. J. Plauger本人的版权声明。P. J. Plauger是标准C中stdio库的早期实现者，现在是C/C++ User's Journal的主编，与Microsoft保持着良好的关系。P. J. Plauger STL便是被用于Microsoft的Visual C++中的。在Windows平台下的同类版本中，其性能不错，但是queue组件（队列，一种容器）的效率不理想，同时由于Visual
 C++对C++语言标准的支持不是很好（至少直到VC6.0为止，还是如此），因此一定程度上影响了P. J. Plauger STL的性能。此外，该版本的源代码可读性较差，你可以在VC的Include子目录下找到所有源文件（比如：C:/Program Files/Microsoft Visual Studio/VC98/Include）。因为不是开放源码的（open source），所以这些源代码是不能修改和销售的，目前P.J. Plauger STL由Dinkumware公司提供相关服务，详情请见[http://www.dinkumware.com](http://www.dinkumware.com)。据称Visual
 Studio.NET中的Visual C++.NET（即VC7.0），对C++标准的支持有所提高，并且多了以哈希表（hash table）为基础而实现的map容器，multimap容器和set容器。
Rouge Wave STL
Rouge Wave STL是由Rouge Wave公司实现的，也是HP STL的一个继承版本，除了HP STL的相关声明之外，还有Rouge Wave公司的版权声明。同时，它也不是开放源码的，因此无法修改和销售。该版本被Borland C++ Builder所采用，你可以在C++ Builder的Include子目录下找到所有头文件（比如：C:/Program Files/Borland/Cbuilder5/Include）。尽管Rouge Wave STL的性能不是很好，但由于C++ Builder对C++语言标准的支持还算不错，使其表现在一定程度上得以改善。此外，其源代码的可读性较好。可以从如下网站得到更详细的情况介绍：[http://www.rougewave.com](http://www.rougewave.com)。遗憾的是该版本已有一段时间没有更新且不完全符合标准。因此在Borland
 C++ Builder 6.0中，它的地位被另一个STL的实现版本--STLport（见后）取代了。但是考虑到与以前版本的兼容，C++ Builder 6.0还是保留了Rouge Wave STL，只是如果你想查看它的源代码的话，需要在别的目录中才能找到（比如：C:/Program Files/Borland/Cbuilder6/Include/oldstl）。
STLport
STLport最初源于俄国人Boris Fomitchev的一个开发项目，主要用于将SGI STL的基本代码移植到其他诸如C++Builder或者是Visual C++这样的主流编译器上。因为SGI STL属于开放源码，所以STLport才有权这样做。目前STLport的最新版本是4.5。可以从如下网站得到更详细的情况介绍：[http://www.stlport.org](http://www.stlport.org)，可以免费下载其源代码。STLport已经被C/C++技术委员会接受成为工业标准，且在许多平台上都支持。根据测试STLport的效率比VC中的STL要快。比Rouge
 Wave STL更符合标准，也更容易移植。Borland C++ Builder已经在其6.0版中加入了对STLport的支持，它使用的STLport就是4.5版的，C++ Builder 6.0同时还提供了STLport的使用说明。你可以在C++ Builder的Include/Stlport子目录下找到所有头文件（比如：C:/Program Files/Borland/Cbuilder6/Include/Stlport）。
SGI STL
SGI STL是由Silicon Graphics Computer System, Inc公司实现的，其设计者和编写者包括Alexander Stepanov和Matt Austern，同样它也是HP STL的一个继承版本。它属于开放源码，因此你可以修改和销售它。SGI STL被GCC（linux下的C++编译器）所采用，你可以在GCC的Include子目录下找到所有头文件（比如：C:/cygnus/cygwin-b20/include/g++/include）。由于GCC对C++语言标准的支持很好，SGI
 STL在linux平台上的性能相当出色。此外，其源代码的可读性也很好。可以从如下网站得到更详细的情况介绍：[http://www.sgi.com](http://www.sgi.com)，可以免费下载其源代码。目前的最新版本是3.3。
![](http://www.7747.net/kf/UploadFiles_7205/200609/2006921101637525.gif)
![](http://www.7747.net/kf/UploadFiles_7205/200609/2006921101637529.gif)
                                                       STL家族的谱系 
摘自红色黑客联盟([www.7747.net](http://www.7747.net)) 原文：[http://www.7747.net/kf/200609/12755.html](http://www.7747.net/kf/200609/12755.html)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/beick/archive/2004/10/05/125336.aspx](http://blog.csdn.net/beick/archive/2004/10/05/125336.aspx)

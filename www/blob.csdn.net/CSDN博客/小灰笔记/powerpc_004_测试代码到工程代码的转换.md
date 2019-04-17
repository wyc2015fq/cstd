# powerpc_004_测试代码到工程代码的转换 - 小灰笔记 - CSDN博客





2018年08月29日 23:07:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：32
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









            前面经过几次尝试，已经做出了一个简单的小功能：实现一个软件执行时间的检测，检测到的单位是系统时钟的周期。

            如何能够把我们的代码变得更加工程化一点呢？那就是进行一个软件模块化的管理改进，毕竟，我们很难在一个C文件中把几十万行代码写完。

            关于模块，我们这里的定义是包含一个或几个功能的几个代码文件，包括C文件与头文件。通常，一个模块是可以独立通过编译的。

            接下来，我把这个软件执行时间测试的功能分离到一个叫timer模块中，存储于一个名称为timer的文件夹。首先创建如下文件：

![](https://img-blog.csdn.net/20180829230637490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            这算是构建一个模块比较简单的几个我呢间组合了，timer.c用于存放程序代码，timer.h用于声明函数接口，data相关的文件用于变量、常量的定义，types文件用于数据类型以及各种宏相关的处理。大致如此。

            创建文件之后，我可以把之前的软件代码全都转移到上述文件中。首先是程序代码：

![](https://img-blog.csdn.net/20180829230637503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            其中，头文件中包含其他的几个头文件。

            接下来处理数据文件：

![](https://img-blog.csdn.net/20180829230637475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            再往下，处理宏定义等信息。如同我在代码中的标注，其实这个适合再增加一个关于MCU的模块进行管理。目前，可以暂时放在这里。

![](https://img-blog.csdn.net/20180829230637573?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            剩下的就是几个变量、函数的声明了。解决后，可以尝试编译，根据错误或者警告提示修改出现的问题。值得一说的是，这是一次移植修改的过程，如果是做软件开发，从头开始构建一个模块的时候，应该先确认模块的文件，然后再设计头文件中的数据结构和接口。最后，完成相应的代码实现。软件其实还只是一个表达描述，困扰我们开发速度的一般不是编程语言而是我们对被控对象的认知。




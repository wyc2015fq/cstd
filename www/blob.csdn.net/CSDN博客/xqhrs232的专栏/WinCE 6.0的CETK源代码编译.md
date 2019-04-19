# WinCE 6.0的CETK源代码编译 - xqhrs232的专栏 - CSDN博客
2013年06月25日 23:18:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：744
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/js_gary/article/details/6264582](http://blog.csdn.net/js_gary/article/details/6264582)

最近做的camera和fir驱动老是有CETK过不了，经常要和这个CETK打交道，但仅仅通过CETK的log信息有很多时候不能得出正确得判断，而 且有时候都怀疑CETK本身就有bug，所以就在想能不能通过微软提供的CETK的源代码自己编译出相应的测试dll。
        微软提供的CETK的源代码在WINCE600/PRIVATE/TEST下可以找到，这个目录下有buildall.cmd这么个bat文件，照道理说 只要在visual studio 2005 platform builder的命令行下执行这个bat文件应该就可以生成所有的CETK DLL文件了，但是很不幸的是，不行，编译过程中会出很多错。
        仔细看了下源代码，才发现这些源代码中很多都没有dir文件和sources文件，难怪编译会出错的，看来只能自己想办法了，经过很多次的探索，终于发现了如果编译出相应的dll文件。
        以FIR的测试DLL文件为例，先找到源代码，我要找的FIR的测试源代码在
                WINCE600/PRIVATE/TEST/NET/IR/CETK/IRAPITST
        目录下，仔细看了下，这个目录下所有的dir文件和sources文件都没有，只有一堆的source code。经过分析，这个目录下几个有用的目录分别是：
        IOLIB         io库代码
        IRLIB        一些IR SOCKET的接口封装
        INC            头文件目录
         APITST     FIR CETK测试的client端代码
        IRSRV       FIR CETK测试的server端代码
我现在只想编译出client端的dll文件，所以将IRAPITST文件夹整个copy到自己的工作目录下（当然也可以直不copy，直接就在cetk目录下编译）。
        首先肯定要先编译出iolib和irlib，分别在IOLIB和IRLIB的目录下建立dir文件（做过WINCE的都应该知道怎么建立吧），然后分别为 他们编写sources文件，我就直接找一个现成的sources文件过来修改，分别将这两个文件夹下的文件编成iolib.lib和 irlib.lib（库的名字可以自己随便定义，自己认得就好），然后开始编主测试代码，同样给APITST建立dir文件和sources文件，在 sources文件里面加上链接进刚刚编译出的iolib.lib和irlib.lib，然后编译，结果一堆的错，很多接口找不到，其中包括kato的接
 口，kato的库好找，就在platform builde的安装目录
        Microsoft Platform Builder/6.00/cepb/wcetk/tux/lib/armv4i/retail
这个目录里面有很多CETK公用的lib库，需要自己一一确认链接。
        经过这么一折腾，kato的错误是少了，但是还有很多链接错误，后来仔细看了下，都是socket网络连接相关的，所以索性将ws2.lib库也链接进去，这样一改，发现居然就可以了。
        后来经过测试，这个编译出的dll和微软提供的dll是一样的，因此，CE6的CETK测试代码也是可以自己编译的。
        通过这次，总结下：
        1.微软的CETK的测试源代码不光可以用来定位错误，而且是可以编译的，也许微软自己也不能保证CETK的测试源代码是完全没问题的，事实证明，微软的CETK测试代码确实是有问题的，至少我测的FIR的源代码是有一个小BUG。
        2.CETK的代码不用做任何的修改，就可以直接编译通过，不过里面会有很多warning，需要在sources文件里面用WARNISERROR=3来控制warning级别，不然编译器会将warning当成error而编不过。
        3.CETK的测试源代码编译过程很繁琐，需要有耐心，自己去找出相互的依赖关系，如果有sources文件的话，那是最好，可以直接看出依赖于哪些 lib库，然后先编译出这些依赖的lib库，然后在编译主程序进行链接就可以了，如果没有sources文件，那就需要自己自己分析了。

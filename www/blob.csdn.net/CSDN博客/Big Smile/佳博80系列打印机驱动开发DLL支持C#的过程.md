# 佳博80系列打印机驱动开发DLL支持C#的过程 - Big Smile - CSDN博客
2017年10月22日 23:19:38[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1893
        最近小编的公司需要使用到佳博的打印机进行一系列的打印开发以及智能驱动，所以小编联系了佳博的官方客服，然后找到的他们的官方SDK开发包，进行开发，由于小编使用的是.net平台的C#语言，而官方的开发包里面没有对于C#的支持，所以弄出了一系列的情况，和大家分享一下，最后说一下如何用VB支持的DLL的API驱动去调用API。
资料地址：[http://download.csdn.net/download/tr1912/10035062](http://download.csdn.net/download/tr1912/10035062)
# 一、驱动分析
        我们看到了在VB中API驱动的样子，包括了所有的DLL读取和方法，都和API文档中有很大的区别，如图：
![](https://img-blog.csdn.net/20171022234347687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         如上图所示，在VB中是这样去调用DLL的，和C#中是类似的，我们来看一下C#中的dll调用：
![](https://img-blog.csdn.net/20171023003003001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        我们可以看到除了语法上有些许的不同之外，其他的其实都是一样的。我们在调用这些用老版本IDE或者规范生成的DLL的时候一定要记得把项目改为X86支持的，否则驱动的时候会有意想不到的情况。
        我们可以通过看里面的API文档（详见上方资源）可以发现，我们在文档中见到的变量类型还有返回值，和在VB中见到的是有差别的，当时我只是看到了VB中可以使用的资源，然后通过这些资源，改写到了C#中，结果出现了不能使用的问题，并且说是调用堆栈的不匹配的问题，这个问题说的就是API里面定义的标准和自己写的程序实际定义的不一样，比如在C#中是int类型，到了VB中就是long类型（详见上方代码比较），对于这一点需要注意语言的开发区别。只要按照文档中说的走一般就没有问题了。
# 二、驱动使用
        这里除了开发之外，我们还要讲一下使用的问题，其实在他的API定义程序中就已经可以见到最最简单的使用了。包括一个实例的程序和一个调用接口进行通信的一个样例。
         在这里我只是陈述一下连接打印机的一个实例，并且是用USB进行连接的，由于使用网络进行连接的需要用到socket的接口通信，所以这里不再做麻烦的陈述。
         我们会在API文档中找到一个函数POS_Open，这个函数用来使用我们的连接驱动，访问我们的打印机，并且可以获得打印机的状态。
        我们可以通过如下语句访问：
`int state = POSDLLFunctions.POS_Open("GP-L80160 Series", 0, 0, 0, 0, POSDLLFunctions.POS_OPEN_PRINTNAME);`        访问成功后，state中会存储了当前打印机句柄的一些数字，我们可以通过这些东西去访问到打印机这个本体。
        其他函数的使用，在VB源程序中都有相应的书写，大家可以直接转化过来使用。

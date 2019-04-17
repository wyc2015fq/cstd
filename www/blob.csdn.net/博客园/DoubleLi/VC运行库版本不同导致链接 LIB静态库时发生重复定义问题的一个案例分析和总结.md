# VC运行库版本不同导致链接.LIB静态库时发生重复定义问题的一个案例分析和总结 - DoubleLi - 博客园






MSDN中对于在不同的配置下Link的LIB作了说明：

**C Runtime Library****：**


|**开关**|**对应的库**|**版本**|
|----|----|----|
|/MD|MSVCRT.LIB|多线程DLL的Release版本|
|/MDd|MSVCRTD.LIB|多线程DLL的Debug版本|
|/MT|LIBCMT.LIB|多线程静态链接的Release版本|
|/MTd|LIBCMTD.LIB|多线程静态链接的Debug版本|
|/clr|MSVCMRT.LIB|托管代码和非托管代码混合|
|/clr:pure|MSVCURT.LIB|纯托管代码|





**C++ Standard Library****：**


|**开关**|**对应的库**|**版本**|
|----|----|----|
|/MD|MSVCPRT.LIB|多线程DLL的Release版本|
|/MDd|MSVCPRTD.LIB|多线程DLL的Debug版本|
|/MT|LIBCPMT.LIB|多线程静态链接的Release版本|
|/MTd|LIBCPMTD.LIB|多线程静态链接的Debug版本|





编译器会自动根据编译选项，选择对应的LIB文件。一般情况下这不会出现问题。

然而，在部分情况下，一旦你的程序的各个部分（LIB, OBJ…）并非由相同的编译选项编译出，而Link在一起的话，会出现各种各样的看似很难解决的问题，这类问题主要以重复定义的错误形式存在，通常的解决方法也很简单，就是选择同样的编译选项进行编译之后再Link。

......................


- 错误来源主要是重复定义的问题，而且重复定义的基本上都是VC Runtime和Standard C++ Library中的函数
- LIBCMT和LIBCPMT为Release下的Lib，本来不应该出现在Debug版本的链接的Lib中
- 重复定义的问题主要出现在：LIBCMT, LIBCPMT, MSVCPRTD, MSVCRTD

来看看出问题的LIB是那些：
- LIBCMT：C Runtime库的多线程静态链接的Release版本
- LIBCPMT：C++ Standard Library的多线程静态链接的Release版本
- MSVCPRTD：C++ Standard Library的多线程DLL的Debug版本
- MSVCRTD：C Runtime Library的多线程DLL的Debug版本

当前我们的配置是多线程DLL的Debug版，因此3和4是应该出现在link的列表中的，不属于多余。而后两者则是只是当多线程静态链接Release版中才会出现。这提示我在项目中加入的ANTLR.LIB可能是造成这个问题的根源，因为静态库的编译选项很容易和主程序发生冲突，并且根据实际信息我们可以看出ANTLR.LIB应该是用多线程静态链接的Release版本来编译的。

这样，解决方法就很清楚了：
- 切换到Release，因为ANTLR.LIB是在Release下面编译的
- 把Run Time库的版本修改成多线程静态链接

做了这两个修改之后编译通过。

还有一种方法是，自己用多线程DLL的Debug版重新编译一次ANTLR，生成一个新的ANTLRD.LIB，再link这个Lib也可以解决这个问题。

# Summary
- 知道各个不同的LIB代表的版本信息非常重要，可以帮助快速定位问题
- 在编程的时候，一定要把所有的项目的编译选项（是静态链接Runtime库还是动态链接Runtime库，Debug/Release）配置成一样的。如果部分LIB/OBJ是由第三方提供（OBJ情况很少见），一般情况下只能调整自己的编译选项，除非你可以要求第三方提供其他版本的编译好的LIB
- 在发布可重用的静态LIB库供其他人调用的时候，最好对应不同的编译选项，乃至VC版本，提供不同版本的LIB。VC自己的Runtime就是最好的例子。










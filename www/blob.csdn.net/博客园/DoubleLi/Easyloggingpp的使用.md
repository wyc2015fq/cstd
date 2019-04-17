# Easyloggingpp的使用 - DoubleLi - 博客园






对于有开发经验的程序员来说，记录程序执行日志是一件必不可少的事情。通过查看和分析日志信息，不仅可以有效地帮助我们调试程序，而且当程序正式发布运行之后，更是可以帮助我们快速、准确地定位问题。在现在这个开源的年代，并不需要我们自己去写一套日志库，开源的版本已经有很多了。然而遗憾的是，在目前开源的C++日志库中，并没有一个在使用份额上占绝对优势的，我自己之前用过[log4cplus](https://github.com/log4cplus/log4cplus)和[glog](https://github.com/google/glog)，这两个库都比较成熟了，而且网上教程也很多，所以一直都很喜欢使用这两个日志库。



前段时间，在GitHub上我偶然看到了另外一个开源的C++日志库，当时还只是粗略地研究了一下，结果竟然让我爱不释手，它就是Easylogging++。和log4cplus和glog相比，Easylogging++最大的特点就是它只有一个头文件，并且无需任何外部依赖，我从来没有见过哪一个日志库可以做到如此轻量而且还能兼顾性能的。除此之外，Easylogging++使用起来非常简单，同时还支持文件配置，提供了强大的自定义日志格式的能力，还提供对第三方库，STL容器的支持。面对如此炫酷的开源日志，怎么能不去学习学习！！！



Easylogging++项目在GitHub的开源地址：[https://github.com/easylogging/easyloggingpp](https://github.com/easylogging/easyloggingpp)



功能特征


- 高度可配置
- 速度非常快
- 线程和类型安全
- 跨平台
- 自定义日志模式
- 条件日志和偶然日志
- 性能跟踪
- 详细记录日志
- 中断处理
- 辅助CHECK宏
- STL 日志
- 第三方库日志（比如Qt, boost, wxWidgets）
- 可扩展性
- 支持Debug功能
- 系统日志记录
- perror风格日志
- C++流操作符风格日志
- C 语言printf风格日志
-  ……



版本说明



写这篇文章时，最新的 Easylogging++ 版本是V9.80，如果编译器不支持[C++](http://lib.csdn.net/base/cplusplus)11的话，是无法编译的。对于Visual Studio系列来说，必须是VS2012或以上版本才行。如果还停留在VS2010、VS2008，VS2005的小伙伴，可以考虑使用[EasyLogging++ V8.91](https://github.com/easylogging/easyloggingpp/tree/v8.91)版本。需要注意的是，不同的版本在使用方法和功能支持上都会有所差异。特别指出，本系列中所有的学习记录，只适用于 Easylogging++ 版本V9.80。另外，在本系列文章中出现的所有演示代码都是在 64Bit Windows 7 系统下使用 Visual Studio 2013 编译[测试](http://lib.csdn.net/base/softwaretest)的。



快速使用



正如前面所说，Easylogging++使用起来非常简单，简单到什么地步呢？下面是GitHub上给出一个示例代码：





[cpp] [view plain](http://blog.csdn.net/Fish_55_66/article/details/49367197#)[copy](http://blog.csdn.net/Fish_55_66/article/details/49367197#)







- #include "easylogging++.h"  
- 
- INITIALIZE_EASYLOGGINGPP  
- 
- int main(int argv, char* argc[]) {  
-    LOG(INFO) << "My first info log using default logger";  
- return 0;  
- }  


示例代码只有短短几行，每行代码的作用也一目了然：
- 包含easylogging++.h头文件
- 使用宏 INITIALIZE_EASYLOGGINGPP初始化
- 使用宏 LOG(INFO) 开始日志记录

就是这么简单，只需三步，即可完成你的日志记录。这里需要特别说明的是，初始化宏 INITIALIZE_EASYLOGGINGPP 必须使用而且只能使用一次，否则将会产生编译错误。最好的放置这个初始化宏的地方是位于程序入口函数所在文件的顶部，紧跟在包含头文件的代码后面。



自定义日志



Easylogging++强大的自定义日志格式的能力是毋容置疑的，我们可以通过配置文件、配置参数、配置宏定义等方式来定义自己想要的日志格式。下面是一个利用配置文件来自定义日志的一个例子：




[cpp] [view plain](http://blog.csdn.net/Fish_55_66/article/details/49367197#)[copy](http://blog.csdn.net/Fish_55_66/article/details/49367197#)







- #include "easylogging++.h"  
- 
- INITIALIZE_EASYLOGGINGPP  
- 
- int main(int argc, char** argv)  
- {  
-     el::Configurations conf("my_log.conf");  
-     el::Loggers::reconfigureAllLoggers(conf);  
- 
-     LOG(TRACE)   << "***** trace log  *****";  
-     LOG(DEBUG)   << "***** debug log  *****";  
-     LOG(ERROR)   << "***** error log  *****";  
-     LOG(WARNING) << "***** warning log  *****";  
-     LOG(INFO)    << "***** info log  *****";  
- 
-     system("pause");  
- return 0;  
- }  



其中配置文件 my_log.conf 的内容如下：




[plain] [view plain](http://blog.csdn.net/Fish_55_66/article/details/49367197#)[copy](http://blog.csdn.net/Fish_55_66/article/details/49367197#)







- * GLOBAL:  
-     ENABLED                 =   true  
-     TO_FILE                 =   true  
-     TO_STANDARD_OUTPUT      =   true  
-     FORMAT                  =   "[%level | %datetime] | %msg"  
-     FILENAME                =   "log\\log_%datetime{%Y%M%d}.log"  
-     MILLISECONDS_WIDTH      =   3  
-     PERFORMANCE_TRACKING    =   false  
-     MAX_LOG_FILE_SIZE       =   1048576  
-     LOG_FLUSH_THRESHOLD     =   0  
- 
- * TRACE:  
-     FILENAME                =   "log\\trace_log_%datetime{%Y%M%d}.log"  
- 
- * DEBUG:  
-     FILENAME                =   "log\\debug_log_%datetime{%Y%M%d}.log"  
- 
- * FATAL:  
-     ENABLED                 =   false  
- 
- * ERROR:  
-     FILENAME                =   "log\\error_log_%datetime{%Y%M%d}.log"  
- 
- * WARNING:  
-     FILENAME                =   "log\\warning_log_%datetime{%Y%M%d}.log"  
- 
- * INFO:  
-     FILENAME                =   "log\\info_log_%datetime{%Y%M%d}.log"  
- 
- * VERBOSE:  
-     ENABLED                 =   false  



通过这样的一个配置文件生成出来的日志会非常整齐，同时日志信息会保存到不同的文件中，而且还会输出到标准输出窗口。如下：

![](https://img-blog.csdn.net/20151025122229835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



看到这里，相信对于那些以前用过类似log4cplus或者glog等日志库的小伙伴来说，其实已经完全可以上手使用Easylogging++了，剩下的部分都是一些细节或者是Easylogging++扩展性的功能介绍，这些会在后面的学习中逐渐加以记录。如果有和我一样看到并且就喜欢上Easylogging++的朋友，就让我们一起来深入了解Easylogging++并把它应用到实际的开发之中吧。










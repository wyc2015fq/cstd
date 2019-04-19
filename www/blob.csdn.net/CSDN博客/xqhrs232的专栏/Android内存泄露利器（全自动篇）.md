# Android内存泄露利器（全自动篇） - xqhrs232的专栏 - CSDN博客
2012年03月28日 21:39:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：611标签：[android																[excel																[java																[手机																[rss](https://so.csdn.net/so/search/s.do?q=rss&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hubingforever.blog.163.com/blog/static/1710405792011324114317115/](http://hubingforever.blog.163.com/blog/static/1710405792011324114317115/)
我写了可执行文件启动器Launcher.jar及一些批处理，通过它们就可以自动的以一定的时间间隔提取Hprof和进程的内存信息
**一、需要的库**
### 可执行文件启动器：lib\Launcher.jar
**注**：关于Launcher.jar的源码请参考《[可执行文件启动器（上）](http://hubingforever.blog.163.com/blog/static/171040579201132565913624/)》和《[可执行文件启动器（下）](http://hubingforever.blog.163.com/blog/static/171040579201132565913624/)》
二、自动提取Hprof。
runHprofPicker.bat文件：
java -jar lib\Launcher.jar getHprof.bat5000
5000表示5000毫秒执行一次 getHprof.bat来从手机提取Hprof文件。
**注意1**：关于getHprof.bat文件请阅读《[Android内存泄露利器（hprof篇）](http://hubingforever.blog.163.com/blog/static/171040579201132335951549/)》
三、自动提取**进程的**RSS****
runProcessStatePicker.bat文件：
java -jar lib\Launcher.jar getProcessState.bat 5000
5000表示5000毫秒执行一次getProcessState.bat来从手机提取进程的RSS，输出到一个以进程名命名的文本文件，以便在excel中生产图表文件。
**注意1**：关于getProcessState.bat请阅读《[Android内存泄露利器（RSS内存统计篇）](http://hubingforever.blog.163.com/blog/static/17104057920113252542215/)》
四、自动提取**进程的**NativeSize，DalvikSize，NativeAllocated，DalvikAllocated，NativePSS，DalvikPSS
runMemoryPicker.bat文件：
java -jar lib\Launcher.jar getMemoryState.bat 2000
2000表示2000毫秒执行一次getMemoryState.bat来从手机提取进程的内存信息到一个以进程名命名的文本文件，以便在excel中生产图表文件。
**注1**：关于Launcher.jar的源码请参考《[可执行文件启动器（上）](http://hubingforever.blog.163.com/blog/static/171040579201132565913624/)》和《[可执行文件启动器（下）](http://hubingforever.blog.163.com/blog/static/171040579201132565913624/)》
注2：关于getMemoryState.bat请阅读《[Android内存泄露利器（内存统计篇）](http://hubingforever.blog.163.com/blog/static/17104057920114249163667/)》

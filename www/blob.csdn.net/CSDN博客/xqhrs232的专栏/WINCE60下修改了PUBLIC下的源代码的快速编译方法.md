# WINCE60下修改了PUBLIC下的源代码的快速编译方法 - xqhrs232的专栏 - CSDN博客
2009年10月13日 10:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1540
WINCE60下修改了PUBLIC下的源代码的快速编译方法
因为WINCE的组件都是分成一个个LIB/DLL的形式存在，所以只要把单个LIB/DLL编译更新然后再重新连接NK就可以起到快速编译NK的方法。
不过由于NK里面的各个组件都是DLL的形式，这些DLL依存一些对应的LIB文件，所以要更新DLL就得先更新那些LIB的内容先，而WINCE60下的PUBLIC
下的组件一般是以LIB文件的形式存在，所以要快速编译PUBLIC下的源代码就先编译这些LIB文件，更新完这些LIB文件再更新依赖这些LIB文件的DLL
，也只有把DLL也更好了，最后对PUBLIC（LIB形式）的修改才能最后反应到NK里面去。他们有一个顺序：Lib----Dll----NK.
想知道DLL对那些LIB文件依赖，可以看形成DLL文件的文件夹下的SOURCES文件。我是以串口驱动编译为例子做的实验，更新了MDD层代码（PUBLIC）编译完成还得要
更新编译PDD层（BSP）的东西，这样最后的串口驱动程序才能包括PUBLIC里面更新了的代码。
1.rebuild and clean sysgen----更新LIB/DLL（一般是PUBLIC目录下的文件夹）
2.build/rebuild----更新DLL（一般是BSP下的文件夹，BSP里面跟PUBLIC里面的LIB连接产生DLL）
3.viewbin -t nk.bin----查看DLL的时间戳看更新没有！！！
4.特别要注意DLL跟LIB之间的依存关系，如果是依存关系就得先更新LIB再更新DLL
5.应用WINDOWS的搜索文件的工具可以快速地查看对应的文件有没更新，对应的文件的建立时间等等。

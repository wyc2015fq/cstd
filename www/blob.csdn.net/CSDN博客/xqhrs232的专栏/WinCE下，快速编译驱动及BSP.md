# WinCE下，快速编译驱动及BSP - xqhrs232的专栏 - CSDN博客
2013年01月28日 08:50:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：901
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/article/details/1962802](http://blog.csdn.net/nanjianhui/article/details/1962802)
相关网帖
1、[WINCE下如何生成debug版dll文件](http://blog.csdn.net/xqhrs232/article/details/8563907)----[http://blog.csdn.net/xqhrs232/article/details/8563907](http://blog.csdn.net/xqhrs232/article/details/8563907)
2、[WinCE系统的编译过程](http://blog.csdn.net/xqhrs232/article/details/8536111)----[http://blog.csdn.net/xqhrs232/article/details/8536111](http://blog.csdn.net/xqhrs232/article/details/8536111)
3、[wince下，修改private下的代码，如何进行快速编译，比如之编译自己修改部分的代码](http://blog.csdn.net/xqhrs232/article/details/8536097)----[http://blog.csdn.net/xqhrs232/article/details/8536097](http://blog.csdn.net/xqhrs232/article/details/8536097)
4、[在WinCE5.0和WinCE6.0下，编译选项介绍](http://blog.csdn.net/xqhrs232/article/details/8549623)----[http://blog.csdn.net/xqhrs232/article/details/8549623](http://blog.csdn.net/xqhrs232/article/details/8549623)
5、[WinCE系统的编译过程&快速编译(转载)](http://blog.csdn.net/xqhrs232/article/details/4855495)----[http://blog.csdn.net/xqhrs232/article/details/4855495](http://blog.csdn.net/xqhrs232/article/details/4855495)
6、[WinCE Emulator使用介绍](http://blog.csdn.net/xqhrs232/article/details/8563474)----[http://blog.csdn.net/xqhrs232/article/details/8563474](http://blog.csdn.net/xqhrs232/article/details/8563474)

作者：ARM-WinCE
在WinCE下面，创建工程，然后编译是需要花很长时间的。当我们第一编译完工程以后，如果需要改我们的驱动或者BSP，我们是不需要重新编译整个工程的：
1. 如果更改了driver，以WinCE6.0为例，我们可以在菜单里面选择“build”->“Open Release Directory in Build Window” ，然后在弹出的命令行窗口中，通过dos命令切换到你要编译的driver的目录下面，然后运行“build”就可以了。也可以运行“build -c”会强制把所有的文件都编译一遍。被编译后的driver的dll会被自动拷贝到release目录下面，然后再切换的工程的release目录下面，运行一下“make image”就可以了。
2. 如果改变了OAL部分的代码，同样用上面介绍的方法，需要注意的是，OAL部分可能包含多个文件夹，如果改变了OAL里面的代码，不要进入OAL里面的文件夹去编译，一定要在OAL这层进行编译，这样OAL部分的lib，dll才会被重新编译并拷贝到release目录下面。
3. 如果改变了配置文件，比如config.bib，platform.reg文件，那么直接将这些文件拷贝到你的工程目录下面，然后运行一下“make image”就可以了。
4. 如果改变了eboot部分的代码，那按照步骤1的方法就可以了，可能你都不需要运行“make image”命令，因为可能你只需要eboot.bin或者eboot.nb0。
//=====================================================================
备注::
     下面的一个自己的个人总结主要以WINCE60为操作对象．．．．．．
1、第一次创建新的OS工程最好使用Clean Sysgen命令
2、当改变SYSGEN变量或者OS的CATALOG时也最好使用Clean Sysgen命令
3、最好不要去用Build And Sysgen命令，只有改变了Public部分的源代码才用这个命令，不过最好还是一个模块一个模块去弄，不要一下子去重新编译整个Public目录，也就是你要动那个Lib/Dll就去对应编译那个Lib/Dll，所以Rebuild  And Clean Sysgen也建议最好不用
4、怎么编译一个模块，定位到它的目录（在VS2005解决方案视图），选中它鼠标右键弹出命令菜单，执行BUILD/REBUILD命令即可，不要去执行Sysgen/Build And Sysgen/Rebuild And Clean Sysgen。这样编译后通常在D:\WINCE600\PUBLIC\COMMON\OAK\LIB\ARMV4I\RETAIL目录下对应的LIB库文件会得到更新，可以查看文件的属性，注意其文件大小跟建立时间的变化
(今天试了一下，这样是乎不行，选中那个模块用Sysgen/Build
 And Sysgen/Rebuild And Clean Sysgen反可更可以！，直接更新到模拟器，让模拟器马上就可以打印出来！)
5、模块更新后，怎么把更新的模块打包进新的NK.bin，执行Build菜单下的Copy files to Release Directory++++Make Run-time Image就可以了，新的模块有没更新进NK，也可以通过viewbin -t nk.bin命令来看，看新的NK里面包含的对应模块是不是新建立的，可以通过看文件大小和文件的创建时间是不是改变了来得到确认。直接到命令行的Release Directory Windows下执行make image如下......
D:\WINCE600\OSDesigns\2013_1_22_Emulator_PDA_Test\2013_1_22_Emulator_PDA_Test\RelDir\DeviceEmulator_ARMV4I_Release>make image
make: *** No rule to make target `image'.  Stop.
（应该是makeimg命令，而不是make image，make是linux下命令,windows下一般是nmake）
参考网帖----WINCE60下如何进行快速编译----[http://bbs.csdn.net/topics/340054138?page=1#post-393633003](http://bbs.csdn.net/topics/340054138?page=1#post-393633003)WINCE6.0下如何进行快速编译 
6、MS发布WINCE60的时候，已经把OS那块的东西按一个模块一个模块的形式编译好了，所以我们不要去把所有的模块重新编译一遍的，顶多我们想分析那个模块就去编译那个模块，我们要修改那个模块就对应去修改编译那个模块，MS也不希望我们重复造相同的轮子，因为这样没有任何实际的意义，也带不来任何创新。
7、选中模块会弹出一个菜单项，其中有很多的功能！很方便使用。
   1>Build----只针对该模块编译，一般只更新LIB库文件
    2>Rebuild----只针对该模块编译，一般只更新LIB库文件
    3>Sysgen----组装到NK里面去
    4>Build and Sysgen----组装到NK里面去
   5>Rebuild and Clean Sysgen----组装到NK里面去
   6>打开------可以直接打开对应的dirs文件
   7>打开方式....
   8>Open Makefile------可以直接打开对应的Makefile文件
   9>Add-----可以在对应的文件夹下增加头文件/源文件
  10>Find in Files...------直接在对应的文件夹包含的文件里面查找
  11>Explore-----直接定位到对应文件夹的路径
  12>Open Build Window----可以直接定位到对应路径下，直接去进行build/build -c
  13>属性----显示该模块编译的一些属性与特征
  8、特别注意改变了OAL部分的代码----其实直接定位到OAL命令选中它，右键弹出菜单选择REBUILD命令就可以了，单单定位到OALEXE/OALLIB再选择REBUILD命令是更新不到NK里面的！！！
       1》直接选中然后open build window执行build -c命令然后就在OAL命令的DOS窗口下执行makeimg也是可以更新OAL层的代码的！！！没必要一定要切换到release目录下！！！

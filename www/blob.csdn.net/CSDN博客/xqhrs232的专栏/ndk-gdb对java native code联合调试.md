# ndk-gdb对java/native code联合调试 - xqhrs232的专栏 - CSDN博客
2012年05月03日 17:19:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1273
原文地址::[http://archive.cnblogs.com/a/2393959/](http://archive.cnblogs.com/a/2393959/)
网上有不少文章，谈论 ndk-gdb对java/native code联合调试 ，本人参考了不少，虽然这些文章没能解决问题，但是给了不少的帮助，在此，分享本人搭建 java +native code(c c++)环境的整个过程。
      1.  gdb 调试器 与 gdbserver 的关系，就是 gdb 与 stub的关系，如下图所示
![](http://images.cnblogs.com/cnblogs_com/yaozhongxiao/gdb-stub.png)

      2.  调试工具 及调试环境
           在搭建android调试环境过程中，很多文章中原理上做了比较好的介绍，但开始始终无法成功，最后发现运行的平台，使用的工具等一些细节方面是失败的主要原因。
            运行环境 ：   cygwin
             ndk  ：   android-ndk-r7b
             gdb  ：   ${ndk-home}\toolchains\arm-linux-androideabi-4.4.3\prebuilt\windows\bin\arm-linux-androideabi-gdb
                            (version = 6.6)
            gdbserver ： 每个模拟器上， 在system/bin/gdbserver 有默认的程序 
                     失败配置： gdbserver (version 7.1)    :     android emulator : 4.0 (version)     14 (API level)
                  成功配置：   gdbserver()                    :     android
 emulator : 2.3.3 (version)    10 (API level)
       注意：经过尝试必须注意 gdb  vs  gdbserver版本的匹配，
                  如果system/bin下的gdbserver 不匹配，可以把匹配的gdbserver push   emulator 然后指定**./**gdbserver ,
必须指定"./"，因为linux下面，默认是寻找 system目录 
（第一个陷阱）
3.  ndk 与 native code 这里不具体分析，可参考我其他的文章，
             这里我们使用 ndk-build NDK_DEBUG=1 生成带符号表的 .so
4.  gdb  gdbserver 调试过程  [2]
Android
 NDK使用C/C++开发的native code，在Linux下调试主要使用GDB调试。
          
 当然使用Logcat或自己写入文件的方式调试也是可以的。
          
 有关Log打印可以参考Android开发网早期文章。 
    对于使用ndk-build编译出来的工具，除了so库文件外，会有gdbserver和gdb.setup文件，方法为
  (1). 在PC的命令提示符下进入ndk根目录，执行ndk-build NDK_DEBUG=1 这样，将会强制生成debug调试版的文件，将设我们NDK工程名为androidApp ，这里要说明的是编译出来的库文件在libs目录下是没有符号的，我们要obj下的文件，否则symbol无法正确加载。
  (2).  使用ant debug 或者 eclipse 直接生成 androidApp_debug.apk
  (3). 我们将生成的gdbserver和so库文件复制到/data目录下面，使用adb push命令，或者直接使用Eclipse中DDMS的File Explorer手动复制，不过对于真机可能没有这个列举目录的权限，你可以使用Android模拟器。
      或者 使用 adb install -r androidApp_debug.apk 将apk加载到模拟器
  (4).  使用adb shell命令 进入Android手机或模拟器的命令行，进入androidApp 包，
       ps  查看androidApp 进程 pid
      启动gdbserver，如 ./gdbserver :5039  --attach pid
      会看到 Listening on port 5039   这代表正在监听。
(5).  启动客户端  adb forward tcp:5039 tcp:5039  重定向到gdbserver监听端口
 (6) . ${NDK-HOME}\toolchains\arm-linux-androideabi-4.4.3\prebuilt\windows\bin\arm-linux-androideabi-gdb.exe 进入调试状态
![](http://images.cnblogs.com/cnblogs_com/yaozhongxiao/arm_gdb.png)
  (7). (gdb)  target remote :5039  连接 gdbserver进行调试
注意： 在第(6)过程中，可以发现 "no symbol table is loaded"，
             是应为打包在 androidApp_debug.apk 里面的androidApp.so 是不带有符号表的，
           需要设置 solib-search-path 来指定收索so的路径，
           之前可通过show solib-search-path 先观察当前搜索so的路径
          (  陷阱二：在linux下面，路径 使用/，如  debug版本so文件所在路径为
               solib-search-path  ./obj/local/armeabi/***.so，
               而在window下使用cygwin，set solib-search-path .\obj\local\armeabi\***.so )
       此处，由于刚开始对原理不是很清楚，花费了很多时间，都无法加载sysbol  table，痛苦啊！！！
         接下来，就可以使用gdb进行调试。
          5. 上述调试方法，所有的都由使用者完成，对于理解整个调用过程，很有帮助，但在实际的项目过程中，可以采用ndk-gdb 来进行调试
              具体过程可参考如下 [http://www.devdiv.com/thread-32770-1-1.html](http://www.devdiv.com/thread-32770-1-1.html)
              再次强调，
              (1). 利用这种方式，必须保证gdb 版本与 gdbserver一致， android emulator 版本不能太高，
                  我采用的版本为 2.3.3 api level 10 才成功，否则，可能会产生莫名其妙的问题
              (2). 利用这种方式，eclipse 生成的gdb.setup 文件中 set solib-search-path ./obj/local/armeabi/***.so
                   在cygwin下运行需要人工修正     set solib-search-path .\obj\local/armeabi\***.so
         6. 如果你把所有的都集成到eclipse中，ide图形化统一对java，native code 进行编码，运行与调试，
          参考Eclipse+CDT+GDB调试android NDK程序[4]  [http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html](http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html)
参考：
  1.   GDBstub系列剖析与改进  [http://www.jdzj.com/plc/article/2011-12-28/33206-1.htm](http://www.jdzj.com/plc/article/2011-12-28/33206-1.htm)
        GDB 远程调试 gdbserver        [http://zjf30366.blog.163.com/blog/static/411164582009319501626/](http://zjf30366.blog.163.com/blog/static/411164582009319501626/)
  2.    使用GDB调试Android NDK开发的程序    [http://www.android123.com.cn/topic/ndk/962.html](http://www.android123.com.cn/topic/ndk/962.html)
  3.    使用eclipse/ndk-gdb对java/native code联合调试       [http://www.devdiv.com/thread-32770-1-1.html](http://www.devdiv.com/thread-32770-1-1.html)
  4.    Eclipse + CDT +GDB 调试android NDK程序 ： [http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html](http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html)
[》点击查看原文...](http://www.cnblogs.com/yaozhongxiao/archive/2012/03/13/2393959.html)

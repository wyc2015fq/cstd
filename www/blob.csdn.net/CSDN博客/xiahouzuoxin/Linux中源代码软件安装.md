# Linux中源代码软件安装 - xiahouzuoxin - CSDN博客





2012年09月16日 21:47:42[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1476








       我们常常在各种开源站点下载到源代码程序，通常为*.tar.gz格式，这里以一份用于Audio的开源代码库（libsndfile）的编译安装为例，说明源代码的编译安装过程。

**Step1：**打开README文件，几乎所有的开放源代码中都含有该文件。别的不看，我们找到以下几行，

                If you really do need to compile from source it should be as easy as:
                ./configure
                make
                make install




               Since libsndfile optionally links against libFLAC, libogg and libvorbis, you

               will need to install appropriate versions of these libraries before running

               configure as above.


         上面的文字给出了安装步骤。意思是说，我们得按照./configure、make、make install共3步完成安装，而且./configure的执行依赖于libFLAC, libogg and libvorbis中的任何一个库。

**Step2：**配置

         运行./configure --prefix=/usr/libsndfile

         功能是：完成库依赖的检查工作，并且在依赖没问题的情况下生成makefile文件用具接下来的编译。

         我们可以先不管依赖的库是否已经安装，若编译出现错误，则再安装相应的库。

         选项--prefix=/usr/libsndfile作用，指定安装路径为/usr/libsndfile文件夹下，这样利于软件的卸载（直接删除该文件夹就行了）和移植（拷贝该文件夹）。

**Step3：**编译

         运行make，主要根据上步中生成的makefile完成编译功能。

**Step4：**安装

         运行make install，功能是按照Step2中--prefix配置的路径进行安装（实际上是文件拷贝）。




等待所有命令都运行结束，安装完成，再到/usr/libsndfile目录下查看是否有安装文件。
















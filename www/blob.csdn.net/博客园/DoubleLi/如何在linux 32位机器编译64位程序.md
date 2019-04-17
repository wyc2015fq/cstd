# 如何在linux 32位机器编译64位程序 - DoubleLi - 博客园






编译64位程序，不一定要编译机器是64位的，但是32位机器默认安装的gcc编译环境还是不能用来编译64位程序。

编译64位程序，需要加上-m64编译器参数，默认安装的gcc已经支持该参数，但是缺少64位机器指令相关的文件，所以不能编译，会出现下面的错误



**[plain]**[view plain](http://blog.csdn.net/luansxx/article/details/7822373#)[copy](http://blog.csdn.net/luansxx/article/details/7822373#)



- In file included from /usr/include/features.h:378,  
-                  from /usr/include/assert.h:37,  
-                  from ../../../include/tinyxml/tinystr.h:42,  
-                  from ../../../src/tinyxml/tinystr.cpp:32:  
- /usr/include/gnu/stubs.h:9:27: error: gnu/stubs-64.h: 没有那个文件或目录  


这时候需要安装 gcc所有支持文件 sudo apt-get install gcc-multilib



**[plain]**[view plain](http://blog.csdn.net/luansxx/article/details/7822373#)[copy](http://blog.csdn.net/luansxx/article/details/7822373#)



- 将会安装下列额外的软件包：  
-   cpp-4.4 g++-4.4 gcc-4.4 gcc-4.4-base gcc-4.4-multilib lib64gcc1 lib64gomp1 libc6-amd64 libc6-dev-amd64 libgcc1 libgomp1  
-   libstdc++6 libstdc++6-4.4-dev  
- 建议安装的软件包：  
-   gcc-4.4-locales g++-4.4-multilib gcc-4.4-doc libstdc++6-4.4-dbg libmudflap0-4.4-dev libgcc1-dbg libgomp1-dbg libmudflap0-dbg  
-   libcloog-ppl0 libppl-c2 libppl7 lib64mudflap0 libstdc++6-4.4-doc  
- 下列【新】软件包将被安装：  
-   gcc-4.4-multilib gcc-multilib lib64gcc1 lib64gomp1 libc6-amd64 libc6-dev-amd64  
- 下列软件包将被升级：  
-   cpp-4.4 g++-4.4 gcc-4.4 gcc-4.4-base libgcc1 libgomp1 libstdc++6 libstdc++6-4.4-dev  











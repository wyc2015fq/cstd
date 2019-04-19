# CMake 简介与使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年10月31日 13:45:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：173
cross platform make的缩写。
是一个比make更高级的编译配置工具，它可以根据不同平台、不同的编译器，生成相应的Makefile或者vcproj项目文件。通过编写CMakeLists.txt，可以控制生成的Makefile，从而控制编译过程。
Cmake 并不直接建构出最终的软件，而是产生标准的建构档（如 Unix 的 Makefile 或 Windows Visual C++ 的 projects/workspaces），然后再依一般的建构方式使用。这使得熟悉某个集成开发环境（IDE）的开发者可以用标准的方式建构他的软件。
CMake库：[艺搜下载](http://dl.vmall.com/c0itgu97ua)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=CMake&action=edit&section=1)]安装
.**/**bootstrap
**make****make ****install**
如果已经安装了cmake,想要安装新版本:
cmake .   **//**后面有个点    
**make****make ****install****cmake和autotools正是makefile的上层工具，它们的目的正是为了产生可移植的makefile，并简化自己动手写makefile时的巨大工作量。如果你自己动手写过makefile，你会发现，makefile通常依赖于你当前的编译平台，而且编写makefile的工作量比较大，解决依赖关系时也容易出错。因此，对于大多数项目，应当考虑使用更自动化一些的 cmake或者autotools来生成makefile，而不是上来就动手编写。**
 -G <generator-name>         = Specify a build system generator.
如：
 Unix Makefiles              = Generates standard UNIX makefiles.
 -D <var>:<type>=<value>     = Create a cmake cache entry.
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=CMake&action=edit&section=2)]艺搜参考
[下载CMake](http://www.cmake.org/cmake/resources/software.html)
[安装CMake](http://www.cmake.org/cmake/help/install.html)
[http://blog.csdn.net/fly_yr/article/details/49815705](http://blog.csdn.net/fly_yr/article/details/49815705)

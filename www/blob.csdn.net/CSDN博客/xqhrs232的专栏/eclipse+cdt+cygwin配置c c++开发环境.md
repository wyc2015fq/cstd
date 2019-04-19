# eclipse+cdt+cygwin配置c/c++开发环境 - xqhrs232的专栏 - CSDN博客
2011年06月20日 09:32:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1295标签：[eclipse																[gcc																[linker																[compiler																[windows																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx](http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx)
1.下载cygwin，默认的情况下Cygwin没有选择C++开发所需要的包，在安装时需要选中 ：
gcc, 
gcc-core,
gcc-g++, 
gcc-mingw-core,  
gcc-mingw-g++, 
make , 
gdb, 
binutils 
可以参考：[http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300292.aspx](http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300292.aspx)
2. 安装好后，需要把Cygwin加入Windows环境变量. 
因为gcc 和g++ 不能从windows cmd中直接执行， 可以通过直接在cmd中直接运行g++, gcc, 显示“访问被拒绝” (access denied),
进入cygwin安装目录/bin下检查g++.exe, gcc.exe是否只有1k大小，如果是，那么文件是符号链接，那么把文件重命名一下（备份）。 
然后把g++-3.exe(或者g++-4.exe)拷贝一份，重命名为g++.exe。 把gcc-3.exe拷贝一份，重命名为gcc.exe。 
将cygwin安装目录/bin加入到环境变量PATH中。好了，开个cmd命令行窗口，分别试一下gcc -v , make -v 命令。 
3.下载Eclipse CDT插件 
地址：[http://www.eclipse.org/cdt/downloads.php](http://www.eclipse.org/cdt/downloads.php)
4. 配置路径映射 
eclipse中调试时，由于GDB使用的unix格式的路径，而eclipse使用的是windows路径，导致找不到匹配的代码， 
需要手工设置： 
In Eclipse in Preferences->C/C++->Debug->"Common Source Lookup Path" add new "Path Mapping".
把linux的路径映射到windows的路径:
比如  /cygwin/c   映射成 C:/ 
5.选中编译器 
在C++项目右键Properties->C/C++ Build->toolchain 中确定 
Current toolchain = Cygwin Gcc，
Current builder = Gnu Make Builder，
Used tools 有：
GCC Assembler，
GCC Archiver，
Cywgin C++ Compiler，
Cywgin C Compiler，
Cywgin C Linker， 
Cywgin C++ Linker。
最后将Display compatible toolchains only选中。
注：如果在Current toolchai选项中没有 Cygwin GCC选项，需要点击上面Configuration右边的Manage Configuration -> New -> Import predefined - > (Excutable -> Cygwin GCC)
6. 创建一个新cpp项目 
![](http://hi.csdn.net/attachment/201104/3/0_1301839818ma4N.gif)
--------------------------------------------------------------------------------
![](http://hi.csdn.net/attachment/201104/3/0_1301839940JDrj.gif)
第一个cpp编译成功！ 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx](http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx)


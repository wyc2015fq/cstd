# Msys2+mingw-w64 编译VS2013使用的ffmpeg静态库注意事项 - DoubleLi - 博客园






1、环境准备

第一步：从http://sourceforge.net/projects/msys2/下载msys2的安装程序安装msys2;

第二步：通过msys2的包管理工具pacman安装mingw-w64，包括（gcc，make）等工具；

第三步：从github下载ffmpeg最新源代码；

2、编译

第一步：启动VS2013的命令行工具，在命令行中启动msys2安装目录下的mingw32_shell.bat；

第二步：移除或重命名msys2/usr/bin/link.exe，使用VS2013的link工具；

第三步：配置ffmpeg编译，./configure --toolchain=msvc，其它配置选项参照./configure --help;

第四步：编译、链接分别使用make、make install命令；

3、使用

第一步：在VS2013工程中引入编译好的库，配置好include路径和lib库路径；

第二步：在预编译头文件或其它合适的地方定义 #define inline __inline,详细参见[http://ffmpeg.org/platform.html](http://ffmpeg.org/platform.html)

第三步：设置线程选项为多线程MT，编译，链接成功！



另：对于SAFESEH处理，在链接器->命令行的其它选项中填入/SAFESEH:NO，应用即可。 










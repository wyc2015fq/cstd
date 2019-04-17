# vs2013编译ffmpeg之二  CMake - 枫林港的博客 - CSDN博客





2018年07月07日 22:40:19[枫林港](https://me.csdn.net/u013601132)阅读数：460标签：[ffmpeg																[vs2013																[cmake](https://so.csdn.net/so/search/s.do?q=cmake&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





很多ffmpeg依赖的第三方库都是用cmake作为构建系统的，主要的好处是可以跨平台编译。详细信息及使用方法请[参考官网](https://cmake.org/)。 

编译ffmpeg所依赖的部分第三方库时使用的是cmake version 3.11.2，cmake安装完毕后，就已经将cmake.exe加到环境变量里面了，可以直接在msys终端下执行cake命令：

```bash
$ which cmake
/c/Program Files/CMake/bin/cmake.exe
```

## 使用方法

```bash
cmake -G "Visual Studio 12 2013"  source
```

参数`"Visual Studio 12 2013"`，这个名称是固定的（请参考[官网](https://cmake.org/cmake/help/v3.1/manual/cmake-generators.7.html#ide-build-tool-generators)）： 

vs2010：Visual Studio 10 2010 

vs2012：Visual Studio 11 2012 

vs2013：Visual Studio 12 2013 

vs2015：Visual Studio 14 2015 
`source`是源代码目录，下面必须有CMakeLists.txt文件。
## 遇到的错误

**–debug-output –trace**  可以打印很多信息(debug、trace前面是两个横杠)，有助于查问题。

对于cmake产生的MSVC工程，如果代码的路径变了，需要用cmake重新生成。重新生成前需要将源代码目录下的CMakeCache.txt、CMakeFiles目录删除，不然很多变量都会使用CMakeCache.txt里面的。

下面这个错误：

```
节点 1 上的项目“E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\x265_20160531\x265\build\vc12-x86\CMakeFiles\3.6.0-rc3\CompilerIdC\CompilerIdC.vcxproj”(默认目标)。
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\x265_20160531\x265\build\vc12-x86\CMakeFiles\3.6.0-rc3\CompilerIdC\CompilerIdC.vcxproj(18,3): error MSB4019: 未找到导入的项目“E:\Microsoft.Cpp.Default.props”。请确认 <Import> 声明中的路径正确，且磁盘上存在该文件。
已完成生成项目“E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\x265_20160531\x265\build\vc12-x86\CMakeFiles\3.6.0-rc3\CompilerIdC\CompilerIdC.vcxproj”(默认目标)的操作 - 失败。
```

是因为cmake命令里面的generator写的不对，应该是：

```bash
cmake -G "Visual Studio 12 2013" ..\\..\\source
```

参考：[https://cmake.org/cmake/help/v3.1/manual/cmake-generators.7.html#ide-build-tool-generators](https://cmake.org/cmake/help/v3.1/manual/cmake-generators.7.html#ide-build-tool-generators)

同一个目录只能用cmake产生一种编译系统文件，即不能在同一个目录下用cmake产生makefile和MSVC的sln文件，执行多次也不行。用cmake产生sln文件后，再执行cmake命令（不带参数），也不能产生makefile。会报下面的错误：

```bash
$ cmake -G "MSYS Makefiles"  ./
CMake Error: Error: generator : MSYS Makefiles
Does not match the generator used previously: Visual Studio 12 2013
```

重复执行cmake，需要删除源代码目录下的CMakeCache.txt、CMakeFiles目录。

## cmake中使用pkg-config检测库文件

C:\Program Files\CMake\share\cmake-3.11\Modules\FindPkgConfig.cmake中的规则就是使用pkg-config来检测库文件和头文件的。同样可以用“export PKG_CONFIG_PATH=”命令来指定.pc文件的路径：

```bash
export PKG_CONFIG_PATH=path_of_pc; cmake ...
```](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)





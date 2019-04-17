# Solve Error: Unhandled exception at 0x00905a4d in xxx.exe: 0xC0000005: Access violation. - Grandyang - 博客园







# [Solve Error: Unhandled exception at 0x00905a4d in xxx.exe: 0xC0000005: Access violation.](https://www.cnblogs.com/grandyang/p/7781793.html)







在使用Visual Studio进行项目开发的时候，有时候会遇到下面这个错误：

Unhandled exception at 0x00905a4d in xxx.exe: 0xC0000005: Access violation

产生这个错误的原因可能是在Visual Studio中链接了后缀为"*.a"的库文件，这种库文件应该是在Linux系统或者是使用MinGW编译时链接的，用VS的话应该链接"*.lib"的库文件，而下载的第三方库中一般会包含"*.def"文件，比如科学计算库libgsl里就有这两个文件libgsl.def和libgslcblas.def，我们可以将其转换为"*.lib"的库文件：



Start the "Visual Studio 2010 command lines"

Goto the gsl lib directory, and type:

lib /machine:i386 /def:libgsl.def

lib /machine:i386 /def:libgslcblas.def



这会生成libgsl.lib和libgslcblas.lib这两个文件，加到Linker中即可。















# (原)win8下编译GLUT - L_Andy的专栏 - CSDN博客

2017年10月09日 17:59:19[卡哥](https://me.csdn.net/L_Andy)阅读数：264


- 到opengl官网下载glut源代码
- 修改glutwin32.mak下

```
# MSVC install directories
LIBINSTALL     = XXXXX\VC\lib //vs安装目录下 例如G:\application\VS2013\VC\lib
INCLUDEINSTALL = xxxxxx\VC\include\GL //同上 没有GL目录的 自己新建
# WinNT dll directory DLLINSTALL     = C:\Windows\system32
```
- 修改glut-3.7\lib\glut\Makefile.win

```python
将!include <win32.mak>修改为!include <C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include/win32.mak>
$(link) $(LFLAGS) -out:$(GLUTDLL) -def:glut.def  $(OBJS) $(LIBS)修改为
$(link) $(LFLAGS) -out:$(GLUTDLL) -def:glut.def  $(OBJS) $(LIBS) -NODEFAULTLIB:$(GLUTLIB)
```
- 找到vs下面 x86的命令行工具

cd到glut目录，执行glutmake.bat

执行完这个以后 会将生成的dll lib 头文件 自动拷贝到对应目录，命令行会报找不到win32.mak  这个可以忽略

备注：目前64位glut在windows下貌似不行，我试过用vs下64位命令行编过 报错


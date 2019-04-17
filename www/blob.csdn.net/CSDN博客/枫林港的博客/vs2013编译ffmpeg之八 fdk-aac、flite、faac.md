# vs2013编译ffmpeg之八 fdk-aac、flite、faac - 枫林港的博客 - CSDN博客





2018年07月07日 22:48:04[枫林港](https://me.csdn.net/u013601132)阅读数：330








# fdk-aac

对应configure选项–enable-libfdk-aac。 

官网代码：[https://github.com/mstorsjo/fdk-aac](https://github.com/mstorsjo/fdk-aac)

AAC的编解码库实现，实际用的代码是从[ShiftMediaProject](https://github.com/ShiftMediaProject/fdk-aac)上下载的，官网上的没有MSVC工程。编译过程没有报错。
# flite

对应configure选项–enable-libflite。 
[http://www.speech.cs.cmu.edu/flite/](http://www.speech.cs.cmu.edu/flite/)
[http://festvox.org/flite/index.html](http://festvox.org/flite/index.html)

音频滤镜。第一个链接只能下载到1.4版本的，2009年以后就没有更新了。第二个链接可以下载到2.0版本的，2014年更新的。 

flite-2.0.0-release里面有MSVC工程，但没有使用，用msys编译的，msys编译的库也可以用在vs2013工程中。这里描述的是用msys编译的，由于flite的库比较多，为了不修改ffmpeg configure，直接修改库名： 

flite-2.0.0-release\config\ common_make_rules里面将：
```
@ (cd $(OBJDIR) && $(AR) cruv ../../../$(LIBDIR)/lib$(LIBNAME).a $(OBJS))
    @ $(RANLIB) $(LIBDIR)/lib$(LIBNAME).a
```

改成：

```
(cd $(OBJDIR) && $(AR) cruv ../../../$(LIBDIR)/$(LIBNAME).lib $(OBJS))
    $(RANLIB) $(LIBDIR)/$(LIBNAME).lib
```

FLITELIBS = $(BUILDDIR)/lib/libflite.a改成：FLITELIBS = $(BUILDDIR)/lib/flite.lib 

去掉@就可以看到打印。 

另外flite-2.0.0-release\main\Makefile里面：
```
flite_LIBS_deps = $(flite_LIBS:%=$(LIBDIR)/lib%.a)
flite_time_LIBS_deps = $(flite_time_LIBS:%=$(LIBDIR)/lib%.a)
```

改成：

```
flite_LIBS_deps = $(flite_LIBS:%=$(LIBDIR)/%.lib)
flite_time_LIBS_deps = $(flite_time_LIBS:%=$(LIBDIR)/%.lib)
```

从编译打印看，flite的编译选项很简单，应该很容易改造成MSVC编译。

# faac

ffmpeg-3.2.2已经不支持这个模块了（前面不知道是哪个版本开始不支持的），根据官方说明，ffmpeg自带的aac编解码器已经超过了faac，所以用ffmpeg自带的。 

从[https://sourceforge.net/projects/faac/files/faac-src/](https://sourceforge.net/projects/faac/files/faac-src/)上下载faac-1.28.tar.gz。MSVC2013或MSVC2010打开faac-1.28\frontend\ faac.sln就可以编译。







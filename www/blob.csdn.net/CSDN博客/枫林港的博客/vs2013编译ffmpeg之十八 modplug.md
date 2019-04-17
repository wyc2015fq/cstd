# vs2013编译ffmpeg之十八 modplug - 枫林港的博客 - CSDN博客





2018年07月09日 00:14:36[枫林港](https://me.csdn.net/u013601132)阅读数：47标签：[modplug](https://so.csdn.net/so/search/s.do?q=modplug&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)









# modplug

对应ffmpeg configure选项–enable-libmodplug。

[ShiftMediaProject上](https://github.com/ShiftMediaProject/modplug)下载的。

ffmpeg-3.3在configure的时候报下面的错误：

```
LINK : warning LNK4044: 无法识别的选项“/I../../contribute/MSVC/modplug-master/vs2013_build/include”；已忽略

test.o : error LNK2019: 无法解析的外部符号 impModPlug_Load，该符号在函数 _check_ModPlug_Load 中被引用
```

在modplug.h里面，ffmpeg-3.3在configure的时候走的是#   define MODPLUG_EXPORT __declspec(dllimport)  分支，而modplug在编译的时候走的是#   define MODPLUG_EXPORT分支。Modplug的工程里面有定义MODPLUG_EXPORT宏，所以解决的办法是在libmodplug.pc文件里面的Cflags里面加上-DMODPLUG_EXPORT。

```
#if defined(_WIN32) || defined(CYGWIN)

# if defined(MODPLUG_BUILD) && defined(DLL_EXPORT)  /* building libmodplug as a dll for windows */

#   define MODPLUG_EXPORT __declspec(dllexport)

# elif defined(MODPLUG_BUILD) || defined(MODPLUG_STATIC)    /* building or using static libmodplug for windows */

#   define MODPLUG_EXPORT

# else

#   define MODPLUG_EXPORT __declspec(dllimport)         /* using libmodplug dll for windows */

# endif

#elif defined(MODPLUG_BUILD) && defined(SYM_VISIBILITY)

#   define MODPLUG_EXPORT attribute((visibility("default")))

#else

#define MODPLUG_EXPORT

#endif
```

下面这个错误的解决办法是在src\libmodplug\stdafx.h的#ifdef _WIN32分支里面加上“#define wsprintf sprint”：
`libmodplugd.lib(sndfile.obj) : error LNK2019: 无法解析的外部符号 impwsprintfA，该符号在函数 "public: void __thiscall CSoundFile::ResetMidiCfg(void)" (?ResetMidiCfg@CSoundFile@@QAEXXZ) 中被引用`



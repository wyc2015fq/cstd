# vs2013编译ffmpeg之三十 twolame、vo-amrwbenc、wavpack - 枫林港的博客 - CSDN博客





2018年07月10日 23:26:16[枫林港](https://me.csdn.net/u013601132)阅读数：102








# twolame

对应ffmpeg configure选项–enable-libtwolame。

twolame有vs工程，将库名改成twolame.lib，这样不需要修改ffmepg的configure： 
![这里写图片描述](https://img-blog.csdn.net/2018070700230012?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但有下面的错误：

```
1>C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppBuild.targets(1361,5): warning MSB8012: TargetPath(E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\twolame-0.3.13\win32\lib\twolame.lib) does not match the Library's OutputFile property value (E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\twolame-0.3.13\win32\lib\twolame_staticD.lib). This may cause your project to build incorrectly. To correct this, please make sure that (OutDir), (TargetName) and $(TargetExt) property values match the value specified in %(Lib.OutputFile).

     1>C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\Microsoft.CppBuild.targets(1363,5): warning MSB8012: TargetName(twolame) does not match the Library's OutputFile property value (twolame_staticD). This may cause your project to build incorrectly. To correct this, please make sure that (OutDir), (TargetName) and $(TargetExt) property values match the value specified in %(Lib.OutputFile).
```

生成的库名字还是twolame_staticD.lib，原因是工程里面的链接参数中加了/out，解决的办法是修改Output File，改成下图的样子： 
![这里写图片描述](https://img-blog.csdn.net/20180707002314890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ffmpeg-3.3 configure在检测twolame的时候会报错，从config.log里面看，编译出来的符号加了**imp**前缀，twolame.h里面可以看到相关的定义。解决办法，将规则
`enabled libtwolame        && require libtwolame twolame.h twolame_init -ltwolame &&`
改成：
`enabled libtwolame        && require libtwolame twolame.h twolame_init -ltwolame -DLIBTWOLAME_STATIC &&`
**imp**前缀是因为加了__declspec(dllimport)导致的，这个只应该出现在动态库中。

# vo-amrwbenc

对应ffmpeg configure选项–enable-libvo-amrwbenc。

[https://github.com/mstorsjo/vo-amrwbenc](https://github.com/mstorsjo/vo-amrwbenc)

[https://sourceforge.net/projects/opencore-amr/files/vo-amrwbenc/](https://sourceforge.net/projects/opencore-amr/files/vo-amrwbenc/)

github上下载的代码没有configure文件，没有找到编译方法。sourceforge上的可以。编译方法：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build; make; make install`
只能用msys编译，不能产生MSVC工程。

# wavpack

对应ffmpeg configure选项–enable-libwavpack。

[http://www.wavpack.com/](http://www.wavpack.com/)

官网上下载的代码有MSVC工程，打开就能编译成功。






# vs2013编译ffmpeg之二十一 opencore-amr、opencv - 枫林港的博客 - CSDN博客





2018年07月09日 23:19:01[枫林港](https://me.csdn.net/u013601132)阅读数：93








# opencore-amr

对应ffmpeg configure选项–enable-libopencore-amrnb和–enable-libopencore-amrwb。

从sourceforge上下载[opencore-amr-0.1.3](https://sourceforge.net/projects/opencore-amr/)，github上的master没有configure文件。编译方法，在opencore-amr-0.1.3下面执行：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build; make; make install`
# opencv

对应ffmpeg configure选项–enable-libopencv。

官网[http://opencv.org/downloads.html](http://opencv.org/downloads.html)

跨平台的计算机视觉库，用于开发实时的图像处理、计算机视觉以及模式识别程序。

从官网上下载opencv-2.4.13.exe，这个是2016年5月的版本，2015年有个版本opencv-3.1.0.exe看上去比2016年的版本还要新，不知道是什么原因。这里选择的是opencv-2.4.13.exe，双击运行，选择要解压的目录，会将opencv的库和头文件解压到这个目录下。

ffmpeg configure和链接时的库选择opencv\build\x86\vc12\staticlib\opencv_core2413d.lib。

## 编译错误一

由于所有的代码编译都加了MTd，opencv的库要选择opencv_core2413d.lib，不然会报下面的错误。
`opencv_core2413.lib(persistence.obj) : error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项:  值“0”不匹配值“2”(x265-static.lib(api.obj) 中)`
## 编译错误二

下面这个错误：

```
libavfilter.a(vf_libopencv.o) : error LNK2001: 无法解析的外部符号 _cvSmooth

libavfilter.a(vf_libopencv.o) : error LNK2001: 无法解析的外部符号 _cvCreateStructuringElementEx

libavfilter.a(vf_libopencv.o) : error LNK2001: 无法解析的外部符号 _cvReleaseStructuringElement

libavfilter.a(vf_libopencv.o) : error LNK2001: 无法解析的外部符号 _cvErode

libavfilter.a(vf_libopencv.o) : error LNK2001: 无法解析的外部符号 _cvDilate
```

opencv\build\x86\vc12\staticlib下面的库非常多，写个脚本来搜索符号：

```bash
!/bin/sh

for i in *.lib

do

    echo "File ${i}"

    dumpbin.exe -SYMBOLS ${i} | grep cvSmooth

done
```

脚本在msys+vcvars32.bat环境下执行，得到类似的打印，贴一部分：

```
File opencv_features2d2413.lib

540 00000000 UNDEF  notype ()    External     | _cvSmooth

File opencv_imgproc2413.lib

847 00000000 SECT3C4 notype ()    External    | _cvSmooth

A43 00000067 SECT3C6 notype ()    Static      | _ehhandler$cvSmooth

File opencv_imgproc2413d.lib

9D8 00000000 SECT461 notype ()    External    | _cvSmooth

C5C 00000076 SECT463 notype ()    Static      | _ehhandler$cvSmooth
```

UNDEF表示是调用cvSmooth函数，有SECT的就是函数定义的地方，这里选择debug版本的库opencv_imgproc2413d.lib。




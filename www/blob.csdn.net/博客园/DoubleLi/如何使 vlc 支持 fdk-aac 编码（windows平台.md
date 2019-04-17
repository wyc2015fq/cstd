# 如何使 vlc 支持 fdk-aac 编码（windows平台 - DoubleLi - 博客园






可能是由于fdk-aac开源协议的原因，VLC默认是不支持fdk-aac编码的，fdk-aac 是非常优秀的AAC编码库，并且支持AAC-LD AAC-ELD, 对于要求低延迟的场景下很有用。

可以通过修改VLC的编译脚本，并加入fdkaac库来使用VLC支持fdkaac编码器。

我是在64位的ubuntu16.04下编译的，vlc的编译方法参考VLC官网的编译文档。



1  修改vlc/extras/package/win32/configure.sh ， 加入--enable-fdkaac， 这样VLC就会编译fdkaac这个模块。

  执行make,会出现编译错误

as/package/win32/../../../modules/codec/fdkaac.c:56:32: fatal error: fdk-aac/aacenc_lib.h: No such file or directory

这是因为缺少fdk-aac库相关的头文件。



2  编译fdk-aac库

 下载源码[https://github.com/mstorsjo/fdk-aac.git](https://github.com/mstorsjo/fdk-aac.git)

在terminal下进入源码目录，输入

mkdir winBuild

cd winBuild

../configure --host=i686-w64-mingw32 --prefix=VLCPATH/contrib/i686-w64-mingw32

make install





VLCPATH 替换成VLC代码的路径



3 编译VLC代码

make packag-win32-zip

这时应该可以编译通过了， 在module/codec/ 目录下会生成libfakaac_plugin.dll文件。

还需要把这个模块依赖的VLCPATH/contrib/i686-w64-mingw32/libfdk-aac-1.dll动态库复制到这个目录下。





现在VLC就可以支持fdk-aac编码了。










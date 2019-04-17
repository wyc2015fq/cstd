# ffmpeg 纯静态编译，以及添加自定义库流程摘要 - DoubleLi - 博客园






需求：

   1. 纯静态编译ffmpeg ,即ldd ./ffmpeg 的结果是：not a dynamic executable

   2.  修改ffmpeg 项目，添加自定义功能库

   3. 自定义库由c++实现，要求能被纯c的ffmpeg项目调用

   4. 自定义库必须使用g++ 的一些高级特性编译，要求g++支持c++11

   5. 自定义库使用了pthread库 和openmp 库

   6. 自定义库使用了opencv 3.0.0库，

   7. 禁用所有的图形显示库x11,xcb,声音设备avdevice等等，静态链接这些库，会很痛苦。



动手之前需要注意：

A. ffmpeg.org ([CentOS](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos)或 [Ubuntu](https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu))的方法是不能达到需求1的，因为它大量使用包管理软件(yum 或apt-get)安装一些ffmpeg的依赖库，而这些依赖库大多是动态版本(.so.*) 。这就使得编译ffmpeg时，无法静态链接这些依赖库。因而，非开发工具(如gcc g++ pkg-config aotoconf etc )的依赖库（如ass,freetype,sdl theora, jpeg png,etc），需要手动下载它们的源码(以及这些依赖库的依赖库)，然后编译得到静态库(.a)。一般来说，这些依赖库多数是configure 配置，然后make,make install。

./configure *** **--enable-shared=no --enable-static=yes** --with-***=/dir



B.自定义功能库也类似于上述方法写好myfunc.cpp,然后g++编译，ar打包成.a。但是由于我们写好的cpp要求c++11特性，而CentOS6.5上的gcc/g++版本为4.4.7不支持c++11。因而，需要安装gcc4.8以上版本。从repo里面无法安装较新的gcc的话，只能自行下载gcc源码，自行编译了。然而发现了一个第三方的编译好的工具链[devtool3](http://linuxsoft.cern.ch/cern/devtoolset/slc6-devtoolset.repo)，它提供了gcc 4.9.2 以及配套的binutils工具。然而，不要急，先不要安装它，看完本注意事项。



C.要求c++打包的库能够被ffmpeg的项目链接，要求libstdc++.a （-lstdc++）；能够使用openmp，要求libgomp.a ( -fopenmp); libstdc++.a可以通过yum install libstdc++-static得到，但是libgomp.a则没有对应的方法。



综合B和C的注意事项，我们应该自行编译gcc，而不是使用系统自带的gcc或者yum apt-get下载安装的，因为自行编译gcc能够直接得到libstdc++.a和libgomp.a这些静态库。我选用的版本是gcc4.8.4，同样地配置它编译出静态库。配套的Binutils也需要重新编译。



D. glibc是GNU的标准C库实现，如果要静态链接glibc这些底层的库_——libc.a,libm.a,librt.a,libdl.a,libpthread.a ,需要安装glibc的静态版本

#yum install glibc-static, 

#whereis libc.a（找到路径）

#cp 路径/libc.a ~mydir/（其它4个库类似，重新拷贝到一个新的位置，以防链接时优先选择动态链接，当然把这个路径放在LDFLAGS最前面）



E.opencv 和ffmpeg在配置阶段(分别使用cmake和configure)，务必关闭所有的与图形IO相关的依赖，因为图形库的静态版本，太难找了，依赖关系特别冗长繁杂。



F. 最终链接ffmpeg时候，由于ffmpeg依赖了一些库如x264,x265，freetype等，它们各自依赖了glibc的一些库如librt,libm,libc,libdl。因而原始命令(#make -n|grep ffmpeg_g)中glibc的一些库(libm,libc,librt,libdl,libpthread)被反复调用，分布于多个地方，如下：

 gcc  -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample -L/home/ffmpeg/ffmpeg_sjtu/lib  -Wl,--as-needed -Wl,-z,noexecstack -Wl,--warn-common -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavdevice:libavformat:libavcodec:libavutil:libavresample   -o ffmpeg_g cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o   -lavdevice -lavfilter -lavformat -lavcodec -lpostproc -lswresample -lswscale -lavutil  -L/home/ffmpeg/ffmpeg_build/lib -lx265 -lstdc++**-lm -lrt -ldl** -L/home/ffmpeg/ffmpeg_build/lib -lx264 **-lpthread -lm -ldl** -L/home/ffmpeg/ffmpeg_build/lib -lvpx -lm -lpthread -L/home/ffmpeg/ffmpeg_build/lib -lvpx -lm -lpthread -L/home/ffmpeg/ffmpeg_build/lib -lvpx **-lm -lpthread** -L/home/ffmpeg/ffmpeg_build/lib -lvpx -lm -lpthread -ltheoraenc -ltheoradec -logg -L/home/ffmpeg/ffmpeg_build/lib -lopus -lm -lmp3lame -L/home/ffmpeg/ffmpeg_build/lib -lfribidi -L/home/ffmpeg/ffmpeg_build/lib -lfreetype -lz -L/home/ffmpeg/ffmpeg_build/lib -lfontconfig -lxml2 -llzma -lm -lfreetype -lz -L/home/ffmpeg/ffmpeg_build/lib -lfdk-aac -lm -L/home/ffmpeg/ffmpeg_build/lib -lass -lharfbuzz -lfontconfig -lenca -lfribidi -lxml2 -llzma -lm -lfreetype -lz -lm -llzma -lbz2 -lz **-pthread -lrt** -lstdc++      -L/home/ffmpeg/sjtu_fruc/lib -lsjtu_fruc  -fopenmp   -L/usr/local/lib -L/usr/local/share/OpenCV/3rdparty/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal -lzlib -llibjpeg -llibwebp -llibpng -llibtiff -llibjasper -ltbb -lippicv -lstdc++**-ldl -lm -lpthread -lrt  **



因此，为了能够静态链接glibc的几个库，需要手动修改这条最终的链接命令：把所有glibc的库放到最后，以如下方式：

 -lm -lrt -ldl  -lm -lpthread -lrt   **-static -lc**



注意，-static -lc （libc.a）必须放到最后(pthread之后)，因为pthread和c库有函数重定义的冲突。



这条语句最终为：

printf "LD\t%s\n" ffmpeg_g; gcc  -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample -L/home/ffmpeg/ffmpeg_sjtu/lib  -Wl,--as-needed -Wl,-z,noexecstack -Wl,--warn-common -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavdevice:libavformat:libavcodec:libavutil:libavresample   -o ffmpeg_g cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o   -lavdevice -lavfilter -lavformat -lavcodec -lpostproc -lswresample -lswscale -lavutil  -L/home/ffmpeg/ffmpeg_build/lib -lx265 -L/home/ffmpeg/ffmpeg_build/lib -lx264 -L/home/ffmpeg/ffmpeg_build/lib -lvpx  -L/home/ffmpeg/ffmpeg_build/lib -lvpx  -L/home/ffmpeg/ffmpeg_build/lib -lvpx  -L/home/ffmpeg/ffmpeg_build/lib -lvpx -ltheoraenc -ltheoradec -logg -L/home/ffmpeg/ffmpeg_build/lib -lopus -lmp3lame -L/home/ffmpeg/ffmpeg_build/lib -lfribidi -L/home/ffmpeg/ffmpeg_build/lib -lfreetype -lz -L/home/ffmpeg/ffmpeg_build/lib -lfontconfig -lxml2 -llzma -lfreetype -lz -L/home/ffmpeg/ffmpeg_build/lib -lfdk-aac -L/home/ffmpeg/ffmpeg_build/lib -lass -lharfbuzz -lfontconfig -lenca -lfribidi -lxml2 -llzma -lfreetype -lz  -llzma -lbz2 -lz   -L/home/ffmpeg/sjtu_fruc/lib -lsjtu_fruc  -fopenmp   -L/usr/local/lib -L/usr/local/share/OpenCV/3rdparty/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal -lzlib -llibjpeg -llibwebp -llibpng -llibtiff -llibjasper -ltbb -lippicv -lstdc++ **-lm -lrt -ldl  -lm -lpthread -lrt   -static -lc**



尽管它执行会报出一些warnning,但是，不影响生成ffmpeg_g可执行文件。

然后执行，

#cp ffmpeg_g ffmpeg

#strip ffmpeg

即可



测试ffmpeg










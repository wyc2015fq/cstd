# vs2013编译ffmpeg之二十三 openh264 - 枫林港的博客 - CSDN博客





2018年07月09日 23:29:02[枫林港](https://me.csdn.net/u013601132)阅读数：422








# openh264

对应ffmpeg configure选项–enable-libopenh264。

从[https://github.com/cisco/openh264](https://github.com/cisco/openh264)下载openh264-1.5.0，最开始是在ffmpeg-3.0增加了openh264的支持，ffmpeg-3.0＋openh264-master编译会有问题，ffmpeg-3.0最高只支持openh264-1.5.0，后续一直沿用openh264-1.5.0，ffmpeg-3.4.2+openh264-1.5.0也是ok的。

编译需要nasm，下载地址：[http://www.nasm.us/](http://www.nasm.us/)。nasm.exe放到C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin。

用MSVC打开openh264-master\test\build\win32\codec_ut\Codec_UT.sln，由于缺少gtest工程，直接用libvpx-v1.3.0/gtest.vcxproj，gtest的头文件从libvpx-v1.3.0拷贝到openh264-master\gtest\include下面，不知道为何直接在Codec_UT工程里面设置会报找不到gtest/gtest.h的错误。原始的工程会生成动态库，为了方便调试避免其他工程引用时需要拷贝dll文件，将工程设置都改成静态库。

将库文件gtestmdd.lib（在libvpx-v1.3.0下面）、welsecore.lib、welsdcore.lib、WelsDecPlus.lib、WelsEncPlus.lib、1.5.0版本还需要WelsVP.lib，加到Codec_UT工程里面。这样就可以编译通过了。




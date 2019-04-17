# VLC播放RTSP视频延迟问题 - DoubleLi - 博客园






之前写过一篇关于在Linux平台上编译[Android](http://lib.csdn.net/base/15)平台上VLC播放器源代码的文章，vlc这款播放器非常优秀而且是开源的，它的核心是开源视频编解码库ffmpeg。而且这款播放器还支持RTSP协议，这个主要是用开源的live555来实现的，live555这个库以后还需要认真研习。

# **一：PC端（Windows平台）VLC播放RTSP视频延迟问题解决**

前几天用PC平台上的VLC播放RTSP流媒体时延迟时间较长，存在1s左右的延迟，效果不是很好，后来查了资料，发现这个延迟时间是可以修改的。

找到工具->首选项，然后参数设置左下角选择“全部”，左边选择 “输入编解码”->“网络缓存”选项，可以根据具体需要加以修改，具体见下图不过这个值不要太小，否则缓存太小，播放视频的过程中会很卡
![](http://blog.chinaunix.net/attachment/201306/2/26611383_1370183332e9eg.png)



如果网络环境较好，300ms以内效果很好，实时性较好。

**二：PC端（Linux平台）VLC播放RTSP视频延迟问题解决**

这里我的测试平台是ubuntu，linux平台上的vlc可以以命令行的方式来运行，而命令行是可以带参数的，我们只需要在参数里面指定这个延迟时间就可以了，下面是具体的命令行形式，大家可以根据各自需要加以调整

vlc rtsp://192.168.1.1:8556/test.avi  --newwork-caching=300

rtsp的地址要根据自己需要播放的视频地址修改，网络延迟时间也可以根据需要修改，这个值越小，实时性越好，当然太小了播放会很卡，或者设置的值失效，一般建议在100-300之间

**三． Android平台VLC播放RTSP视频延迟问题解决**


首先在Android平台上的VLC播放器不像windows平台上UI里提供了设置延迟时间这一项功能，其次也不像Linux平台上可以以命令行的方式来运行，我们也就不能通过设置参数的方式来修改这个延迟时间，怎么办呢，只剩下最后一条路，修改源代码，将延迟时间写死在源码里，有了前面linux平台下设置参数的经验，就可以在源码里面通过grep工具来查找”network-caching”，发现有很多文件包含这个值，下面是搜索的结果





点击(此处)折叠或打开


- vlc/src/libvlc-module.c: add_integer( "network-caching", CLOCK_FREQ / 1000,
- vlc/NEWS: - --network-caching specifies caching for network resources,
- vlc/modules/gui/macosx/simple_prefs.m: TestCaC("network-caching", 10/3);
- vlc/modules/gui/macosx/simple_prefs.m: CaC("network-caching", 10/3);
- vlc/modules/gui/qt4/components/open_panels.cpp: emit methodChanged( qfu( "network-caching" ) );
- vlc/modules/gui/qt4/components/simple_preferences.cpp: TestCaC( "network-caching", 10/3 );
- vlc/modules/gui/qt4/components/simple_preferences.cpp: CaC( "network-caching", 10/3 );
- vlc/modules/access/file.c: *pi_64 = var_InheritInteger (p_access, "network-caching");
- vlc/modules/access/rtp/rtp.c: *v = INT64_C(1000) * var_InheritInteger (demux, "network-caching");
- vlc/modules/access/ftp.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/live555.cpp: * var_InheritInteger( p_demux, "network-caching" );
- vlc/modules/access/http.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/sftp.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/udp.c: * var_InheritInteger(p_access, "network-caching");
- vlc/modules/access/smb.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/tcp.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/rtsp/access.c: * var_InheritInteger(p_access, "network-caching");
- vlc/modules/access/mms/mmsh.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/mms/mmstu.c: * var_InheritInteger( p_access, "network-caching" );
- vlc/modules/access/vnc.c: * var_InheritInteger( p_demux, "network-caching" );
- vlc-android/jni/libvlcjni.c: libvlc_media_add_option(p_md, ":network-caching=1500");



对这些文件的代码进行了一些分析，最后发现目标在vlc/src/libvlc-module.c这个文件，于是修改这个文件的1832行，如下图所示，将CLOCK_FREQ / 1000改为 CLOCK_FREQ / 3000，这里可以根据各自的需要加以修改，分母越大，这个值就越小，实时性也就越好，当然跟前面所说一样，不能太小





点击(此处)折叠或打开


- 1832 //add_integer( "network-caching", CLOCK_FREQ / 1000,
- 1833 // NETWORK_CACHING_TEXT, NETWORK_CACHING_LONGTEXT, true )
- 1834 add_integer( "network-caching", CLOCK_FREQ / 3000,
- 1835 NETWORK_CACHING_TEXT, NETWORK_CACHING_LONGTEXT, true )



最后再编译运行发现延迟时间大概在200ms左右，比修改之前实时性有明显改善，实时性很好，希望这篇文章能给需要相关资料的朋友提供一些帮助，如果有什么问题欢迎给我留言。










# 最简单的基于FFmpeg的移动端例子：IOS 视频转码器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年08月01日 16:34:20[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：21447
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================

最简单的基于FFmpeg的移动端例子系列文章列表：

[最简单的基于FFmpeg的移动端例子：Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47008825)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47010637)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器-单个库版](http://blog.csdn.net/leixiaohua1020/article/details/47011021)

[最简单的基于FFmpeg的移动端例子：Android 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47056051)

[最简单的基于FFmpeg的移动端例子：Android 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47056365)

[最简单的基于FFmpeg的移动端例子附件：Android 自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47056733)

[最简单的基于FFmpeg的移动端例子附件：SDL Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47059553)

[最简单的基于FFmpeg的移动端例子：IOS HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47071547)

[最简单的基于FFmpeg的移动端例子：IOS 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47072257)

[最简单的基于FFmpeg的移动端例子：IOS 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47072519)

[最简单的基于FFmpeg的移动端例子：IOS 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47072673)

[最简单的基于FFmpeg的移动端例子附件：IOS自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47072861)

[最简单的基于FFmpeg的移动端例子：Windows Phone HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47191283)

=====================================================


本文记录IOS平台下基于FFmpeg的视频转码器。该转码器实际上移植自ffmpeg工程中的ffmpeg.c源代码。有关ffmpeg.c的源代码可以参考文章《f[fmpeg.c函数结构简单分析（画图）](http://blog.csdn.net/leixiaohua1020/article/details/39760711)》，在这里就不重复记录了。

![](https://img-blog.csdn.net/20150801182823034)





## 源代码

项目的目录结构如图所示。

![](https://img-blog.csdn.net/20150726220618975)


下列C语言文件拷贝自FFmpeg源代码：

> *cmdutils.ccmdutils.hcmdutils_common_opts.hconfig.hffmpeg.hffmpeg_filter.cffmpeg_opt.c*

此外在编译ffmpeg.c的时候，除了需要FFmpeg SDK中的头文件之外，还需要从源代码中拷贝下面头文件。

> *libavformat/ffm.hlibavformat/network.hlibavformat/os_support.hlibavformat/url.hlibavutil/libm.h*

fmpeg_mod.c是修改后的ffmpeg.c文件。ffmpeg.c本身是用于命令行使用的程序，因此在改为类库调用的时候需要对源代码做一些修改。例如去掉一些exit(0)，释放一些变量，main()函数改名为ffmpegmain()等。
Objective-C调用FFmpeg的代码位于ViewController.m文件中，内容如下所示。

```
/**
 * 最简单的基于FFmpeg的转码器-IOS
 * Simplest FFmpeg IOS Transcoder
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 马小雨 Ma Xiaoyu
 * maxiaoyucuc@163.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是IOS平台下的转码器。它移植于ffmpeg.c命令行工具。
 *
 * This software is a Transcoder in IOS. It is transplanted from ffmpeg.c
 * command line tools.
 */

#import "ViewController.h"
#include "ffmpeg.h"

int ffmpegmain(int argc, char **argv);

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)clickRunButton:(id)sender {
    char command_str_full[1024]={0};
    

    NSString *command_str= [NSString stringWithFormat:@"%@",self.command.text];
    NSArray *argv_array=[command_str componentsSeparatedByString:(@" ")];
    int argc=argv_array.count;
    char** argv=(char**)malloc(sizeof(char*)*argc);
    for(int i=0;i<argc;i++)
    {
        argv[i]=(char*)malloc(sizeof(char)*1024);
        strcpy(argv[i],[[argv_array objectAtIndex:i] UTF8String]);
    }
    
    ffmpegmain(argc, argv);
    
    for(int i=0;i<argc;i++)
        free(argv[i]);
    free(argv);
  }
@end
```


## 运行结果

App在手机上运行后的结果如下图所示。

![](https://img-blog.csdn.net/20150726220606444)

单击“Run”按钮之后，程序会根据输入的命令进行转码。例如对于上述截图的情况，就会将/Users/leixiaohua1020/Desktop/test.mp4转码为/Users/leixiaohua1020/Desktop/test.mov。生成的视频文件如下图所示。


## 下载

**simplest ffmpeg mobile**


**项目主页**

Github：[https://github.com/leixiaohua1020/simplest_ffmpeg_mobile](https://github.com/leixiaohua1020/simplest_ffmpeg_mobile)

开源中国：[https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile](https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile)

SourceForge：[https://sourceforge.net/projects/simplestffmpegmobile/](https://sourceforge.net/projects/simplestffmpegmobile/)





CSDN工程下载地址：[http://download.csdn.net/detail/leixiaohua1020/8924391](http://download.csdn.net/detail/leixiaohua1020/8924391)

[](http://download.csdn.net/detail/leixiaohua1020/8924391)
本解决方案包含了使用FFmpeg在移动端处理多媒体的各种例子：

> [Android]
simplest_android_player: 基于安卓接口的视频播放器
simplest_ffmpeg_android_helloworld: 安卓平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_android_decoder: 安卓平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_android_decoder_onelib: 安卓平台下最简单的基于FFmpeg的视频解码器-单库版
simplest_ffmpeg_android_streamer: 安卓平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_android_transcoder: 安卓平台下移植的FFmpeg命令行工具
simplest_sdl_android_helloworld: 移植SDL到安卓平台的最简单程序
[IOS]
simplest_ios_player: 基于IOS接口的视频播放器
simplest_ffmpeg_ios_helloworld: IOS平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_ios_decoder: IOS平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_ios_streamer: IOS平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_ios_transcoder: IOS平台下移植的ffmpeg.c命令行工具
simplest_sdl_ios_helloworld: 移植SDL到IOS平台的最简单程序








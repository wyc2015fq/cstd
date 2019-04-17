# FFmpeg浅尝辄止（一）——工欲善其事，必先利其器 - 迭代的是人，递归的是神 - CSDN博客





2012年06月28日 09:41:03[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：26935








好久没更新blog了，最近一直在看ffmpeg方面的资料，也是应急，学的也是皮毛。我只是把我学习的一些弯路跟大家分享一下，希望能对大家有些许帮助，如果哪里说的不对，还请大神指点啊。

首先，当然还是开发工具的选择，我去ffmpeg的官网看了一下，吓死人，竟然都更新到0.11.1，真是版本更新的快啊，我看网上广泛使用的都是6.0以下的版本，即使我们最爱的OpenCV也是用FFmpeg0.7.x进行的开发。于是去[http://ffmpeg.org/](http://ffmpeg.org/)看了一下。n多的下载链接让我凌乱了，下载下来也发现是n多源代码，泪奔啊。我只是想用用其中的dll和lib开发一下而已，他们藏哪去了，还好我找到了组织，在一个中文开发网站[http://www.ffmpeg.com.cn](http://www.ffmpeg.com.cn/)，我找到了我想要的利器——FFmpeg
 SDK。现在这个SDK的版本做到了3.2，对应的FFmpeg版本是0.4.9，虽然有些古老，但我还是忍忍用了吧。说白了SDK其实就是几个lib，几个dll，几个对应的头文件，特别是有个FFmpeg SDK 开发手册，可以读读。

这个SDK的安装也比较容易，我想对于配置过OpenCV的大虾们肯定不在话下。也就是把lib和头文件添加到vs开发环境的目录里，还有把dll所在的路径添加到系统的环境变量path中。

配置好了，感觉自己还是不太会用，网上关于FFmpeg的资料都没有很系统的，估计跟FFmpeg版本更新的太快也有关。只好自己搜集了一些。

首先，关于编码解码，大家普遍推荐的是直接看ffmpeg.c，关于播放器，看ffplay.c。当然，看这两个文件完全是高端选手，因为它们实在是太大了。。。

其次，我这里推荐[http://dranger.com/ffmpeg/](http://dranger.com/ffmpeg/)。网站的开头就提醒大家，他这个教程有些过时，但我感觉里面的内容对于入门级选手和轻量级的ffmpeg开发是很不错的。

这里还要提一点，如果想实现视频播放，必须得和硬件打交道了，大多数的方案是结合SDL进行的开发，关于SDL，有百度百科哦，亲，我就不多介绍了。我这里推荐的学习SDL的链接，[http://lazyfoo.net/SDL_tutorials/index.php](http://lazyfoo.net/SDL_tutorials/index.php)。

最后还要介绍我的一点心得，因为我源文件是cpp文件，但是FFmpeg是c风格的代码，所以在头文件的包含时要使用extern “C”，声明为c编译链接方式。例如：



```cpp
extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
};
```

还有一点就是如果要添加stdio.h等其他头文件的话，一定要放在上述extern “C”代码的前面，不然也会报错。让人很烦恼。

最后祝大家开发的愉快，见笑啦




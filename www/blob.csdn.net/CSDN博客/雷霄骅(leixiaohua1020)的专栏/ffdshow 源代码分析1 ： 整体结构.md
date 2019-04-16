# ffdshow 源代码分析1 ： 整体结构 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 15:32:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：36
个人分类：[ffdshow](https://blog.csdn.net/leixiaohua1020/article/category/1638191)










ffdshow是一个非常强大的DirectShow解码器，封装了ffmpeg，libmpeg2等解码库。它也提供了丰富的加工处理选项，可以锐化画面，调节画面的亮度等等。不止是视频，FFDShow现在同样可以解码音频，AC3、MP3等音频格式都可支持。并且可以外挂winamp 的DSP插件，来改善听觉效果。一个词形容：强大。

因为项目的要求，需要对ffdshow进行二次开发，正好有这个机会，分析研究一下ffdshow的源代码。

![](https://img-blog.csdn.net/20140616103533156)

ffdshow项目的资源可以从sourceforge下载。包括编译好的程序，以及原代码等，下载地址：[http://sourceforge.net/projects/ffdshow-tryout/](http://sourceforge.net/projects/ffdshow-tryout/)

注意：sourceforge上有两个版本的ffdshow：ffdshow以及ffdshow-tryout。其中前一个版本很早之前已经停止开发了，因此我们需要选择后一个（ffdshow-tryout）。

下载源代码的方法不再赘述，下面直接进入正题。源代码下载后，需要进行编译，推荐使用源代码根目录下的bat脚本一次性完成所有的资源编译。

编译完成后我们就可以打开源代码根目录里的工程了。我自己的开发环境是VC2010，打开后工程如下图所示（解决方案的名字被我修改了= =）：

![](https://img-blog.csdn.net/20130925150500281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

由图可见，ffdshow由一大堆工程组成，乍一看给人一种杂论无章的感觉，其实大部分工程我们不用去理会，我们重点研究最重要的工程就是那个名字叫“ffdshow”的工程。

下面我介绍几个最重要的文件夹里包含的代码的功能：



audiofilters：音频滤镜都在这里面（例如EQ，调节高低频等）

baseclasses：微软自带directshow的sdk里面就有，主要是微软为了方便DirectShow开发而提供的一些基本的类

codecs：支持的解码器都在这里（例如libavcodec，libmpeg2等）

convert：色彩转换的一些功能（没太用过）

decss：解除版权加密的一些功能（没太用过）

**dialog：音频视频滤镜的配置页面**

doc：文档，不是程序

ffvfw：VFW相关（目前没太用过）

Header Files：核心代码的头文件

**imgfilters：视频滤镜都在这里（显示QP/MV，加LOGO，显示视频信息等）**

Resource Files：资源文件

**settings：音频视频滤镜的配置信息**

**Source Files：核心代码的源文件**

subtitles：字幕相关的功能



以上用红色标出的，是我们二次开发中最有可能会涉及到的三个部分。掌握了这三个部分，就可以往ffdhow中添加自己写的滤镜（注意：这里说的是视频滤镜，音频的方法是一样的）

黄色背景标出的部分，虽然我们可能不需要做出什么改变，但是为了了解ffdshow的架构，我们需要分析其中的代码。



打了好多字啊。。。休息休息先





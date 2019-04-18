# 《精通Linux设备驱动程序开发》——音频驱动程序 - 一世豁然的专栏 - CSDN博客





2015年04月04日 23:13:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1043








一、音频架构

  1、PC环境下的音频

![](https://img-blog.csdn.net/20150403161047252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、南桥上的音频控制器与外部编码器（codec）一起，与模拟音频电路实现对接。

   2）、音频编码器将数字音频信号转换成扬声器播放所需的模拟信号，而通过麦克风录音时则执行相反的过程。

   3）、其他常见的与编解码器连接的音频输入输出包括头戴式耳麦、耳机、话筒、音频输入输出线。编解码器也提供混音器功能，它将所有这些音频输入和输出混合，并控制

              有关音频信号的音量。

   4）、数字音频数据是通过PCM技术对模拟声音信号以某个比特率采样得到的。




 2、AC‘97和I2S总线是连接音频控制器和编解码器的工业标准接口。




 3、嵌入式系统的音频连接

![](https://img-blog.csdn.net/20150403162257096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




二、Linux声音子系统

  1、ALSA（高级Linux声音架构）是2.6版本内核中的声音子系统。ALSA提供了OSS（开发声音系统）模拟，它允许程序不用改变OSS API就可以在ALSA的平滑过渡。




  2、Linux声音架构使音频应用程序域底层硬件无关。




  3、Linux声音（ALSA）子系统

![](https://img-blog.csdn.net/20150404223148268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、声音核心。

   2）、与控制器硬件相关的音频控制驱动程序。

   3）、协助控制器和编码器间通信的音频编解码接口。

   4）、在OSS应用程序和由ALSA启用的内核之间充当通道的OSS模拟层。

   5）、procfs和sysfs接口实现，用于通过/proc/asoud和/sys/class/soud获取信息。

   6）、用户空间ALSA库alsa-lib。

   7）、alsa-utils工具包，包括alsamixer、amixer、alsactl、aplay等工具。







三、设备实例：MP3播放器

 一）、简介

  1、Linux MP3播放器上的音频

![](https://img-blog.csdn.net/20150404224143597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、MPS的音频软件由两部分组成：

   1）、解码从CF盘读取的MP3文件并将其转换成原始PCM码流的用户程序。

   2）、一个底层内核ALSA音频驱动程序。




  3、音频驱动程序的组成：

   1）、处理播放器的例程。

   2）、处理录音的例程。

   3）、混合器控制功能。




  4、音频硬件的寄存器

![](https://img-blog.csdn.net/20150404224736126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 二）、驱动程序函数和结构体

  1、mycard_audio_probe()执行步骤：

   1）、通过调用snd_card_new()创建一个声卡实例：

       struct  snd_card  *card = snd_card_new(-1,id[dev-id], THIS_MODULE, 0);

       snd_card表示创建的声卡：

![](https://img-blog.csdn.net/20150404225512502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、使用snd_pcm_new()创建播放实例，并将它与步骤（1）中创建的卡绑定：

      int  snd_pcm_new(struct  snd_card  *card,  card  *id,

                                          int  device,

                                          int  playback_count,  int  capture_count,

                                          struct  snd_pcm  **pcm);

      snd_pcm结构体：

![](https://img-blog.csdn.net/20150404230109851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、通过调用sna_pcm_set_ops()播放操作与步骤与步骤（2）创建的PCM示例联系在一起。结构体snd_pcm_ops规定了将PCM音频传输到编解码器的操作。

   4）、使用snd_pcm_lib_preallocate_for_all()预分配缓冲区。

   5）、用snd_ctl_add()将声卡与一个混频控制单元关联，用于控制音量。

     snd_ctl_add(card,  snd_new1(&mycard_playback_vol,  &myctl_private))

   5）、将声卡注册进ALSA框架。




  2、Linux MP3播放器的ALSA驱动程序。（P282  代码清单13-1）







 三）、ALSA编程（略）







四、调试（略）





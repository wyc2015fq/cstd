
# java入门教程-11.8Java多媒体基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:44:03[seven-soft](https://me.csdn.net/softn)阅读数：214个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



本节介绍 Java程序播放幻灯片和动画，播放声音和视频的方法。
## 播放幻灯片和动画
用实例说明播放幻灯片和动画的方法。
【例 12-7】小应用程序先将幻灯片读入数组在存储，单击鼠标变换幻灯片，逐张显示（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/12-7.txt)）。
【例 12-8】小应用程序说明播放动画的方法，要求播放的图片和小程序放在相同的目录中，程序通过快速显示一组图片造成显示动画的效果。小应用程序利用线程控制动画图片的逐显示（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/12-8.txt)）。
## 播放声音
Java语言老根据地的音频格式有多种：au、aiff、wav、midi、rfm等。小程序要播放音频文件，可使用类AudioClip,该类在java.applet.AudioClip类库中定义。小程序先创建AudioClip对象，并用getAudioClip()方法为其初始化。代码形式如下：
AudioClip audioClip = getAudioClip(getCodeBase(),”myAudioClipFile.au”);
如果要从网上获得音频文件，可用方法getAudioClip(URL url, String name),根据url地址及音频文件name获得可播放的音频对象。
控制声音的播放有3个方法：play()播放声音，loop()循环播放和stop()停止播放。
【例 12-9】能播放声音的小应用程序（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/12-9.txt)）。
【例 12-10】如果声音文件较大或网络速度慢会影响小程序的初始化工作。这可用多线程技术解决。在一个级别较低的线程中完成音频对象的创建，即由后台载入声音文件，前台播放（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/12-10.txt)）。


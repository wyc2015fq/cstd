# Qt 播放音频文件 - xqhrs232的专栏 - CSDN博客
2018年05月30日 16:57:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：433
原文地址::[https://www.cnblogs.com/wanghuaijun/p/7899141.html](https://www.cnblogs.com/wanghuaijun/p/7899141.html)
Qt播放音频文件的方法有好多中，简单介绍几种
不过一下几种方式都需要在Qt工程文件中添加
[cpp][view plain](http://blog.csdn.net/z609932088/article/details/51248596#)[copy](http://blog.csdn.net/z609932088/article/details/51248596#)
- QT       += multimedia  
第一
QMediaPlayer类
可以播放MP3文件，同时使用也是最简单的了，具体代码
[cpp][view plain](http://blog.csdn.net/z609932088/article/details/51248596#)[copy](http://blog.csdn.net/z609932088/article/details/51248596#)
- {  
-     player = new QMediaPlayer;  
-     connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));  
-     player->setMedia(QUrl::fromLocalFile("E:\\CloudMusic\\林沁简介 - 太阳的后裔 MIX.mp3"));  
-     player->setVolume(30);  
-     player->play();  
- }  
该中实现方式适合单个文件的，如果有多个文件需要播放，那么需要使用下面的方式
[cpp][view plain](http://blog.csdn.net/z609932088/article/details/51248596#)[copy](http://blog.csdn.net/z609932088/article/details/51248596#)
- {  
-         playlist = new QMediaPlaylist;  
-         playlist->addMedia(QUrl::fromLocalFile("E:\\CloudMusic\\林沁简介 - 太阳的后裔 MIX.mp3"));  
-         playlist->addMedia(QUrl::fromLocalFile("E:\\CloudMusic\\芭比 - 触电 (完整版).mp3"));  
-         playlist->addMedia(QUrl::fromLocalFile("E:\\CloudMusic\\郭芯语 - 快点告诉你.mp3"));  
-         playlist->setCurrentIndex(1);  
- 
-         player = new QMediaPlayer;  
-         player->setPlaylist(playlist);  
- 
-         player->play();  
-     }  
源代码工程：[http://download.csdn.net/detail/z609932088/9502687](http://download.csdn.net/detail/z609932088/9502687)
第二
使用QsoundEffect类
核心代码
[cpp][view plain](http://blog.csdn.net/z609932088/article/details/51248596#)[copy](http://blog.csdn.net/z609932088/article/details/51248596#)
- effect.setSource(QUrl::fromLocalFile("E:\\CloudMusic\\林沁简介 - 太阳的后裔 MIX.wav"));  
- effect.setLoopCount(QSoundEffect::Infinite);  
- effect.setVolume(1.0f);  
- effect.play();  
源代码工程：[http://download.csdn.net/detail/z609932088/9502709](http://download.csdn.net/detail/z609932088/9502709)
第三种，是播放pcm裸流的
详见“灿哥哥的博客”
连接地址：[http://blog.csdn.net/caoshangpa/article/details/51224678](http://blog.csdn.net/caoshangpa/article/details/51224678)
原文连接内容
这两种方法都需要在.pro文件中加入multimedia模块。方法一是通过QAudioOutput实现的，先用QFile打开PCM文件，然后进行音频参数设置，最后调用QAudioOutput的start函数进行播放；方法二也用到了QAudioOutput，先将PCM文件读取到内存，然后进行音频参数设置，最后从内存中定时读取部分数据，调用QIODevice的write函数将这部分数据写入到扬声器。与方法一相比，方法二虽然麻烦，但是这种方法是进行语音编解码和语音网络传输的基础。
方法一：
[cpp]
[](http://blog.csdn.net/caoshangpa/article/details/51224678#)#include <QApplication>#include <QFile>#include <QAudioFormat>#include <QAudioOutput>**int argc,  *argv[])  **
- );  
- 
-     QAudioFormat audioFormat;  
- 
-     audioFormat.setSampleRate(44100);  
- 
-     audioFormat.setChannelCount(2);  
- 
-     audioFormat.setSampleSize(16);  
- 
-     audioFormat.setCodec();  
- 
-     audioFormat.setByteOrder(QAudioFormat::LittleEndian);  
- 
-     audioFormat.setSampleType(QAudioFormat::UnSignedInt);  
-  QAudioOutput( audioFormat, 0);  
-  a.exec();  
- }  
方法二：
[cpp]
[](http://blog.csdn.net/caoshangpa/article/details/51224678#)Widget::Widget(QWidget *parent) :  
-     QWidget(parent),  
-  Ui::Widget)  
- {  
- );  
- 
- 
-  *file=fopen(,);  
- (!file)  
-     {  
- ;  
- 
- 
- 
- [fileLength+1];  
- 
- 
- 
- 
- 
- 
- 
- 
- 
- );  
- 
- 
- 
-  (!info.isFormatSupported(audioFormat)) {  
-  QAudioOutput(audioFormat, );  
- 
-  QTimer();  
-     connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));    
- }  
[cpp]
[](http://blog.csdn.net/caoshangpa/article/details/51224678#)void i=0;  
- (i<fileLength/1764)  
- (audioOutput&&audioOutput->state()!=QAudio::StoppedState&&  
-  chunks = audioOutput->bytesFree()/audioOutput->periodSize();  
-  (chunks)  
-  (tempBuffer.length() >= audioOutput->periodSize())  
- 
- 
- 
- ;  
-                 }  
- }  
PCM裸流1秒的数据量是44100×16×2/8=176400字节，那么10毫秒的数据量就是1764字节，所以定时器定时10毫秒，每隔10毫秒从数组中顺序读取1746个字节。
![](https://img-blog.csdn.net/20160423103016701)
PCM音频文件的制作参考：http://blog.csdn.net/caoshangpa/article/details/51218994
音频参数解析参考：http://blog.csdn.net/caoshangpa/article/details/51218597
测试用PCM音频文件下载链接：http://download.csdn.net/detail/caoshangpa/9497751
方法二源码：见http://blog.csdn.net/caoshangpa/article/details/51224678

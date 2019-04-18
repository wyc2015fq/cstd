# soundtouch 变声使用和算法 - 简书


# soundtouch 变声使用和算法



### soundtouch 简介

[soundtouch 源码下载地址](https://link.jianshu.com?t=http://www.surina.net/soundtouch/)

编译完成之后$soundstretch --help

> 
This program is subject to (L)GPL license. Run "soundstretch -license" for

more information.

This application processes WAV audio files by modifying the sound tempo,

pitch and playback rate properties independently from each other.

Usage :

soundstretch infilename outfilename [switches]

To use standard input/output pipes, give 'stdin' and 'stdout' as filenames.

Available switches are:

-tempo=n : Change sound tempo by n percents  (n=-95..+5000 %)

-pitch=n : Change sound pitch by n semitones (n=-60..+60 semitones)

-rate=n  : Change sound rate by n percents   (n=-95..+5000 %)

-bpm=n   : Detect the BPM rate of sound and adjust tempo to meet 'n' BPMs.

If '=n' is omitted, just detects the BPM rate.

-quick   : Use quicker tempo change algorithm (gain speed, lose quality)

-naa     : Don't use anti-alias filtering (gain speed, lose quality)

-speech  : Tune algorithm for speech processing (default is for music)

-license : Display the program license text (LGPL)
### 测试与结果

音频原样为附件中的：

y.wav

实验若干次发现 其实都是在修改如下三个参数

pitch（音调）  tempo（节拍）  rate（速度）

测试使用音频为资源文件中的y.wav- 
pitch 变调不变速

设定pitch 分别为+10 和 -10

pitch+10 为资源文件中的pitch10.wav

pitch-10 为资源文件中的pitch-10.wav

打开音频对比波形：





![](https://upload-images.jianshu.io/upload_images/1802307-906e2499bd8b25cc.png)





发现是在插入或者删除波动周期。例如降低pitch 就是将多次波动，改为一次均值的波动，从而使得音调降低，而音量和时间长度都保持不变。
- 
tempo 变速不变调；

分别设定参数为tempo+20 和tempo-20

tempo+20 的输出效果为资源文件中的 tempo20.wav

tempo-20 的输出效果为资源文件中的 tempo-20.wav





![](https://upload-images.jianshu.io/upload_images/1802307-e8dcb1fd23bcaeed.png)





说明tempo的修改是将音频波形线性延长或缩短。
- 
rate 变速和变调

rate+20rate20.wav 为资源文件中的rate20.wav

rate-20  rate-20.wav为资源文件中的rate-20.wav





![](https://upload-images.jianshu.io/upload_images/1802307-2dedf2e0ed96676c.png)





通过线性插值抽值实现。

### 附

[soundtouch变声之后效果下载](https://link.jianshu.com?t=http://download.csdn.net/detail/dangxw_/9471798)
















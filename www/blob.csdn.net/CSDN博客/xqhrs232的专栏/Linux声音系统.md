# Linux声音系统 - xqhrs232的专栏 - CSDN博客
2017年02月08日 17:40:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：234
原文地址::[http://www.cnblogs.com/jiqingwu/p/linux_sound_system.html](http://www.cnblogs.com/jiqingwu/p/linux_sound_system.html)
相关文章
1、[Linux下的声音编程方法](http://blog.csdn.net/liuchao35758600/article/details/6880910)----[http://blog.csdn.net/liuchao35758600/article/details/6880910](http://blog.csdn.net/liuchao35758600/article/details/6880910)
2、**让LINUX发出声音----[http://xghe110.blog.51cto.com/337884/65713/](http://xghe110.blog.51cto.com/337884/65713/)**
3、**Linux下扬声器发声的小程序----[http://www.2cto.com/kf/200609/12832.html](http://www.2cto.com/kf/200609/12832.html)**
Linux声音系统有些混乱，它有三套音频驱动: OSS (Open Sound System), 商业版的OSS，和ALSA (Advanced Linux Sound Architechture)。 所以底层驱动有OSS和ALSA两套API。
### 一、声卡的工作原理
音频驱动的主要作用是驱动声卡工作，所以也叫声卡驱动。 声卡的主要作用是完成数模转换。 当电脑播放声音时，将数字数据送到声卡，由声卡转换成模拟数据给扬声器等外放设备。 当录音时，声卡将从麦克风送来的模拟数据采样成数字数据，送给计算机处理。
### 二、Linux音频驱动
早期的Linux音频驱动是OSS (Open Sound System)， 但ALSA (Advanced Linux Sound Architechture)后来居上， 大有完全取代OSS的趋势。
Linux都是通过设备文件访问外部设备， ALSA和OSS有不同的设备文件。
```
+----------+--------------+--------------------+---------+
|设备类型  |    OSS       |  ALSA              | 备注    |
+----------+--------------+--------------------+---------+
|声音采样  |   /dev/dsp0  | /dev/snd/pcmC0D0   |         |
+----------+--------------+--------------------+---------+
| 混音器   |  /dev/mixer0 | /dev/snd/mixerC0D0 | 控制音量|
+----------+--------------+--------------------+---------+
|高层音序器|  /dev/music0 |    N/A             | MIDI支持|
+----------+--------------+--------------------+---------+
|低层音序器|  /dev/midi0  |    N/A             | MIDI支持|
+----------+--------------+--------------------+---------+
|声音状态  |  /dev/sndstat|    N/A             |         |
+----------+--------------+--------------------+---------+
|控制文件  |   N/A        | /dev/snd/controlC0 |         |
+----------+--------------+--------------------+---------+
```
ALSA提供了和OSS不同的API，因此如果你使用ALSA做音频驱动， 一些通过OSS操作声音的程序将不能正常发声。
为了解决这一问题，ALSA提供了兼容OSS的库， 因此，如果你使用 ALSA 驱动声卡，也安装了兼容OSS的库， 那些使用OSS API的程序也能正常工作。
### 三、应用程序和声卡驱动的交互方式
#### 直接和底层声音驱动打交道
有一些应用程序直接和最底层的声音驱动（OSS或ALSA）打交道， 程序内部直接调用 OSS 或 ALSA 的API。
#### 通过声音服务器
声音服务器介于应用程序和声卡驱动之间。 当不同的应用调用声音服务器的API来播放声音时，它们把音频数据送到服务器， 服务器将一个以上的播放请求混音后，再发送给底层的声卡驱动(ALSA或OSS)。 由ALSA或OSS来驱动声卡播放混音后的数据。
 基于ESD开发音频程序的好处有：（1）简化开发。（2）即使底层驱动不支持多线程， 通过声音服务器也能实现多个应用程序同时发声。（3）有更好的音效。
- ESD
ESD 一直是 Gnome 桌面环境的声音服务器， Gnome 应用多是通过 ESD 处理声音的。
- aRts
和 ESD 对应，aRts 是 KDE 桌面环境的声音服务器， 底层通过 ALSA 驱动声卡。KDE 应用多通过 aRts 处理声音。
- PulseAudio
PulseAudio是新一代声音服务器，能提供更好的音效， 已经有越来越多的应用通过PulseAudio处理声音。 为了让使用ESD的程序能继续在PulseAudio 上工作， PulseAudio 提供了 ESD 的兼容层。
 Gnome 未来将采用PulseAudio取代ESD。
#### 通过其它库
除了声音服务器，为了简化开发，还出现了其它的一些声音库。 这些声音库有的和声音服务器打交道，有的直接和最底层的声音驱动（OSS或ALSA）打交道。 比如游戏程序使用的SDL库，就是直接和声音驱动打交道。
### 四、总结
Linux从应用程序到声音驱动的结构如下图所示。
```
+-------+  +------+     +-----+  +-----+ +----+
   | Apps  |  | Apps |     |Apps |  |Apps | |Apps|
   +---+---+  +---+--+     +--+--+  +--+--+ +-+--+
       |          |           |        |      |
   +---+---+  +---+------+  +-+--+  +--+--+   |
   | ESD   |  |PulseAudio|  |aRts|  |Other|   |
   | Gnome |  |ESD compat|  |KDE |  | SDL |   |
   +---+---+  +----+-----+  +-+--+  +--+--+   |
       |           |          |        |      |
   +---+-----------+----------+--------+------+---+
   | ALSA (Advanced Linux Sound Architechture)    |
   |    OSS (Open Sound System)                   |
   +----------------------------------------------+
```

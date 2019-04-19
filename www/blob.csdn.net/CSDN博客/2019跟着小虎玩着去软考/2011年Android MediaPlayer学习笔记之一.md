# 2011年Android MediaPlayer学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月08日 15:19:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3585
# 2011年Android MediaPlayer学习笔记之一
**一.MediaPlay实现什么主要功能？**
1.Audio的播放；
2.Video的播放；
**二.在Android界面上，哪些应用程序调用了MediaPlayer实现**1.Music；
2.Video；
**三.MediaPlayer是底层是基于什么库实现的？**OpenCore(PacketVideo)
**四.Application 源代码路径：**
Packages/apps/Music/src/com/android/music
**五. Framework的路径是什么？**frameworks/base/media/java/android/media/MediaPlayer.java
**六.JAVA本地调用部分(JNI)**frameworks/base/media/jni/android_media_MediaPlayer.cpp
该代码编译成：libmedia_jni.so
**七.多媒体Service**frameworks/base/media/libmediaplyerservice
文件有：
1.mediaplayerservice.h；
2.mediaplayerservice.cpp；
该代码编译成：libmediaplayerservice.so
**八.基于OpenCore的多媒体播放**external/opencore
该代码编译成：libopencoreplayer.so
**九.多媒体底层库**frameworks/base/media/libmedia
该代码编译成：libmedia.so
**十.在Framework里的主要的头文件**frameworks/base/include/media
**十一.4大动态链接库**
1.libmedia_jni.so;
2.libmedia.so;
3.libmediaplayerservice.so;
4.libopencoreplayer.so;
**十二.如何定义MediaPlayer的接口和架构**1.IMediaPlayerService.h;
2.IMediaPalyerClient.h;
3.MediaPlayer.h;
**十三.哪些文件实现MediaPlayer的架构**1.MediaPlayerService.cpp;
2.mediaplayer.cpp;
**十四.MediaPlayer具体功能如何实现**1.通过PVPlayer(libopencoreplayer.so)实现；
2.实现 android.media.MediaPlayer类；
**十五.libmedia_jni.so的作用**通过调用MediaPlayer类，来提供对JAVA的接口；
**十六.libmedia.so的作用**1.实现MediaPlayer类，该类对上层提供接口；
2.进程间通讯，即与libmediaplayerservice.so通讯；
**十七.MediaPlayer类接口是如何实现**在libmedia.so中实现；
**十八.谁来实现Media的服务器功能**1.libmediaplayerservice.so来实现;
2. libmediaplayerservice.so是Media的服务器;
3. libmediaplayerservice.so她通过继承libmedia.so来实现服务器功能;
**十九.哪个头文件提供对上层的接口**1.mediaplayer.h;
2. mediaplayer.h的目录在:
frameworks/base/include/media
**二十.如何定义对外接口IMediaPlayerClient**IMediaPlayerClient.h用于描述一个MediaPlayer客户端的接口，描述如下所示：
class IMediaPlayerClient: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayerClient);
virtual void notify(int msg, int ext1, int ext2) = 0;
};
class BnMediaPlayerClient: public BnInterface<IMediaPlayerClient>
{
public:
virtual status_t   onTransact( uint32_t code,
                              const Parcel& data,
                              Parcel* reply,
                              uint32_t flags = 0);
};
1. IMediaPlayerClient类继承IInterface，
2.定义了一个MediaPlayer客户端的接口，BnMediaPlayerClient继承了BnInterface<IMediaPlayerClient>，这是为基于Android的基础类Binder机制实现在进程通讯而构建的。事实上，根据BnInterface类模版的定义BnInterface<IMediaPlayerClient>类相当于双继承了BnInterface和ImediaPlayerClient。这是Android一种常用的定义方式。
**二十一.对外接口类MediaPlayer**
mediaplayer.h是对外的接口类，它最主要是定义了一个MediaPlayer类：
class MediaPlayer : public BnMediaPlayerClient
{
public:
MediaPlayer();
~MediaPlayer();
void onFirstRef();
void disconnect();
status_t setDataSource(const char *url);
status_t setDataSource(int fd, int64_t offset, int64_t length);
status_t setVideoSurface(const sp<Surface>& surface);
status_t setListener(const sp<MediaPlayerListener>& listener);
status_t prepare();
status_t prepareAsync();
status_t start();
status_t stop();
status_t pause();
bool        isPlaying();
status_t getVideoWidth(int *w);
status_t getVideoHeight(int *h);
status_t seekTo(int msec);
status_t getCurrentPosition(int *msec);
status_t getDuration(int *msec);
status_t reset();
status_t setAudioStreamType(int type);
status_t setLooping(int loop);
status_t setVolume(float leftVolume, float rightVolume);
void notify(int msg, int ext1, int ext2);
static sp<IMemory> decode(const char* url, uint32_t *pSampleRate, int* pNumChannels);
static sp<IMemory> decode(int fd, int64_t offset, int64_t length, uint32_t *pSampleRate, int* pNumChannels);
//……
}
从接口中可以看出MediaPlayer类刚好实现了一个MediaPlayer的基本操作，例如播放（start）、停止（stop）、暂停（pause）等。
另外的一个类DeathNotifier在MediaPlayer类中定义，它继承了IBinder类中的DeathRecipient类：
class DeathNotifier: public IBinder:: DeathRecipient
{
public:
DeathNotifier() {}
virtual ~DeathNotifier();
virtual void binderDied(const wp<IBinder>& who);
};
事实上，MediaPlayer类正是间接地继承了IBinder，而MediaPlayer:: DeathNotifier类继承了IBinder:: DeathRecipient，这都是为了实现进程间通讯而构建的。
**二十二.实现MediaPlayer功能的接口IMediaPlayer**
   IMediaPlayer.h主要的的内容是一个实现MediaPlayer功能的接口，它的主要定义如下所示：
class IMediaPlayer: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayer);
virtual void disconnect() = 0;
virtual status_t setVideoSurface(const sp<ISurface>& surface) = 0;
virtual status_t prepareAsync() = 0;
virtual status_t start() = 0;
virtual status_t stop() = 0;
virtual status_t pause() = 0;
virtual status_t isPlaying(bool* state) = 0;
virtual status_t getVideoSize(int* w, int* h) = 0;
virtual status_t seekTo(int msec) = 0;
virtual status_t getCurrentPosition(int* msec) = 0;
virtual status_t getDuration(int* msec) = 0;
virtual status_t reset() = 0;
virtual status_t setAudioStreamType(int type) = 0;
virtual status_t setLooping(int loop) = 0;
virtual status_t setVolume(float leftVolume, float rightVolume) = 0;
};
class BnMediaPlayer: public BnInterface<IMediaPlayer>
{
public:
virtual status_t onTransact( uint32_t code,
                                 const Parcel& data, 
                                 Parcel* reply,
                                 uint32_t flags = 0);
};
在IMediaPlayer类中，主要定义MediaPlayer的功能接口，这个类必须被继承才能够使用。值得注意的是，这些接口和MediaPlayer类的接口有些类似，但是它们并没有直接的关系。事实上，在MediaPlayer类的各种实现中，一般都会通过调用IMediaPlayer类的实现类来完成。
**二十三.MediaPlayer服务的接口: IMediaPlayerService**
   IMediaPlayerService.h用于描述一个MediaPlayer的服务，定义方式如下所示：
class IMediaPlayerService: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayerService);
virtual sp<IMediaPlayer> create(pid_t pid, const sp<IMediaPlayerClient>& client, const char* url) = 0;
virtual sp<IMediaPlayer> create(pid_t pid, const sp<IMediaPlayerClient>& client, int fd, int64_t offset, int64_t length) = 0;
virtual sp<IMemory> decode(const char* url, uint32_t *pSampleRate, int* pNumChannels) = 0;
virtual sp<IMemory> decode(int fd, int64_t offset, int64_t length, uint32_t *pSampleRate, int* pNumChannels) = 0;
};
class BnMediaPlayerService: public BnInterface<IMediaPlayerService>
{
public:
virtual status_t onTransact( uint32_t code,
                                 const Parcel& data,
                                 Parcel* reply,
                                 uint32_t flags = 0);
};
由于具有纯虚函数，IMediaPlayerService 以及BnMediaPlayerService必须被继承实现才能够使用，在IMediaPlayerService定义的create和decode等接口，事实上是必须被继承者实现的内容。注意，create的返回值的类型是sp<IMediaPlayer>，这个IMediaPlayer正是提供实现功能的接口。

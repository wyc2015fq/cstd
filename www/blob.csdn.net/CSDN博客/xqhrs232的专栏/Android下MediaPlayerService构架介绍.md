# Android下MediaPlayerService构架介绍 - xqhrs232的专栏 - CSDN博客
2012年03月08日 11:35:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：729
原文地址::[http://www.eetrend.com/forum/100019624](http://www.eetrend.com/forum/100019624)
本文主要介绍的是Android中很重要也最为复杂的媒体播放器（MediaPlayer）部分的架构。对于Android这样一个完整又相对复杂的系统，一个MediaPlayer功能的实现不在其具体的功能，而是具体功能如何适应Android系统Android MediaPlayer的主要具体实现在OpenCore的Player中，这部分不是本文的关注点。本文关注的是MediaPlayer系统的架构，其他的一些Android的应用程序也使用类似的架构。
对于开源事业在中国的发展，hanchao3c认为应该共享的不仅仅是代码，文档、设计思想、理念甚至对于技术的理解都应该得到充分的共享。Android为中国人进入大规模的开源项目提供了很好的机遇，对于走在技术前沿的人们，不应将技术视为私有财产，而应该将自己理解更好地奉献给大众，提高大众的学习速度，从中也可以得到反馈，从而促进自己的进步。仅以此文奉献给所有关系技术的朋友，希望可以抛砖引玉，促进我们共同的技术进步！
**第一部分 MediaPlayer概述g>Android的MediaPlayer包含了Audio和video的播放功能，在Android的界面上，Music和Video两个应用程序都是调用MediaPlayer实现的。**
MediaPlayer在底层是基于OpenCore(PacketVideo)的库实现的，为了构建一个MediaPlayer程序，上层还包含了进程间通讯等内容，这种进程间通讯的基础是Android基本库中的Binder机制。
以开源的Android为例MediaPlayer的代码主要在以下的目录中：
JAVA程序的路径：packages/apps/Music/src/com/android/music/
JAVA类的路径：frameworks/base/media/java/android/media/MediaPlayer.java
JAVA本地调用部分（JNI）：frameworks/base/media/jni/android_media_MediaPlayer.cpp
这部分内容编译成为目标是libmedia_jni.so。
主要的头文件在以下的目录中：frameworks/base/include/media/
多媒体底层库在以下的目录中：frameworks/base/media/libmedia/
这部分的内容被编译成库libmedia.so。
多媒体服务部分：frameworks/base/media/libmediaplayerservice/
文件为mediaplayerservice.h和mediaplayerservice.cpp
这部分内容被编译成库libmediaplayerservice.so。
基于OpenCore的多媒体播放器部分 external/opencore/
这部分内容被编译成库libopencoreplayer.so。
从程序规模上来看，libopencoreplayer.so是主要的实现部分，而其他的库基本上都是在其上建立的封装和为建立进程间通讯的机制。
**第二部分 MediaPlayer的接口与架构**
2.1 整体框架图
MediaPlayer的各个库之间的结构比较复杂，可以用下图的表示
![MediaPlayer的各个库之间的结构](http://www.eetrend.com/files-eetrend/mediaplayer-1.png)
在各个库中，libmedia.so位于核心的位置，它对上层的提供的接口主要是MediaPlayer类，类libmedia_jni.so通过调用MediaPlayer类提供对JAVA的接口，并且实现了android.media.MediaPlayer类。
libmediaplayerservice.so是Media的服务器，它通过继承libmedia.so的类实现服务器的功能，而libmedia.so中的另外一部分内容则通过进程间通讯和libmediaplayerservice.so进行通讯。libmediaplayerservice.so的真正功能通过调用OpenCore Player来完成。
MediaPlayer部分的头文件在frameworks/base/include/media/目录中，这个目录是和libmedia.so库源文件的目录frameworks/base/media/libmedia/相对应的。主要的头文件有以下几个：
IMediaPlayerClient.h
mediaplayer.h
IMediaPlayer.h
IMediaPlayerService.h
MediaPlayerInterface.h
在这些头文件mediaplayer.h提供了对上层的接口，而其他的几个头文件都是提供一些接口类（即包含了纯虚函数的类），这些接口类必须被实现类继承才能够使用。
整个MediaPlayer库和调用的关系如下图所示：
![整个MediaPlayer库和调用的关系](http://www.eetrend.com/files-eetrend/mediaplayer-2.png)
整个MediaPlayer在运行的时候，可以大致上分成Client和Server两个部分，它们分别在两个进程中运行，它们之间使用Binder机制实现IPC通讯。从框架结构上来看，IMediaPlayerService.h、IMediaPlayerClient.h和MediaPlayer.h三个类定义了MeidaPlayer的接口和架构，MediaPlayerService.cpp和mediaplayer.coo两个文件用于MeidaPlayer架构的实现，MeidaPlayer的具体功能在PVPlayer（库libopencoreplayer.so）中的实现。
2.2 头文件IMediaPlayerClient.h
IMediaPlayerClient.h用于描述一个MediaPlayer客户端的接口，描述如下所示：
class IMediaPlayerClient: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayerClient);
virtual void notify(int msg, int ext1, int ext2) = 0;
};
class BnMediaPlayerClient: public BnInterface
{
public:
virtual status_t onTransact( uint32_t code,
const Parcel& data,
Parcel* reply,
uint32_t flags = 0);
};
在定义中，IMediaPlayerClient类继承IInterface，并定义了一个MediaPlayer客户端的接口，BnMediaPlayerClient继承了BnInterface，这是为基于Android的基础类Binder机制实现在进程通讯而构建的。事实上，根据BnInterface类模版的定义BnInterface类相当于双继承了BnInterface和ImediaPlayerClient。这是Android一种常用的定义方式。
2.3 头文件mediaplayer.h
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
status_t setVideoSurface(const sp& surface);
status_t setListener(const sp& listener);
status_t prepare();
status_t prepareAsync();
status_t start();
status_t stop();
status_t pause();
bool isPlaying();
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
static sp decode(const char* url, uint32_t *pSampleRate, int* pNumChannels);
static sp decode(int fd, int64_t offset, int64_t length, uint32_t *pSampleRate, int* pNumChannels);
//……
}
从接口中可以看出MediaPlayer类刚好实现了一个MediaPlayer的基本操作，例如播放（start）、停止（stop）、暂停（pause）等。
另外的一个类DeathNotifier在MediaPlayer类中定义，它继承了IBinder类中的DeathRecipient类：
class DeathNotifier: public IBinder:: DeathRecipient
{
public:
DeathNotifier() {}
virtual ~DeathNotifier();
virtual void binderDied(const wp& who);
};
事实上，MediaPlayer类正是间接地继承了IBinder，而MediaPlayer:: DeathNotifier类继承了IBinder:: DeathRecipient，这都是为了实现进程间通讯而构建的。
2.4 头文件IMediaPlayer.h
IMediaPlayer.h主要的的内容是一个实现MediaPlayer功能的接口，它的主要定义如下所示：
class IMediaPlayer: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayer);
virtual void disconnect() = 0;
virtual status_t setVideoSurface(const sp& surface) = 0;
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
class BnMediaPlayer: public BnInterface
{
public:
virtual status_t onTransact( uint32_t code,
const Parcel& data,
Parcel* reply,
uint32_t flags = 0);
};
在IMediaPlayer类中，主要定义MediaPlayer的功能接口，这个类必须被继承才能够使用。值得注意的是，这些接口和MediaPlayer类的接口有些类似，但是它们并没有直接的关系。事实上，在MediaPlayer类的各种实现中，一般都会通过调用IMediaPlayer类的实现类来完成。
2.5 头文件IMediaPlayerService.h
IMediaPlayerService.h用于描述一个MediaPlayer的服务，定义方式如下所示：
class IMediaPlayerService: public IInterface
{
public:
DECLARE_META_INTERFACE(MediaPlayerService);
virtual sp create(pid_t pid, const sp& client, const char* url) = 0;
virtual sp create(pid_t pid, const sp& client, int fd, int64_t offset, int64_t length) = 0;
virtual sp decode(const char* url, uint32_t *pSampleRate, int* pNumChannels) = 0;
virtual sp decode(int fd, int64_t offset, int64_t length, uint32_t *pSampleRate, int* pNumChannels) = 0;
};
class BnMediaPlayerService: public BnInterface
{
public:
virtual status_t onTransact( uint32_t code,
const Parcel& data,
Parcel* reply,
uint32_t flags = 0);
};
由于具有纯虚函数，IMediaPlayerService 以及BnMediaPlayerService必须被继承实现才能够使用，在IMediaPlayerService定义的create和decode等接口，事实上是必须被继承者实现的内容。注意，create的返回值的类型是sp，这个IMediaPlayer正是提供实现功能的接口。
**第三部分 MediaPlayer的主要实现分析3.1 JAVA程序部分**
在packages/apps/Music/src/com/android/music/目录的MediaPlaybackService.java文件中，包含了对MediaPlayer的调用。
在MediaPlaybackService.java中包含对包的引用：
import android.media.MediaPlayer;
在MediaPlaybackService类的内部，定义了MultiPlayer类：
private class MultiPlayer {
private MediaPlayer mMediaPlayer = new MediaPlayer();
}
MultiPlayer类中使用了MediaPlayer类，其中有一些对这个MediaPlayer的调用，调用的过程如下所示：
mMediaPlayer.reset();
mMediaPlayer.setDataSource(path);
mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
reset、setDataSource和setAudioStreamType等接口就是通过JAVA本地调用（JNI）来实现的。
3.2 MediaPlayer的JAVA本地调用部分
MediaPlayer的JAVA本地调用部分在目录frameworks/base/media/jni/的android_media_MediaPlayer.cpp中的文件中实现。
android_media_MediaPlayer.cpp之中定义了一个JNINativeMethod（JAVA本地调用方法）类型的数组gMethods，如下所示：
static JNINativeMethod gMethods[] = {
{"setDataSource", "(Ljava/lang/String;)V", (void *)android_media_MediaPlayer_setDataSource},
{"setDataSource", "(Ljava/io/FileDescriptor;JJ)V", (void *)android_media_MediaPlayer_setDataSourceFD},
{"prepare", "()V", (void *)android_media_MediaPlayer_prepare},
{"prepareAsync", "()V", (void *)android_media_MediaPlayer_prepareAsync},
{"_start", "()V", (void *)android_media_MediaPlayer_start},
{"_stop", "()V", (void *)android_media_MediaPlayer_stop},
{"getVideoWidth", "()I", (void *)android_media_MediaPlayer_getVideoWidth},
{"getVideoHeight", "()I", (void *)android_media_MediaPlayer_getVideoHeight},
{"seekTo", "(I)V", (void *)android_media_MediaPlayer_seekTo},
{"_pause", "()V", (void *)android_media_MediaPlayer_pause},
{"isPlaying", "()Z", (void *)android_media_MediaPlayer_isPlaying},
{"getCurrentPosition", "()I", (void *)android_media_MediaPlayer_getCurrentPosition},
{"getDuration", "()I", (void *)android_media_MediaPlayer_getDuration},
{"_release", "()V", (void *)android_media_MediaPlayer_release},
{"_reset", "()V", (void *)android_media_MediaPlayer_reset},
{"setAudioStreamType", "(I)V", (void *)android_media_MediaPlayer_setAudioStreamType},
{"setLooping", "(Z)V", (void *)android_media_MediaPlayer_setLooping},
{"setVolume", "(FF)V", (void *)android_media_MediaPlayer_setVolume},
{"getFrameAt", "(I)Landroid/graphics/Bitmap;", (void *)android_media_MediaPlayer_getFrameAt},
{"native_setup", "(Ljava/lang/Object;)V", (void *)android_media_MediaPlayer_native_setup},
{"native_finalize", "()V", (void *)android_media_MediaPlayer_native_finalize},
}
JNINativeMethod的第一个成员是一个字符串，表示了JAVA本地调用方法的名称，这个名称是在JAVA程序中调用的名称；第二个成员也是一个字符串，表示JAVA本地调用方法的参数和返回值；第三个成员是JAVA本地调用方法对应的C语言函数。
其中android_media_MediaPlayer_reset函数的实现如下所示：
static void android_media_MediaPlayer_reset(JNIEnv *env, jobject thiz)
{
sp mp = getMediaPlayer(env, thiz);
if (mp == NULL ) {
jniThrowException(env, "java/lang/IllegalStateException", NULL);
return;
}
process_media_player_call( env, thiz, mp->reset(), NULL, NULL );
}
在android_media_MediaPlayer_reset的调用中，得到一个MediaPlayer指针，通过对它的调用实现实际的功能。
register_android_media_MediaPlayer用于将gMethods注册为的类"android/media/MediaPlayer"，其实现如下所示。
static int register_android_media_MediaPlayer(JNIEnv *env)
{
jclass clazz;
clazz = env->FindClass("android/media/MediaPlayer");
// ......
return AndroidRuntime::registerNativeMethods(env, "android/media/MediaPlayer", gMethods, NELEM(gMethods));
}
"android/media/MediaPlayer"对应JAVA的类android.media.MediaPlayer。
3.3 mediaplayer的核心库libmedia.so
libs/media/mediaplayer.cpp文件用于实现mediaplayer.h提供的接口，其中一个重要的片段如下所示：
const sp& MediaPlayer::getMediaPlayerService()
{
Mutex::Autolock _l(mServiceLock);
if (mMediaPlayerService.get() == 0) {
sp sm = defaultServiceManager();
sp binder;
do {
binder = sm->getService(String16("media.player"));
if (binder != 0)
break;
LOGW("MediaPlayerService not published, waiting...");
usleep(500000); // 0.5 s
} while(true);
if (mDeathNotifier == NULL) {
mDeathNotifier = new DeathNotifier();
}
binder->linkToDeath(mDeathNotifier);
mMediaPlayerService = interface_cast(binder);
}
LOGE_IF(mMediaPlayerService==0, "no MediaPlayerService!?");
return mMediaPlayerService;
}
其中最重要的一点是binder = sm->getService(String16("media.player"));这个调用用来得到一个名称为"media.player"的服务，这个调用返回值的类型为IBinder，根据实现将其转换成类型IMediaPlayerService使用。
一个具体的函数setDataSource如下所示：
status_t MediaPlayer::setDataSource(const char *url)
{
LOGV("setDataSource(%s)", url);
status_t err = UNKNOWN_ERROR;
if (url != NULL) {
const sp& service(getMediaPlayerService());
if (service != 0) {
sp player(service->create(getpid(), this, url));
err = setDataSource(player);
}
}
return err;
}
在函数setDataSource函数中，调用getMediaPlayerService得到了一个IMediaPlayerService，又从IMediaPlayerService中得到了IMediaPlayer类型的指针，通过这个指针进行着具体的操作。
其他一些函数的实现也与setDataSource类似。
libmedia.so中的其他一些文件与头文件的名称相同，它们是：
libs/media/IMediaPlayerClient.cpp
libs/media/IMediaPlayer.cpp
libs/media/IMediaPlayerService.cpp
为了实现Binder的具体功能，在这些类中还需要实现一个BpXXX的类，例如IMediaPlayerClient.cpp的实现如下所示：l
class BpMediaPlayerClient: public BpInterface
{
public:
BpMediaPlayerClient(const sp& impl)
: BpInterface(impl){}
virtual void notify(int msg, int ext1, int ext2)
{
Parcel data, reply;
data.writeInterfaceToken(IMediaPlayerClient::getInterfaceDescriptor());
data.writeInt32(msg);
data.writeInt32(ext1);
data.writeInt32(ext2);
remote()->transact(NOTIFY, data, &reply, IBinder::FLAG_ONEWAY);
}
};
还需要实现定义宏IMPLEMENT_META_INTERFACE，这个宏将被展开，生成几个函数：
IMPLEMENT_META_INTERFACE(MediaPlayerClient, "android.hardware.IMediaPlayerClient");
以上的实现都是基于Binder框架的实现方式，只需要按照模版实现即可。其中BpXXX的类为代理类（proxy），BnXXX的类为本地类（native）。代理类的transact函数和本地类的onTransact函数实现对应的通讯。
3.4 media服务libmediaservice.so
frameworks/base/media\libmediaplayerservice目录中的MediaPlayerService.h和MediaPlayerService.cpp用于实现一个
servers/media/的服务，MediaPlayerService是继承BnMediaPlayerService的实现，在这个类的内部又定义了类Client，MediaPlayerService::Client继承了BnMediaPlayer。
class MediaPlayerService : public BnMediaPlayerService
{
class Client : public BnMediaPlayer
}
在MediaPlayerService中具有如下一个静态函数instantiate：
void MediaPlayerService::instantiate() {
defaultServiceManager()->addService(
String16("media.player"), new MediaPlayerService());
}
在instantiate函数中，调用IServiceManager的一个函数addService，向其中增加了一个名为"media.player"的服务。
这个名为"media.player"的服务和mediaplayer.cpp中调用getService中得到的使用一样名称。因此，在这里调用addService增加服务在mediaplayer.cpp中可以按照名称"media.player"来使用。这就是使用Binder实现进程间通讯的（IPC）的作用，事实上这个MediaPlayerService类是在服务中运行的，而mediaplayer.cpp调用的功能在应用中运行，二者并不是一个进程。但是在mediaplayer.cpp却像一个进程的调用一样调用MediaPlayerService的功能。
在MediaPlayerService.cpp中的createPlayer函数如下所示：
static sp createPlayer(player_type playerType, void* cookie,
notify_callback_f notifyFunc)
{
sp p;
switch (playerType) {
case PV_PLAYER:
LOGV(" create PVPlayer");
p = new PVPlayer();
break;
case SONIVOX_PLAYER:
LOGV(" create MidiFile");
p = new MidiFile();
break;
case VORBIS_PLAYER:
LOGV(" create VorbisPlayer");
p = new VorbisPlayer();
break;
}
//……
return p;
}
在这里根据playerType的类型建立不同的播放器：对于大多数情况，类型将是PV_PLAYER，这时会调用了new PVPlayer()建立一个PVPlayer，然后将其指针转换成MediaPlayerBase来使用；对于Mini文件的情况，类型为SONIVOX_PLAYER，将会建立一个MidiFile；对于Ogg Vorbis格式的情况，将会建立一个VorbisPlayer。
(OGG Vobis是一种音频压缩格式，与MP3等的音乐格式类似,它具有完全免费、开放和没有专利限制的特点。)
值得注意的是PVPlayer、MidiFile和VorbisPlayer三个类都是继承MediaPlayerInterface得到的，而MediaPlayerInterface又是继承MediaPlayerBase得到的，因此三者具有相同接口类型。只有建立的时候会调用各自的构造函数，在建立之后，将只通过MediaPlayerBase接口来MediaPlayerBase控制它们。
在frameworks/base/media/libmediaplayerservice目录中，MidiFile.h和MidiFile.cpp的实现MidiFile，VorbisPlayer.h和VorbisPlayer.cpp实现一个VorbisPlayer。
3.5 OpenCorePlayer的实现libopencoreplayer.so
OpenCore Player在external/opencore/中实现，这个实现是一个基于OpenCore的Player的实现。具体实现的文件为playerdriver.cpp。其中实现了两个类：PlayerDriver和PVPlayer。PVPlayer通过调用PlayerDriver的函数实现具体的功能。

# openal   播放器的使用 - 建建的博客 - CSDN博客
2018年03月16日 10:02:37[纪建](https://me.csdn.net/u013898698)阅读数：94
## 简介
本文主要介绍如何使用OpenAL进行PCM数据的播放，文中会讲解我在项目中遇到的问题以及如何解决的，对于什么是采样率等基本知识，在此不做介绍。 
OpenAL有使用手册，具体的API作用，可以自己进行查阅。 
刚进公司，就被分配来做一个项目，项目是接收胎心仪蓝牙传输的数据，进行实时绘制胎心率曲线和实时播放胎心音，其中播放胎心音我使用的便是OpenAL。 
先上代码：
OGOpenAL.h
```
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <UIKit/UIKit.h>
#define AUDIO_SIMPLE_RATE 8000//采样率
#define SOUND_SAMPLES AL_FORMAT_MONO8//声道，8位
//最大缓存个数，如果数据缓存多余MAX_BUFFERS时，数据不加载，抛弃掉。解决延时问题
#define MAX_BUFFERS 13
@interface OGOpenAl : NSObject {
    //内容，相当于给音频播放器提供一个环境描述
    ALCcontext         *m_Context; 
    //硬件，获取电脑或者ios设备上的硬件，提供支持
    ALCdevice          *m_Device;   
    //音源，相当于一个ID,用来标识音源            
    ALuint              m_sourceID;    
    //线程锁         
    NSCondition        *m_DecodeLock; 
}
//初始化播放器
-(BOOL)initOpenAl;
//连续传入PCM音频数据的方法
-(void)openAudio:(NSData *)data length:(UInt32)pLength;
//停止播放
-(void)stopSound;
//释放播放器占用
-(void)clearOpenAL;
@end
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
OGOpenAL.m
```
#import "OGOpenAl.h"
#import <AVFoundation/AVFoundation.h>
@implementation OGOpenAl
-(BOOL)initOpenAl {
    NSLog(@"初始化播放器");
    if (m_Device ==nil) {
        //参数为NULL , 让ALC 使用默认设备，默认一个只能指定一个设备，多次指定会一直返回NULL，导致下面m_Device为nil
        m_Device = alcOpenDevice(NULL);
    }
    if (m_Device==nil) {
        //注：执行clearOpenAL方法可以清除Device占用。initOpenAL需与clearOpenAL一对一使用
        NSLog(@"初始化播放器失败，播放器Device已被占用，请清除Device占用后再进行初始化");
        return NO;
    }
    if (m_Context==nil) {
        if (m_Device) {
            //与初始化device是同样的道理
            m_Context =alcCreateContext(m_Device, NULL);
            alcMakeContextCurrent(m_Context);
        }
    }
    //设置播放方式：扬声器。否则扬声器不发音，只有耳机
    NSArray* output = [[AVAudioSession sharedInstance] currentRoute].outputs;
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    NSLog(@"current output:%@",output);
    //初始化音源ID
    alGenSources(1, &m_sourceID);
    // 设置音频播放是否为循环播放，AL_FALSE是不循环
    alSourcei(m_sourceID, AL_LOOPING, AL_FALSE);
    // 设置声音数据为流试，（openAL 针对PCM格式数据流）
    alSourcef(m_sourceID, AL_SOURCE_TYPE, AL_STREAMING);
    //设置音量大小，1.0f表示最大音量。openAL动态调节音量大小就用这个方法
    alSourcef(m_sourceID, AL_GAIN, 1.0f);
    //多普勒效应，这属于高级范畴，不是做游戏开发，对音质没有苛刻要求的话，一般无需设置
    alDopplerVelocity(1.0);
    //同上
    alDopplerFactor(1.0);
    //设置声音的播放速度
    alSpeedOfSound(1.0);
    //初始化线程锁
    m_DecodeLock =[[NSCondition alloc] init];
    if (m_Context==nil) {
        return NO;
    }
    return YES;
}
//这个函数就是比较重要的函数了， 将收到的pcm数据放到缓存器中，再拿出来播放
-(void)openAudio:(NSData *)data length:(UInt32)pLength {
    //1.对缓存操作进行加锁，操作过程中不允许其他操作，避免多线程调用
    [m_DecodeLock lock];
    //2.读取错误信息
    ALenum error;
    error = alGetError();
    if (error != AL_NO_ERROR) {
        [m_DecodeLock unlock];
        NSLog(@"插入PCM数据时发生错误, 错误信息: %d", error);
        [self clearBuffer];
        return;
    }
    //3.常规安全性判断
    if (data == NULL) {
        [m_DecodeLock unlock];
        NSLog(@"插入PCM数据为空, 返回");
        return;
    }
    //4.建立缓存区
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        [m_DecodeLock unlock];
        NSLog(@"建立缓存区发生错误, 错误信息: %d", error);
        return;
    }
    //5.将数据添加到缓存区中
    SignedByte bytes[pLength];
    [data getBytes:bytes length: pLength];
    //6.将转好的NSData存放到之前初始化好的一块buffer区域中并设置好相应的播放格式
    alBufferData(bufferID, SOUND_SAMPLES, bytes , (ALsizei)pLength, AUDIO_SIMPLE_RATE);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        [m_DecodeLock unlock];
        NSLog(@"向缓存区内插入数据发生错误, 错误信息: %d", error);
        return;
    }
    //7.清除缓存
    [self clearBuffer];
    //8.读取队列信息。获取音源的缓冲队列，以便监听控制播放的延迟
    int processed ,queued;
    alGetSourcei(m_sourceID, AL_BUFFERS_PROCESSED, &processed);
    alGetSourcei(m_sourceID, AL_BUFFERS_QUEUED, &queued);
    NSLog(@"缓存队列数 %d", queued);
    //如果缓冲队列大于MAX_BUFFERS则将该包数据抛弃，不添加进入缓冲区，不进行播放。
    //目的是降低延迟，例如：延迟时间控制在210ms，每30ms发送一包，则MAX_BUFFERS=7
    if (queued <= MAX_BUFFERS) {
        //将缓存添加到声源上（添加便会进行播放，不添加不播放）
        alSourceQueueBuffers(m_sourceID, 1, &bufferID);
        error = alGetError();
        if (error != AL_NO_ERROR) {
            [m_DecodeLock unlock];
            NSLog(@"将缓存区插入队列发生错误, 错误信息: %d", error);
            return;
        }
    }
    //9.判断是否有错，有错误则清除缓存
    if ((error=alGetError())!=AL_NO_ERROR) {
        NSLog(@"play failed");
        alDeleteBuffers(1, &bufferID);
        [m_DecodeLock unlock];
        return;
    }
    [m_DecodeLock unlock];
    //10.播放声音
    ALint  state;
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    if (state !=AL_PLAYING) {
        alSourcePlay(m_sourceID);
    }
}
/**
 *  播放声音
 */
-(void)playSound {
    ALint  state;
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        alSourcePlay(m_sourceID);
    }
}
/**
 *  停止播放
 */
-(void)stopSound {
    [self playSound];
    ALint  state;
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED) {
        alSourceStop(m_sourceID);
    }
}
/**
 *  清空播放器
 */
-(void)clearOpenAL {
    //删除声源
    alDeleteSources(1, &m_sourceID);
    if (m_Context != nil) {
        //删除环境
        alcDestroyContext(m_Context);
        m_Context=nil;
        NSLog(@"删除环境");
    }
    if (m_Device !=nil) {
        //关闭设备
        alcCloseDevice(m_Device);
        m_Device=nil;
        NSLog(@"关闭设备");
    } 
}
//清楚缓存
-(void)clearBuffer{
    ALint processed;
    //获取音源的缓冲队列
    alGetSourcei(m_sourceID, AL_BUFFERS_PROCESSED, &processed);
    //遍历清空缓冲区
    while (processed--) {
        ALuint  buffer;
        alSourceUnqueueBuffers(m_sourceID, 1, &buffer);
        alDeleteBuffers(1, &buffer);
    }
}
//获取队列信息
- (void)getInfo {
    ALint queued;
    ALint processed;
    alGetSourcei(m_sourceID, AL_BUFFERS_PROCESSED, &processed);
    alGetSourcei(m_sourceID, AL_BUFFERS_QUEUED, &queued);
    NSLog(@"process = %d, queued = %d", processed, queued);
}
@end
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
## 方法解读
1.initOpenAl 
该方法是初始化播放器方法，播放前必须初始化播放器，目的是设置播放器的参数。 
2.openAudio:length: 
该方法是播放数据的方法，通过连续不断的调用该方法，传入PCM音频数据，将数据添加到播放队列里面，实现不间断PCM数据播放。 
3.clearOpenAL 
该方法是释放播放器，清空播放器占用。
## 使用经验
1. 
initOpenAl方法和clearOpenAL方法必须一对一使用，为什么呢？查看方法内部，你会看到，初始化播放器时，会调用alcOpenDevice的方法，该方法是获取到播放器的device，一旦获取过device后，如果没有释放掉，下次再获取时，会返回null，届时无论你怎么传入播放数据，都会没有声音，因为device是空的。而clearOpenAL便是释放掉device。我刚入手时就遇到这个坑，播放数据怎么也听不到声音。建议外面调用initOpenAl和clearOpenAL时，使用一个标志参数来控制，如isPlay，当为NO时才可以初始化，当为YES时才可以停止。避免连续初始化。 
 2.  
 我发现网上很多人的播放器封装，并没有设置扬声器，导致播放音频时，只能带上耳机听声音，扬声器没有声音，因此我在播放器初始化方法里将播放通道设置为扬声器（当然不会影响耳机的播放，因为苹果系统设置了优先级，一旦插入耳机，变强制默认为耳机播放）。 
 3.  
 播放的数据，我在使用OpenAL前，试用过AudioQueue框架，他们两在播放数据上的区别是OpenAL播放的数据基线（音量为0时）数值在8位播放器下为127~128，即播放的数据范围是0~255，而AudioQueue的基线是0，播放的数据范围是-127~128，因此如果不进行数据转化，在OpenAL上播放是正常的，拿到AudioQueue下进行播放，底噪会非常大，原声会很模糊很低。包括16位播放器，在使用我的播放器封装前，应该先打印出播放的PCM数据，了解清楚数据的基线是什么，如果不合适需要手动将每个数据进行转化。 
 4.  
 我看到很多博客上的播放器封装，都会有播放和停止播放的方法，但是很多人使用的时候并不知道如何使用。盲目的直接调用停止方法，而不停止播放数据的传入，会导致播放器已经停止了播放，但是播放缓存还一直在添加数据，播放缓存最大个数是1024个，当数据超过缓存个数时，程序会崩溃。因此建议在我们APP中使用到停止播放器时（比如APP进入后台需要停止播放、或者其他停止播放），停止播放并释放播放器，重新开始时再初始化播放器进行播放。我在开发过程中就经常遇到APP被打断，然后听不到声音的情况，包括接听电话、进入后台、闹铃等等。 
 5.  
 很多人使用播放器进行PCM播放，都是实时播放，那么如何控制播放器的延时呢？我这个进行了控制延时的操作，宏定义了一个延时的缓存个数MAX_BUFFERS，当添加的缓存个数大于MAX_BUFFERS时，后面新添加的缓存不进行播放，直到缓存的音频数据被播放后，缓存个数少于MAX_BUFFERS时才继续进行添加缓存。我们都直到，缓存的多少决定了延时的长短。如果不需要控制延时，建议将延时代码注释掉。使用时需注意，如果方法2.openAudio:length:给入数据过快，而播放过慢导致大量缓存数据堆积，延时的方法会过滤掉大部分数据，而导致播放声音不连续。 
 6.  
 很多新手不知道如何控制缓存和播放数据的速度。我在这里简单介绍一下，设每包数据长度为125，采样率位4000Hz，那么你应该每125/4000=1/32=0.03125秒传入一包数据（即调用方法2）。如果你传入数据的速度过快，播放器缓存的数据会越来越多，延时会越来越长，如果传入的数据过慢，播放器播放完数据后，会默认重新播放最后一包数据。
平时很少写博客，表达得不是很流畅，只是想到什么写什么，忘见谅。另外文中避免不了有不足的地方，请大神多多指教。我建了个小QQ群143898492，欢迎大家的加入，希望我的分享能够帮助大家。谢谢！

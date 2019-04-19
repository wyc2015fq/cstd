# openal 基础知识 - 建建的博客 - CSDN博客
2018年03月15日 18:15:34[纪建](https://me.csdn.net/u013898698)阅读数：278
## [openal 基础知识](http://www.cnblogs.com/dragon2012/p/3228910.html)
**原文地址：[http://blog.csdn.net/woker/article/details/8687380](http://blog.csdn.net/woker/article/details/8687380)**
**一简介**
OpenAL抽象出三种基本对象：buffers(缓冲区)、sources(源)、listener(听者)。Buffer用来填充声音数据，然后附加到一个Source上，Source可以被定位并播放。声音播放的效果取决于source相对于listener的位置和方向。通过创建数个sources、buffers和一个唯一的listener，并动态更新sources的位置和方向，就可以产生逼真的3D音效。
OpenAL基本对象及其与context和device之间的关系：
![](https://img-my.csdn.net/uploads/201303/18/1363592460_4085.png)
初始化OpenAL时必须打开至少一个device。这个device中，至少要创建1个context。context中含有一个listener，并可创建若干个source。每个source可以附加若干个buffer。Buffer是共享的，不属于某个context。
*1.**设备枚举*
alcOpenDevice()传入一个字符串参数，打开对应的device。字符串应该是一个有效的OpenALrendering device的名字，或者NULL用以请求默认device。PC系统中可能存在多个OpenALrendering device，所以OpenAL的应用程序需要有区分不同设备的能力，可以使用OpenAL的Enumerationextension(枚举扩展包)。Enumerationextension允许开发者获得一个字符串，是一个包含了可用设备的列表【alcGetString(NULL,ALC_DEVICE_SPECIFIER)】，也可以获得默认设备的名字【alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER)】。对于录音设备，相应的参数为ALC_CAPTURE_DEVICE_SPECIFIER和ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER。
用户可以通过alcIsExtensionPresent和alIsExtensionPresent来获取每个device支持的extensions。
*2.**初始化**/**退出*
初始化OpenAL的第一步是打开一个device，再为这个device创建一个context。然后就可以操控基本对象了。buffer操作的各种函数及步骤：
创建buffers：(1)alGetError重置错误状态;(2) alGenBuffers创建多个buffer;(3) alGetError检测是否有错误发生。
填充buffer：alBufferData
创建sources：(1)alGetError重置错误状态;(2) alGenSources创建多个source;(3) alGetError检测是否有错误发生。
附加buffers到sources：alSourcei
source播放buffer：alSourcePlay
动态更新source和listener属性：alGetListenerfv,alListener3f, alSourcei, alGetSource3f
示例：
**[cpp]**[view plain](http://blog.csdn.net/woker/article/details/8687380)[copy](http://blog.csdn.net/woker/article/details/8687380)
- // Initialization
- Device = alcOpenDevice(NULL); // select the "preferred device"
- if (Device) {  
-     Context=alcCreateContext(Device,NULL);  
-     alcMakeContextCurrent(Context);  
- }  
- 
- // Check for EAX 2.0 support
- g_bEAX = alIsExtensionPresent("EAX2.0");  
- // Generate Buffers
- alGetError(); // clear error code
- alGenBuffers(NUM_BUFFERS, g_Buffers);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alGenBuffers :", error);  
- return;  
- }  
- 
- // Load test.wav
- loadWAVFile("test.wav",&format,&data,&size,&freq,&loop);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alutLoadWAVFile test.wav : ", error);  
-     alDeleteBuffers(NUM_BUFFERS, g_Buffers);  
- return;  
- }  
- 
- // Copy test.wav data into AL Buffer 0
- alBufferData(g_Buffers[0],format,data,size,freq);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alBufferData buffer 0 : ", error);  
-     alDeleteBuffers(NUM_BUFFERS, g_Buffers);  
- return;  
- }  
- 
- // Unload test.wav
- unloadWAV(format,data,size,freq);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alutUnloadWAV : ", error);  
-     alDeleteBuffers(NUM_BUFFERS, g_Buffers);  
- return;  
- }  
- 
- // Generate Sources
- alGenSources(1,source);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alGenSources 1 : ", error);  
- return;  
- }  
- 
- // Attach buffer 0 to source
- alSourcei(source[0], AL_BUFFER, g_Buffers[0]);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alSourcei AL_BUFFER 0 : ", error);  
- }  
- 
- // Exit
- Context=alcGetCurrentContext();  
- Device=alcGetContextsDevice(Context);  
- alcMakeContextCurrent(NULL);  
- alcDestroyContext(Context);  
- alcCloseDevice(Device);  
3.*listener**属性*
每个context含有一个listener，函数集alListener[f,3f, fv, i]和alGetListener[f,3f, fv, i]（——注：A[B,C,D]表示AB,AC, AD）可以用来设置或获取以下listener属性：
|属性|数据类型|描述|
|----|----|----|
|AL_GAIN|f, fv|主增益，应为正数|
|AL_POSITION|fv, 3f, iv, 3i|X，Y，Z位置|
|AL_VELOCITY|fv, 3f, iv, 3i|速度向量|
|AL_ORIENTATION|fv, iv|由"at"和"up"向量描述的方向|
示例：
**[cpp]**[view plain](http://blog.csdn.net/woker/article/details/8687380)[copy](http://blog.csdn.net/woker/article/details/8687380)
- ALfloat listenerPos[]={0.0,0.0,0.0};  
- ALfloat listenerVel[]={0.0,0.0,0.0};  
- ALfloat listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};  
- 
- // Position ...
- alListenerfv(AL_POSITION,listenerPos);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alListenerfv POSITION : ", error);  
- return;  
- }  
- 
- // Velocity ...
- alListenerfv(AL_VELOCITY,listenerVel);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alListenerfv VELOCITY : ", error);  
- return;  
- }  
- 
- // Orientation ...
- alListenerfv(AL_ORIENTATION,listenerOri);  
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alListenerfv ORIENTATION : ", error);  
- return;  
- }  
4.*buffer**属性*
每个由alGenBuffers创建的buffer都有属性，函数集alGetBuffer[f,i]可用来获取下述属性：
|属性|数据类型|描述|
|----|----|----|
|AL_FREQUENCY|i, iv|频率(Hz)|
|AL_BITS|i, iv|位宽|
|AL_CHANNELS|i, iv|channel数，可以大于1，但播放时不能定位|
|AL_SIZE|i, iv|总大小(Byte)|
|AL_DATA|i,iv|数据拷贝的初始位置，一般没有用|
示例：
**[cpp]**[view plain](http://blog.csdn.net/woker/article/details/8687380)[copy](http://blog.csdn.net/woker/article/details/8687380)
- // Retrieve Buffer Frequency
- alBufferi(g_Buffers[0], AL_FREQUENCY, iFreq);  
5.*source**属性*
每个由alGenSources创建的source都有可配置的属性，函数集alSource[f,3f, fv, i]和alGetSource[f,3f, fv, i]可以用来配置或获取以下source属性：
|属性|数据类型|描述|
|----|----|----|
|AL_PITCH|f, fv|source的buffer的频率(采样率)倍增器(pitchmultiplier)，总是正数|
|AL_GAIN|f, fv|source增益，应为正数|
|AL_MAX_DISTANCE|f, fv, i, iv|用于反钳位距离模型(InverseClamped Distance Model)，大于次距离，source不再衰减|
|AL_ROLLOFF_FACTOR|f, fv, i, iv|source的衰减率，默认为1.0|
|AL_REFERENCE_DISTANCE|f, fv, i, iv|小于此距离时，source的音量减半(在衰减率和AL_MAX_DISTANCE作用之前)|
|AL_MIN_GAIN|f, fv|source的最小增益|
|AL_MAX_GAIN|f, fv|source的最大增益|
|AL_CONE_OUTER_GAIN|f, fv|外锥(outercone)外的增益|
|AL_CONE_INNER_ANGLE|f, fv, i, iv|内锥覆盖的角度，在此之中，source不会衰减|
|AL_CONE_OUTER_ANGLE|f, fv, i, iv|外锥覆盖的角度，在此之外，source完全衰减。内锥和外锥之间平滑衰减|
|AL_POSITION|fv, 3f|X,Y,Z位置|
|AL_VELOCITY|fv, 3f|速度矢量|
|AL_DIRECTION|fv, 3f, iv, 3i|方向矢量|
|AL_SOURCE_RELATIVE|i, iv|指示position是否是相对于listener|
|AL_SOURCE_TYPE|i, iv|source类型，AL_UNDETERMINED,AL_STATIC, AL_STREAMING|
|AL_LOOPING|i, iv|设置循环播放，AL_TURE,AL_FALSE|
|AL_BUFFER|i, iv|附加的buffer的ID|
|AL_SOURCE_STATE|i, iv|source的状态，AL_STOPPED,AL_PLAYING, ...|
|AL_BUFFERS_QUEUED|i, iv|【只读】用alSourceQueueBuffers入队列的buffer个数，出队用alSourceUnqueueBuffers|
|AL_BUFFERS_PROCESSED|i, iv|【只读】队列中已经被处理的buffer数|
|AL_SEC_OFFSET|f, fv, i, iv|播放的位置，秒|
|AL_SAMPLE_OFFSET|f, fv, i, iv|播放的位置，Sample数|
|AL_BYTE_OFFSET|f, fv, i, iv|播放的位置，字节数|
示例：
**[cpp]**[view plain](http://blog.csdn.net/woker/article/details/8687380)[copy](http://blog.csdn.net/woker/article/details/8687380)
- alGetError(); // clear error state
- alSourcef(source[0],AL_PITCH,1.0f);  
- if ((error = alGetError()) != AL_NO_ERROR)  
-     DisplayALError("alSourcef 0 AL_PITCH : \n", error);  
- 
- alGetError(); // clear error state
- alSourcef(source[0],AL_GAIN,1.0f);  
- if ((error = alGetError()) != AL_NO_ERROR)  
-     DisplayALError("alSourcef 0 AL_GAIN : \n", error);  
- 
- alGetError(); // clear error state
- alSourcefv(source[0],AL_POSITION,source0Pos);  
- if ((error = alGetError()) != AL_NO_ERROR)  
-     DisplayALError("alSourcefv 0 AL_POSITION : \n", error);  
- 
- alGetError(); // clear error state
- alSourcefv(source[0],AL_VELOCITY,source0Vel);  
- if ((error = alGetError()) != AL_NO_ERROR)  
-     DisplayALError("alSourcefv 0 AL_VELOCITY : \n", error);  
- 
- alGetError(); // clear error state
- alSourcei(source[0],AL_LOOPING,AL_FALSE);  
- if ((error = alGetError()) != AL_NO_ERROR)  
-     DisplayALError("alSourcei 0 AL_LOOPING true: \n", error);  
6.*buffer**队列**(bufferqueuing)*
buffer队列用以保证声音的连续播放。使用buffer队列时，buffer和source都是普通方法创建，但是附加buffer到source时使用alSourceQueueBuffers和alSourceUnqueueBuffers来替代alSourcei。alSourceQueueBuffers可以附加一个或一组buffer到一个source上，然后调用alSourcePlay来播放。播放source时，可以调用alSourceUnqueueBuffers来移除已经播放了的buffer。这些buffer可以重填新的数据或者丢弃。新的或重填的buffer可以通过alSourceQueueBuffers附加到原来正在播放的source上。只要buffer队列中一直有新的buffer用以播放，source就不会停。
1)用于流(streaming)的source不应该用alSourcei来附加其第一个buffer，始终用alSourceQueueeBuffers。任何source都可以用alSourcei(...,AL_BUFFER,0)来卸载其全部buffer，这些buffer可以接着用作流buffer或非流buffer，取决于附加到source的方式(alSourcei或alSourceQueueBuffers)。
2)所有通过alSourceQueueBuffers附加到某个source的buffer，都应该有相同的声音格式。
7.*多普勒**偏移**(DopplerShift)*
多普勒效应取决于source和listener相对于介质的速度，以及介质中声音的传播速度。应用程序可能希望强化或弱化多普勒效应，因为精确计算可能达不到期望的声音效果。频率偏移(音调改变)的量正比于listener和source接近或远离的速度。OpenAL中多普勒效应的实现是通过下述公式，忽略介质(空气、水等)移动的效应。
SS: AL_SPEED_OF_SOUND = 声音传播速度，默认值343.3
DF: AL_DOPPLER_FACTOR = 多普勒系数，默认为1.0
vls: listener速度标量（在source-listener直线上投影值，向内为正）
vss: source速度标量（在source-listener直线上投影值，向内为正）
f: 采样频率
F : 多普勒频率偏移量
Mag(vec) = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z)
DotProduct(v1, v2) = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z)
SL = source指向listener的向量
SV = source速度向量
LV = listener速度向量
vls = DotProduct( SL, LV ) / Mag(SL)
vss = DotProduct( SL, SV ) / Mag(SL)
多普勒计算：
vss = min( vss, SS/DF )
vls = min (vls, SS/DF )
F = f * ( SS - DF * vls ) / ( SS - DF * vss )
有两个API用以控制声音传播速度和多普勒系数。AL_DOPPLER_FACTOR是source和listener速度的一个系数，用以放大或减小多普勒偏移量。
        void alDopplerFacor ( ALfloat dopplerFactor );
负数会导致AL_INVALID_VALUE错误，命令失效。默认值为1。通过alGetFloat{v}和AL_DOPPLER_FACTOR可以获取当前配置。
AL_SPEED_OF_SOUND允许应用程序改变传播速度，source、listener以及声音传播的速度三者应该使用相同的速度单位。
        void alSpeedOfSound ( Alfloat speed );
负数会导致AL_INVALID_VALUE错误，命令失效。默认值为343.3（声音在空气中的传播速度）。通过alGetFloat{v}和AL_SPEED_OF_SOUND获取当前值。距离与速度的单位完全独立，可以使用不同的单位。如果不想有多普勒效应，可以将所有速度设置成0。
8.*错误处理*
alGetError可在任何时刻获取OpenAL的错误状态，同时清除错误状态，所以通常在重要操作前后都调用。错误码如下：
|错误码|描述|
|----|----|
|AL_NO_ERROR|没有错误|
|AL_INVALID_NAME|传入错误的名字(ID)|
|AL_INVALID_ENUM|传入无效的枚举值|
|AL_INVALID_VALUE|传入无效的值|
|AL_INVALID_OPERATION|无效的请求|
|AL_OUT_OF_MEMORY|请求操作导致OpenAL内存溢出|
示例：
**[cpp]**[view plain](http://blog.csdn.net/woker/article/details/8687380)[copy](http://blog.csdn.net/woker/article/details/8687380)
- alGetError(); // Clear Error Code
- 
- // Generate Buffers
- alGenBuffers(NUM_BUFFERS, g_Buffers);  
- 
- if ((error = alGetError()) != AL_NO_ERROR)  
- {  
-     DisplayALError("alGenBuffers :", error);  
-     exit(-1);  
- }  
9.*扩展包*(*Extensions*)
OpenAL的extension机制可以让开发商增加新特性的API。创新科技(CreativeLabs)已经增加了多个extension，包括EAX,X-RAM, Multi-Channel Buffer playback，EffectExtension(EFX)。应用程序可以根据extension的种类调用alIsExtensionPresent或者alcIsExtensionPresent来判断某个extension是否有效。

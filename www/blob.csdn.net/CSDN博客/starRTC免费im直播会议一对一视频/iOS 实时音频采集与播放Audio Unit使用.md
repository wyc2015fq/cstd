# iOS 实时音频采集与播放Audio Unit使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月29日 16:12:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：596
前言
在iOS中有很多方法可以进行音视频采集。如 AVCaptureDevice, AudioQueue以及Audio Unit。其中 Audio Unit是最底层的接口，它的优点是功能强大，延迟低; 而缺点是学习成本高，难度大。
对于一般的iOS应用程序，AVCaptureDevice和AudioQueue完全够用了。但对于音视频直播，最好还是使用 Audio Unit 进行处理，这样可以达到最佳的效果，著名的 WebRTC 就使用的 Audio Unit 做的音频采集与播放。今天我们就重点介绍一下Audio Unit的基本知识和使用。
Audio Unit在 iOS架构中所处的位置：
![](http://note.youdao.com/yws/res/103110/C1D72913CC68473F873227F9AFDD2215)
基本概念
- Audio Unit的种类
共可分为四大类，並可细分为七种：
![](http://note.youdao.com/yws/res/103111/F501ACB5303340A6A052007D02023516)
- Audo Unit 的内部结构
Audio Unit 内部结构分为两大部分，Scope 与Element。其中 scope 又分三种，分別是 input scope, output scope, global scope。而 element 则是 input scope 或 output scope 內的一部分。
![](http://note.youdao.com/yws/res/103109/3919E224F4A449719327532A6A71F661)
- Audio Unit 的输入与输出
下图是一个 I/O type 的 Audio Unit，其输入为麦克风，其输出为喇叭。这是一个最简单的Audio Unit使用范例。
![](http://note.youdao.com/yws/res/103112/78DE8244D07741A99F7FFA8D3C6BB8D2)
ioUnit.png
The input element is element 1 (mnemonic device: the letter “I” of the word “Input” has an appearance similar to the number 1)
The output element is element 0 (mnemonic device: the letter “O” of the word “Output” has an appearance similar to the number 0)
使用流程概要
- 描述音频元件（kAudioUnitType_Output／kAudioUnitSubType_RemoteIO ／kAudioUnitManufacturerApple）
- 使用 AudioComponentFindNext(NULL, &descriptionOfAudioComponent) 获得 AudioComponent。AudioComponent有点像生产 Audio Unit 的工厂。
- 使用 AudioComponentInstanceNew(ourComponent, &audioUnit) 获得 Audio Unit 实例。
- 使用 AudioUnitSetProperty函数为录制和回放开启IO。
- 使用 AudioStreamBasicDescription 结构体描述音频格式，并使用AudioUnitSetProperty进行设置。
- 使用 AudioUnitSetProperty 设置音频录制与放播的回调函数。
- 分配缓冲区。
- 初始化 Audio Unit。
- 启动 Audio Unit。
初始化
初始化看起来像下面这样。我们有一个 AudioComponentInstance 类型的成员变量，它用于存储 Audio Unit。
下面的音频格式用16位表式一个采样。
#define kOutputBus 0#define kInputBus 1// ... OSStatus status;AudioComponentInstance audioUnit; // 描述音频元件AudioComponentDescription desc;desc.componentType = kAudioUnitType_Output;desc.componentSubType = kAudioUnitSubType_RemoteIO;desc.componentFlags = 0;desc.componentFlagsMask = 0;desc.componentManufacturer = kAudioUnitManufacturer_Apple; // 获得一个元件AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc); // 获得 Audio Unitstatus = AudioComponentInstanceNew(inputComponent, &audioUnit);checkStatus(status); // 为录制打开 IOUInt32 flag = 1;status = AudioUnitSetProperty(audioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, kInputBus, &flag, sizeof(flag));checkStatus(status); // 为播放打开 IOstatus = AudioUnitSetProperty(audioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, kOutputBus, &flag, sizeof(flag));checkStatus(status); // 描述格式audioFormat.mSampleRate = 44100.00;audioFormat.mFormatID = kAudioFormatLinearPCM;audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;audioFormat.mFramesPerPacket = 1;audioFormat.mChannelsPerFrame = 1;audioFormat.mBitsPerChannel = 16;audioFormat.mBytesPerPacket = 2;audioFormat.mBytesPerFrame = 2; // 设置格式status = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, kInputBus, &audioFormat, sizeof(audioFormat));checkStatus(status);status = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, kOutputBus, &audioFormat, sizeof(audioFormat));checkStatus(status); // 设置数据采集回调函数AURenderCallbackStruct callbackStruct;callbackStruct.inputProc = recordingCallback;callbackStruct.inputProcRefCon = self;status = AudioUnitSetProperty(audioUnit, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, kInputBus, &callbackStruct, sizeof(callbackStruct));checkStatus(status); // 设置声音输出回调函数。当speaker需要数据时就会调用回调函数去获取数据。它是 "拉" 数据的概念。callbackStruct.inputProc = playbackCallback;callbackStruct.inputProcRefCon = self;status = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Global, kOutputBus, &callbackStruct, sizeof(callbackStruct));checkStatus(status); // 关闭为录制分配的缓冲区（我们想使用我们自己分配的）flag = 0;status = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_ShouldAllocateBuffer, kAudioUnitScope_Output, kInputBus, &flag, sizeof(flag)); // 初始化status = AudioUnitInitialize(audioUnit);checkStatus(status);
开启 Audio Unit
OSStatus status = AudioOutputUnitStart(audioUnit);checkStatus(status);
关闭 Audio Unit
OSStatus status = AudioOutputUnitStop(audioUnit);checkStatus(status);
结束 Audio Unit
AudioComponentInstanceDispose(audioUnit);
录制回调
static OSStatus recordingCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) { // TODO:// 使用 inNumberFrames 计算有多少数据是有效的// 在 AudioBufferList 里存放着更多的有效空间 AudioBufferList *bufferList; //bufferList里存放着一堆 buffers, buffers的长度是动态的。 // 获得录制的采样数据 OSStatus status; status = AudioUnitRender([audioInterface audioUnit], ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, bufferList); checkStatus(status); // 现在，我们想要的采样数据已经在bufferList中的buffers中了。 DoStuffWithTheRecordedAudio(bufferList); return noErr;}
播放回调
static OSStatus playbackCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) { // Notes: ioData 包括了一堆 buffers // 尽可能多的向ioData中填充数据，记得设置每个buffer的大小要与buffer匹配好。return noErr;}
结束
Audio Unit可以做很多非常棒的的工作。如混音，音频特效，录制等等。它处于 iOS 开发架构的底层，特别合适于音视频直播这种场景中使用。
“知识无穷尽，只取我所需”。
ios音视频演示app：[https://github.com/starrtc/ios-demo](https://github.com/starrtc/ios-demo)

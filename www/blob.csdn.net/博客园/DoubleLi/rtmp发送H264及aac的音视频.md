# rtmp发送H264及aac的音视频 - DoubleLi - 博客园






RTMP推送的音视频流的封装形式和FLV格式相似，由此可知，向FMS推送H264和AAC直播流，需要首先发送"AVC sequence header"和"AAC sequence header"，这两项数据包含的是重要的编码信息，没有它们，解码器将无法解码。

　　AVC sequence header就是AVCDecoderConfigurationRecord结构，该结构在标准文档“ISO-14496-15 AVC file format”中有详细说明。

![](https://pic002.cnblogs.com/images/2011/254714/2011122913292955.jpg)

　　AAC sequence header存放的是AudioSpecificConfig结构，该结构则在“ISO-14496-3 Audio”中描述。AudioSpecificConfig结构的描述非常复杂，这里我做一下简化，事先设定要将要编码的音频格式，其中，选择"AAC-LC"为音频编码，音频采样率为44100，于是AudioSpecificConfig简化为下表：

![](https://pic002.cnblogs.com/images/2011/254714/2011122913505721.jpg)

　　这样，AVC sequence header和AAC sequence header的内容可以基本确定了，更详细的信息，大家可以去翻阅相关文档。

在发送这两个header需要在前面分别加上 VideoTags、AudioTags  这连个tags都是1个字节（8bits）的数据

其中AudioTags每bit表示的意义如下图：

![](https://blog.csdn.net/lcjatf/article/details/18001351)

![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://img-blog.csdn.net/20140108152351078)

其中SoundData 的组成如下：

![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://img-blog.csdn.net/20140108152452187)

当数据的第一个字节为0时，后面跟AAC sequence header；

当数据的第一个字节为1时，后面跟AAC 数据；

其中VideoTags每bit表示的意义如下图：

![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://img-blog.csdn.net/20140108152428781)

发送的为avc数据，所以，CodecID（后4bit）的值为7

所以videodata的数据打包方式为 ![](https://blog.csdn.net/lcjatf/article/details/18001351)，具体的信息见下图：

![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://blog.csdn.net/lcjatf/article/details/18001351)![](https://img-blog.csdn.net/20140108152518031)

具体代码实现：

//添加Flv的VideoTags
char* RtmpLiveEncoder::AddVideoTags(char* buf,bool isKeyframe)
{
//前面4字节表示FrameType，后面4字节表示CodecID
unsigned char flag = 0;
if (isKeyframe)
flag = 0x17;
else
flag = 0x27;


buf = UI08ToBytes(buf, flag);
buf = UI08ToBytes(buf, 1);    // avc packet type (0, nalu)  包的类型，同步包为0
buf = UI24ToBytes(buf, 0);    // composition time  0为打开，1为关闭


return buf;
}
//添加Flv的AudioTags
char* RtmpLiveEncoder::AddAudioTags(char* buf)
{
//
unsigned char flag = 0;
flag = (10 << 4) |  // soundformat "10 == AAC"
(3 << 2) |      // soundrate   "3  == 44-kHZ"
(1 << 1) |      // soundsize   "1  == 16bit"
1;              // soundtype   "1  == Stereo"


buf = UI08ToBytes(buf, flag); 
buf = UI08ToBytes(buf, 1);    // aac packet type (1, raw)  包的类型，同步包为0


return buf;
}

这两个函数返回的buf后面接需要发送的数据



from:https://blog.csdn.net/lcjatf/article/details/18001351










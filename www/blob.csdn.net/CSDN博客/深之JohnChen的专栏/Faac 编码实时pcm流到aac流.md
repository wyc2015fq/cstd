# Faac 编码实时pcm流到aac流 - 深之JohnChen的专栏 - CSDN博客

2017年01月03日 16:29:52[byxdaz](https://me.csdn.net/byxdaz)阅读数：1637


                
编码流程：

首先调用faacEncHandle hEncoder=faacEncOpen(samplerate,channels,& samplesInput,

&maxBytesOutput);

**1.**打开aac编码引擎，创建aac编码句柄。

参数 samplerate 为要编码的音频pcm流的采样率，channels为要编码的音频pcm流的的频道数（原有的例子程序是从wav文件中读出这些信息），sampleInput在编码时要用到，意思是每次要编码的采样数，参数maxBytesOutput为编码时输出地最大字节数。

通过 sampleInput、通道数、采样率多占位宽可以知道采集PCM数据多少数据打成一帧，sampleInput*channels*wBitsPerSample/8字节打包成一帧，其中wBitsPerSample表示每个采样率占用的位宽（一般为16）。

**2.**然后在设置一些编码参数，如

int version=MPEG4;    //设置版本,录制MP4文件时要用MPEG4

int objecttype=LOW;    //编码类型

int midside=1;          //M/S编码

int usetns=DEFAULT_TNS;   //瞬时噪声定形(temporal noise shaping，TNS)滤波器

int shortctl=SHORTCTL_NORMAL;

int inputformat=FAAC_INPUT_16BIT;  //输入数据类型

int outputformat=0; //录制MP4文件时，要用raw流。检验编码是否正确时可设置为adts传输流，把aac 流写入.aac文件中，如编码正确，用千千静听就可以播放。

//output format (0 = Raw; 1 = ADTS)，如果为1，acc流中会自动加上ADTS头。

其他的参数可根据例子程序设置。

设置完参数后就调用faacEncSetConfiguration(hEncoder, aacFormat)设置编码参数。

**3.**如编码完的aac流要写入MP4文件时，要调用

faacEncGetDecoderSpecificInfo(hEncoder,&(ASC), &(ASCLength));//得到解码信息

                                                                                                                         //（mpeg4ip mp4 录制使用）

此函数支持MPEG4版本，得到的ASC 和ACSLength 数据在录制MP4(mpegip库)文件时用。

**4.**然后就是编码了，每次从实时的pcm音频队列中读出samplesInput* channels*（量化位数/8），

字节数的pcm数据。然后再把得到pcm流转变一下存储位数，我是转化为16位的了，这部分

可以根据例子程序写一个函数，这是我写的一个，

size_t read_int16(AACInfo *sndf, int16_t *outbuf, size_t num, unsigned char *inputbuf)

{

       size_t i=0,j=0;

       unsigned char bufi[8];

       while(i<num)

       {

                memcpy(bufi,inputbuf+j,sndf->samplebytes);

               j+=sndf->samplebytes;

              int16_t s=((int16_t*)bufi)[0];

             outbuf[i]=s;

             i++;

       }

       return i;

}

也可以写一个read_float32(AACInfo *sndf, float *outbuf, size_t num ,unsigned char *inputbuf)，

和size_t read_int24(AACInfo *sndf, int32_t *outbuf, size_t num, unsigned char *inputbuf)。

处理完数据转换后就调用

bytesWritten = faacEncEncode(hEncoder,

                                                        (int *)pcmbuf,

                                                        samplesInput,

                                                        outbuff,

                                                        maxbytesoutput);

进行编码，pcmbuf为转换后的pcm流数据，samplesInput为调用faacEncOpen时得到的输入采样数，outbuff为编码后的数据buff，maxbytesoutput为调用faacEncOpen时得到的最大输出字节数。然后每次从outbuff中得到编码后的aac数据流，放到数据队列就行了，如果还要录制MP4文件，在编码完samplesInput（一帧）个采样数时，打上时间戳（mpegip库用于音视频同步）后再放到输出队列中。如果想测试看编码的aac流是否正确，设置输出格式为ADTS_STREAM，把aac数据写入到.aac文件中，看能否用千千静听播放。

**5.**释放资源，调用faacEncClose(hEncoder);就行了

FAAC和FAAD的下载地址  [http://www.audiocoding.com/downloads.html](http://www.audiocoding.com/downloads.html)



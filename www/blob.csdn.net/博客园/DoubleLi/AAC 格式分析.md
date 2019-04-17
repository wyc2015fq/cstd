# AAC 格式分析 - DoubleLi - 博客园






**一SADTS格式：**

       ADTS的全称是Audio Data Transport Stream。是AAC音频的传输流格式。

       AAC音频格式在MPEG-2（ISO-13318-7 2003）中有定义。AAC后来又被采用到MPEG-4标准中。

       1. adts_sequence()

       {

              while (nextbits() == syncword)

              {

                     adts_frame();

              }

       }



       2. adts_frame()

       {

              adts_fixed_header();

              adts_variable_header();

              if (number_of_raw_data_blocks_in_frame == 0)

              {

                     adts_error_check();

                     raw_data_block();

              } else

              {

                     adts_header_error_check();

                     for (i = 0; i <= number_of_raw_data_blocks_in_frame; i++)

                     {

                             raw_data_block();

                             adts_raw_data_block_error_check();

                     }

              }

       }



       3. adts_fixed_header()

       {

              syncword: 12 bslbf

              ID: 1 bslbf

              layer: 2 uimsbf

              protection_absent: 1 bslbf

              profile: 2 uimsbf

              sampling_frequency_index: 4 uimsbf

              private_bit: 1 bslbf

              channel_configuration: 3 uimsbf

              original/copy: 1 bslbf

              home: 1 bslbf

       }



       adts_variable_header()

       {

              copyright_identification_bit: 1 bslbf

              copyright_identification_start: 1 bslbf

              frame_length: 13 bslbf

              adts_buffer_fullness: 11 bslbf

              number_of_raw_data_blocks_in_frame: 2 uimsfb

       }



       详细说明下ADTS头的重要数据部分：

       syncword 同步字The bit string ‘1111 1111 1111’，说明一个ADTS帧的开始。

       ID    MPEG 标示符, 设置为1.

       layer Indicates which layer is used. Set to ‘00’

       protection_absent 表示是否误码校验

       profile 表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AACLC

       sampling_frequency_index 表示使用的采样率下标

       sampling_frequency_index sampling frequeny [Hz]

       0x0       96000

       0x1           88200

       0x2          64000

       0x3          48000

       0x4          44100

       0x5          32000

       0x6            24000

       0x7          22050

       0x8          16000

       0x9          2000

       0xa          11025

       0xb          8000

       0xc          reserved

       0xd          reserved

       0xe          reserved

       0xf           reserved

       channel_configuration 表示声道数

       frame_length 一个ADTS帧的长度包括ADTS头和raw data block.

       adts_buffer_fullness  0x7FF 说明是码率可变的码流

       number_of_raw_data_blocks_in_frame

      表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.

       所以说number_of_raw_data_blocks_in_frame == 0 表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)

**二 封装AAC为ADTS帧 **

       一个AAC原始数据块长度是可变的，对原始帧加上ADTS头进行ADTS 的封装，就形成了ADTS帧。通常我们将得到的AAC原始帧进行封装后写入文件，用常用的播放器如千千静听即可播放，这是个验证AAC数据是否正确的方法。   

       进行封装前，需要了解相关参数，如采样率，声道数，原始数据块的长度等。下面把AAC原始数据帧加工为ADTS帧，据相关参数填写组成7字节的ADTS头。

       The ADTS header is defined below -    

       unsigned int obj_type = 0;    

       unsigned int num_data_block = frame_length / 1024; 

       // include the header length also     

       frame_length += 7;   

       /* We want the same metadata */    

       /* Generate ADTS header */    

       if(adts_header == NULL) return;    

       /* Sync point over a full byte */    

       adts_header[0] = 0xFF;    

       /* Sync point continued over first 4 bits + static 4 bits    

      * (ID, layer, protection)*/    

       adts_header[1] = 0xF9;    

       /* Object type over first 2 bits */    

       adts_header[2] = obj_type << 6;//    

       /* rate index over next 4 bits */    

       adts_header[2] |= (rate_idx << 2);    

       /* channels over last 2 bits */    

       adts_header[2] |= (channels & 0x4) >> 2;   

       /* channels continued over next 2 bits + 4 bits at zero */    

       adts_header[3] = (channels & 0x3) << 6;    

       /* frame size over last 2 bits */    

       adts_header[3] |= (frame_length & 0x1800) >> 11;    

       /* frame size continued over full byte */    

       adts_header[4] = (frame_length & 0x1FF8) >> 3;    

       /* frame size continued first 3 bits */    

       adts_header[5] = (frame_length & 0x7) << 5;    

       /* buffer fullness (0x7FF for VBR) over 5 last bits*/    

       adts_header[5] |= 0x1F;    

       /* buffer fullness (0x7FF for VBR) continued over 6 first bits + 2 zeros     

       * number of raw data blocks */

       adts_header[6] = 0xFC;// one raw data blocks .

       adts_header[6] |= num_data_block & 0x03; //Set raw Data blocks.



       在CMMB中，采用AAC音频压缩标准，默认状况下，编码参数如下：双声道，采样率24KHZ，帧长变长，码流可变码率的码流，一般采用的AAC profile为



AAC-LC。将从CMMB复用帧解析的一个AAC原始帧封装为ADTS帧的方法如下：

uint8 aac_buf[ADTS_FRAME_SIZE]={0x0ff,0x0f9,0x058,0x80,0,0x1f,0xfc};

       从上述7个字节分析音频参数如下:

       synword--0xfff

       ID:0x1--- 1--- MPEG2 identifier,

       LAYER--00

       protection_absent ---01

       profile--01   1 Low Complexity profile (LC) AAC-LC

       smaping_freuency_index---0110-->0x06--->采样率24KHZ 

       channel_configuration --- aac_buf[3] = 0x08---->2---->双声道。。

       adts_buffer_fullness--->0x7ff 码率可变的码流 

       现插入长度参数 wDataLen;

       void OnAudioAacFrame(byte* data, uint16 wDataLen)

       {

              unsigned int num_data_block = wDataLen / 1024; 

              uint16 frame_Length; 

              frame_Length = wDataLen + 7;     

               /* frame size over last 2 bits */         

              aac_buf[3] |= (frame_length & 0x1800) >> 11;// the upper 2 bit         

              /* frame size continued over full byte */         

              aac_buf[4] = (frame_length & 0x1FF8) >> 3;// the middle 8 bit        

              /* frame size continued first 3 bits */         

              aac_buf[5] |= (frame_length & 0x7) << 5;//the last 3 bit 

              aac_bug[6] |= num_data_block & 0x03; //Set raw Data blocks.

              emcpy(&aac_buf[7],data,wDataLen);

              //形成一个ADTS帧写入文件。

              fwrite(aac_buf,wDataLen+7,sizeof(byte),f_audio);

       }

**三 LATM格**

       LATM 的全称为“Low-overhead MPEG-4 Audio TransportMultiplex”（低开销音频传输复用），是MPEG-4 AAC制定的一种高效率的码流传输方式，MPEG-2 TS 流也采用LATM 作为AAC 音频码流的封装格式之 LATM格式也以帧为单位，主要由AudioSpecificConfig（音频特定配置单元）与音频负载组成。

AudioSpecificConfig 描述了一个LATM 帧的信息，音频负载主要由PayloadLengthInfo（负载长度信息）和PayloadMux（负载净荷）组成。

       AudioSpecificConfig 信息可以是带内传，也可以是带外传。所谓带内传，就是指每一个LATM 帧，都含有一个AudioSpecificConfig 信息；而带外传，

则每一个LATM帧都不含有AudioSpecificConfig 信息，而通过其他方式把AudioSpecificConfig信息发送到解码端，由于AudioSpecificConfig 信息一般是不变的，所以只需发送一次即可。由此可见，AudioSpecificConfig 信息采用带内传输可适应音频编码信息不断变化的情况，而采用带外传输，可以节省音频传输码率。带内或带外传，由muxconfigPresent 标志位决定。例如流媒体应用中，muxconfigPresent 可设置为0，这样LATM帧中将不含有AudioSpecificConfig 信息，LATM帧通过RTP包发送出去，AudioSpecificConfig 可通过SDP文件一次性传送到解码端。

       AudioSpecificConfig 主要参数

       numSubFrames  子帧的数目

       numProgram    复用的节目数

       numLayer      复用的层数

       frameLengthType  负载的帧长度类型，包括固定长度与可变长度

       audioObjectType   音频对象类型

       samplingFrequency   采样率

       channelConfiguration   声道配置

       音频负载由若干子帧组成，每个子帧由PayloadLengthInfo和PayloadMux组成，与ADTS帧净荷一样，音频负载主要包含原始帧数据。 

       AAC打包成TS流通常有两种方式，分别是先打包成ADTS或LATM。ADTS的每一帧都有个帧头，在每个帧头信息都一样的状况下，会有很大的冗余。LATM格式具有很大的灵活性，每帧的音频配置单元既可以带内传输，有可以带外传输。正因为如此，LATM不仅适用于流传输还可以用于RTP传输，RTP传输时，若音频数据配置信息是保持不变，可以先通过SDP会话先传输StreamMuxConfig（AudioSpecificConfig）信息，由于LATM流由一个包含了一个或多个音频帧的audioMuxElements序列组成。一个完整或部分完整的audioMuxElement可直接映射到一个RTP负载上。 

       下面是一个audoMuxEmlemt

       AudioMuxElement(muxConfigPresent)

       { 

              if (muxConfigPresent)  

              { 

                     useSameStreamMux;    

                     if (!useSameStreamMux)       

                            StreamMuxConfig();

             }



              if (audioMuxVersionA == 0) 

              { 

                     for (i = 0; i <= numSubFrames; i++)    

                     {

                             PayloadLengthInfo();       

                            PayloadMux();   

                    }.  

              }

       }



       可以很简单的把ADTS帧转换为LATM帧，根据ADTS头的信息，生成StreamMuxConfig，将ADTS中的原始帧提取出来，前面加上PayloadLengthInfo做为LATM的音频帧。按照上述格式打包生成AudioMuxElement，作为RTP的负载传输.



**四、 CMMB中的LATM **



       当CMMB中音频压缩标准为AAC时，默认采用LATM封装。StreamMuxConfig采用带外传输。StreamMuxConifg中的若干默认参数如下：

       audioMuxVersion:0标志流语法版本号为0

       allStreamSameTiemFraming：1，标志复用到PayLoadMux()中的所有负载共享一个共同的时基

       umSubFrames:0  表示只有一个音频子帧. 

       audioObjectType:2 AAC-LC freameLengthType:0 帧长度是可变的

       latmBufferFullness:0xFF 码率可变的码流



参考：

[1]ISO/IEC 13818-7(2003 MPEG-2 AAC, Second Edition)

[2] ISO13818-7（2006 Fourth edition AAC）

[3] RFC 3016 (rfc3016) - RTP Payload Format for MPEG-4 Audio-Visual Streams

[4] AAC音频压缩编码标准的ADTS与LATM格式分析

[5] GYZ 234-2008： CMMB复用实施指南










# AAC中ADTS创建与解析 - 深之JohnChen的专栏 - CSDN博客

2017年10月15日 11:32:29[byxdaz](https://me.csdn.net/byxdaz)阅读数：1269


一般的AAC解码器都需要把AAC的ES流（AAC裸数据）打包成ADTS+ES的格式（ADTS头+ES裸数据），一般是在AAC ES流前添加7个字节的ADTS header。

在AAC编码过程中，如果对原始采样声音进行AAC编码之后的数据是AAC裸数据（不包含ADTS头），需要增加ADTS头，形成ADTS+ES完成可播放的数据。在AAC解码过程中，如果对AAC数据进行解码，需要通过ADTS数据解析，才知道AAC裸数据的大小，将数据传入libfaad2库的NeAACDecDecode函数进行解码成原始PCM数据。
AAC编码一般使用libfaac库，函数faacEncSetConfiguration设置编码参数，faacEncConfigurationPtr结构体中outputFormat属性为0时编码之后是AAC裸数据，为1时编码之后是ADTS头+AAC裸数据。

AAC解码一般使用libfaad2库。
在AAC编码过程中，会涉及到ADTS的解析与生成，下面列出相关函数。

创建ADTS

/**

     *  Add ADTS header at the beginning of each and every AAC packet.

     *  This is needed as MediaCodec encoder generates a packet of raw

     *  AAC data.

     *

     *  Note the packetLen must count in the ADTS header itself !!! .

     *注意，这里的packetLen参数为raw aac Packet Len + 7; 7 bytes adts header

     **/

    private void addADTStoPacket(byte[] packet, int packetLen) {

        int profile = 2;  //AAC LC，MediaCodecInfo.CodecProfileLevel.AACObjectLC;

        int freqIdx = 5;  //32K, 见后面注释avpriv_mpeg4audio_sample_rates中32000对应的数组下标，来自ffmpeg源码

        int chanCfg = 2;  //见后面注释channel_configuration，Stero双声道立体声

        /*int avpriv_mpeg4audio_sample_rates[] = {

            96000, 88200, 64000, 48000, 44100, 32000,

                    24000, 22050, 16000, 12000, 11025, 8000, 7350

        };

        channel_configuration: 表示声道数chanCfg

        0: Defined in AOT Specifc Config

        1: 1 channel: front-center

        2: 2 channels: front-left, front-right

        3: 3 channels: front-center, front-left, front-right

        4: 4 channels: front-center, front-left, front-right, back-center

        5: 5 channels: front-center, front-left, front-right, back-left, back-right

        6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel

        7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel

        8-15: Reserved

        */

        // fill in ADTS data

        packet[0] = (byte)0xFF;

        packet[1] = (byte)0xF9;

        packet[2] = (byte)(((profile-1)<<6) + (freqIdx<<2) +(chanCfg>>2));

        packet[3] = (byte)(((chanCfg&3)<<6) + (packetLen>>11));

        packet[4] = (byte)((packetLen&0x7FF) >> 3);

        packet[5] = (byte)(((packetLen&7)<<5) + 0x1F);

        packet[6] = (byte)0xFC;

    }

参考文章

http://stackoverflow.com/questions/18862715/how-to-generate-the-aac-adts-elementary-stream-with-android-mediacodec

解析ADTS

/*

功能：解析ADTS

参数：buffer,ADTS+aac源数据 ；buf_size, aac源数据长度 + 7个字节ADTS头

*/

int get_one_ADTS_frame(unsigned char* buffer, size_t buf_size, unsigned char* data ,size_t* data_size)

{

    size_t size = 0;

    if(!buffer || !data || !data_size )

    {

        return -1;

    }

    while(1)

    {

        if(buf_size  < 7 )

        {

            return -1;

        }

if ((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0))
{
// profile; 2 uimsbf
// sampling_frequency_index; 4 uimsbf
// private_bit; 1 bslbf
// channel_configuration; 3 uimsbf
// original/copy; 1 bslbf
// home; 1 bslbf
// copyright_identification_bit; 1 bslbf
// copyright_identification_start; 1 bslbf
// frame_length; 13 bslbf

size |= (((buffer[3] & 0x03)) << 11);//high 2 bit
size |= (buffer[4] << 3);//middle 8 bit
size |= ((buffer[5] & 0xe0) >> 5);//low 3bit

//printf("len1=%x\n", (buffer[3] & 0x03));
//printf("len2=%x\n", buffer[4]);
//printf("len3=%x\n", (buffer[5] & 0xe0) >> 5);
//printf("size=%d\r\n", (int)size);
break;
}

        --buf_size;

        ++buffer;

    }

    if(buf_size < size)

    {

        return -1;

    }

    memcpy(data, buffer, size);

    *data_size = size;

    return 0;

}


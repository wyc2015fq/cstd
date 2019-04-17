# h264 aac 封装 flv - DoubleLi - 博客园






# Part 1flvtag组成

FLV 文件结构由 FLVheader和FLVBody组成。（注意flv文件是大端格式的）
FLV头组成（以c为例子，一字节对齐）：
FLVBody是由若干个Tag组成的；
     Tag=Tag头(11字节)+数据



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- typedef struct _FLV_HEADER  
- {  
- char FLV[3];//={0x46,0x4c,0x56};  
- char Ver;   //版本号  
- char StreamInfo;// 有视频又有音频就是0x01 | 0x04（0x05）  
- int HeaderLen; /*****头的长度*****/  
- } FLV_HEADER;  


# Part2 h264


H264是一个个NALU单元组成的，每个单元以00 00 01 或者 00 00 00 01分隔开来，每2个00 00 00 01之间就是一个NALU单元。我们实际上就是将一个个NALU单元封装进FLV文件。
每个NALU单元开头第一个byte的低5bits表示着该单元的类型，即NAL nal_unit_type：



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- #define NALU_TYPE_SLICE 1  
- #define NALU_TYPE_DPA 2  
- #define NALU_TYPE_DPB 3  
- #define NALU_TYPE_DPC 4  
- #define NALU_TYPE_IDR 5    /**关键帧***/  
- #define NALU_TYPE_SEI 6    /*****曾强帧******/       
- #define NALU_TYPE_SPS 7  
- #define NALU_TYPE_PPS 8  
- #define NALU_TYPE_AUD 9  
- #define NALU_TYPE_EOSEQ 10  
- #define NALU_TYPE_EOSTREAM 11  
- #define NALU_TYPE_FILL 12  




每个NALU第一个byte & 0x1f 就可以得出它的类型，比如上图第一个NALU：67 & 0x1f = 7，则此单元是SPS，第三个：68 & 0x1f = 8，则此单元是PPS。

# Part3 h264 封装flv


 我们现在开始把H264，AAC封装为FLV文件。
 首先定义一个函数(功能反向拷贝)：



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- void ReverseMemcpy(void* dest,size_t destLen, const void* src, size_t n)  
- {  
- char*     d= (char*) dest;  
- const char*  s= (const char*)src;  
-         s=s+n-1;  
- while(n--&&destLen--)  
-         {  
-             *d++=*s--;  
-         }  
- return dest;  
- }  




              1.写入FLV头。
              2.写入FLV脚本Tag；
              3.由于分装的是H264，AAC所以所写入一个视频配置信息，和一个音频配置信息
              3.写入视频Tag.由于是H264,Tag的数据就需要按照AVC格式封装。Tag数据区有两种，一种是视频(0x17),一种是音频（0x27）。
                 AVC格式：AVCPacketType(1字节)+CompositionTime（3字节）
                               如果AVCPacketType=0x00,这格式为AVCPacketType(1字节)+CompositionTime（3字节）+AVCDecoderConfigurationRecord。
                               如果AVCPacketType=0x01,这格式为AVCPacketType(1字节)+CompositionTime（3字节）+ 4个bytes的NALU单元长度 + N个bytes的NALU数据。
AVCDecoderConfigurationRecord结构信息    



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- typedef struct _AVC_DEC_CON_REC  
- {  
- char cfgVersion;//configurationVersion  //0x01  
- char avcProfile;//AVCProfileIndication  //sps[1]  
- char profileCompatibility;//profile_Compatibility //sps[2]  
- char avcLevel;//AVCLevelIndication //sps[3]  
- //lengthSizeMinusOne:indicates the length in bytes of the NALUnitLength field in an AVC video  
- char reserved6_lengthSizeMinusOne2;//  
- char reserved3_numOfSPS5;//个数  
- long spsLength;//sequenceParameterSetLength  
- void *sps;  
- char numOfPPS;//个数  
- long ppsLength;  
- void *pps;  
- }AVC_DEC_CON_REC;  
- 
- char *pH264Data=....;//h264数据。  
- int H264DataLen=....;//h264数据长度  
- FLV_TAG_HEADER tagHeader;  
- char AVCPacket[4]={0x00,0x00,0x00,0x00}  
- memset(tagHeader,0,sizeof(FLV_TAG_HEADER));  
- int Index=0;//分隔符长度  
- if(*pH264Data==0x00&&(*pH264Data+1)==0x00&&(*pH264Data+2)==0x01)  
- {  
-      Index=3;  
- }else if(*pH264Data==0x00&&(*pH264Data+1)==0x00&&(*pH264Data+2)==0x00&&(*pH264Data+4)==0x01)  
- {  
-      Index=4;  
- }else{  
-     Err//错误不是h264数据  
- }  
- if(*(pH264Data+Index)&0x1f==0x07)//sps帧，此h264数据还有一帧，pps。  
- {  
- int PreTagLen=.....//前一个Tag长度  
-      ReverseMemcpy(&tagHeader.PreTagLen,4,&PreTagLen,4);//大端字节序；  
-      tagHeader.TagType=0x09;//视频类型  
- //AVCPacket应全为0x00.  
- 
- }  


# part 4.音频AAC封装flv。


    AAC音频格式有ADIF和ADTS：ADIF：Audio Data Interchange Format 音频数据交换格式。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。ADTS：Audio Data Transport Stream 音频数据传输流。这种格式的特征是它是一个有同步字的比特流，解码可以在这个流中任何位置开始。它的特征类似于mp3数据流格式。简单说，ADTS可以在任意帧解码，也就是说它每一帧都有头信息。ADIF只有一个统一的头，所以必须得到所有的数据后解码。且这两种的header的格式也是不同的，目前一般编码后的和抽取出的都是ADTS格式的音频流。语音系统对实时性要求较高，基本是这样一个流程，采集音频数据，本地编码，数据上传，服务器处理，数据下发，本地解码ADTS是帧序列，本身具备流特征，在音频流的传输与处理方面更加合适。
因此我们在音频编码时常选择ADTS的，以下是我们常用的配置




**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



-  m_hAacEncoder= faacEncOpen(capability.nSamplesPerSec,capability.nChannels,  
- &m_nAacInputSamples, &m_nAacMaxOutputBytes);  
-    m_nAacnMaxInputBytes=m_nAacInputSamples*capability.wBitsPerSample/8;  
-    m_pAacConfig = faacEncGetCurrentConfiguration(m_hAacEncoder);//获取配置结构指针  
-    m_pAacConfig->inputFormat = FAAC_INPUT_16BIT;//16精度  
- m_pAacConfig->outputFormat=1; //   设置为 ADTS     
- m_pAacConfig->useTns=true;  
- m_pAacConfig->useLfe=false;  
- m_pAacConfig->aacObjectType=LOW;  
- m_pAacConfig->shortctl=SHORTCTL_NORMAL;  
- m_pAacConfig->quantqual=100;  
- m_pAacConfig->bandWidth=0;  
- m_pAacConfig->bitRate=capability.nAvgBytesPerSec;  




对于flv的aac音频和视频一样需要在第一帧写入配置信息。




**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- flv_packet flvpacket=GetErrPacket();          
- int TagDataLen=1000;  
- char *pTagBuffer=(char *)::malloc(TagDataLen);  
-  memset(pTagBuffer,0,TagDataLen);  
-  KKMEDIA::FLV_TAG_HEADER Tag_Head;  
-  memset(&Tag_Head,0,sizeof(Tag_Head));  
-  FlvMemcpy(&Tag_Head.PreTagLen,4,&m_nPreTagLen,4);  
-  memset(&Tag_Head.Timestamp,0,3);  
-  Tag_Head.TagType=0x08;///音频  
- int datalen=0;  
-  memcpy(pTagBuffer,&Tag_Head,sizeof(KKMEDIA::FLV_TAG_HEADER));  
-  datalen+=sizeof(KKMEDIA::FLV_TAG_HEADER);  
- //前4bits表示音频格式（全部格式请看官方文档）：  
- //1 -- ADPCM  
- //2 -- MP3  
- //4 -- Nellymoser 16-kHz mono  
- //5 -- Nellymoser 8-kHz mono  
- //10 -- AAC  
- //面两个bits表示samplerate：  
- //·0 -- 5.5KHz  
- //·1 -- 11kHz  
- //·2 -- 22kHz  
- //·3 -- 44kHz 1100=0x0C  
- //下面1bit表示采样长度：  
- //·0 -- snd8Bit  
- //·1 -- snd16Bit  
- //下面1bit表示类型：  
- //·0 -- sndMomo  
- //·1 -- sndStereo    
- char TagAudio=0xAF; //1010,11,1,1  
- //TagAudio &=0x0C;//3  
- //TagAudio &=0x02;//1  
- //TagAudio &=0x01;//sndStereo  
-  memcpy(pTagBuffer+datalen,&TagAudio,1);  
-  datalen++;  
- char AACPacketType=0x00;//012->  
-  memcpy(pTagBuffer+datalen,&AACPacketType,1);  
-  datalen++;  
- ///两个字节  
- char AudioSpecificConfig[2]={0x12,0x90};///32000hz  
-  memcpy(pTagBuffer+datalen,&AudioSpecificConfig,2);  
-  datalen+=2;  
-  m_nPreTagLen=datalen-4;///（tag长度值）  
-  TagDataLen=datalen-15;//(11+4(tag长度值+tag的头)  
- //Tag 数据区长度  
-  FlvMemcpy(pTagBuffer+5,3,&TagDataLen,3);  
-  flvpacket.buf =(unsigned char*)pTagBuffer;            
-  flvpacket.bufLen=datalen;  
-  flvpacket.taglen=m_nPreTagLen;  
- return flvpacket;  




关于上面的代码中AudioSpecificConfig的值是怎样计算来的，可以直接中aac编码库中获取，或者采用公式计算出来,请看一下代码。



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- ///索引表  
- static unsigned const samplingFrequencyTable[16] = {  
-   96000, 88200, 64000, 48000,  
-   44100, 32000, 24000, 22050,  
-   16000, 12000, 11025, 8000,  
-   7350,  0,     0,      0  
- };  
- int profile=1;  
- int samplingFrequencyIndex=0;  
- for(int i=0;i<16;i++)  
- {  
- if(samplingFrequencyTable[i]==32000)  
-         {  
-            samplingFrequencyIndex =i;  
- break;  
-         }  
- }  
- char channelConfiguration =0x02;//(立体声)  
- UINT8 audioConfig[2] = {0};    
- UINT8 const audioObjectType = profile + 1;  ///其中profile=1;  
- audioConfig[0] = (audioObjectType<<3) | (samplingFrequencyIndex>>1);    
- audioConfig[1] = (samplingFrequencyIndex<<7) | (channelConfiguration<<3);    
- printf("%02x%02x", audioConfig[0], audioConfig[1]);  




   最后就写入aac帧数据了，请看以下代码：




**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- flv_packet flvpacket=GetErrPacket();  
- int TagDataLen=1000+srcLen;  
- char *pTagBuffer=(char *)::malloc(TagDataLen);  
-             memset(pTagBuffer,0,TagDataLen);  
-             KKMEDIA::FLV_TAG_HEADER Tag_Head;  
-             memset(&Tag_Head,0,sizeof(Tag_Head));  
- //FlvMemcpy等同于ReverseMemcpy  
-             FlvMemcpy(&Tag_Head.PreTagLen,4,&m_nPreTagLen,4);  
-             FlvMemcpy(&Tag_Head.Timestamp,3,&pts,3);  
- 
- 
-             Tag_Head.TagType=0x08;///音频  
- int datalen=0;  
-             memcpy(pTagBuffer,&Tag_Head,sizeof(KKMEDIA::FLV_TAG_HEADER));  
-             datalen+=sizeof(KKMEDIA::FLV_TAG_HEADER);  
- char TagAudio=0xAF;   
-             memcpy(pTagBuffer+datalen,&TagAudio,1);  
-             datalen++;  
- 
- 
- char AACPacketType=0x01;  
-             memcpy(pTagBuffer+datalen,&AACPacketType,1);  
-             datalen++;  
- 
- //src aac数据指针(不包含ADTS头长度)，srcLenAAC数据长度  
-             memcpy(pTagBuffer+datalen,src,srcLen);  
-             datalen+=srcLen;  
- 
- 
-             m_nPreTagLen=datalen-4;///（tag长度值）  
-             TagDataLen=datalen-15;//(11+4(tag长度值+tag的头)  
- 
- 
- //Tag 数据区长度  
-             FlvMemcpy(pTagBuffer+5,3,&TagDataLen,3);  
-             flvpacket.buf =(unsigned char*)pTagBuffer;             
-             flvpacket.bufLen=datalen;  
-             flvpacket.taglen=m_nPreTagLen;  
- return flvpacket;  




注意在使用ADTS头输出的AAC编码数据，在打包flv格式时，需要跳过其adts头长度（7字节）。
例如：



**[cpp]**[view plain](https://blog.csdn.net/lssaint/article/details/78590557#)[copy](https://blog.csdn.net/lssaint/article/details/78590557#)



- AudioPacket((const unsigned  char *)(pDataNALU+7),PktSize-7,Pts);     











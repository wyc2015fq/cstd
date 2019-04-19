# h.264 SODB RBSP EBSP的区别 - maopig的专栏 - CSDN博客
2011年09月09日 11:03:50[maopig](https://me.csdn.net/maopig)阅读数：1975标签：[h.264																[buffer																[byte																[go																[struct																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=go&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)
个人分类：[x264 h264](https://blog.csdn.net/maopig/article/category/847676)
SODB　数据比特串－－＞最原始的编码数据
RBSP　原始字节序列载荷－－＞在SODB的后面填加了结尾比特（RBSP trailing bits　一个bit“1”）若干比特“0”,以便字节对齐。
EBSP　扩展字节序列载荷-->在RBSP基础上填加了仿校验字节（0X03）它的原因是：　在NALU加到Annexb上时，需要填加每组NALU之前的开始码StartCodePrefix,如果该NALU对应的slice为一帧的开始则用4位字节表示，ox00000001,否则用3位字节表示ox000001.为了使NALU主体中不包括与开始码相冲突的，在编码时，每遇到两个字节连续为0，就插入一个字节的0x03。解码时将0x03去掉。也称为脱壳操作。
网上查询的区别：
在对整帧图像的数据比特串(SODB)添加原始字节序列载荷(RBSP)结尾比特(RBSP
 trailing bits,添加一比特的“1”和若干比特“0”,以便字节对齐)后,再检查RBSP
中是否存在连续的三字节“00000000 00000000 000000xx”;若存在这种连续的三字节码,在第三字节前插入一字节的“0×03”,以免与起始码竞争,形成EBSP码流,这需要将近两倍的整帧图像码流大小。为了减小存储器需求,在每个宏块编码结束后即检查该宏块SODB中的起始码竞争问题,并保留SODB最后两字节的零字节个数,以便与下一宏块的SODB的开始字节形成连续的起始码竞争检测;对一帧图像的最后一个宏块,先添加结尾停止比特,再检测起始码竞争。
程序：
typedef struct
{
intbyte_pos;//!< current position in bitstream;
intbits_to_go;//!< current bitcounter
bytebyte_buf;//!< current buffer for last written byte
intstored_byte_pos;//!< storage for position in bitstream;
intstored_bits_to_go;//!< storage for bitcounter
bytestored_byte_buf;//!< storage for buffer of last written byte
bytebyte_buf_skip;//!< current buffer for last written byte
intbyte_pos_skip;//!< storage for position in bitstream;
intbits_to_go_skip;//!< storage for bitcounter
byte*streamBuffer;//!< actual buffer for written bytes
intwrite_flag;//!< Bitstream contains data and needs to be written
} Bitstream;　定义比特流结构
static byte *NAL_Payload_buffer;
void SODBtoRBSP(Bitstream *currStream)
{
currStream->byte_buf <<= 1;　　//左移1bit
currStream->byte_buf |= 1;　　　//在尾部填一个“1”占1bit
currStream->bits_to_go--;
currStream->byte_buf <<= currStream->bits_to_go;
currStream->streamBuffer[currStream->byte_pos++] = currStream->byte_buf;
currStream->bits_to_go = 8;
currStream->byte_buf = 0;
}
int RBSPtoEBSP(byte *streamBuffer, int begin_bytepos, int end_bytepos, int min_num_bytes)
{
int i, j, count;
for(i = begin_bytepos; i < end_bytepos; i++)
    NAL_Payload_buffer[i] = streamBuffer[i];
count = 0;
j = begin_bytepos;
for(i = begin_bytepos; i < end_bytepos; i++) 
{
    if(count == ZEROBYTES_SHORTSTARTCODE && !(NAL_Payload_buffer[i] & 0xFC)) 
    {
      streamBuffer[j] = 0x03;
      j++;
      count = 0;   
    }
    streamBuffer[j] = NAL_Payload_buffer[i];
    if(NAL_Payload_buffer[i] == 0x00)      
      count++;
    else 
      count = 0;
    j++;
}
while (j < begin_bytepos+min_num_bytes) {
    streamBuffer[j] = 0x00; // cabac stuffing word
    streamBuffer[j+1] = 0x00;
    streamBuffer[j+2] = 0x03;
    j += 3;
    stat->bit_use_stuffingBits[img->type]+=16;
}
return j;
}

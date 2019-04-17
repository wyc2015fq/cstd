# AAC的ADTS头文件信息介绍 - DoubleLi - 博客园






遵循：[BY-SA](https://creativecommons.org/licenses/by-sa/4.0/)

[署名-相同方式共享 4.0协议](https://creativecommons.org/licenses/by-sa/4.0/)





作者：谭东

时间：2016年10月28日

环境：Windows 7





## ADTS是Audio Data Transport Stream的简称。



是AAC音频文件常见的传输格式。

有的时候当你编码AAC裸流的时候，会遇到写出来的AAC文件并不能在PC和手机上播放，很大的可能就是AAC文件的每一帧里缺少了ADTS头信息文件的包装拼接。只需要加入头文件ADTS即可。一个AAC原始数据块长度是可变的，对原始帧加上ADTS头进行ADTS的封装，就形成了ADTS帧。



下面分析下ADTS头文件结构和信息：

AAC音频文件的每一帧由ADTS Header和AAC Audio Data组成。



![](http://img.blog.csdn.net/20161028125743877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



大家可以用AAC Audio ES Viewer工具来查看AAC的ADTS Header。



![](http://img.blog.csdn.net/20161028125949801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

每一帧的ADTS的头文件都包含了音频的采样率，声道，帧长度等信息，这样解码器才能解析读取。


一般情况下ADTS的头信息都是7个字节，分为2部分：

**adts_fixed_header();**

**adts_variable_header();**


先来看：**adts_fixed_header();**

**![](http://img.blog.csdn.net/20161028130235945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



- **syncword ：**总是0xFFF, 代表一个ADTS帧的开始, 用于同步.
解码器可通过0xFFF确定每个ADTS的开始位置.
因为它的存在，解码可以在这个流中任何位置开始, 即可以在任意帧解码。
- **ID：**MPEG Version: 0 for MPEG-4，1 for MPEG-2
- **Layer**：always: '00'
- **protection_absent：**Warning, set to 1 if there is no CRC and 0 if there is CRC
- **profile：**表示使用哪个级别的AAC，如01 Low Complexity(LC) -- AAC LC
profile的值等于 Audio Object Type的值减1.
profile = MPEG-4 Audio Object Type - 1![](http://img.blog.csdn.net/20161028130919091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- **sampling_frequency_index：**采样率的下标![](http://img.blog.csdn.net/20161028131005767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- **channel_configuration：**声道数，比如2表示立体声双声道![](http://img.blog.csdn.net/20161028131140139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接下来看下**adts_variable_header();**

**![](http://img.blog.csdn.net/20161028131447640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

- **aac_frame_length：**一个ADTS帧的长度包括ADTS头和AAC原始流。frame length, this value must include 7 or 9 bytes of header length:
aac_frame_length = (protection_absent == 1 ? 7 : 9) + size(AACFrame)

protection_absent=0时, header length=9bytes
protection_absent=1时, header length=7bytes
- **adts_buffer_fullness：**0x7FF 说明是码率可变的码流。
- **number_of_raw_data_blocks_in_frame：**表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧。
所以说number_of_raw_data_blocks_in_frame == 0 表示说ADTS帧中有一个AAC数据块。
(一个AAC原始帧包含一段时间内1024个采样及相关数据)



两个头文件就讲到这里。


**这里给出JAVA和Android里的给AAC添加ADTS头文件的部分方法代码，针对ByteBuffer。**






**[java]**[view plain](http://blog.csdn.net/jay100500/article/details/52955232#)[copy](http://blog.csdn.net/jay100500/article/details/52955232#)



- /** 
-   * 添加ADTS头 
-   * 
-   * @param packet 
-   * @param packetLen 
-   */  
- private void addADTStoPacket(byte[] packet, int packetLen) {  
- int profile = 2; // AAC LC  
- int freqIdx = 4; // 44.1KHz  
- int chanCfg = 2; // CPE  
- 
- // fill in ADTS data  
-      packet[0] = (byte) 0xFF;  
-      packet[1] = (byte) 0xF9;  
-      packet[2] = (byte) (((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));  
-      packet[3] = (byte) (((chanCfg & 3) << 6) + (packetLen >> 11));  
-      packet[4] = (byte) ((packetLen & 0x7FF) >> 3);  
-      packet[5] = (byte) (((packetLen & 7) << 5) + 0x1F);  
-      packet[6] = (byte) 0xFC;  
-  }  


这里是调用：（可根据自己需求，这里的AudioBufFrame是我们自己项目里的类，其实就是个ByteBuffer封装的类）




**[java]**[view plain](http://blog.csdn.net/jay100500/article/details/52955232#)[copy](http://blog.csdn.net/jay100500/article/details/52955232#)



- AudioBufFrame audioBufFrame = (AudioBufFrame) o;  
- int outBufferSize = audioBufFrame.buf.limit() + 7;  
- byte[] aacBytes = new byte[outBufferSize];  
- try {  
- if (!write) {  
-                 fout = new FileOutputStream(MyConfiguration.VIDEO_PATH + "/audio.aac");  
-                 write = true;  
-             }  
-             addADTStoPacket(aacBytes, outBufferSize);  
-             audioBufFrame.buf.get(aacBytes, 7, audioBufFrame.buf.limit());  
-             fout.write(aacBytes);  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  





下面是FFMPEG参考的C的添加ADTS头文件代码，供大家参考。




**[java]**[view plain](http://blog.csdn.net/jay100500/article/details/52955232#)[copy](http://blog.csdn.net/jay100500/article/details/52955232#)



- int ff_adts_write_frame_header(ADTSContext *ctx,    
-                                uint8_t *buf, int size, int pce_size)    
- {    
-     PutBitContext pb;    
- 
-     init_put_bits(&pb, buf, ADTS_HEADER_SIZE);    
- 
- /* adts_fixed_header */    
-     put_bits(&pb, 12, 0xfff);   /* syncword */    
-     put_bits(&pb, 1, 0);        /* ID */    
-     put_bits(&pb, 2, 0);        /* layer */    
-     put_bits(&pb, 1, 1);        /* protection_absent */    
-     put_bits(&pb, 2, ctx->objecttype); /* profile_objecttype */    
-     put_bits(&pb, 4, ctx->sample_rate_index);    
-     put_bits(&pb, 1, 0);        /* private_bit */    
-     put_bits(&pb, 3, ctx->channel_conf); /* channel_configuration */    
-     put_bits(&pb, 1, 0);        /* original_copy */    
-     put_bits(&pb, 1, 0);        /* home */    
- 
- /* adts_variable_header */    
-     put_bits(&pb, 1, 0);        /* copyright_identification_bit */    
-     put_bits(&pb, 1, 0);        /* copyright_identification_start */    
-     put_bits(&pb, 13, ADTS_HEADER_SIZE + size + pce_size); /* aac_frame_length */    
-     put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */    
-     put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */    
- 
-     flush_put_bits(&pb);    
- 
- return 0;    
- }    





![](http://img.blog.csdn.net/20161028132427552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











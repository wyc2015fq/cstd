# 将H264与AAC打包Ipad可播放的TS流的总结（h5的实现） - 建建的博客 - CSDN博客
2018年04月23日 14:39:55[纪建](https://me.csdn.net/u013898698)阅读数：39
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)
由于要在crtmpserver中实现Http Live Streaming ，本人花了接近3个星期的时间，研究将H264与AAC打包为TS流并能在Ipad上通过HTML5播放，由于没有任何现成代码可供参考，打包代码全部手写，打包格式主要参考ISO/ICE 18318-1.pdf。期间碰到很多问题，走了不少弯路，符合标准的TS不一定能在Ipad上播放，但是Ipad上播放的TS一定是符合标准的，可以说是TS标准中的特例。现将遇到的主要问题以及要点总结如下：
【打包流程以及相关图：】
![](http://images.cnitblog.com/blog/46825/201303/28235915-3453c02a933542c0ac147e8ac506e8aa.jpg)
【视频H264到TS注意要点：】
<1>. 如果h264的包大于 65535  的话，可以设置PES_packet_length为0 ，具体参见ISO/ICE 13818-1.pdf  49 / 174 中关于PES_packet_length的描述
打包PES, 直接读取一帧h264的内容, 此时我们设置PES_packet_length的值为0000
表示不指定PES包的长度,ISO/ICE 13818-1.pdf 49 / 174 有说明,这主要是方便
当一帧H264的长度大于PES_packet_length(2个字节)能表示的最大长度65535
的时候分包的问题, 这里我们设置PES_packet_length的长度为0000之后 ,  那么即使该H264视频帧的长度
大于65535个字节也不需要分多个PES包存放, 事实证明这样做是可以的, ipad可播放
-----------------
<2>. PES头中的stream_id 好像是无所谓，随便改了几个都可以。  不过找到的几个可以ipad播放的样本文件中都设置的是0xe0
而在苹果的http://developer.apple.com/library/[iOS](http://lib.csdn.net/base/swift)/#documentation/AudioVideo/Conceptual/HTTP_Live_Streaming_Metadata_Spec/2/2.html
里面的2.4 PES Stream Format 提供的表里面，使用的是0xbd 表示的是private_stream_id_1，不知道这个说明是否专门针对ID3，但是建议设置为0xeo，我看到的几个TS样本文件都是这么设置的。
-----------------
<3>. 对于视频来说PTS和DTS都需要，可以设置为一样的，如果只有PTS，没有DTS是不行的。可以将pts的值赋值给dts，pts_dts_flag的值应该设置为0x03,也就是二进制的'11'
-----------------
<4>. pmt中的  stream_type = 0x1b 表示 H264  ， stream_type = 0x0f 表示AAC
-----------------
<5>. ipad不需要pcr都可以播放，可以将pmt头中的 PCR_PID设置为0x1fff 表示没有PCR ,参考ISO/ICE 13818-1.pdf 65 / 174,  在之后的所有关于adaptation_field中设置PCR_flag: 00 就可以了，并且在adaptation_field中也不需要写入pcr部分的值
-----------------
<6>. 在每一帧的视频帧被打包到pes的时候，其开头一定要加上 00 00 00 01 09 xx  这个nal。否则就有问题
苹果官网有如下说明:
https://developer.apple.com/library/ios/#documentation/networkinginternet/conceptual/streamingmediaguide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html
10.These settings are the current recommendations. There are also certain requirements. 
The current mediastreamsegmenter tool works only with MPEG-2 Transport Streams as defined in ISO/IEC 13818. 
The transport stream must contain H.264 (MPEG-4, part 10) video and AAC or MPEG audio. 
If AAC audio is used, it must have ADTS headers. H.264 video access units must use Access Unit Delimiter NALs, 
and must be in unique PES packets.
其中 H.264 video access units must use Access Unit Delimiter NALs  中的 Access Unit Delimiter NAL 在H264的文档中是09 ,但是有什么用，我也不太清楚。根据我的实验09后面的一个字节好像设置成什么都可以，但是不能设置为00
-----------------
<7>. 关于pts的计算方法
// 1s = 90000 time scale , 一帧就应该是  90000/video_frame_rate 个timescale
static uint32_t video_frame_rate = 30;
static uint32_t video_pts_increment = 90000 / video_frame_rate;   //用一秒钟除以帧率,得到每一帧应该耗时是多少，单位是 timescale单位
static uint64_t video_pts = 0;
在以后的计算中，每生成一帧的ts，该帧的pts值应该是 video_pts += video_pts_increment; 伪代码如下：
while(..)
{
   ReadH264(h264_buffer)
   PacketTS(h264_buffer,out_ts_buffer,video_pts);
   video_pts += video_pts_increment;
}
<8>. 音频应该是只需要pts，不需要dts。
音频pts的计算方法同上，只不过不是通过帧率，而是通过采样率。
uint32_t audio_pts_increment = (90000 * audio_samples_per_frame) / audio_sample_rate;
上面的伪代码就不贴了。
<9>. 关于pat,pmt的插入时间，实际上pat，pmt是要连续插入到ts流中的，并不是开始插入之后就完了。因为ts要保证从任何时候都可以开始播放。目前我是每4帧视频(这4帧H264可能被打包成很多的ts包，而不止4包)插入一次pat，pmt包。
<10>. PES音视频数据包的ts头中的continuity_counter 必须是连续的，从小到大的，当增长到15之后再从0开始循环。第一包可以不从0开始。但是之后的continuity_counter 必须是连续的，并且是按照步长1增长的。continuity_counter的增长相对于所有PID相同的包他们的增长是独立的，例如第一个PAT包其continuity_counter值是0，那么第二个PAT包的continuity_counter是1。  第一个PMT包其continuity_counter值是2，第二个PMT包其continuity_counter值是3。  
【音频AAC到TS注意要点：】
aac不需要设置DTS，只要PTS就可以了,加上DTS可能还播放不了
aac打包成PES的时候，要想在ipad上播放必须设置PES_packet_length的长度，而视频可以设置为0，但是音频必须设置为正确的长度值，aac的长度不可能超过65535，所以也不可能导致PES_packet_length溢出。
否则ipad播放不了。但是QQ影音可以播放。这也许是ipad的特性。
 //data_len:AAC音频裸流的长度
 // 3:PES头
 //5: 如果是音频的话会有5个字节的PTS
_ts_packet_pes_header.PES_packet_length_16bit = data_len + 3 + 5 ;
【关于CRC32校验[算法](http://lib.csdn.net/base/datastructure)：】
static uint32_t crc32table[256] = {   
0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,   
0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,   
0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,   
0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,   
0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,   
0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,   
0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,   
0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,   
0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,   
0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,   
0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,   
0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,   
0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,   
0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,   
0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,   
0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,   
0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,   
0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,   
0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,   
0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,   
0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,   
0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,   
0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,   
0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,   
0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,   
0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,   
0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,   
0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,   
0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,   
0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,   
0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,   
0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,   
0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,   
0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,   
0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,   
0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,   
0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,   
0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,   
0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,   
0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,   
0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,   
0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,   
0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};
uint32_t CRC32(const uint8_t *data, int len)
{   
    int i;   
    uint32_t crc = 0xFFFFFFFF;       
    for(i = 0; i < len; i++) 
    crc = (crc << 8) ^ crc32table[((crc >> 24) ^ *data++) & 0xFF];       
    return crc;
}
【关于TS流中的填充字节】
TS流中有2中不同的填充形式，因为TS每一包要求是188个字节，当不足188个字节的时候， 必须要补充到188个字节，这就涉及到填充的问题。
<1>. 如果TS包中承载的是PSI数据(PAT,PMT等)，那么其填充是在该包的最后一个有效字节的后面填充0xFF直到满足188个字节为止。
解码器会丢弃这些字节，具体说明参考 ISO_IEC 13818-1.pdf   60 / 174
<2>. 如果TS包中承载的是PES数据，那么当不足188个字节的时候，需要使用adaptation_field 这个域，也就是指定TS包中的adaptation_field_control的值来控制payload与adaptation_field的承载关系，同时指定PES中的adaptation_field_length指定填充多少字节，具体的填充字节值应该是随意的建议使用FF，并且此adaptation_field的使用好像只能在PES中使用。  

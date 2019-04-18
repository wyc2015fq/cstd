# FFMPEG学习【libavutil】：音频相关（一） - 一世豁然的专栏 - CSDN博客





2017年07月25日 10:26:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：531








一、音频FIFO缓冲区

一）、函数


|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_audio_fifo_free](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga74e029e47f7aa99217ad1f315c434875) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af)|
|----|----|

释放AVAudioFifo。






|[AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *|[av_audio_fifo_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga9d792394f0615a329aec47847f8f8784) (enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) sample_fmt, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) channels, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

分配AVAudioFifo。

参数：sample_fmt：采样格式

channels：频道数量

nb_samples：初始分配大小，在样本中

返回：新分配的AVAudioFifo，或错误的NULL





|[av_warn_unused_result](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_realloc](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga27c1e16e5f09940d6016b1971c0b5742) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

重新分配AVAudioFifo。

参数：af：AVAudioFifo重新分配

nb_samples：新分配大小，在样本中

返回：如果OK，则为0，否则为ERROR代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_write](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga0e7fadeea09c52a96eb4082a9e169bb4) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

将数据写入AVAudioFifo。

如果可用空间小于nb_samples，则AVAudioFifo将自动重新分配。


参数：af：写入的AVAudioFifo

   data：音频数据平面指针

nb_samples：要写的样本数

返回：实际写入的样本数，或失败的负错误代码。 如果成功，实际写入的样本数将始终为nb_samples。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_peek](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga0a5cdd4193a4bce3c9b3df7a8fd12ed6) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

从AVAudioFifo中查看数据。

参数：af：从中读取的AVAudioFifo

   data：音频数据平面指针

nb_samples：要窥探的样本数量

返回：样品数量实际窥视，否则AVERROR代码失败。 实际窥视的样本数量将不会大于nb_samples，如果av_audio_fifo_size小于nb_samples，则它将仅小于nb_samples。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_peek_at](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga4ada8857bd36876cd543f1dbd435f295) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[offset](http://ffmpeg.org/doxygen/trunk/vf__uspp_8c.html#a06b941e27d7180a9a7c906f9907682d6))|
|----|----|

从AVAudioFifo中查看数据。

参数：af：从中读取的AVAudioFifo

   data：从中读取的AVAudioFifo

nb_samples：要窥探的样本数量

   offset：偏离当前读取位置

返回：样品数量实际窥视，否则AVERROR代码失败。 实际窥视的样本数量将不会大于nb_samples，如果av_audio_fifo_size小于nb_samples，则它将仅小于nb_samples。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_read](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#gab256fc29188d91311bd2fbd78eb356af) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

从AVAudioFifo读取数据。

参数：af：从中读取的AVAudioFifo

   data：从中读取的AVAudioFifo

nb_samples：要读取的样本数

返回：实际读取的样本数，或失败时为负的AVERROR代码。 实际读取的样本数量不会大于nb_samples，如果av_audio_fifo_size小于nb_samples，则它将仅小于nb_samples。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_drain](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#gacac9b187b9c27c5a0374f6f83224a77b) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) nb_samples)|
|----|----|

从AVAudioFifo中排出数据。

删除数据而不读取它。


参数：af：排除的AVAudioFifo

nb_samples：排出的样本数目

返回：如果OK，则为0，否则为ERROR代码





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_audio_fifo_reset](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga3f070386d11d425302437a3b60fc5dd0) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af)|
|----|----|

重置AVAudioFifo缓冲区。

这将清空缓冲区中的所有数据。


参数：af：重置的AVAudioFifo





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_size](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#gaa0a4742ecac52a999e8b4478d27f3b9b) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af)|
|----|----|

获取可用于阅读的AVAudioFifo中的当前样本数。

参数：af：进行查询的AVAudioFifo

返回：可用于读取的样本数





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_audio_fifo_space](http://ffmpeg.org/doxygen/trunk/group__lavu__audiofifo.html#ga2bed2f01fe34228ee8a73617b3177d00) ([AVAudioFifo](http://ffmpeg.org/doxygen/trunk/structAVAudioFifo.html) *af)|
|----|----|

获取AVAudioFifo当前可用于编写的样本数。

参数：af：进行查询的VAudioFifo

返回：可用于写入的样本数量







二、音频通道掩码

通道布局是一个64位整数，每个通道都有一个位设置。


设置的位数必须等于通道数。 值0表示通道布局未知。


注意：这种数据结构不够强大，无法处理具有相同通道多次的通道组合，例如双单声道。





|#define|[AV_CH_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaf9c5619f74c99ea8a8606719c01fe7df)   0x00000001|
|----|----|
|| |
|#define|[AV_CH_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga394b4b2c87790178bf7b683b45320f31)   0x00000002|
|| |
|#define|[AV_CH_FRONT_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga88f2868b232a89fd11ad9d6c39d502a1)   0x00000004|
|| |
|#define|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc)   0x00000008|
|| |
|#define|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)   0x00000010|
|| |
|#define|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb)   0x00000020|
|| |
|#define|[AV_CH_FRONT_LEFT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6c9ea2ea60044bfe844174300b237386)   0x00000040|
|| |
|#define|[AV_CH_FRONT_RIGHT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga558dc25b0ce327d2547d3cc9e4bd6a4e)   0x00000080|
|| |
|#define|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935)   0x00000100|
|| |
|#define|[AV_CH_SIDE_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga02e55f35249dfa57a2e4edfd3acfec4f)   0x00000200|
|| |
|#define|[AV_CH_SIDE_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga947b60ab70c428ebb07c8b9c87023b39)   0x00000400|
|| |
|#define|[AV_CH_TOP_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga922199ce7638a651d66a423379b4c5dc)   0x00000800|
|| |
|#define|[AV_CH_TOP_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga352bec66d2e1bef67efe5b7f0a4207f3)   0x00001000|
|| |
|#define|[AV_CH_TOP_FRONT_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6b558fe85e67383a5b04e56b97b145d0)   0x00002000|
|| |
|#define|[AV_CH_TOP_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gabce29ea1d7e2d433263b2cbf28232360)   0x00004000|
|| |
|#define|[AV_CH_TOP_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaac082f244f37f9c5d8e4f7bf96cc76dc)   0x00008000|
|| |
|#define|[AV_CH_TOP_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga439a8664c452f9782b460f61a3908451)   0x00010000|
|| |
|#define|[AV_CH_TOP_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga8fa88c3c74339f4218a3e70286fdbdaa)   0x00020000|
|| |
|#define|[AV_CH_STEREO_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaea8f62dcaa4774307a8fa9b52172c688)   0x20000000|

立体声下混


|#define|[AV_CH_STEREO_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab68e7edb9182b1b22898b960707143c1)   0x40000000|
|----|----|

见AV_CH_STEREO_LEFT。


|#define|[AV_CH_WIDE_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga11e273c4872335063b5059cca52218c0)   0x0000000080000000ULL|
|----|----|
|| |
|#define|[AV_CH_WIDE_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga2c7ff738c3481d2891d052f5bca55248)   0x0000000100000000ULL|
|| |
|#define|[AV_CH_SURROUND_DIRECT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gafc04912a8294d8788e4162092df8a549)   0x0000000200000000ULL|
|| |
|#define|[AV_CH_SURROUND_DIRECT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga0c9137cc9fd60d15be588e2eff3cfa23)   0x0000000400000000ULL|
|| |
|#define|[AV_CH_LOW_FREQUENCY_2](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga72d2eaef6727608379ab29859f5d5ee2)   0x0000000800000000ULL|
|| |
|#define|[AV_CH_LAYOUT_NATIVE](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7ab04ac641edef25c03de0a577eab57d)   0x8000000000000000ULL|

用于AVCodecContext.request_channel_layout的通道掩码值，用于指示用户请求解码器输出的通道顺序为本地编解码器通道顺序。




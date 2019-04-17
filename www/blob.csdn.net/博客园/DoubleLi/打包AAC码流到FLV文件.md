# 打包AAC码流到FLV文件 - DoubleLi - 博客园






AAC编码后数据打包到FLV很简单。


1. FLV音频Tag格式
                              字节位置    意义
0x08,                        // 0,       TagType
0xzz, 0xzz, 0xzz,            // 1-3,     DataSize,    
0xzz, 0xzz, 0xzz, 0xzz,      // 4-6, 7   TimeStamp | TimeStampExtend    
0x00, 0x00, 0x00,            // 8-10,    StreamID

0xzz,                        // 11,      AudioTag Header  
0x0b,                        // 12,      AACPacketType    (如果不是AAC编码 没有这个字节)       
0xzz ... 0xzz                // 音频数据



2. AudioTagHeader
音频Tag头一般由一个字节定义（AAC用两个字节），第一个字节的定义如下：
音频格式 4bits | 采样率 2bits | 采样精度 1bits | 声道数 1bits|

音频格式 4bits
0x00 = Linear PCM, platform endian
0x01 = ADPCM
0x02 = MP3
0x03 = Linear PCM, little endian
0x04 = Nellymoser 16-kHz mono
0x05 = Nellymoser 8-kHz mono
0x06 = Nellymoser
0x07 = G.711 A-law logarithmic PCM
0x08 = G.711 mu-law logarithmic PCM
0x09 = reserved
0x0A = AAC
0x0B = Speex
0x0E = MP3 8-Khz
0x0F = Device-specific sound

采样率 2bits
0 = 5.5-kHz
1 = 11-kHz
2 = 22-kHz
3 = 44-kHz
对于AAC总是3，这里看起来FLV不支持48K AAC，其实不是的，后面还是可以定义为48K。

采样精度 1bits
0 = snd8Bit
1 = snd16Bit
压缩过的音频都是16bit

声道数 1bits
0 = sndMono
1 = sndStereo
对于AAC总是1


综上，如果是AAC 48K 16比特精度 双声道编码，该字节为 0b1010 1111 = 0xAF。

看第2个字节，如果音频格式AAC（0x0A），AudioTagHeader中会多出1个字节的数据AACPacketType，这个字段来表示AACAUDIODATA的类型：
0x00 = AAC sequence header，类似h.264的sps,pps，在FLV的文件头部出现一次。
0x01 = AAC raw，AAC数据



3. AAC Sequence header
AAC sequence header定义AudioSpecificConfig，AudioSpecificConfig包含着一些更加详细的音频信息，它的定义在ISO14496-3中1.6.2.1。
简化的AudioSpecificConfig 2字节定义如下：
AAC Profile 5bits | 采样率 4bits | 声道数 4bits | 其他 3bits |

AAC Profile 5bits，参考ISO-14496-3 Object Profiles Table
AAC Main 0x01
AAC LC    0x02
AAC SSR  0x03
...

（为什么有些文档看到profile定义为4bits，实际验证是5bits）


采样率 4bits
Value samplingFrequencyIndex
0x00   96000
0x01   88200
0x02   64000
0x03   48000
0x04   44100
0x05   32000
0x06   24000
0x07   22050
0x08   16000
0x09   12000
0x0A   11025
0x0B     8000
0x0C   reserved
0x0D   reserved
0x0E   reserved
0x0F   escape value


声道数 4bits
0x00 - defined in audioDecderSpecificConfig
0x01 单声道（center front speaker）
0x02 双声道（left, right front speakers）
0x03 三声道（center, left, right front speakers）
0x04 四声道（center, left, right front speakers, rear surround speakers）
0x05 五声道（center, left, right front speakers, left surround, right surround rear speakers）
0x06 5.1声道（center, left, right front speakers, left surround, right surround rear speakers, front low frequency effects speaker)
0x07 7.1声道（center, left, right center front speakers, left, right outside front speakers, left surround, right surround rear speakers, front low frequency effects speaker)
0x08-0x0F - reserved


其他3bits设置为0即可。

AAC-LC， 48000，双声道 这样的设置 Sequence header 为 0b 00010 0011 0010 000 = 0x11 0x90。
因此 AAC Sequence header的整个音频Tag包为 0x08, 00 00 04, 00 00 00 00, 00 00 00, AF 00 11 90 | 00 00 00 0F

AAC Sequence header这个音频包有些FLV文件里面没有也可以正确解码。但对于RTMP播放，必须要在发送第一个音频数据包前发送这个header包。






4. AAC音频包
结构为：0x08, 3字节包长度，4字节时间戳，00 00 00，AF 01 N字节AAC数据 | 前包长度
其中编码后AAC纯数据长度为N，3字节包长度 = N + 2

前包长度 = 11 + 3字节包长度 = 11 + N + 2 = 13 + N。










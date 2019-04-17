# AAC_LC用LATM封装header信息解析 Audio Specific Config格式分析 - DoubleLi - 博客园







通常来说AAC的头信息在编解码过程中是可以获取到的，但今天需要根据音频参数生成相应的AAC头。项目中使用的是AAC_LC，今天先对它的结构进行分析。

    项目中使用ffmpeg进行音频编码，音频编码库为FAAC，好吧，直接看代码吧。

    ffmpeg调用Faac_encode_init()初始化编码器；在Faac_encode_init()调用faacEncGetDecoderSpecificInfo()获取AAC_LC头信息内容及长度。







[?](http://doc.okbase.net/zhangxu0512/archive/55448.html#)

```
```cpp
int
```

```cpp
FAACAPI faacEncGetDecoderSpecificInfo(faacEncHandle hEncoder,unsigned
```

```cpp
char
```

```cpp
** ppBuffer,unsigned
```

```cpp
long
```

```cpp
* pSizeOfDecoderSpecificInfo)
```



```cpp
{
```



```cpp

```

```cpp
BitStream* pBitStream = NULL; <span style=
```

```cpp
"font-family:Times New Roman;"
```

```cpp
></span>
```

```cpp
if
```

```cpp
((hEncoder == NULL) || (ppBuffer == NULL) || (pSizeOfDecoderSpecificInfo == NULL)) {
```



```cpp

```

```cpp
return
```

```cpp
-1;
```



```cpp

```

```cpp
}
```





```cpp

```

```cpp
if
```

```cpp
(hEncoder->config.mpegVersion == MPEG2){
```



```cpp

```

```cpp
return
```

```cpp
-2;
```

```cpp
/* not supported */
```



```cpp

```

```cpp
}
```





```cpp

```

```cpp
*pSizeOfDecoderSpecificInfo = 2;
```



```cpp

```

```cpp
*ppBuffer =
```

```cpp
malloc
```

```cpp
(2);
```





```cpp

```

```cpp
if
```

```cpp
(*ppBuffer != NULL){
```





```cpp

```

```cpp
memset
```

```cpp
(*ppBuffer,0,*pSizeOfDecoderSpecificInfo);
```



```cpp

```

```cpp
pBitStream = OpenBitStream(*pSizeOfDecoderSpecificInfo, *ppBuffer);
```



```cpp

```

```cpp
PutBit(pBitStream, hEncoder->config.aacObjectType, 5);
```



```cpp

```

```cpp
PutBit(pBitStream, hEncoder->sampleRateIdx, 4);
```



```cpp

```

```cpp
PutBit(pBitStream, hEncoder->numChannels, 4);
```



```cpp

```

```cpp
CloseBitStream(pBitStream);
```





```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp

```

```cpp
}
```

```cpp
else
```

```cpp
{
```



```cpp

```

```cpp
return
```

```cpp
-3;
```



```cpp

```

```cpp
}
```



```cpp
}
```
```










    从代码中可以看出，头数据长度固定为2；

    数据内容由高位到低位依次为：aacObjectType（5bits）,sampleRateIdx(4bits),numChannels(4bits)

例如：音频编码参数为：

aacObjectType:AAC_LC,对应值为2，用5bit二进制表示为00010;

sampleRate:44100KHz, 对应的IDX值为4, 用4bit二进制表示为0100; 

numChannels:2，对应的值为2，用4bit二进制表示为0010；

将它们由高位到低位串起来：0001,0010,0001,0000，

则，对应的十六进制值为:0x1220



引一些参考资料：http://wiki.multimedia.cx/index.php?title=MPEG-4_Audio



## Audio Specific Config

The Audio Specific Config is the global header for MPEG-4 Audio:
5 bits: object type
if (object type == 31)
    6 bits + 32: object type
4 bits: frequency index
if (frequency index == 15)
    24 bits: frequency
4 bits: channel configuration
var bits: AOT Specific Config
## Audio Object Types

MPEG-4 Audio Object Types:
- 0: Null
- 1: [AAC](http://wiki.multimedia.cx/index.php?title=AAC) Main
- 2: [AAC](http://wiki.multimedia.cx/index.php?title=AAC) LC (Low Complexity)
- 3: [AAC](http://wiki.multimedia.cx/index.php?title=AAC) SSR (Scalable Sample Rate)
- 4: [AAC](http://wiki.multimedia.cx/index.php?title=AAC) LTP (Long Term Prediction)
- 5: SBR ([Spectral Band Replication](http://wiki.multimedia.cx/index.php?title=Spectral_Band_Replication))
- 6: [AAC](http://wiki.multimedia.cx/index.php?title=AAC) Scalable
- 7: [TwinVQ](http://wiki.multimedia.cx/index.php?title=TwinVQ)
- 8: [CELP](http://wiki.multimedia.cx/index.php?title=CELP&action=edit&redlink=1) (Code Excited Linear Prediction)
- 9: HXVC (Harmonic Vector eXcitation Coding)
- 10: Reserved
- 11: Reserved
- 12: TTSI (Text-To-Speech Interface)
- 13: Main Synthesis
- 14: Wavetable Synthesis
- 15: General MIDI
- 16: Algorithmic Synthesis and Audio Effects
- 17: ER (Error Resilient) [AAC](http://wiki.multimedia.cx/index.php?title=AAC) LC
- 18: Reserved
- 19: ER [AAC](http://wiki.multimedia.cx/index.php?title=AAC) LTP
- 20: ER [AAC](http://wiki.multimedia.cx/index.php?title=AAC) Scalable
- 21: ER [TwinVQ](http://wiki.multimedia.cx/index.php?title=TwinVQ)
- 22: ER [BSAC](http://wiki.multimedia.cx/index.php?title=BSAC) (Bit-Sliced Arithmetic Coding)
- 23: ER [AAC](http://wiki.multimedia.cx/index.php?title=AAC) LD (Low Delay)
- 24: ER [CELP](http://wiki.multimedia.cx/index.php?title=CELP&action=edit&redlink=1)
- 25: ER HVXC
- 26: ER HILN (Harmonic and Individual Lines plus Noise)
- 27: ER Parametric
- 28: SSC (SinuSoidal Coding)
- 29: PS ([Parametric Stereo](http://wiki.multimedia.cx/index.php?title=Parametric_Stereo&action=edit&redlink=1))
- 30: [MPEG Surround](http://wiki.multimedia.cx/index.php?title=MPEG_Surround)
- 31: (Escape value)
- 32: [Layer-1](http://wiki.multimedia.cx/index.php?title=MP1&action=edit&redlink=1)
- 33: [Layer-2](http://wiki.multimedia.cx/index.php?title=MP2)
- 34: [Layer-3](http://wiki.multimedia.cx/index.php?title=MP3)
- 35: DST (Direct Stream Transfer)
- 36: [ALS](http://wiki.multimedia.cx/index.php?title=MPEG-4_ALS) (Audio Lossless)
- 37: [SLS](http://wiki.multimedia.cx/index.php?title=MPEG-4_SLS) (Scalable LosslesS)
- 38: [SLS](http://wiki.multimedia.cx/index.php?title=MPEG-4_SLS) non-core
- 39: ER [AAC](http://wiki.multimedia.cx/index.php?title=AAC) ELD (Enhanced Low Delay)
- 40: SMR (Symbolic Music Representation) Simple
- 41: SMR Main
- 42: [USAC](http://wiki.multimedia.cx/index.php?title=USAC&action=edit&redlink=1) (Unified Speech and Audio Coding) (no [SBR](http://wiki.multimedia.cx/index.php?title=SBR))
- 43: SAOC (Spatial Audio Object Coding)
- 44: LD [MPEG Surround](http://wiki.multimedia.cx/index.php?title=MPEG_Surround)
- 45: [USAC](http://wiki.multimedia.cx/index.php?title=USAC&action=edit&redlink=1)

## Sampling Frequencies

There are 13 supported frequencies:
- 0: 96000 Hz
- 1: 88200 Hz
- 2: 64000 Hz
- 3: 48000 Hz
- 4: 44100 Hz
- 5: 32000 Hz
- 6: 24000 Hz
- 7: 22050 Hz
- 8: 16000 Hz
- 9: 12000 Hz
- 10: 11025 Hz
- 11: 8000 Hz
- 12: 7350 Hz
- 13: Reserved
- 14: Reserved
- 15: frequency is written explictly

## Channel Configurations

These are the channel configurations:
- 0: Defined in AOT Specifc Config
- 1: 1 channel: front-center
- 2: 2 channels: front-left, front-right
- 3: 3 channels: front-center, front-left, front-right
- 4: 4 channels: front-center, front-left, front-right, back-center
- 5: 5 channels: front-center, front-left, front-right, back-left, back-right
- 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
- 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
- 8-15: Reserved





from:[http://my.oschina.net/zhangxu0512/blog/204070](http://my.oschina.net/zhangxu0512/blog/204070)










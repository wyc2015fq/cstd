# webrtc（一） agc - xmdxcsj的专栏 - CSDN博客





2015年12月03日 21:33:23[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4741








## 一、参数

### （一）、mode

```
enum Mode {
    // Adaptive mode intended for use if an analog volume control is available
    // on the capture device. It will require the user to provide coupling
    // between the OS mixer controls and AGC through the |stream_analog_level()|
    // functions.
    //  
    // It consists of an analog gain prescription for the audio device and a
    // digital compression stage.
    kAdaptiveAnalog,

    // Adaptive mode intended for situations in which an analog volume control
    // is unavailable. It operates in a similar fashion to the adaptive analog
    // mode, but with scaling instead applied in the digital domain. As with
    // the analog mode, it additionally uses a digital compression stage.
    kAdaptiveDigital,

    // Fixed mode which enables only the digital compression stage also used by
    // the two adaptive modes.
    //  
    // It is distinguished from the adaptive modes by considering only a
    // short time-window of the input signal. It applies a fixed gain through
    // most of the input level range, and compresses (gradually reduces gain
    // with increasing level) the input signal at higher levels. This mode is
    // preferred on embedded devices where the capture signal level is
    // predictable, so that a known gain can be applied.
    kFixedDigital
  };
```
- kAdaptiveAnalog 

适合用desktop platforms，模拟信号可控音量
- kAdaptiveDigital 

适合用mobile device，没有音量控制
- kFixedDigital 

固定增益

### （二）、parameter

**以下摘自于 [http://rg4.net/archives/797.html](http://rg4.net/archives/797.html)**

——————————————- 

AGC parameters: 

——————————————- 

1. Target level DBFS 

——————————————- 

According to the description of webrtc: 

 Change of this parameter will set the target peak |level| (or envelope) of the AGC in dBFs (decibels from digital full-scale). 

 The convention is to use positive values. 

 For instance, passing in a value of 3 corresponds to -3 dBFs, or a target level 3 dB below full-scale.
Value range: limited to [0, 31].

TODO(ajm): use a negative value here instead, if/when VoE will similarly update its interface. 

——————————————- 

2. Compression gain DB 

——————————————- 

Sets the maximum |gain| the digital compression stage may apply, in dB. A higher number corresponds to greater compression, while a value of 0 will leave the signal uncompressed.
Value range: limited to [0, 90]

——————————————- 

3. Enable limiter or not 

——————————————- 

When enabled, the compression stage will hard limit the signal to the target level. Otherwise, the signal will be compressed but not limited above the target level.
——————————————- 

4. Analog level limits 

——————————————- 

Sets the |minimum| and |maximum| analog levels of the audio capture device. 

 Must be set if and only if an analog mode is used.
Value range: limited to [0, 65535].

——————————————- 

5. Stream saturated or not 

——————————————- 

Returns true if the AGC has detected a saturation event (period where the signal reaches digital full-scale) in the current frame and the analog level cannot be reduced. 

 This could be used as an indicator to reduce or disable analog mic gain at the audio HAL. 
**以上摘自于 [http://rg4.net/archives/797.html](http://rg4.net/archives/797.html)**
## 二、使用

由于不同版本的代码差距较大，首先声明下面使用例子来自于下面的commit

```
commit 99f8566524b3764bf84b7f4cbd8232bdf6c142a5
Author: kjellander <kjellander@webrtc.org>
Date:   Sun Nov 29 20:50:44 2015 -0800
```

使用自动增益控制放大的音量比较有限，所以使用了固定增益模式

```cpp
int agcTest2(char *filename, char *outfilename, int mode)
{
    //init agc
    void *agcInst = WebRtcAgc_Create();
    int minLevel = 0;
    int maxLevel = 255;
    int agcMode  = kAgcModeFixedDigital;
    int fs       = 16000;
    int status   = 0; 
    status = WebRtcAgc_Init(agcInst, minLevel, maxLevel, agcMode, fs);
    if(status != 0)
    {
        printf("failed in WebRtcAgc_Init\n");
        return -1;
    }
    WebRtcAgcConfig agcConfig;
    agcConfig.compressionGaindB = 20;//在Fixed模式下，越大声音越大
    agcConfig.limiterEnable = 1;
    agcConfig.targetLevelDbfs = 3;   //dbfs表示相对于full scale的下降值，0表示full scale，越小声音越大
    status = WebRtcAgc_set_config(agcInst, agcConfig);
    if(status != 0)
    {
        printf("failed in WebRtcAgc_set_config\n");
        return -1;
    }


    //alloc
    FILE *infp=fopen(filename,"r");
    int nBands = 1;
    int frameSize = 160;//10ms对应于160个short
    short **pData = (short**)malloc(nBands*sizeof(short*));
    pData[0] = (short*)malloc(frameSize*sizeof(short));
    short **pOutData = (short**)malloc(nBands*sizeof(short*));
    pOutData[0] = (short*)malloc(frameSize*sizeof(short));

    //process
    FILE *outfp = fopen(outfilename,"w");
    int len = frameSize;
    int micLevelIn = 0;
    int micLevelOut = 0;
    while(len > 0)
    {
        memset(pData[0], 0, frameSize*sizeof(short));
        len = fread(pData[0], sizeof(short), frameSize, infp);

        int inMicLevel  = micLevelOut;
        int outMicLevel = 0;
        uint8_t saturationWarning;
        status = WebRtcAgc_Process(agcInst, pData, nBands, frameSize, pOutData, inMicLevel, &outMicLevel, 0, &saturationWarning);
        if (status != 0)
        {
            printf("failed in WebRtcAgc_Process\n");
            return -1;
        }
        if (saturationWarning == 1)
        {
            printf("saturationWarning\n");
        }
        micLevelIn = outMicLevel;
        //write file
        len = fwrite(pOutData[0], sizeof(short), len, outfp);
    }
    fclose(infp);
    fclose(outfp);

    WebRtcAgc_Free(agcInst);
    free(pData[0]);
    free(pData);
    free(pOutData[0]);
    free(pOutData);
```

代码使用参考：webrtc/modules/audio_processing/gain_control_impl.cc

## 三、原理

**经过google邮件和论坛考证，确定没有公布出相关的参考文献和原理说明文档。**

想不通为什么代码都开源了，好歹说几个参考文献啊！源码也没怎么看，大致看了下注释，所以我在使用过程中，参数的含义和使用基本上靠猜和试验，后续有时间读几篇ns的文献再来细聊，有什么错误麻烦指正！









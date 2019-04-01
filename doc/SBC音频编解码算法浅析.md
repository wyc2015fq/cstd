# SBC音频编解码算法浅析

2018年05月10日 17:02:23 [ceetoo](https://me.csdn.net/ceetoo) 阅读数：3709



转自：https://blog.csdn.net/wzz4420381/article/details/48676921

原作者：[wzz4420381](https://blog.csdn.net/wzz4420381)

 

## 1. SBC算法简介

- SBC是sub band code的缩写，也可称为子带编码
- 在A2DP协议中，SBC算法是默认支持的
- 蓝牙SBC算法是一种以中等比特率传递高质量音频数据的低计算复杂度的音频编码算法

### 1.1 算法基本框图

- SBC系统使用一个余弦调制的滤波器组，用来解析和同步。滤波器组可设定成4或8个子带
- 子带信号的量化采用比特分配器和自适应脉冲编码器组调制
- 可用的比特位数和编码器的块数都是可配置的
- 编码后的数据由比特流打包器打包，以有线或无线方式传输
- 解码是编码的逆过程
  ![这里写图片描述](https://img-blog.csdn.net/20150930092917968)

### 1.1 算法所用参数

- sampling_frequency：采样频率。常用的是16KHz，32KHz，44.1KHz和48KHz
- channel mode：通道模式。可以是单声道，双声道，立体声和联合立体声
- block length：块长度。可以是4,8,12,16
- subbands：子带数量。可以是4或8
- allocation method：分配方式。可以是SNR或loudness
- bitpool：比特池。范围是2-250。此值越大，编码产生的数据越长

## 2 SBC算法实现

### 2.1 SBC编码算法实现

![这里写图片描述](https://img-blog.csdn.net/20150930101554558)

- PCM格式的左右声道进入多相解析器，输出尺度因子和子带采样数据
- 每个尺度因子分别对应一个子带
- 量化后的子带采样数据需要进行打包，打包方式可以是分段或不分段
  ![这里写图片描述](https://img-blog.csdn.net/20150930102223178)
- 多相解析器的代码实现较为复杂，流程图如下[具体请看参考文献2的Appendix B]：
  ![这里写图片描述](https://img-blog.csdn.net/20150930100848819)

### 2.2 SBC解码算法实现

![这里写图片描述](https://img-blog.csdn.net/20150930101512518)

- 解码过程是编码过程的逆
- 多相综合器的代码实现较为复杂，流程图如下[具体请看参考文献2的Appendix B]：
  ![这里写图片描述](https://img-blog.csdn.net/20150930101443244)

## 3 SBC解码算法在某蓝牙主设备上的应用

### 3.1 帧格式

![这里写图片描述](https://img-blog.csdn.net/20150930111949493)

### 3.2 参数选择

- sampling frequency：16
- channel mode：单声道
- number of subbands：8
- number of channels：1
- number of blocks：15
- allocation method：SNR
- bitpool：26

### 3.3 代码示例

```cpp
void SBC_Decode(uint8_t * DataIn, FILE * fOutput)
{
#define SBC_SAMPLING_FREQ 16
#define SBC_CHANNEL_MODE 0
#define SBC_NUM_OF_SUBBANDS 8
#define SBC_NUM_OF_CHANNELS 1
#define SBC_NUM_OF_BLOCKS   15
#define SBC_ALLOC_METHOD    0
#define SBC_BITPOOL 26          
#define SBC_DECODED_BUFFER_SIZE     (16*8)
 
    uint8_t blocks_per_packet = SBC_NUM_OF_BLOCKS;
    uint8_t num_bits = SBC_BITPOOL;
    const uint8_t * buf = (DataIn+1);//ignore CRC byte
    uint16_t len = SBC_GROUP_SIZE;
    uint16_t usDecodedBuffer[SBC_DECODED_BUFFER_SIZE];
 
    /* convenience  */
    const uint8_t * end = buf + len;
#define left (end - buf)
    uint16_t * outBufPtr = usDecodedBuffer;
 
    /* workspace */
    static INSAMPLE samples[16][8]; /*  We blow the stack if this is not static. */             
    ITER i, j, k;
    uint32_t scaleFactors[8]; //= {0x0f, 0x0c, 0x0b, 0x0b, 0x0a, 0x0a, 0x09, 0x09};
    int32_t bitneed[8];
    uint32_t bits[8];
    int32_t bitcount, slicecount, bitslice;
    uint8_t samplingRate, blocks, snr, numSubbands, bitpoolSz, bitpos = 0x80;
    int8_t max_bitneed = 0; 
#ifndef SPEED_OVER_ACCURACY
    int32_t levels[8];
#endif
 
#if (DEBUG_DECODING == 1)
    const uint8_t *start_buf = buf;
    pr_info("%s: blocks_per_packet = %d, num_bits = %d, buf = %p, len = %d\n",
        __func__, blocks_per_packet, num_bits, buf, len);
    for (i = 0; i < len; i++) {
        pr_info("buf[%d] = 0x%02x\n", i, buf[i]);
    }
#endif
 
    /* look into the frame header */
    if (left < SBC_GROUP_SIZE) goto out;/* too short a frame header  */
 
    /*  use Bemote specific constants  */
    samplingRate = 0; /*  always 16000 Hz */
    blocks = blocks_per_packet;
    snr = 0;
    numSubbands = SBC_NUM_OF_SUBBANDS;
    bitpoolSz = num_bits;               
 
    /* read scale factors */
    /* pr_info("sbc_decode: read scale factors, numSubbands = %d\n", numSubbands); */
    /**/
    for(i = 0; i < numSubbands; i++){
 
        if(bitpos == 0x80){
 
            scaleFactors[i] = (*buf) >> 4;
            bitpos = 0x08;
        }
        else{
 
            scaleFactors[i] = (*buf++) & 0x0F;
            bitpos = 0x80;
        }
    }
 
    /* calculate bitneed table and max_bitneed value (A2DP 12.6.3.1)  */
    if(snr){
 
        for(i = 0; i < numSubbands; i++){
 
            bitneed[i] = scaleFactors[i];
            if(bitneed[i] > max_bitneed) max_bitneed = bitneed[i];
        }
    }
    else{
 
        const signed char* tbl;
 
        if(numSubbands == 4) tbl = (const signed char*)loudness_4[samplingRate];
        else tbl = (const signed char*)loudness_8[samplingRate];
 
        for(i = 0; i < numSubbands; i++){
 
            if(scaleFactors[i]){
 
                int loudness = scaleFactors[i] - tbl[i];
 
                if(loudness > 0) loudness /= 2;
                bitneed[i] = loudness;
            }
            else bitneed[i] = -5;
            if(bitneed[i] > max_bitneed) max_bitneed = bitneed[i];
        }
    }               
 
    /* fit bitslices into the bitpool */
    bitcount = 0;
    slicecount = 0;
    bitslice = max_bitneed + 1;
    /* pr_info("sbc_decode: fit bitslices into the bitpool, bitslice = %d\n", bitslice ); */
    do{
        bitslice--;
        bitcount += slicecount;
        slicecount = 0;
        for(i = 0; i < numSubbands; i++){
 
            if(bitneed[i] > bitslice + 1 && bitneed[i] < bitslice + 16) slicecount++;
            else if(bitneed[i] == bitslice + 1) slicecount += 2;
        }
 
    }while(bitcount + slicecount < bitpoolSz);              
 
    /* distribute bits */
    for(i = 0; i < numSubbands; i++){
 
        if(bitneed[i] < bitslice + 2) bits[i] = 0;
        else{
 
            int8_t v = bitneed[i] - bitslice;
            if(v > 16) v = 16;
            bits[i] = v;
        }
    }       
 
    /* allocate remaining bits */
    for(i = 0; i < numSubbands && bitcount < bitpoolSz; i++){
 
        if(bits[i] >= 2 && bits[i] < 16){
 
            bits[i]++;
            bitcount++;
        }
        else if(bitneed[i] == bitslice + 1 && bitpoolSz > bitcount + 1){
 
            bits[i] = 2;
            bitcount += 2;
        }
    }
    for(i = 0; i < numSubbands && bitcount < bitpoolSz; i++){
 
        if(bits[i] < 16){
 
            bits[i]++;
            bitcount++;
        }
    }               
 
    /* reconstruct subband samples (A2DP 12.6.4) */
#ifndef SPEED_OVER_ACCURACY
        for(i = 0; i < numSubbands; i++) levels[i] = (1 << bits[i]) - 1;
#endif
 
    /* pr_info("sbc_decode: reconstruct subband samples, blocks = %d\n", blocks );  */
    for(j = 0; j < blocks; j++){
 
        for(i = 0; i < numSubbands; i++){
 
            if(bits[i]){
 
                uint32_t val = 0;
                k = bits[i];
                do{
 
                    val <<= 1;
#if (DEBUG_DECODING == 1)
                    pr_info("%s: buf = %p, offset %d\n",
                        __func__, buf, buf-start_buf);
#endif
                    if(*buf & bitpos) val++;
                    if(!(bitpos >>= 1)){
                        bitpos = 0x80;
                        buf++;
                    }
                }while(--k);
 
                val = (val << 1) | 1;
                val <<= scaleFactors[i];
 
                #ifdef SPEED_OVER_ACCURACY
                    val = mulshift(val, bits[i]);
                #else
                    val /= levels[i];
                #endif
 
                val -= (1 << scaleFactors[i]);
 
                samples[j][i] = SAMPLE_CVT(val);
            }
            else samples[j][i] = SAMPLE_CVT(0);
        }
    }       
 
    //sbc_decoder_reset();
 
    for(j = 0; j < blocks; j++){
        synth(outBufPtr, samples[j], numSubbands, gV);
        outBufPtr += numSubbands;
    }
 
    /* if we used a byte partially, skip the rest of it, it is "padding"  */
    if(bitpos != 0x80) buf++;
 
    out:
#if (DEBUG_DECODING == 1)
        if(left < 0)
            pr_err("SBC: buffer over-read by %d bytes.\n", -left);
        if(left > 0)
            pr_err("SBC: buffer under-read by %d bytes.\n", left);
#endif              
 
    fwrite(usDecodedBuffer, sizeof(uint16_t), 120, fOutput);
    fflush(fOutput);
    memset(usDecodedBuffer, 0, sizeof(usDecodedBuffer));
 
}
```

## 4 总结

- 在章节3中给出的例子中，压缩前的数据有120*2=240Byte，压缩后的数据有54Byte，压缩比接近4.4：1，压缩比是可调的，其与编码参数有关
- 压缩后的帧数据长度可以由编码参数计算出来。此例中，
  帧长度=4+（4 * 子带数量 * 通道数量）/8+（块数量 * 通道数量 * bitpool）/8
  =4+（4*8*1）/8+（15*1*26）/8=8+48.75=57（加上CRC正好58字节）
- 如果在传输过程中丢失了个别帧，解压后回放时会出现一小段音频的整体丢失，对整体的解压没有影响

## 参考文献

[1. LOW-POWER IMPLEMENTATION OF THE BLUETOOTH SUBBAND AUDIO CODEC.pdf](http://pan.baidu.com/s/1sjyy1wL)
[2. A2DP_SPEC_V12.pdf](http://pan.baidu.com/s/1mgF3FVM)
[3. 音频数据SBC算法编码方法及蓝牙立体声子系统.pdf](http://pan.baidu.com/s/1eQlTBCu)
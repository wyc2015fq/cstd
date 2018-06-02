#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <time.h>
#include "portableio.h"


#ifdef PI
#undef PI
#define         PI                      3.14159265358979
#endif
#define         PI4                     PI/4
#define         PI64                    PI/64
#define SQRT2       1.41421356237
#define LOGBLKSIZE  10
#define LN_TO_LOG10 0.2302585093
#define BLKSIZE     1024
#define HAN_SIZE    512
#define SCALE_BLOCK 12
#define SCALE_RANGE 64
#define SCALE       32768
#define SBLIMIT     32


typedef struct {
    FILE *file;
    int  type;
    int  channels;
    int  bits;
    long samplerate;
    long total_samples;
    long length;
} wave_t;

typedef struct {
    FILE *file;     //文件名
    int  type;      //MPEG的类型，有I和II两种类型  
    int  layr;      //层次
    int  mode;      //播放模式，例如是否为立体音  
    int  bitr;      //位率
    int  psyc;      //生理模型的类型
    int  emph;
    int  padding;   //填充位，只有在采样频率为44.1KHZ时用到填塞位
    long samples_per_frame;   //第三层为1152
    long bits_per_frame;      //每帧的总位数
    long bits_per_slot;     //处理位长
    long total_frames;      //总帧数
    int  bitrate_index;      //位率索引
    int  samplerate_index;   //抽样率检索
    int crc;      //校验位
    int ext;      
    int mode_ext;   //延伸模式
    int copyright;
    int original;
    int mode_gr;
} mpeg_t;

typedef struct {
    time_t start_time;
    enum e_byte_order byte_order; 

    char* infile;
    wave_t wave;

    char* outfile;
    mpeg_t mpeg;
} config_t;
extern config_t config;


#ifndef bool
typedef unsigned char bool;
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define WAVE_RIFF_PCM 0
#define WAVE_PCM_LOHI 1
#define WAVE_PCM_HILO 2
#define WAVE_PCM_AIFF 3

#define TYPE_MPEG_I   1
#define TYPE_MPEG_II  0

#define LAYR_I        0
#define LAYR_II       1
#define LAYR_III      2

#define MODE_STEREO   0
#define MODE_JSTEREO  1
#define MODE_DUAL     2
#define MODE_MONO     3

#define PSY_NONE      0
#define PSYC_MUSICAM  1
#define PSYC_ATT      2

#define EMPH_NONE     0
#define EMPH_5015     1
#define EMPH_CITT     2

#endif

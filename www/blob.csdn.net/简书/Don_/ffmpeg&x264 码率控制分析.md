# ffmpeg&x264 码率控制分析 - 简书


# ffmpeg&x264 码率控制分析



## 相关

[ffmpeg编码示例](https://www.jianshu.com/p/7e2e02500a3a)
[x264编码示例](https://www.jianshu.com/p/33591c32a15b)

## 概念

h264编码算法复杂、参数众多，单码率控制又分为三种模式。
- VBR（Variable Bit Rate）即动态比特率，其码率可以随着图像的复杂程度的不同而变化，因此其编码效率比较高，Motion发生时，马赛克很少。码率控制算法根据图像内容确定使用的比特率，图像内容比较简单则分配较少的码率(似乎码字更合适)，图像内容复杂则分配较多的码字，这样既保证了质量，又兼顾带宽限制。这种算法优先考虑图像质量。
- CBR（Constant Bit Rate）是以恒定比特率方式进行编码，有Motion发生时，由于码率恒定，只能通过增大QP来减少码字大小，图像质量变差，当场景静止时，图像质量又变好，因此图像质量不稳定。这种算法优先考虑码率(带宽)。
- CVBR（Constrained VariableBit Rate）它是VBR的一种改进方法。这种方法的兼顾了以上两种方法的优点：在图像内容静止时，节省带宽，有Motion发生时，利用前期节省的带宽来尽可能的提高图像质量，达到同时兼顾带宽和图像质量的目的。同时i_bitrate也需要设置。

## 码率控制

#### 间接影响

除了视频质量外有效影响视频码率的有
- 关键帧间隔





![](https://upload-images.jianshu.io/upload_images/1802307-67638e9c590496a4.png)





码率∝关键帧间隔（视频质量其他参数恒定）
- 分辨率





![](https://upload-images.jianshu.io/upload_images/1802307-ebb12e897e0a4f0a.png)





码率∝分辨率（视频质量其他参数恒定）
- 帧率







![](https://upload-images.jianshu.io/upload_images/1802307-13ffb3d226d3384d.png)





码率∝帧率（视频质量其他参数恒定）

注：

x264：i_fps_num = 15;i_fps_den= 1 ;//帧率15。

ffmpeg：time_base.num=1;time_den=1;//帧率15。

等等视频质量的参数，无可厚非，视频质量提升，要么编码速度降低，要么码率增大。

- B帧数





![](https://upload-images.jianshu.io/upload_images/1802307-6205d9d7cfa7993a.png)





码率∝1/b帧数（视频质量其他参数恒定）

#### 直接影响







![](https://upload-images.jianshu.io/upload_images/1802307-9211325fccbd0bf4.png)





X264_RC_CQP         动态比特率

X264_RC_CRF         恒定比特率

X264_RC_ABR         平均比特率

未选择时，优先选择的顺序是 bitrate > QP > CRF，会按照该顺序排查参数，直到发现某种类型参数合法时确定类型。
`if( bitrate ) rc_method = ABR; else if ( qp || qp_constant ) rc_method = CQP; else rc_method = CRF;`


### 附：

x264中x264_param_t->rc 结构体

```
struct
{
    int         i_rc_method;    /* X264_RC_* */

    int         i_qp_constant;  /* 0 to (51 + 6*(x264_bit_depth-8)). 0=lossless */
    int         i_qp_min;       /* min allowed QP value */
    int         i_qp_max;       /* max allowed QP value */
    int         i_qp_step;      /* max QP step between frames */

    int         i_bitrate;
    float       f_rf_constant;  /* 1pass VBR, nominal QP */
    float       f_rf_constant_max;  /* In CRF mode, maximum CRF as caused by VBV */
    float       f_rate_tolerance;
    int         i_vbv_max_bitrate;
    int         i_vbv_buffer_size;
    float       f_vbv_buffer_init; /* <=1: fraction of buffer_size. >1: kbit */
    float       f_ip_factor;
    float       f_pb_factor;

    /* VBV filler: force CBR VBV and use filler bytes to ensure hard-CBR.
     * Implied by NAL-HRD CBR. */
    int         b_filler;

    int         i_aq_mode;      /* psy adaptive QP. (X264_AQ_*) */
    float       f_aq_strength;
    int         b_mb_tree;      /* Macroblock-tree ratecontrol. */
    int         i_lookahead;

    /* 2pass */
    int         b_stat_write;   /* Enable stat writing in psz_stat_out */
    char        *psz_stat_out;  /* output filename (in UTF-8) of the 2pass stats file */
    int         b_stat_read;    /* Read stat from psz_stat_in and use it */
    char        *psz_stat_in;   /* input filename (in UTF-8) of the 2pass stats file */

    /* 2pass params (same as ffmpeg ones) */
    float       f_qcompress;    /* 0.0 => cbr, 1.0 => constant qp */
    float       f_qblur;        /* temporally blur quants */
    float       f_complexity_blur; /* temporally blur complexity */
    x264_zone_t *zones;         /* ratecontrol overrides */
    int         i_zones;        /* number of zone_t's */
    char        *psz_zones;     /* alternate method of specifying zones */
} rc;
```









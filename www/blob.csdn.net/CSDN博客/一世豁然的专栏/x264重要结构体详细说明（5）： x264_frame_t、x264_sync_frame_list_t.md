# x264重要结构体详细说明（5）： x264_frame_t、x264_sync_frame_list_t - 一世豁然的专栏 - CSDN博客





2016年08月18日 11:23:19[一世豁然](https://me.csdn.net/Explorer_day)阅读数：879








本博客金额转载于：[http://nkwavelet.blog.163.com/blog/static/22775603820143114415628/](http://nkwavelet.blog.163.com/blog/static/22775603820143114415628/)





x264_frame_t是帧列表中帧的结构，定义在common/frame.h中，接下来详细分析其元素：



/* number of pixels past the edge of the frame, for motion estimation/compensation */

#define PADH 32// 水平方向像素填充数目

#define PADV 32// 垂直方向像素填充数目


typedef struct **x264_frame**

{

/* */

uint8_t *base;/* Base pointer for all malloced data in this frame. */

int     i_poc;

int     i_delta_poc[2];

int     i_type;

int     i_qpplus1;

int64_t i_pts;

int64_t i_dts;

int64_t i_reordered_pts;

int64_t i_duration;/* in SPS time_scale units (i.e 2 * timebase units) used for vfr */

float   f_duration;
/* in seconds */

int64_t i_cpb_duration;

int64_t i_cpb_delay;
/* in SPS time_scale units (i.e 2 * timebase units) */

int64_t i_dpb_output_delay;

x264_param_t *param;// 编码器参数集


int     i_frame;     /* Presentation frame number，播放序号，即poc */

int     i_coded;     /* Coded frame number，编码序号 */

int64_t i_field_cnt; /* Presentation field count，播放场数目 */

int     i_frame_num; /* 7.4.3 frame_num */

int     b_kept_as_ref;// 是否作为参考帧

int     i_pic_struct;// 图片结构类型，定义在x264.h中，取值PIC_STRUCT_xxxx

int     b_keyframe;// 是否是关键帧

uint8_t b_fdec;

uint8_t b_last_minigop_bframe; /* this frame is the last b in a sequence of bframes，连续B帧序列中的最后一个B帧 */

uint8_t i_bframes;/* number of bframes following this nonb in coded order，非B帧之后B帧的数目 */

float   f_qp_avg_rc; /* QPs as decided by ratecontrol */

float   f_qp_avg_aq; /* QPs as decided by AQ in addition to ratecontrol */

float   f_crf_avg;   /* Average effective CRF for this frame */

int     i_poc_l0ref0; /* poc of first refframe in L0, used to check if direct temporal is possible */




待续！！！！


} **x264_frame_t**;

/**

 * @brief synchronized frame list

 */ 


typedef struct

{

x264_frame_t **list;

int   i_max_size;// 同步帧列表中最大帧数

int   i_size;// 同步帧列表中帧的数目

x264_pthread_mutex_t    mutex;

x264_pthread_cond_t      cv_fill;  /* event signaling that the list became fuller */

x264_pthread_cond_t      cv_empty; /* event signaling that the list became emptier */

} **x264_sync_frame_list_t**;












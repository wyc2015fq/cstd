
#ifndef _IRIS_H_
#define _IRIS_H_

// 虹膜归一化大小
#define IRIS_H  30
#define IRIS_W  360
#define IRIS_S  (IRIS_W/8)
#define IRIS_BLOCK_H  16
#define IRIS_BLOCK_W  (64/8)

#define IRIS_BLOCK_TY  2
#define IRIS_BLOCK_TX  1

#define IRIS_BLOCK_NUM  ( ((IRIS_H-IRIS_BLOCK_H)/IRIS_BLOCK_TY + 1) * ((IRIS_S-IRIS_BLOCK_W)/IRIS_BLOCK_TX + 1) )

//定序图大小
#define IRIS_OMLEN   (IRIS_H * IRIS_S)
#define IRIS_SHIFT1    8
#define IRIS_SHIFT2    16

#define IR_DEBUG         (1<<0)  // 调试模式时使用
#define IR_BOOST_DETECT  (1<<1)  // 使用 boost初检
#define IR_N_DETECT      (1<<2)  // 内圆检测
#define IR_U_DETECT      (1<<3)  // 上眼睑检测
#define IR_W_DETECT      (1<<4)  // 外圆检测
#define IR_D_DETECT      (1<<5)  // 下眼睑检测
#define IR_STDIMG        (1<<6)  // 归一化
#define IR_FEATURE       (1<<7)  // 提取特征
#define IR_RECOG         (1<<8)  // 比对
#define IR_FLIP          (1<<9)  // 比对

// 背景
#define IR_BG_INIT       (1<<10) // 重新初始化
#define IR_BG_UPDATE     (1<<11) // 更新背景

//#define IR_DRAW_DETECT      (1<<10)  // 绘制结果

#define IR_NU_DETECT     (IR_N_DETECT|IR_U_DETECT)
#define IR_NUW_DETECT    (IR_NU_DETECT|IR_W_DETECT)
#define IR_NUWD_DETECT   (IR_NUW_DETECT|IR_D_DETECT)
#define IR_ALL_DETECT    (IR_BOOST_DETECT|IR_NUWD_DETECT)

#define FR_IRIS_DET  IR_ALL_DETECT
#define FR_IRIS_REG  IR_ALL_DETECT|IR_STDIMG|IR_FEATURE
#define FR_IRIS_REC  IR_ALL_DETECT|IR_STDIMG|IR_FEATURE|IR_RECOG


typedef struct irisrecog_t {
  int in[3]; // 内圆 x=n[0] y=n[1] r=n[2]
  int iw[3]; // 外圆
  int iu[3]; // 上眼睑抛物线 y[0] * (x * x) + y[1] * x + y[2]
  int id[3]; // 下眼睑抛物线

  int height, width; // 灰度图片高宽
  int color_type; // 颜色类型 T_GRAY T_UYVY
  uchar* data; // 图像数据在buf的偏移 大小=height * width
  //int std_off; // 归一化图像数据在buf的偏移 大小= IRIS_H * IRIS_W
  uchar* buf;  // 缓冲区偏移量 大小= buflen
  uchar* feature; // 特征缓冲区偏移量 大小=featlen
  uchar* featlib;  // 特征库
  int mode; // 模式选择
  // 虹膜检测结果
  XRECT rc; // boost初检结果 cx==0 表示没检测到
  int blurrate; // 模糊系数：表示眼睛区域模糊程度，越小越模糊（显示出来时要除以 256.00），一般小于26就比较模糊了。
  int cnt_zero; // stdimg 中0点个数
  int featlen;  // 特征长度,mode==0时也能获取featlen，一般不会超过 10000 ,现在用的特征==5376
  int distthd;  // 推荐阈值,由内间最小值统计而来
  int distmax;  // 最大距离,由内间最大值统计而来
  int buflen;   // 缓冲区总长度
  int featnum;  // 特征个数
  int featstep; // 特征步进，必须>=特征长度 featlen(==5376)
  int maxdist;  // 最小距离值
  int maxdist_id; // 最小距离对应的id
  int dbgdata[32]; // 输出调试模式下的数据
  //int bufused; //buf已经使用的长度 buflen-bufused==算法可用的空间
  unsigned char* stdiris;//[IRIS_H*IRIS_W]; // 归一化图片
} irisrecog_t;

void* dsp_malloc( size_t n );
void dsp_free( void* p );
// 给dsp调用
void iris_recog_open();
void iris_recog_close();
int iris_recog_process( void* p0);
// wline线宽
int iris_draw(int h, int w, unsigned char* img, int step, int cn, const irisrecog_t* ir, unsigned long color0, char* s, int isflip, int wline);

#endif // _IRIS_H_

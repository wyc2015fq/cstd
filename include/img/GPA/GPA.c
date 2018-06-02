

#include "chalg.h"
#include "macro.h"
#include "chreg.h"

#define REAL float
#define REG_SAVE()  chreg_regsave("GPAdll.dll")
#ifdef _DEMO
#define REG_CHECK() { }
#else
#define REG_CHECK() if (!chreg_check("GPAdll.dll")) { return 0; }
#endif

/*
%本程序的功能是抑制摄像机抖动(电子稳像)，用到的好像是灰度投影算法(GPA)
%适用于当前测试帧相对于当前参考帧最大平移20个像素(单方向)以内的情况，也就是说测试序列的前后两帧最大平移20个像素(单方向)以内的情况
%而且摄像头与场景不能太近，实质上离得太近的话场景内的前景会移动过快，也就是前后两帧的同一移动物体移动得幅度较大，所以会影响计算运动矢量
%参考文献：
%(1)一种基于灰度投影算法的电子稳像方法，朱娟娟 郭宝龙 冯宗哲
%(2)获取动态图像位移矢量的灰度投影算法的应用,赵红颖，熊经武,中国科学院长春光学精密机械及物理研究所，吉林长春
%(3)提高用于电子稳像的灰度投影算法精度的方法,钟 平，于前洋，王明佳，金 光(中国科学院长春光学精密机械与物理研究所，吉林长春130022)
*/

static REAL sum(int h, int w, const unsigned char* A, int al, int ai, REAL* B, int flag) {
  int i, j;
  REAL s=0;
  assert(2==flag || 1==flag);
  if (2==flag) {
    for (i=0; i<h; ++i) {
      REAL b=0;
      const unsigned char* A2 = A + i * al;
      for (j=0; j<w; ++j, A2+=ai) {
        b += *A2;
      }
      B[i] = b;
      s += b;
    }
  } else if (1==flag) {
    for (j=0; j<w; ++j) {
      B[j] = 0;
    }
    for (i=0; i<h; ++i) {
      const unsigned char* A2 = A + i * al;
      for (j=0; j<w; ++j, A2+=ai) {
        B[j] += *A2;
      }
    }
    for (j=0; j<w; ++j) {
      s += B[j];
    }
  }
  return s;
}

// 对于边缘进行余弦滤波
static int edge_cos(REAL* x, int len, int off) {
  int i;
  for (i=0; i<len; ++i) {
    if ((i<off)||(i>len-off)) {
      x[i] = (REAL)(x[i]*((1+cos(PI*(off-1-i)/off))/2));//%或者改为：refprojrow[i]=refprojrow[i]*(1+cos(pi*(offset_V-1-i)/offset_V/2));哪个效果更好就用哪个，改的话以下类似的地方都改
    }
  }
  return 0;
}

  //% 参考帧的行投影和列投影以及余弦滤波 %
static REAL* proj(int h, int w, const unsigned char* A, int al, int ai, int off, int flag) {
  REAL promean, prosum;
  REAL* pro;
  int i, len=0;
  assert(2==flag || 1==flag);
  len = (2==flag) ? h : w;
  pro = MALLOC(REAL, len);//%存储参考帧的行投影
  FILL(len, pro, 0.f);
  //refprojrow     = sum(referenceframe,2);
  //refprojrowsum  = sum(refprojrow);
  //%**********投影*********%
  prosum = sum(h, w, A, al, ai, pro, flag);
  promean = prosum;
  promean /= len;
  //refprojrow     = refprojrow - refprojrowmean;
  for (i=0; i<len; ++i) {
    pro[i] -= promean;
  }
  //%**********对于边缘进行余弦滤波*********%
  edge_cos(pro, len, off);
  return pro;
}

static int projcmp(int len, REAL* refproj, REAL* curproj, int off) {
  REAL* correlation_row;
  REAL* refproj2;
  int i, j, dy, correlation_len, refproj2_len, min_index;
  correlation_len = 2*off+1;
  refproj2_len = len+2*off+1;
  //%**********互相关运算计算运动矢量*********%
  correlation_row = MALLOC(REAL, correlation_len);
  refproj2 = MALLOC(REAL, refproj2_len);
  //refprojrow = [refproj;extend_row];
  FILL(refproj2_len, refproj2, 0.f);
  FILL(correlation_len, correlation_row, 0.f);
  COPY(len, refproj, refproj2);
  for (j=0; j<correlation_len; ++j) {
    for (i=0; i<len-2*off; ++i) {
      REAL x = refproj2[j+i-1]-curproj[off+i];
      correlation_row[j]=correlation_row[j]+(x*x);
    }
  }
  //[temp,min_index]=min(correlation_row);
  MIN_ELEMENT(correlation_len, correlation_row, min_index);
  SAFEFREE(correlation_row);
  SAFEFREE(refproj2);
  dy=(off+1)-min_index;//%垂直抖动量(正负),即垂直方向运动矢量
  return dy;
}

static int off_copy(int h, int w, const unsigned char* A, int al, unsigned char* B, int bl, int cn, int x, int y) {
  int i, j, k;
  int h0, w0;
  int h1, w1;
  w0 = MAX(0, x), w1 = MIN(w, w+x);
  h0 = MAX(0, y), h1 = MIN(h, h+y);
  for (i=h0; i<h1; ++i) {
    const unsigned char* A0 = A+i*al+w0*cn;
    unsigned char* B0 = B+(i-y)*bl+(w0-x)*cn;
    for (j=w0; j<w1; ++j, A0+=cn, B0+=cn) {
      for (k=0; k<cn; ++k) {
        B0[k] = A0[k];
      }
    }
  }
  return 0;
}

typedef struct GPA {
  int offset_V;// = 20;//%上下边缘去除量，注意：offset_V 和 offset_H要根据实际可能的最大抖动量适当取值，但不能太大，太大的话相关计算时移动位置的候选位置就少，所以会效果反而会降低，取20和30比较折中
  int offset_H;// = 30;//%左右边缘去除量
  int height,width;
  REAL* refprojrow, * refprojcol;
  unsigned char* data;
} GPA;

int GPA_reinit(GPA* g) {
  unsigned char* gry;
  int h = g->height, w = g->width;
  SAFEFREE(g->refprojrow);
  SAFEFREE(g->refprojcol);
  g->offset_V = 20;//%上下边缘去除量，注意：offset_V 和 offset_H要根据实际可能的最大抖动量适当取值，但不能太大，太大的话相关计算时移动位置的候选位置就少，所以会效果反而会降低，取20和30比较折中
  g->offset_H = 30;//%左右边缘去除量
  gry = MALLOC(unsigned char, h*w);
  //colorcvt( h, w, g->data, w*3, 3, gry, w, 1, T_BGR, T_GRAY );
  IMTRANS(h, w, g->data, w*3, 3, gry, w, 1, BGR2GRAY);
  g->refprojrow = proj(h, w, gry, w, 1, g->offset_V, 2);
  g->refprojcol = proj(h, w, gry, w, 1, g->offset_H, 1);
  SAFEFREE(gry);
  return 0;
}

GPA* GPA_new(int height, int width, const unsigned char* reference_frame_bgr, int step) {
  GPA* g;
  //REG_SAVE();
  REG_CHECK();
  g = MALLOC(GPA, 1);
  //int offset_V, offset_H;
  memset(g, 0, sizeof(GPA));
  g->offset_V = (int)(height*0.1);//%上下边缘去除量，注意：offset_V 和 offset_H要根据实际可能的最大抖动量适当取值，但不能太大，太大的话相关计算时移动位置的候选位置就少，所以会效果反而会降低，取20和30比较折中
  g->offset_H = (int)(width*0.1);//%左右边缘去除量
  g->data = MALLOC(unsigned char, height*width*3);
  off_copy(height, width, reference_frame_bgr, step, g->data, width*3, 3, 0, 0);
  g->height = height, g->width = width;
  GPA_reinit(g);
  return g;
}

int GPA_update(GPA* g, const unsigned char* current_frame_bgr, int step, unsigned char* out, int out_step) {
  int dx, dy;
  REAL* curprojrow, *curprojcol;
  unsigned char* gry;
  int h = g->height, w = g->width;
  gry = MALLOC(unsigned char, h*w);
  //colorcvt( h, w, current_frame_bgr, step, 3, gry, w, 1, T_BGR, T_GRAY );
  IMTRANS(h, w, current_frame_bgr, step, 3, gry, w, 1, BGR2GRAY);
  curprojrow = proj(h, w, gry, w, 1, g->offset_V, 2);
  curprojcol = proj(h, w, gry, w, 1, g->offset_H, 1);
  SAFEFREE(gry);
  dy = projcmp(h, g->refprojrow, curprojrow, g->offset_V);
  dx = projcmp(w, g->refprojcol, curprojcol, g->offset_H);
  //printf("%2d, %2d\n", dx, dy);
  off_copy(h, w, current_frame_bgr, step, g->data, w*3, 3, dx, dy);
  if (out) {
    off_copy(h, w, g->data, w*3, out, out_step, 3, 0, 0);
  }
  SAFEFREE(gry);
  SAFEFREE(curprojrow);
  SAFEFREE(curprojcol);
  //GPA_reinit(g);
  return 1;
}

void GPA_free(GPA* g) {
  SAFEFREE(g->data);
  SAFEFREE(g->refprojrow);
  SAFEFREE(g->refprojcol);
  SAFEFREE(g);
}

/*摄像机抖动抑制算法
本算法主要从视觉上抑制抖动(即稳像)，用到的是灰度投影相关算法(GPA)
采用的是灰度图片进行处理，步骤如下：
1) 以开始抖动的第一帧作为初始参考帧
2) 对于参考帧计算归一化的水平投影和垂直投影
3) 对于归一化后的水平和垂直投影进行余弦滤波
4) 获得第二帧/当前帧/下一帧，同样按照步骤1)—3)进行处理
5) 利用参考帧和当前帧的水平投影和垂直投影进行互相关运算来计算运动
矢量(即抖动偏移量dy 和dx),具体算法较简单，见matlab 代码
6) 运动补偿，即将当前帧向参考帧进行平移，平移量为dy 和dx
若dy<0,则当前帧相对于参考帧向上运动了|dy|，即当前帧需要向下平移
|dy|个像素才能与参考帧对齐，超出原图像尺寸的部分被截去，多出来的
部分补0
若dy>0,则当前帧相对于参考帧向下运动了|dy|
若dx<0,则当前帧相对于参考帧向左运动了|dx|
若dx>0,则当前帧相对于参考帧向右运动了|dx|
经过以上的平移便得到当前帧的稳定帧
7) 把当前稳定帧做为下一帧的参考帧重复步骤2)到6)
注意：保存稳定帧时要对稳定帧进行上下左右4 个边缘的截断，也就是说最
后的稳定帧序列要比原始序列的尺寸要小
*/

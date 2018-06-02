
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include <windows.h>

//King Blizzard  Demo - Hss   2002.9
typedef struct CSceneKing2 {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CPointMover* m_MoverList;
  CColorMover* m_ColorMoverList;
  int m_MoverListSize;
  int m_ColorMoverListSize;
  long m_DrawCount;
}
CSceneKing2;


// 几个无符号类型别称
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

// 用到的宏
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// 初始化一个 Direct Draw struct
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }


//===================================
// 公共变量

// 屏幕大小、颜色数
char* Caption = "King Blizzard  Demo - Hss";
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int SCREEN_BPP = 32;  //bit
int ColorSize = 4;  //byte

int iRGB = 0;
__int64 t1, t0, p0; //时间计算用(帧速计算)
__int64 mst0, mst1;  //鼠标自动隐藏用
__int64 msShowTimeCount; //显示时间(QCount)//=5*p0;
int msShow = 1; //鼠标是否可见


unsigned int* myData;          //图像数据
CPointMover ca1, ca2, cb1, cb2, cc, cd, ce, cf; //参数
double vv = 0.07;    //总体速度
//double vvSpeed=50;          //总体速度调节值
double sLeft, sTop, sRight, sButoon;  //可视区域
double sx0, sy0, sz0, sxn, syn, szn;    //迭代变量
int DrawCount;       //绘点数
int SSe1, SSe2, SSe3;  //基颜色
int LightSSe1, LightSSe2;
int LightSSew1, LightSSew2;

double Kse1, Kse2, Kse3;  //颜色变化强度
int ColorTwo = 415, ColorOne = 205;
int PicCount = 0;        //帧计数
double fps;     //帧速
int SCRRUN = 0; //是否按屏保运行
int ColorM[ 50 * 50 * 4 ]; //绘制星时使用的数组

int SelectSpeed;

//==========
void DrawLight(CSceneKing2* ssf, UCHAR* dst, int px0, int py0, int dst_step, int WidthDiv2, int SSe)
{
  //return;

  //const double PI=3.1415926;
  int px, py;

  //int *ColorM=new int [(WidthDiv2+1)*(WidthDiv2+1)];
  //*
  for (py = py0; py <= py0 + WidthDiv2; py++) {
    for (px = px0; px <= px0 + WidthDiv2; px++) {
      if ((2 <= px) && (px < SCREEN_WIDTH - 2) && (2 <= py) && (py < SCREEN_HEIGHT - 2)) {
        //圆环
        double xx = abs(px - px0);
        double yy = abs(py - py0);
        double cr = sqrt(xx * xx + yy * yy);
        int index = (px - px0) + (py - py0) * (WidthDiv2 + 1);
        double myc;
        double cpi = (10.0 * cr / WidthDiv2 - 3) * PI;

        int rWidthDiv2;
        double CR;

        if ((0 <= cpi) && (cpi <= PI)) {
          myc = sin(cpi);
          ColorM[ index ] = (int)(255 * myc) >> 3;
        }
        else {
          ColorM[ index ] = 0;
        }

        //闪光

        rWidthDiv2 = WidthDiv2;
        CR = sqrt(2.0 * (rWidthDiv2 * rWidthDiv2));
        xx = rWidthDiv2 - xx;
        yy = rWidthDiv2 - yy;
        cr = sqrt(xx * xx + yy * yy);

        if ((cr >= rWidthDiv2)) {
          int b;
          myc = (cr - rWidthDiv2) / (CR - rWidthDiv2);
          b = ((int)(255 * myc) + ColorM[ index ]);

          if (b > 255) {
            b = 255;
          }

          ColorM[ index ] = b;
        }

      }
    }
  }

  //*/
  for (py = py0 - WidthDiv2; py <= py0 + WidthDiv2; py++) {
    for (px = px0 - WidthDiv2; px <= px0 + WidthDiv2; px++) {
      int co = ColorM[ abs(px - px0) + abs(py - py0) * (WidthDiv2 + 1) ];

      if ((co > 0) && (2 <= px) && (px < SCREEN_WIDTH - 2) && (2 <= py) && (py < SCREEN_HEIGHT - 2)) {
        int r, g, b;
        double myc = co / 255.0;
        int absxy = (abs(px - px0) + abs(py - py0)) * 5;
#define DGetPixel(dst,dst_step,px,py,r,g,b) GETPIX3(dst+py*dst_step+px*3,r,g,b)
#define DSetPixel(dst,dst_step,px,py,r,g,b) PUTPIX3(dst+py*dst_step+px*3,r,g,b)

        DGetPixel(dst, dst_step, px, py, r, g, b);
        b = ((int)((255 - abs((absxy + SSe + SSe1) % ColorTwo - ColorOne)) * myc) + b) * 3 >> 1;
        g = ((int)((255 - abs((absxy + SSe + SSe2) % ColorTwo - ColorOne)) * myc) + g) * 3 >> 1;
        r = ((int)((255 - abs((absxy + SSe + SSe3) % ColorTwo - ColorOne)) * myc) + r) * 3 >> 1;

        if (b > 255) {
          b = 255;
        }

        if (g > 255) {
          g = 255;
        }

        if (r > 255) {
          r = 255;
        }

        DSetPixel(dst, dst_step, px, py, r, g, b);
      }
    }
  }

  //delete ColorM;

}

///////////////////////////////////////////////////////////
#define MyMemset memset
//产生下一帧图像
//缓冲表面指针，像素宽，像素高，数据字节宽度，颜色数据位数（字节）
int CSceneKing2DoDraw(CSceneKing2* ssf, int dst_height, int dst_width, UCHAR* dst, int dst_step, int cn)
{
  //*
  // 清除缓冲表面
  {
    int x = 0, y = 0;

    //填充0
    if (dst_step == dst_width * cn) {
      MyMemset(dst, 0, dst_height * dst_step);
    }
    else {
      // 非线性内存

      // 缓冲表面指针
      UCHAR* dest_ptr = dst;

      // 一行一行的清除数据
      for (y = 0; y < dst_height; y++) {
        // 清除一行
        MyMemset(dest_ptr, 0, dst_width * cn);
        // 指针指向下一行
        dest_ptr += dst_step;

      } // end for y

    } // end else
  }

  //*/
  /*
  {

    for (int y=0; y<dst_height; y++)
   {
     //for (int x=0; x<dst_width; x++)
    {
    int r=255,g=80,b=160;
       //DLine(dst,dst_step,x,0,x,dst_height,r-x,g-x,b-x);
       DLine(dst,dst_step,0,y,dst_width,y,r,g,b);
    //DLineHorizontal(dst,dst_step,0,y,dst_width,r,g,b);
    //DLineVertical(dst,dst_step,x,0,dst_height,r,g,b);
    //DSetPixel(dst,dst_step,x,y,r,g,b);
      //DGetPixel(dst,dst_step,x,y,rgb);
    //DSetPixel(dst,dst_step,x,y,rgb);

    //DGetPixel(dst,dst_step,x,y,rgb);
    } // end for x
   } // end for y
  }
  //*/
  /*
   //过渡
   for (int y=0 ;y<dst_height;y++)
   {
    for (int x=0 ;x<dst_width;x++)
    {
     int r,g,b,rgb;
     rgb=myData[x+y*dst_width];
     b=rgb>>24;
     if (b>5) b=b-5; else b=0;
     g=rgb<<8>>24;
     if (g>5) g=g-5; else g=0;
     r=rgb<<16>>24;
     if (r>5) r=r-5; else r=0;
     myData[x+y*dst_width] = (r | (g << 8) | (b << 16))<<8;
    }
   }
  */

  /*
  使用的方程:
  x_(n+1)=sin(b*y_n)+c*sin(b*x_n),
  y_(n+1)=sin(a*x_n)+d*sin(a*y_n),

  x:=0.1;y:=0.1;
  a:=-0.9666918; {-0.97}
  b:=2.679879; {2.8}
  c:=0.565145; {0.45,0.76}
  d:=0.744728; {0.71}
  {-1.86＜x＜1.86}; {-1.51＜y＜1.51};

  //================
  x_(n+1)=sin(a*y_n)-z*cos(b*x_n),
  y_(n+1)=z*sin(c*x_n)-cos(d*y_n),
  z_(n+1)=e*sin(x_n).

  a=2.24,b=0.43,c=-0.65,d=-2.43，0.5<e<1.0

    */
  {
    int sCount = 200;
    int sCountdiv2 = sCount / 2;
    double pi = PI;
    double pi2 = pi * 2;
    double seta = 0;
    double rr = 1;
    int px0 = -1, py0 = -1;
    int px00 = -1, py00 = -1;
    int i, j;
    double Light_x0, Light_y0, Light_x1, Light_y1, Light_x2, Light_y2; //灯
    int wi = 0;
    double lightW;
    double fv;

    //DrawCount=0;
    for (i = 0; i < sCount; i++) {
      int px, py;

      if (i < sCountdiv2) {
        seta = i * pi2 / sCountdiv2;
        sx0 = rr * sin(seta);
        sy0 = rr * cos(seta);
      }
      else {
        sx0 = (double)(i - sCountdiv2) * 2 / sCountdiv2 + 0.00 - 1;
        sy0 = (double)(i - sCountdiv2) * 2 / sCountdiv2 + 0.05 - 1;
      }

      for (j = 0; j * sCount < DrawCount; j++) {

        //sxn=sin(ca*sy0)+sz0*cos(cb*sx0);
        //syn=sz0*sin(cc*sx0)-cos(cd*sy0);
        //szn=ce*sin(sx0);

        sxn = ce.m_Value * sin(cb1.m_Value * sy0) + cc.m_Value * sin(cb2.m_Value * sx0);
        syn = cf.m_Value * sin(ca1.m_Value * sx0) + cd.m_Value * sin(ca2.m_Value * sy0);

        px = (int)(dst_width * (sxn - sLeft) / (sRight - sLeft));
        py = (int)(dst_height * (syn - sTop) / (sButoon - sTop));

        if ((2 <= px) && (px < dst_width - 2) && (2 <= py) && (py < dst_height - 2)) {
          {
            double ax, ay;
            int r, g, b;
            int index = px * cn + py * dst_step;
            ax = fabs(sxn - sx0);
            ay = fabs(syn - sy0);
            //ax=sqrt(ax*ax+ay*ay);
            //ay=ax;
            b = 255 - abs(abs(SSe3 - (int)(Kse3 * (ax + ay))) % ColorTwo - ColorOne);
            g = 255 - abs(abs(SSe2 - (int)(Kse2 * ay)) % ColorTwo - ColorOne);
            r = 255 - abs(abs(SSe1 - (int)(Kse1 * ax)) % ColorTwo - ColorOne);

            //DClipSetPixel(&rect,dst,dst_step,px,py,r,g,b);
            //DRgnSetPixel(rgn,dst,dst_step,px,py,r,g,b);
            //DSetPixel(dst,dst_step,px,py,r,g,b);
            if (px00 > 0) {
              //DRgnLine(rgn,dst,dst_step,px00,py00,px,py,r,g,b);
              //DRgnLine(rgn,dst,dst_step,px0,py0,px,py,r,g,b);
              //DClipLine(&rect,dst,dst_step,px00,py00,px,py,r,g,b);
              //DClipLine(&rect,dst,dst_step,px0,py0,px,py,r,g,b);
              //DLine(dst,dst_step,px00,py00,px,py,r,g,b);
              //DLine(dst,dst_step,px0,py0,px,py,r,g,b);
              unsigned char* pix = dst + px * cn + py * dst_step;
              PUTPIX3(pix, r, g, b);
            }

            px00 = px0;
            py00 = py0;
            px0 = px;
            py0 = py;
            //DLineHorizontal(dst,dst_step,px,py,px+100,r,g,b);
            //DLineVertical(dst,dst_step,px,py,py+100,r,g,b);
          }
        }

        sx0 = sxn;
        sy0 = syn;
        //sz0=szn;
      } //end for j
    } //end for i

    //绘灯
    LightSSe1 = (LightSSe1 + 7 + rand() % 8) % ColorTwo;
    LightSSe2 = (LightSSe2 + 8 + rand() % 8) % ColorTwo;
    LightSSew1 = (LightSSew1 + 3 + rand() % 4) % ColorTwo;
    LightSSew2 = (LightSSew2 + 4 + rand() % 4) % ColorTwo;

    Light_x1 = (double)(sCountdiv2 - sCountdiv2) * 2 / sCountdiv2 + 0.00 - 1;
    Light_y1 = (double)(sCountdiv2 - sCountdiv2) * 2 / sCountdiv2 + 0.05 - 1;
    Light_x2 = (double)(sCount - sCountdiv2) * 2 / sCountdiv2 + 0.00 - 1;
    Light_y2 = (double)(sCount - sCountdiv2) * 2 / sCountdiv2 + 0.05 - 1;

    for (lightW = 24; lightW > 0.3; lightW = (lightW * 17) / 18) {
      int px, py;

      wi++;
      Light_x0 = Light_x1;
      Light_y0 = Light_y1;
      Light_x1 = ce.m_Value * sin(cb1.m_Value * Light_y0) + cc.m_Value * sin(cb2.m_Value * Light_x0);
      Light_y1 = cf.m_Value * sin(ca1.m_Value * Light_x0) + cd.m_Value * sin(ca2.m_Value * Light_y0);
      Light_x0 = Light_x2;
      Light_y0 = Light_y2;
      Light_x2 = ce.m_Value * sin(cb1.m_Value * Light_y0) + cc.m_Value * sin(cb2.m_Value * Light_x0);
      Light_y2 = cf.m_Value * sin(ca1.m_Value * Light_x0) + cd.m_Value * sin(ca2.m_Value * Light_y0);

      px = (int)(dst_width * (Light_x1 - sLeft) / (sRight - sLeft));
      py = (int)(dst_height * (Light_y1 - sTop) / (sButoon - sTop));
      DrawLight(ssf, dst, px, py, dst_step, (int)(lightW * (fabs(abs(wi * 12641 + LightSSe2) % ColorTwo - ColorOne) / (double) ColorOne)), LightSSew1 + wi * 1013);
      px = (int)(dst_width * (Light_x2 - sLeft) / (sRight - sLeft));
      py = (int)(dst_height * (Light_y2 - sTop) / (sButoon - sTop));
      DrawLight(ssf, dst, px, py, dst_step, (int)(lightW * (fabs(abs(wi * 15675 + LightSSe1) % ColorTwo - ColorOne) / (double) ColorOne)), LightSSew1 + wi * 1321);
    }

    SSe1 = (SSe1 + 2) % ColorTwo;
    SSe2 = (SSe2 + 1) % ColorTwo;
    SSe3 = (SSe3 + 3) % ColorTwo;

    if ((fps > 0.0) && (fps < 1000.0)) {
      fv = vv * (exp(log(20.0) / 50.0 * (SelectSpeed - 50.0)) * 18 / fps);
    }
    else {
      fv = vv * (exp(log(20.0) / 50.0 * (SelectSpeed - 50.0)));
    }

#define GetNextCV(pm, vvv) CPointMoverGetNextCV(&pm, vvv)

    GetNextCV(ca1, fv);
    GetNextCV(ca2, fv);
    GetNextCV(cb1, fv);
    GetNextCV(cb2, fv);
    GetNextCV(cc, fv);
    GetNextCV(cd, fv);
    GetNextCV(ce, fv);
    GetNextCV(cf, fv);


    PicCount++;
  }
  return (1);
}


void CSceneKing2Update(CSceneKing2* pf0, unsigned long StepTime_ms) {}

void CSceneKing2Clear(CSceneKing2* pf0)
{
  SAFEFREE(pf0);
}

CSceneKing2* CSceneKing2Inti()
{
  CSceneKing2* ssf;
  //初始化
  ssf = MALLOC(CSceneKing2, 1);

  //QueryPerformanceFrequency(p0);
  //QueryPerformanceCounter(t0);
  //QueryPerformanceCounter(t1);
  //QueryPerformanceCounter(mst0);
  //QueryPerformanceCounter(mst1);
  msShowTimeCount = 5 * p0;

  PicCount = 0;

#define SetLU(CA1, _L, _U)  CPointMoverInti(&CA1, _L, _U, 0., 0.)

  SetLU(ca1, -0.5, -1.5);
  SetLU(ca2, -0.5, -1.5);
  SetLU(cb1, 4, -3.5);
  SetLU(cb2, 4, -3.5);
  SetLU(cc, 1, -1);
  SetLU(cd, 1, -1);
  SetLU(ce, 1.1, 0.9);
  SetLU(cf, 1.1, 0.9);
  CPointMoverGetRandomValue(&ca1);
  CPointMoverGetRandomValue(&ca2);
  CPointMoverGetRandomValue(&cb1);
  CPointMoverGetRandomValue(&cb2);
  CPointMoverGetRandomValue(&cc);
  CPointMoverGetRandomValue(&ce);
  CPointMoverGetRandomValue(&cf);
  CPointMoverGetRandomV(&ca1);
  CPointMoverGetRandomV(&ca2);
  CPointMoverGetRandomV(&cb1);
  CPointMoverGetRandomV(&cb2);
  CPointMoverGetRandomV(&cc);
  CPointMoverGetRandomV(&ce);
  CPointMoverGetRandomV(&cf);
  //vv=0.03;

  sLeft = -2;
  sRight = 2;
  sTop = -2;
  sButoon = 2;
  SSe1 = rand() * 255 / RAND_MAX;
  SSe2 = rand() * 255 / RAND_MAX;
  SSe3 = rand() * 255 / RAND_MAX;
  LightSSe1 = rand() * 255 / RAND_MAX;
  LightSSe2 = rand() * 255 / RAND_MAX;
  LightSSew1 = rand() * 255 / RAND_MAX;
  LightSSew2 = rand() * 255 / RAND_MAX;

  Kse1 = 100;
  Kse2 = 100;
  Kse3 = 100;

  DrawCount = 5000;
  SCENE_FRACTAL_SET_CLASS(ssf, CSceneKing2);
  return (ssf);
}



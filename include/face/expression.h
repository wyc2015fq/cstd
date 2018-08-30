
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#ifdef __cplusplus
extern "C" {;
#endif

enum { ET_EFFECT1 = 0x01,   // 哈哈镜
       ET_EFFECT2 = 0x02,   // 面具
       ET_EFFECT3 = 0x04,   // 加前景
       ET_EFFECT4 = 0x08,   // 玻璃飞溅
       ET_EFFECT5 = 0x10,   // 马赛克
       ET_EFFECT6 = 0x20,   // 风化效果
       ET_EFFECT7 = 0x40,   // 带状效果
       ET_EFFECT8 = 0x80    // 明星脸面具，必须指定一张明星人脸图片
};

typedef struct tagEFFECTA {
  int height, width;     // 图像的高宽
  int type;              // 可以是 ET_EFFECT1|ET_EFFECT2|ET_EFFECT3的任意组和
  int picidx;
  int splash_radius;     // 玻璃飞溅半径
  int mosaic_size;       // 马赛克大小
  int windy_step;        // 风化程度
  int stripe_swing;      // 带状振幅[0..100]
  int stripe_frequency;  // 带状频率 >=0
  float eyes_sphere;     // 眼睛球面化参数[-1..1]
  float face_sphere;     // 人脸球面化参数[-1..1]
  CvPoint leye, reye;
  char picspec[ 256 ];
  int fg_h;              // 前景图片高
  int fg_w;              // 前景图片宽
  int fg_cn;             // 前景图片通道数
  unsigned char* foreground;  // 前景图片
  int starface_height, starface_width;
  unsigned char* starface;  // 明星脸图片
}
EFFECTA;
typedef struct tagEFFECTW {
  int height, width;     // 图像的高宽
  int type;              // 可以是 ET_EFFECT1|ET_EFFECT2|ET_EFFECT3的任意组和
  int picidx;
  int splash_radius;     // 玻璃飞溅半径
  int mosaic_size;       // 马赛克大小
  int windy_step;        // 风化程度
  int stripe_swing;      // 带状振幅[0..100]
  int stripe_frequency;  // 带状频率 >=0
  float eyes_sphere;     // 眼睛球面化参数[-1..1]
  float face_sphere;     // 人脸球面化参数[-1..1]
  CvPoint leye, reye;
  wchar_t picspec[ 256 ];
  int fg_h;              // 前景图片高
  int fg_w;              // 前景图片宽
  int fg_cn;             // 前景图片通道数
  unsigned char* foreground;     // 前景图片
  int starface_height, starface_width;
  unsigned char* starface;  // 明星脸图片
}
EFFECTW;

#ifdef UNICODE
#define EFFECT  EFFECTW
#define PEFFECT  EFFECTW*
#else
#define EFFECT  EFFECTA
#define PEFFECT  EFFECTA*
#endif // !UNICODE
/*
使用例子：
PEFFECT expr = new_expression ();
config_expression (expr, ET_EFFECT3, _T ("配置文件.ini"), _T ("相框1"));
add_expression (expr, h, w, img, al, ai);
del_expression (&expr); // 使用完了要释放
assert (NULL==expr);    // 确保指针为空
*/ 
// 分配表情结构
EFFECTA* new_expressionA(int height, int width);
EFFECTW* new_expressionW(int height, int width);

// 释放表情结构
// pexpr               - 指向PEFFECT结构的指针
void del_expressionA( EFFECTA** pexpr );
void del_expressionW( EFFECTW** pexpr );

// 配置表情
// expr                - 指向EFFECT结构的指针
// type                - 可以是 ET_EFFECT1|ET_EFFECT2|ET_EFFECT3的任意组和
// inifile             - ini文件路径
// effect_name         - ini文件方括号中的字符串
void config_expressionA( EFFECTA* expr, int type, const char* inifile, const char* effect_name );
void config_expressionW( EFFECTW* expr, int type, const wchar_t* inifile, const wchar_t* effect_name );
// 加表情
// expr                - 指向EFFECT结构的指针
// [h, w, img, al, ai] - 图片[高,宽,左上角指针,步进数,通道数]
int add_expressionA( EFFECTA* expr, unsigned char * img, int al, int ai );
int add_expressionW( EFFECTW* expr, unsigned char * img, int al, int ai );
#ifdef UNICODE
#define new_expression new_expressionW
#define del_expression del_expressionW
#define config_expression  config_expressionW
#define add_expression  add_expressionW
#else
#define new_expression new_expressionA
#define del_expression del_expressionA
#define config_expression  config_expressionA
#define add_expression  add_expressionA
#endif // !UNICODE


#ifdef __cplusplus
}
#endif

#endif // _EXPRESSION_H_

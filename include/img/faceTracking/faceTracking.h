
#ifndef _FACETRACKING_H_
#define _FACETRACKING_H_

#ifdef __cplusplus
extern "C" {
#endif

  struct XRECT;
  
  typedef struct TRACKER {
    size_t count;   // 矩形框数目
    size_t maxcount; // 最大矩形框容量
    size_t baseid; // 最小矩形框id号
    real zoom; // 缩放系数
    real mindist; // 合并最小覆盖率
    real minarea; // 最小面积
    real ssmin, ss, thd;
    int stepxy, mincnt;   // 检测参数
    real** phist; // 直方图指针
    PXRECT ptrack; // 被跟踪的矩形框组
  }
  TRACKER, *PTRACKER;
  
  // 创建跟踪类
  // maxcount - 最大矩形框容量（就是最大可跟踪的目标数）
  PTRACKER new_tracker( size_t maxcount );
  
  // 释放跟踪类
  void free_tracker( PTRACKER tk );
  
  //
  // [h, w, A, al, ai] - BGR彩色图片[高, 宽, 左上角指针, 列步进, 行步进]
  // tk  - new_tracker 创建的跟踪类指针
  // cas - 人脸分类器
  // mark, mark_step -掩码指针,掩码step,
  // detectface - 是否作全局检测 1=检测 0=不检测
  int camtrack( int h, int w, unsigned char * img, int al, int ai,
                unsigned char * mark, int mark_step, PTRACKER tk,
                const void * cas, int detectface );
                
#ifdef __cplusplus
}
#endif

#endif // _FACETRACKING_H_

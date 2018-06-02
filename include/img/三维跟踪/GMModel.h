#ifndef _GMMODEL_H_
#define _GMMODEL_H_
#include <stdlib.h>
#include "sort.h"
struct BlobRect {
  int l,t,r,b; //左上右下
  int n;
};
// 高斯背景建模
struct GMM {
public:
  double afa;               // 用于调整背景模型更新速度的参数
  double thrd;              // 前景判定时的权重阈值
  double vir_v;             // 初始化方差值
  double vir_w;             // 初始化权重值
  double shadowp[ 4 ];      // 第二类阴影修正方法所使用的参数
  int row;                  // 场景像素高度
  int col;                  // 场景像素宽度
  int kind;                 // 背景模型个数
  unsigned int min_pts;     // 根据团块大小决定团块是否去除所使用的阈值
  bool rlabel;              // 去除小团块修正方法的开关
  bool rshadowx;            // 第一类阴影修正方法开关
  bool rshadowy;            // 第二类阴影修正方法开关
  bool rshape;              // 几何修正方法开关
  int *s;
  double *w;
  double *m;
  double *v;
  double *b;
  //  blob 团块分类之后的结果
  // 图像尺寸与设置的检测高宽同尺寸
  // 输出结果为已经做过连通区域的无符号整型图像, 0值像素表示背景, 大于0表示前景
  unsigned int *blob;       // 团块分类之后的结果
  unsigned char *label;
  unsigned char* bk;
public:
  // 当前背景图像
  const double* Background( void ) const {
    return b;
  }
  // 连通区域结果
  const unsigned int* BlobRst( void ) const {
    return blob;
  }
  // 场景高度
  int SceneH( void ) const {
    return row;
  }
  // 场景宽度
  int SceneW( void ) const {
    return col;
  }
public:
  int Init( double Afa = 0.005,
       double Thd = 0.4,
       double VirV = 5.0,
       double VirW = 0.05,
       double shadowp0 = 0.95,
       double shadowp1 = 0.3,
       double shadowp2 = 0.3,
       double shadowp3 = 0.1,
       int R = 120,
       int C = 160,
       int Num = 3,
       int MinPts = 20,
       bool IsRectLabel = true,
       bool IsRectShadowX = true,
       bool IsRectShadowY = false,
       bool IsRectShape = true
     ) {
    afa =( Afa ),
      thrd =( Thd ),
      vir_v=( VirV ),
      vir_w=( VirW ),
      row =( R ),
      col =( C ),
      kind=( Num ),
      min_pts=( MinPts ),
      s=( new int[ row * col ] ),
      w=( new double[ row * col * kind ] ),
      m=( new double[ row * col * kind * 3 ] ),
      v=( new double[ row * col * kind ] ),
      b=( new double[ row * col * 3 ] ),
      blob=( new unsigned int[ col * row ] ),
      label=( new unsigned char[ row * col ] ),
      rlabel =( IsRectLabel ),
      rshadowx=( IsRectShadowX ),
      rshadowy=( IsRectShadowY ),
      rshape =( IsRectShape );
    *( shadowp + 0 ) = shadowp0;
    *( shadowp + 1 ) = shadowp1;
    *( shadowp + 2 ) = shadowp2;
    *( shadowp + 3 ) = shadowp3;
    bk = new uchar[row * col * 3];
    for ( int i = 0; i < row * col * kind; ++i ) {
      w[i] = 0.0;
      v[i] = 0.0;
    }
    for ( i = 0; i < row * col * kind * 3; ++i ) {
      m[i] = 0.0;
    }
    for ( i = 0; i < row * col * 3; ++i ) {
      b[i] = 0.0;
    }
    for ( i = 0; i < row * col; ++i ) {
      blob[i] = 0;
    }
    SetS();
    return 0;
  }
  ~GMM() {
    UnInit();
  }
  int UnInit() {
    delete [] label;
    label = NULL;
    delete [] blob;
    blob = NULL;
    delete [] b;
    b = NULL;
    delete [] v;
    w = NULL;
    delete [] m;
    m = NULL;
    delete [] w;
    v = NULL;
    delete [] s;
    s = NULL;
    delete [] bk;
    bk = NULL;
    return 0;
  }
  // 初始化任意点当前已有的类别数，每一次重新建立背景模型都必须调用此函数
  void SetS( void ) {
    for ( int i = 0; i < row * col; ++i ) {
      *( s + i ) = 1;
    }
  }
  BlobRect br[100];
  int GetRect() {
    int i, j, n=0;
    memset(br, 0, sizeof(BlobRect)*100);
    for (i=0; i<row; ++i) {
      for (j=0; j<col; ++j) {
        int k=blob[i*col+j];
        if (k<100) {
          if (0==br[k].n) {
            br[k].l=br[k].r=j;
            br[k].t=br[k].b=i;
          } else {
            br[k].l=MIN(j, br[k].l);
            br[k].r=MAX(j, br[k].r);
            br[k].t=MIN(i, br[k].t);
            br[k].b=MAX(i, br[k].b);
          }
          br[k].n++;
          n=MAX(n, k);
        }
      }
    }
    ++n;
#define BlobRectCmp(x, y)  (x).n>(y).n
    QSORT(br, br+n, BlobRectCmp, BlobRect);
    return n;
  }
  unsigned char* GetBk() {
    int i, n=row*col*3;
    for (i=0; i<n; ++i) {
      bk[i]=(unsigned char)(b[i]);
    }
    return bk;
  }
  // 背景建模函数，建立背景模型时每帧图片调用一次该函数
  void SetupBG( const unsigned char* src, int step, int num );
  // 将背景建模权重, 均值, 方差结果导出到文本文档
  void Export2File( const char* wf, const char* mf, const char* vf ) const;
  // 前景检测和后期处理, 返回检测结果个数
  int Detect( const unsigned char* src, int step, const unsigned char *pmask = NULL );
  // 计算最大可能背景的函数，每次检测前必须调用
  void GetIb( void );
  // 前景检测
  void DetectForeGround( const unsigned char* pt, int step, const unsigned char *pmask = NULL );
  // 空洞填补修正
  void RectifyLabel( void );
  // 光照突变误检和阴影误检修正
  void Rectify( const unsigned char *pt, int step );
  // 腐蚀, 膨胀修正
  int RectifyShape( void );
};
#endif

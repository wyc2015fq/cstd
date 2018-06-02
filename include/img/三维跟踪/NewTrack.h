#ifndef _TRI_DIM_TRACK_
#define _TRI_DIM_TRACK_
#ifdef __cplusplus
extern "C" {;
#endif
// 整型点, 浮点型点类型.
struct IPoint {
  int x;
  int y;
};
struct FPoint {
  double x;
  double y;
};
// 标定类型前向声明.
struct Calibration;
struct GMM;
struct TriDimModel;
struct TriDimTracker;
typedef unsigned char uchar;
//  1, 三维场景标定.  1.0 建立三维场景标定对象. 函数返回一个指向三维场景标定对象的指针, 创建失败则返回空指针.
// * 该函数创建的对象使用完毕后必须调用函数ReleaseCbrModel删除该对象.
// * refPts    : 标定线段端点图像坐标对. 一般为5条竖直线和2组地面上的平行线共18个点.
// * refHeight : 标定参照物的实际高度, 单位为厘米(cm).
// * vLineNum  : 竖直线条数.
// * hLineNum  : 水平线条数.
// * SceneWidth: 图像像素宽度.
Calibration* CHC_CreateCbrModel( const IPoint * refPts, double refHeight, int vLineNum, int hLineNum, int SceneHeight );
//  1.1 检查标定完成状况. 函数返回0表示标定成功.
// * 返回值说明
// *  0: 标定成功.
// * -1: 内存分配失败.
// * -2: 竖直线中出现平行情况.
// * -3: 所有竖直直线的交点的y坐标都小于0, 全部被忽略.
// * -4: 水平线对中出现平行情况.
// * -5: 计算中出现对负数开方的情况.
// * pCbr: 指向场景标定对象的指针.
int CHC_CheckCbrStatus( const Calibration * pCbr );
//  1.2 地面线段实际长度验证, 函数返回两点间的实际距离.
// * 输入地面上两点在图像上的像素坐标, 计算该两点间的实际距离.
// * pCbr : 指向三维场景标定对象的指针.
// * begin: 地面上的线段的起点在图像上的像素坐标.
// * end  : 地面上的线段的终点在图像上的像素坐标.
double CHC_ValidateLen( const Calibration * pCbr, const IPoint * begin, const IPoint * end );
//  1.3 目标像素高度验证. 函数返回目标顶部像素坐标.
// * 输入目标的实际高度和目标的底部(脚)在图像上的像素坐标, 计算目标的顶部(头)在图像上的像素坐标.
// * pCbr      : 指向三维场景标定对象的指针.
// * tail      : 目标底部在图像上的像素坐标.
// * realHeight: 目标的实际高度.
FPoint CHC_ValidateHeadPos( const Calibration * pCbr, const IPoint * tail, int realHeight );
//  1.4 删除三维场景标定对象.
// * 注意输入是指向标定对象的指针的指针.
// * pCbr: 指向标定对象指针的指针.
void CHC_ReleaseCbrModel( Calibration **pCbr );
// 2, 人体三维模型建立. 
// 2.0 建立人体三维模型对象. 建立的对象不再使用时必须调用函数CHC_ReleaseTDModel释放.
// * 建立人体三维模型对象, 用于根据跟踪对象的大小判定其类型(人, 车..), 判定细节已经封装在三维跟踪类中.
// * h      : 场景像素高度.
// * w      : 场景像素宽度.
// * subH   : 场景平均分割时, 竖直方向上平均划分的子块数目(场景被平均划分为subH * sumW块并在各个子块建立相应的三维人体模型).
// * subW   : 场景平均分割时, 水平方向上平均划分的子块数目.
// * pCbr   : 三维标定对象指针(该指针指向的对象已经在第一步中创建).
// * 以下参数都会根据实际情况给出建议值.
// * sampNum: 模型圆柱上的抽样点的个数.
// * rLow   : 腿部圆柱模型半径.
// * rMid   : 身体圆柱模型半径.
// * rTop   : 头部圆柱模型半径.
// * hLow   : 腿部像素高度.
// * hMid   : 身体像素高度.
// * hTop   : 头部像素高度.
TriDimModel* CHC_CreateTDModel( int h,int w,int subH,int subW,Calibration * pCbr,int sampNum,int rLow,int rMid,int rTop,int hLow,int hMid,int hTop );
// 2.1 销毁人体三维模型对象.
// pTdm: 指向三维人体模型对象的指针的指针.
void CHC_ReleaseTDModel( TriDimModel **pTdm );
//  4, 跟踪.  4.0 创建跟踪对象. 函数失败时返回空指针.
// * 跟踪对象不再需要使用时必须调用函数CHC_ReleaseTDTModel释放该对象.
// * h        : 场景像素高度.
// * w        : 场景像素宽度.
// * maxObjNum: 最大轨迹处理条数.
TriDimTracker* CHC_CreateTDTModel( int h, int w, int maxObjNum );
//  4.1 删除跟踪对象.
// * 注意传入的参数是指向跟踪对象指针的指针.
// * pTdt: 指向跟踪对象指针的指针.
void CHC_ReleaseTDTModel( TriDimTracker **pTdt );
//  4.2 跟踪处理.
// * 参数说明
// * pTdt: 指向跟踪对象的指针.
// * src : 需要做跟踪处理的原始RGB图像数据.
// * pGmm: 高斯背景建模对象指针.
// * num : 当前帧团块检测结果个数, 该值由函数CHC_ForegroundDetect返回.
// * pCbr: 三维标定对象指针.
// * pTdm: 人体三维模型对象指针.
void CHC_TrackProc( TriDimTracker * pTdt, const uchar * src, const GMM * pGmm, int num, const Calibration * pCbr, const TriDimModel * pTdm );
//  4.3 得到存放轨迹的数组中所包含的位置个数.
// * 得到三维跟踪对象中存放轨迹的数组的大小, 即最多可以存放(处理)多少条轨迹, 与创建三维跟踪对象时的maxObjNum值相等.
// * pTdt: 指向跟踪对象的指针.
int CHC_GetCellNum( const TriDimTracker * pTdt );
//  4.4 得到当前轨迹条数. 函数返回正在被处理中的轨迹的条数(返回值 <= maxObjNum).
// * 参数说明
// * pTdt: 指向跟踪对象的指针.
int CHC_GetTraceNum( const TriDimTracker * pTdt );
//  4.5 轨迹数组中第i个位置上是否存放了轨迹.
// * 检测轨迹数组中第i个位置上是否存放了正在被处理的轨迹, 存放了轨迹返回1, 否则返回0.
// * pTdt: 指向三维跟踪对象的指针.
// * i   : 轨迹位置索引.
int CHC_IsExistTrace( const TriDimTracker * pTdt, int i );
//  4.6 轨迹数组中第i个位置上的轨迹的标识.
// * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的标识, 标识使用一个整形数表示.
// * 同函数CHC_IsExistTrace.
int CHC_GetTraceIdentity( const TriDimTracker * pTdt, int i );
//  4.7 轨迹数组中第i个位置上的轨迹的状态.
// * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的状态.
// * 同函数CHC_IsExistTrace.
// * 返回值说明
// * 1: 新出现的轨迹.
// * 2: 正常运行中的轨迹.
// * 3: 发生合并的轨迹.
// * 4: 发生分裂的轨迹.
// * 5: 处于消失状态的轨迹.
int CHC_GetTraceStatus( const TriDimTracker * pTdt, int i );
//  4.8 轨迹数组中第i个位置上的轨迹的外框和质心信息.
// * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的质心和外框信息.
// * pTdt: 指向三维跟踪对象的指针.
// * i   : 轨迹位置索引.
// * msg : 存放6个整形变量的数组指针, 数组空间在外部分配.
//     数据存放顺序为质心x坐标, 质心y坐标, 外框左上角x坐标, 左上角y坐标, 右下角x坐标, 右下角y坐标.
void CHC_GetTraceGeoMsg( const TriDimTracker * pTdt, int i, int * msg );
//  4.9 轨迹数组中第i个位置上的轨迹的分类信息.
// * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹的类别.
// * 同函数CHC_IsExistTrace.
// * 返回值说明
// * 1: 单人.
// * 2: 多人.
// * 4: 车辆.
// * 8: 未知.
int CHC_GetTraceKind( const TriDimTracker * pTdt, int i );
//  4.10 轨迹数组中第i个位置上的轨迹的方向信息.
//  * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹的方向.
//  * 返回值说明
//  * 返回值为范围在[0, 360)区间的浮点数, 当目标质心和上一帧相比没有改变时, 返回的值也是0.
double CHC_GetTraceDirection( const TriDimTracker * pTdt, int i );
//  4.11 轨迹数组中第i个位置上的轨迹所对应团块的面积(像素点个数).
//  * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹所对应团块的像素点个数.
int CHC_GetTraceBlockArea( const TriDimTracker * pTdt, int i );
//  4.12 轨迹数组中第i个位置上的轨迹的移动真实速率.
//  * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹在现实世界中的速率.
double CHC_GetTraceSpeed( const TriDimTracker * pTdt, int i );
//  4.12 轨迹数组中第i个位置上的轨迹的真实高度.
//  * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹在现实世界中的高度.
double CHC_GetTraceHeight( const TriDimTracker * pTdt, int i );
#ifdef __cplusplus
}
#endif
#endif

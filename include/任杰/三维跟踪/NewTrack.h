#ifndef _TRI_DIM_TRACK_
#define _TRI_DIM_TRACK_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USRDLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif


// 整型点, 浮点型点类型.
struct IPoint
{
	int x;
	int y;
};

struct FPoint
{
	double x;
	double y;
};

// 标定类型前向声明.
struct Calibration;
struct GMM;
struct TriDimModel;
struct TriDimTracker;

typedef unsigned char uchar;


/* 1, 三维场景标定. */

/* 1.0 建立三维场景标定对象. 函数返回一个指向三维场景标定对象的指针, 创建失败则返回空指针.

 * 注意事项
 * 该函数创建的对象使用完毕后必须调用函数ReleaseCbrModel删除该对象.

 * 参数说明
 * refPts    : 标定线段端点图像坐标对. 一般为5条竖直线和2组地面上的平行线共18个点.
 * refHeight : 标定参照物的实际高度, 单位为厘米(cm).
 * vLineNum  : 竖直线条数.
 * hLineNum  : 水平线条数.
 * SceneWidth: 图像像素宽度.

 */
DECLSPEC Calibration* CHC_CreateCbrModel(const IPoint *refPts, double refHeight, int vLineNum, int hLineNum, int SceneHeight);

/* 1.1 检查标定完成状况. 函数返回0表示标定成功.

 * 返回值说明
 *  0: 标定成功.
 * -1: 内存分配失败.
 * -2: 竖直线中出现平行情况.
 * -3: 所有竖直直线的交点的y坐标都小于0, 全部被忽略.
 * -4: 水平线对中出现平行情况.
 * -5: 计算中出现对负数开方的情况.

 * 参数说明
 * pCbr: 指向场景标定对象的指针.

 */
DECLSPEC int CHC_CheckCbrStatus(const Calibration *pCbr);

/* 1.2 地面线段实际长度验证, 函数返回两点间的实际距离.

 * 功能说明
 * 输入地面上两点在图像上的像素坐标, 计算该两点间的实际距离.

 * 参数说明
 * pCbr : 指向三维场景标定对象的指针.
 * begin: 地面上的线段的起点在图像上的像素坐标.
 * end  : 地面上的线段的终点在图像上的像素坐标.

 */
DECLSPEC double CHC_ValidateLen(const Calibration *pCbr, const IPoint *begin, const IPoint *end);

/* 1.3 目标像素高度验证. 函数返回目标顶部像素坐标.

 * 功能说明
 * 输入目标的实际高度和目标的底部(脚)在图像上的像素坐标, 计算目标的顶部(头)在图像上的像素坐标.

 * 参数说明
 * pCbr      : 指向三维场景标定对象的指针.
 * tail      : 目标底部在图像上的像素坐标.
 * realHeight: 目标的实际高度.

 */
DECLSPEC FPoint CHC_ValidateHeadPos(const Calibration *pCbr, const IPoint *tail, int realHeight);

/* 1.4 删除三维场景标定对象.

 * 注意事项
 * 注意输入是指向标定对象的指针的指针.

 * 参数说明
 * pCbr: 指向标定对象指针的指针.

 */
DECLSPEC void CHC_ReleaseCbrModel(Calibration **pCbr);

/* 2, 人体三维模型建立. */

/* 2.0 建立人体三维模型对象. 建立的对象不再使用时必须调用函数CHC_ReleaseTDModel释放.

 * 功能说明
 * 建立人体三维模型对象, 用于根据跟踪对象的大小判定其类型(人, 车..), 判定细节已经封装在三维跟踪类中.

 * 参数说明
 * h      : 场景像素高度.
 * w      : 场景像素宽度.
 * subH   : 场景平均分割时, 竖直方向上平均划分的子块数目(场景被平均划分为subH * sumW块并在各个子块建立相应的三维人体模型).
 * subW   : 场景平均分割时, 水平方向上平均划分的子块数目.
 * pCbr   : 三维标定对象指针(该指针指向的对象已经在第一步中创建).

 * 以下参数都会根据实际情况给出建议值.

 * sampNum: 模型圆柱上的抽样点的个数.
 * rLow   : 腿部圆柱模型半径.
 * rMid   : 身体圆柱模型半径.
 * rTop   : 头部圆柱模型半径.
 * hLow   : 腿部像素高度.
 * hMid   : 身体像素高度.
 * hTop   : 头部像素高度.

 */
DECLSPEC TriDimModel* CHC_CreateTDModel(int h, 
							            int w, 
							            int subH, 
							            int subW, 
							            Calibration *pCbr, 
							            int sampNum, 
							            int rLow, 
							            int rMid, 
							            int rTop, 
							            int hLow, 
							            int hMid, 
							            int hTop);

/* 2.1 销毁人体三维模型对象.

 * 参数说明
 * pTdm: 指向三维人体模型对象的指针的指针.

 */
DECLSPEC void CHC_ReleaseTDModel(TriDimModel **pTdm);


/* 3, 高斯背景建模. */

/* 3.0 创建高斯背景建模对象. 

 * 注意事项
 * 函数失败时返回空指针, 背景建模对象使用结束后必须调用函数CHC_DelGMModel删除该对象.

 * 参数说明
 * afa          : 用于调整背景模型更新速度的参数.
 * thrd         : 前景判定时的权重阈值.
 * virV         : 初始化方差值.
 * virW         : 初始化权重值.
 * shadowp0     : 第二类阴影修正方法所使用的参数.
 * shadowp1     : 第二类阴影修正方法所使用的参数.
 * shadowp2     : 第二类阴影修正方法所使用的参数.
 * shadowp3     : 第二类阴影修正方法所使用的参数.
 * r            : 场景像素高度.
 * c            : 场景像素宽度.
 * num          : 背景模型个数.
 * minPts       : 根据团块大小决定团块是否去除所使用的阈值.
 * isRectLabel  : 去除小团块修正方法的开关.
 * isRectShadowX: 第一类阴影修正方法开关.
 * isRectShadowY: 第二类阴影修正方法开关.
 * isRectShape  : 几何修正方法开关.

 */
DECLSPEC GMM* CHC_CreateGMModel(double afa = 0.005, 
		                        double thrd = 0.4, 
		                        double virV = 5.0, 
		                        double virW = 0.05, 
		                        double shadowp0 = 0.95, 
		                        double shadowp1 = 0.3, 
		                        double shadowp2 = 0.3, 
		                        double shadowp3 = 0.1, 
		                        int r = 120, 
		                        int c = 160, 
		                        int num = 3, 
		                        int minPts = 20, 
		                        bool isRectLabel = true, 
		                        bool isRectShadowX = true, 
		                        bool isRectShadowY = false, 
		                        bool isRectShape = true);

/* 3.1 高斯背景模型建立. 

 * 功能说明
 * 每次传入一帧RGB模式的图像数据, 连续对n帧图像调用此函数完成背景模型的建立, n的建议值为500.

 * 参数说明
 * pGmm: 指向高斯背景建模对象的指针.
 * src : RGB模式图像数据的头指针.
 * step: 图像数据步长.
 * num : 当前传入帧的序号, 例如第一帧num值为1.

 */
DECLSPEC void CHC_SetupBG(GMM *pGmm, const uchar *src, int step, int num);

/* 3.2 前景检测. 

 * 功能说明
 * 背景模型建立后, 传入RGB图像数据以检测前景团块. 函数返回当前帧检测结果个数.

 * 参数说明
 * pGmm: 指向高斯背景建模对象的指针.
 * src : RGB模式图像数据的头指针.
 * step: 图像数据步长.
 * mask: 检测区域遮罩. 与场景像素大小相同的二值图像, 只对遮罩上大于0的RGB图像数据对应点作检测. 
         该指针指空时对整个场景都作前景检测.

 */
DECLSPEC int CHC_ForegroundDetect(GMM *pGmm, const uchar *src, int step, const uchar *mask);

/* 3.3  删除高斯背景建模对象. 

 * 注意事项
 * 注意传入的是指向背景建模对象的指针的指针.

 * 参数说明
 * pGmm: 指向高斯背景建模对象的指针的指针.

 */
DECLSPEC void CHC_ReleaseGMModel(GMM** pGmm);

/* 3.4 获取检测二值结果的团块分类之后的结果. 

 * 注意事项
 * 图像尺寸与设置的检测高宽同尺寸.
 * 输出结果为已经做过连通区域的无符号整型图像, 0值像素表示背景, 大于0表示前景.

 */
DECLSPEC const unsigned int* CHC_GetBlobRst(GMM *pGmm);


/* 4, 跟踪. */

/* 4.0 创建跟踪对象. 函数失败时返回空指针.

 * 注意事项
 * 跟踪对象不再需要使用时必须调用函数CHC_ReleaseTDTModel释放该对象.

 * 参数说明
 * h        : 场景像素高度.
 * w        : 场景像素宽度.
 * maxObjNum: 最大轨迹处理条数.

 */
DECLSPEC TriDimTracker* CHC_CreateTDTModel(int h, int w, int maxObjNum);

/* 4.1 删除跟踪对象. 

 * 注意事项
 * 注意传入的参数是指向跟踪对象指针的指针.

 * 参数说明
 * pTdt: 指向跟踪对象指针的指针.

 */
DECLSPEC void CHC_ReleaseTDTModel(TriDimTracker **pTdt);

/* 4.2 跟踪处理. 

 * 参数说明
 * pTdt: 指向跟踪对象的指针.
 * src : 需要做跟踪处理的原始RGB图像数据.
 * pGmm: 高斯背景建模对象指针.
 * num : 当前帧团块检测结果个数, 该值由函数CHC_ForegroundDetect返回.
 * pCbr: 三维标定对象指针.
 * pTdm: 人体三维模型对象指针.

 */
DECLSPEC void CHC_TrackProc(TriDimTracker *pTdt, const uchar *src, const GMM *pGmm, int num, const Calibration *pCbr, const TriDimModel *pTdm);

/* 4.3 得到存放轨迹的数组中所包含的位置个数. 

 * 功能说明
 * 得到三维跟踪对象中存放轨迹的数组的大小, 即最多可以存放(处理)多少条轨迹, 与创建三维跟踪对象时的maxObjNum值相等.

 * 参数说明
 * pTdt: 指向跟踪对象的指针.

 */
DECLSPEC int CHC_GetCellNum(const TriDimTracker *pTdt);

/* 4.4 得到当前轨迹条数. 函数返回正在被处理中的轨迹的条数(返回值 <= maxObjNum).

 * 参数说明
 * pTdt: 指向跟踪对象的指针.

 */
DECLSPEC int CHC_GetTraceNum(const TriDimTracker *pTdt);

/* 4.5 轨迹数组中第i个位置上是否存放了轨迹. 

 * 功能说明
 * 检测轨迹数组中第i个位置上是否存放了正在被处理的轨迹, 存放了轨迹返回1, 否则返回0.

 * 参数说明
 * pTdt: 指向三维跟踪对象的指针.
 * i   : 轨迹位置索引.

 */
DECLSPEC int CHC_IsExistTrace(const TriDimTracker *pTdt, int i);

/* 4.6 轨迹数组中第i个位置上的轨迹的标识. 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的标识, 标识使用一个整形数表示.

 * 参数说明
 * 同函数CHC_IsExistTrace.

 */
DECLSPEC int CHC_GetTraceIdentity(const TriDimTracker *pTdt, int i);

/* 4.7 轨迹数组中第i个位置上的轨迹的状态. 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的状态.

 * 参数说明
 * 同函数CHC_IsExistTrace.

 * 返回值说明
 * 1: 新出现的轨迹.
 * 2: 正常运行中的轨迹.
 * 3: 发生合并的轨迹.
 * 4: 发生分裂的轨迹.
 * 5: 处于消失状态的轨迹.

 */
DECLSPEC int CHC_GetTraceStatus(const TriDimTracker *pTdt, int i);

/* 4.8 轨迹数组中第i个位置上的轨迹的外框和质心信息. 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存在轨迹之后, 调用该函数获取该位置上的轨迹的质心和外框信息.

 * 参数说明
 * pTdt: 指向三维跟踪对象的指针.
 * i   : 轨迹位置索引.
 * msg : 存放6个整形变量的数组指针, 数组空间在外部分配. 
         数据存放顺序为质心x坐标, 质心y坐标, 外框左上角x坐标, 左上角y坐标, 右下角x坐标, 右下角y坐标.

 */
DECLSPEC void CHC_GetTraceGeoMsg(const TriDimTracker *pTdt, int i, int *msg);

/* 4.9 轨迹数组中第i个位置上的轨迹的分类信息. 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹的类别.

 * 参数说明
 * 同函数CHC_IsExistTrace.

 * 返回值说明
 * 1: 单人.
 * 2: 多人.
 * 4: 车辆.
 * 8: 未知.

 */
DECLSPEC int CHC_GetTraceKind(const TriDimTracker *pTdt, int i);

/* 4.10 轨迹数组中第i个位置上的轨迹的方向信息. 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹的方向.

 * 返回值说明
 * 返回值为范围在[0, 360)区间的浮点数, 当目标质心和上一帧相比没有改变时, 返回的值也是0.

 */
DECLSPEC double CHC_GetTraceDirection(const TriDimTracker *pTdt, int i);

/* 4.11 轨迹数组中第i个位置上的轨迹所对应团块的面积(像素点个数). 

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹所对应团块的像素点个数.

 */
DECLSPEC int CHC_GetTraceBlockArea(const TriDimTracker *pTdt, int i);

/* 4.12 轨迹数组中第i个位置上的轨迹的移动真实速率.

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹在现实世界中的速率.

 */
DECLSPEC double CHC_GetTraceSpeed(const TriDimTracker *pTdt, int i);


/* 4.12 轨迹数组中第i个位置上的轨迹的真实高度.

 * 功能说明
 * 使用函数CHC_IsExistTrace判定第i个位置上存放了轨迹之后, 调用该函数获取该位置上的轨迹在现实世界中的高度.

 */
DECLSPEC double CHC_GetTraceHeight(const TriDimTracker *pTdt, int i);


#ifdef __cplusplus
}
#endif

#endif
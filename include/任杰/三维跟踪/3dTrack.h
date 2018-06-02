#ifndef _3D_TRACK_H_
#define _3D_TRACK_H_

#include "algo.h"
#include "3dKalman.h"
#include "TriDimModel.h"
#include "..\三维标定\calibration.h"


// 标定类前向声明.
struct Calibration;


// 轨迹状态枚举体.
typedef enum 
{
	DELETE = 0, // 即将被删除.
	EMERGE,     // 最近出现.
	NORMAL,     // 正常状态.
	COMBINE,    // 合并状态.
	SPLIT,      // 分裂状态.
	FADING,     // 消失状态.
}
STATUS;

// 目标分类.
typedef enum
{
	PERSON  = 1, 
	CROWD   = 2, 
	VEHICLE = 4, 
	UNKNOW  = 8, 
}
CLASS;

// 团块类.
class Block
{
public:
	int cy;
	int cx;

	int l;
	int t;
	int r;
	int b;

	Block(int y = 0, int x = 0, int left = 0, int top = 0, int right = 0, int bottom = 0): 
		cy(y), // 质心y坐标.
		cx(x), // 质心x坐标.
		l(left), 
		t(top), 
		r(right), 
		b(bottom)
	{
	}
};

// 轨迹类.
class TrackTarget
{
public:
	// 构造函数.
	TrackTarget(double length, 
		        double compact, 
				const double *wloc, 
				const int *iloc, 
		        int cy, 
		        int cx, 
		        int left, 
		        int top, 
				int right, 
				int bottom, 
				int size, 
				int name, 
				int h, 
				int w):
	    direction(0.0), 
	    speed(0.0), 
	    len(length), 
		cmt(compact), 
	    rect(cy, cx, left, top, right, bottom), 
		pred(*(wloc + 0), *(wloc + 1)), 
		real(new uchar[h * w * 3]), 
		area(size), 
		s(EMERGE), 
		attri(UNKNOW), 
		score(1), 
		label(name)
	{
		if (NULL == real)
		{
			exit(-1);
		}

		*(ftw + 0) = *(wloc + 0);
		*(ftw + 1) = *(wloc + 1);

		*(fti + 0) = *(iloc + 0);
		*(fti + 1) = *(iloc + 1);

		memset(real, 0, h * w * 3 * sizeof(uchar));
	}

	~TrackTarget()
	{
		delete []real; real = NULL;
	}

public: 
	double direction; // 目标图像上的移动角度.
	double speed;  // 目标实际速率.
	double len;    // 目标实际高度.
	double cmt;    // 目标紧实度.
	double ftw[2]; // 目标底部的世界坐标.
	int fti[2];    // 目标底部的图像坐标.

	kalman pred; // 目标速度和位置预测信息.

	Block  rect; // 目标2d信息.
	int    area; // 目标面积.
	uchar *real; // 目标颜色信息.

	STATUS s; // 目标状态.
	CLASS attri; // 目标分类.

	int score; // 目标状态得分.
	int label; // 目标标识.

private:
	// 禁止使用拷贝构造函数和赋值运算符.
	const TrackTarget& operator = (const TrackTarget&);
	TrackTarget(const TrackTarget&);
};

// 三维跟踪类.
struct TriDimTracker
{
private:
	// 场景像素尺寸.
	int m_SceneH;
	int m_SceneW;

	// 最大处理轨迹数, 当前轨迹数, 下一条新轨迹将被赋予的ID.
	int m_MaxObjNum;
	int m_CurObjNum;
	int m_CurIDNum;

	// 轨迹指针数组.
	TrackTarget **m_TargetList;

private:
	// 分析已有轨迹和当前检测结果之间的关系矩阵.
	void AnalyzeDistMat(const int *map, 
						const uchar *relation, 
						int num, 
						int *norm, 
						int *emerge, 
						int *fade, 
						int *combine, 
						int *split);

	// 处理轨迹消失的情况.
	void ProcFade(const int *map, const int *fade, const Calibration *pCbr);

	// 处理新出现的目标.
	int ProcEmerge(const uchar *src, 
				   const int *emerge, 
				   uchar **bin, 
				   const int *rect, 
				   const double *feet, 
				   const int *area, 
				   const int *center, 
				   const int *feeti, 
				   const double *len, 
				   const double *compact, 
				   int num);

	// 处理单个轨迹对应单个当前检测团块结果的情况.
	void ProcOne2One(const uchar *src, 
		             const int *map, 
					 const int *norm, 
					 uchar **bin, 
					 const int *rect, 
					 const double *feet, 
					 const int *area, 
					 const int *center, 
					 const int *feeti, 
					 const double *len, 
					 const double *compact, 
					 const Calibration *pCbr, 
					 int num);

	// 处理目标分裂的情况.
	int ProcSplit(const uchar *src, 
		          const int *map, 
		 		  const int *split, 
				  uchar **bin, 
				  const int *rect, 
				  const double *feet, 
				  const int *area, 
				  const int *center, 
				  const int *feeti, 
				  const double *len, 
				  const double *compact, 
				  const Calibration *pCbr, 
				  int num);

	// 处理多个目标合并的情况.
	void ProcMerge(const uchar *src, 
		           const int *map, 
		 		   const int *combine, 
				   uchar **bin, 
				   const int *rect, 
				   const double *feet, 
				   const int *area, 
				   const int *center, 
				   const int *feeti, 
				   const double *len, 
				   const double *compact, 
				   const Calibration *pCbr, 
				   int num);

	// 根据当前检测结果团块与已有轨迹的重叠关系生成距离矩阵.
	void DistMatrix(const int *map, uchar **binList, int num, uchar *relation);

	// 分配分析距离矩阵所需的内存.
	void AnalyzeMem(int **norm, int **emerge, int **fade, int **combine, int **split, int num);

	// 颜色团块信息移动.
	void ColorMove(TrackTarget *&pt, int offsetX, int offsetY);

	// 轨迹类型判定.
	void TraceClassify(const TriDimModel *pTDM);

	// 初始化轨迹信息.
	void InitTrace(int loc, 
		           const int *center, 
				   const int *rect, 
				   const double *feet, 
				   int area, 
				   const int *feeti, 
				   double len, 
				   double compact);

	// 生成一个新的ID.
	int NewID(void)
	{
		return m_CurIDNum++;
	}

	// 从轨迹指针数组中获取一个空指针索引.
	int NullPtr(void)
	{
		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			if (NULL == *(m_TargetList + i))
			{
				return i;
			}
		}

		return -1;
	}

	// 生成指针数组映射关系.
	void TargetMap(int *map)
	{
		assert(NULL != map);

		for (int i = 0, j = 0; i < m_MaxObjNum && j < m_CurObjNum; ++i)
		{
			if (NULL != *(m_TargetList + i))
			{
				*(map + j++) = i;
			}
		}
	}

	// 两幅二值图求交集像素点个数.
	int BinImgInter(const uchar *binX, const uchar *binY, int h, int w)
	{
		assert(NULL != binX && NULL != binY && h > 0 && w > 0);

		int sum = 0;

		for (int i = 0; i < h * w; ++i)
		{
			if (0 < *(binX + i) && 0 < *(binY + i)) 
			{
				++sum;
			}
		}

		return sum;
	}

	// 获取二值图对应位置的RGB信息.
	void ColorMsg(const uchar *src, const uchar *bin, int h, int w, uchar *rgb)
	{
		assert(NULL != src && NULL != bin && NULL != rgb && h > 0 && w > 0);

		memset(rgb, 0, h * w * 3 * sizeof(uchar));

		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if (0 < *(bin + i * w + j))
				{
					*(rgb + i * w * 3 + j * 3 + 0) = *(src + i * w * 3 + j * 3 + 0);
					*(rgb + i * w * 3 + j * 3 + 1) = *(src + i * w * 3 + j * 3 + 1);
					*(rgb + i * w * 3 + j * 3 + 2) = *(src + i * w * 3 + j * 3 + 2);
				}
			}
		}
	}

	// 将目标RGB图像移动到新的位置.
	void Move(const uchar *src, int h, int w, int offsetX, int offsetY, uchar *dst)
	{
		assert(NULL != src && NULL != dst && h > 0 && w > 0);

		memset(dst, 0, h * w * 3 * sizeof(uchar));

		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if (0 < *(src + i * w * 3 + j * 3 + 0) || 
					0 < *(src + i * w * 3 + j * 3 + 1) || 
					0 < *(src + i * w * 3 + j * 3 + 2))
				{
					const int M = i + offsetY;
					const int N = j + offsetX;

					if (-1 < M && M < h && -1 < N && N < w)
					{
						*(dst + M * w * 3 + N * 3 + 0) = *(src + i * w * 3 + j * 3 + 0);
						*(dst + M * w * 3 + N * 3 + 1) = *(src + i * w * 3 + j * 3 + 1);
						*(dst + M * w * 3 + N * 3 + 2) = *(src + i * w * 3 + j * 3 + 2);
					}
				}
			}
		}
	}

	// 根据目标彩色信息生成二值图信息.
	void Real2Bin(const uchar *real, uchar *bin, int h, int w)
	{
		assert(NULL != real && NULL != bin && h > 0 && w > 0);

		memset(bin, 0, h * w * sizeof(uchar));

		for (int i = 0; i < h * w; ++i)
		{
			if (0 < *(real + i * 3 + 0) || 
				0 < *(real + i * 3 + 1) || 
				0 < *(real + i * 3 + 2))
			{
				*(bin + i) = 1;
			}
		}
	}

	// 释放分析距离矩阵所需的内存.
	void FreeAnalyzeMem(int **norm, int **emerge, int **fade, int **combine, int **split)
	{
		assert(NULL != norm && NULL != emerge && NULL != fade && NULL != combine && NULL != split);
		assert(NULL != *norm && NULL != *emerge && NULL != *fade && NULL != *combine && NULL != *split);

		delete []*norm;    *norm    = NULL;
		delete []*emerge;  *emerge  = NULL;
		delete []*fade;    *fade    = NULL;
		delete []*combine; *combine = NULL;
		delete []*split;   *split   = NULL;
	}

	// 出现在场景边缘的轨迹.
	int TraceOnBorder(const TrackTarget *&pt, int thrd)
	{
		assert(NULL != pt);

		if (pt->rect.cx > m_SceneW - (thrd + 1) || 
			pt->rect.cy > m_SceneH - (thrd + 1) || 
			pt->rect.cx < thrd || 
			pt->rect.cy < thrd)
		{
			return 1;
		}

		else 
		{
			return 0;
		}
	}

	// 删除所有状态为DELETE的轨迹.
	int DelInvalid(void)
	{
		int num = 0;

		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			TrackTarget *&pt = *(m_TargetList + i);

			if (NULL != pt && DELETE == pt->s)
			{
				delete pt; pt = NULL;
				--num;
			}
		}

		return num;
	}

	// 根据上一帧的质心坐标和当前帧的质心坐标求目标在场景像素上的移动方向.
	double MoveDirection(int preX, int preY, int curX, int curY);

public:
	TriDimTracker(int h, 
		          int w, 
				  int maxObjNum):
		m_SceneH(h), 
		m_SceneW(w), 
		m_MaxObjNum(maxObjNum), 
		m_CurObjNum(0), 
		m_CurIDNum(0), 
		m_TargetList(new TrackTarget*[maxObjNum])
	{
		if (NULL == m_TargetList)
		{
			exit(-1);
		}

		for (int i = 0; i < maxObjNum; ++i)
		{
			*(m_TargetList + i) = NULL;
		}
	}

	~TriDimTracker()
	{
		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			if (NULL != *(m_TargetList + i))
			{
				delete *(m_TargetList + i);
				*(m_TargetList + i) = NULL;
			}
		}

		delete []m_TargetList; m_TargetList = NULL;
	}

public:
	// 跟踪.
	void MultiTrack(const uchar *src, const unsigned int *block, int num, const Calibration *pCbr, const TriDimModel *pTDM);

	// 获取最大轨迹条数.
	int GetMaxObjNum(void) const 
	{
		return m_MaxObjNum;
	}

	// 获取当前轨迹条数.
	int GetCurObjNum(void) const 
	{
		return m_CurObjNum;
	}

	// 获取第i个轨迹指针的指向状况, 指空返回0, 否则返回1.
	int GetTracePtr(int i) const 
	{
		return NULL == *(m_TargetList + i) ? 0 : 1;
	}

	// 获取第i个轨迹指针指向的轨迹的ID.
	int GetIdentity(int i) const 
	{
		return (*(m_TargetList + i))->label;
	}

	// 获取第i个轨迹指针指向的轨迹的状态.
	STATUS GetStatus(int i) const 
	{
		return (*(m_TargetList + i))->s;
	}

	// 获取第i个轨迹指针指向的轨迹的外框, 质心信息.
	const Block& GetGeoMsg(int i) const 
	{
		return (*(m_TargetList + i))->rect;
	}

	// 获取第i个轨迹指针指向的轨迹的类别.
	CLASS GetKind(int i) const 
	{
		return (*(m_TargetList + i))->attri;
	}

	// 获取第i个轨迹指针指向的轨迹的方向.
	double GetDirection(int i) const 
	{
		return (*(m_TargetList + i))->direction;
	}

	// 获取第i个轨迹指针指向的轨迹的团块面积.
	int GetArea(int i) const 
	{
		return (*(m_TargetList + i))->area;
	}

	// 获取第i个轨迹指针指向的轨迹的真实速率.
	double GetSpeed(int i) const 
	{
		return (*(m_TargetList + i))->speed;
	}

	// 获取第i个轨迹指针指向的轨迹的真实高度.
	double GetRealHeight(int i) const 
	{
		return (*(m_TargetList + i))->len;
	}
};

#endif
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_


// 包含NewTrack.h仅为了获得IPoint和FPoint的定义.
#include "E:\work\mushroom2009\三维跟踪\NewTrack.h"

// 输入在图像上的二维坐标(x, y), 输出对应的地面上的三维(x, y)坐标.
inline void DimTrans23(const double *he, const int *loc2d, double *loc3d)
{
	double tmp[3];
	const int a[] = {*(loc2d + 0), *(loc2d + 1), 1};

	MatMul(he, a, tmp, 3, 3, 1);

	*(loc3d + 0) = *(tmp + 0) / *(tmp + 2);
	*(loc3d + 1) = *(tmp + 1) / *(tmp + 2);
}

// 输入三维(x, y)坐标, 输出对应的图像上的二维(x, y)坐标.
inline void DimTrans32(const double *pTMat, const double *loc3d, int *loc2d)
{
	const double HE[] = 
	{
		*(pTMat + 0), *(pTMat + 2), *(pTMat + 3), 
	    *(pTMat + 4), *(pTMat + 6), *(pTMat + 7), 
	    *(pTMat + 8), *(pTMat +10), *(pTMat +11)
	};
	const double a[] = {*(loc3d + 0), *(loc3d + 1), 1};
	double tmp[3];

	MatMul(HE, a, tmp, 3, 3, 1);

	*(loc2d + 0) = int(*(tmp + 0) / *(tmp + 2));
	*(loc2d + 1) = int(*(tmp + 1) / *(tmp + 2));
}

// 标定函数.
int CalibrationX(const IPoint *RefPts,
				 double *MatrixP,
				 double *HLine,
			     double StdHeight,
				 double *CamHeight,
				 FPoint *VertexY,
			     int VerLineNum,
			     int HorLineNum,
			     int Row);

// 计算目标实际高度.
double CalibrationValidateHeihgt(double heightCam, 
								 const double *hLine, 
								 const FPoint *vexY, 
								 const IPoint *head, 
								 const IPoint *tail);

// 计算目标在图像中的顶部坐标.
void CalibrationValidateLoc(const double *matP, 
							const double *matT, 
							const IPoint *tail, 
							int height, 
							FPoint *location);

// 三维标定类.
struct Calibration
{
public:
	// 获取标定完成状态.
	int GetStatus(void) const
	{
		return m_Status;
	}

	// 返回坐标变换矩阵.
	const double* GetTMat(void) const 
	{
		return m_TMat;
	}

	// 返回逆坐标变换矩阵.
	const double* GetITMat(void) const 
	{
		return m_TInv;
	}

	// 根据地面上两点的像素坐标计算两点间的实际距离.
	double RealDist(const IPoint *begin, const IPoint *end) const 
	{
		double realA[2], realB[2];

		const int tmpA[2] = {begin->x, begin->y};
		const int tmpB[2] = {end->x, end->y};

		DimTrans23(m_TInv, tmpA, realA);
		DimTrans23(m_TInv, tmpB, realB);

		return sqrt((realA[0] - realB[0]) * (realA[0] - realB[0]) + (realA[1] - realB[1]) * (realA[1] - realB[1]));
	}

	// 根据目标在图像上的顶部和底部坐标计算物体的实际高度.
	double ValidHeight(const IPoint *head, const IPoint *tail) const 
	{
		return CalibrationValidateHeihgt(m_CamH, m_HLine, &m_VerPt, head, tail);
	}

	// 根据目标的实际高度和在图像上的底部坐标计算目标在图像上的的顶部坐标.
	void ValidPos(FPoint *pos, const IPoint *tail, int height) const 
	{
		CalibrationValidateLoc(m_TMat, m_TInv, tail, height, pos);
	}

	// 根据输入的二值图计算物体高度, 紧实度, 底部世界坐标, 底部图像坐标.
	void TriDimInfo(const uchar *bin, int h, int w, int area, const int *rect, double *targetH, double *compact, double *feetPosW, int *feetPosI) const;

public:
	Calibration(const IPoint *refPt, double refH, int vLineNum, int hLineNum, int h)
	{
		m_Status = CalibrationX(refPt, m_TMat, m_HLine, refH, &m_CamH, &m_VerPt, vLineNum, hLineNum, h);
		CalTInv();
	}

private:
	// 计算坐标逆变换矩阵.
	void CalTInv(void);

private:
	double m_TMat[12]; // 坐标变换矩阵.
	double m_TInv[9];  // 坐标逆转换矩阵.
	double m_HLine[3]; // 水平线.
	double m_CamH;     // 摄像机高度.
	FPoint m_VerPt;    // 垂直没影点.
	int m_Status;      // 标定状态.
};

#endif
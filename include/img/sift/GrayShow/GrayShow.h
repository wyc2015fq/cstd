//////////////////////////////////////////////////////////////////////
// GrayShow.h: interface for the CGrayShow class.
// 纹理分析－灰度共现矩阵算法实现
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYSHOW_H_INCLUDED_)
#define AFX_GRAYSHOW_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CGrayShow  
{
public:
	void ComputeMatrix(BYTE** LocalImage, int LocalImageWidth);
	void ComputeFeature(double& FeatureEnergy, double& FeatureEntropy, double& FeatureInertiaQuadrature, double& FeatureCorrelation, double& FeatureLocalCalm, int** pMatrix, int dim);
	//变量

	int FilterWindowWidth;	//纹理区域块的大小，通常将图像划分成若干个纹理块计算
	long ImageHeight;	//原图像的高度
	long ImageWidth;		//原图像的宽度
	int distance;	//距离，依据不同的应用选取	
	int GrayLayerNum;	//灰度分级
	int** PMatrixRD;	//45度方向上的灰度共现矩阵
	int** PMatrixLD;	//135度方向上的灰度共现矩阵
	int** PMatrixV;		//90度方向上的灰度共现矩阵
	int** PMatrixH;		//0度方向上的灰度共现矩阵
	BYTE** ImageArray;	//原始的图像数据数组－灰度

	RGBQUAD** m_tOriPixelArray;//原始位图数据
	RGBQUAD** m_tResPixelArray;//参考位图数据

	CDib *m_dib;

	//操作
	BOOL IsEmpty;
	void LoadImage(CString &FilePathName);
	CGrayShow();
	virtual ~CGrayShow();

};

#endif // !defined(AFX_GRAYSHOW_H_INCLUDED_)

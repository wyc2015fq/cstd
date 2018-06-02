#include "algo.h"
#include "TriDimModel.h"
#include "..\三维标定\calibration.h"
#include "..\..\GMM_Backup\GMModel_Beta\GMModel.h"
#include "3dTrack.h"
#include "NewTrack.h"


// 1, 三维场景标定.

/* 1.0 建立三维场景标定对象. */
Calibration* CHC_CreateCbrModel(const IPoint *refPts, double refHeight, int vLineNum, int hLineNum, int SceneHeight)
{
	return new Calibration(refPts, refHeight, vLineNum, hLineNum, SceneHeight);
}

/* 1.1 检查标定完成状况. */
int CHC_CheckCbrStatus(const Calibration *pCbr)
{
	return pCbr->GetStatus();
}

/* 1.2 地面线段实际长度验证. */
double CHC_ValidateLen(const Calibration *pCbr, const IPoint *begin, const IPoint *end)
{
	return pCbr->RealDist(begin, end);
}

/* 1.3 目标像素高度验证. */
FPoint CHC_ValidateHeadPos(const Calibration *pCbr, const IPoint *tail, int realHeight)
{
	FPoint pixelHead;

	pCbr->ValidPos(&pixelHead, tail, realHeight);

	return pixelHead;
}

/* 1.4 删除三维场景标定对象. */
void CHC_ReleaseCbrModel(Calibration **pCbr)
{
	delete *pCbr; *pCbr = NULL;
}


// 2, 人体三维模型建立.

/* 2.0 建立人体三维模型对象. */
TriDimModel* CHC_CreateTDModel(int h, 
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
							   int hTop)
{
	return new TriDimModel(h, w, subH, subW, pCbr->GetTMat(), pCbr->GetITMat(), sampNum, rLow, rMid, rTop, hLow, hMid, hTop);
}

/* 2.1 删除人体三维模型对象. */
void CHC_ReleaseTDModel(TriDimModel **pTdm)
{
	delete *pTdm; *pTdm = NULL;
}


// 3, 高斯背景建模.

/* 3.0 创建高斯背景建模对象. */
GMM* CHC_CreateGMModel(double afa, 
		               double thrd, 
		               double virV, 
		               double virW, 
		               double shadowp0, 
		               double shadowp1, 
		               double shadowp2, 
		               double shadowp3, 
		               int r, 
		               int c, 
		               int num, 
		               int minPts, 
		               bool isRectLabel, 
		               bool isRectShadowX, 
		               bool isRectShadowY, 
		               bool isRectShape)
{
	return new GMM(afa, thrd, virV, virW, shadowp0, shadowp1, shadowp2, shadowp3, r, c, num, minPts, isRectLabel, isRectShadowX, isRectShadowY, isRectShape);
}

/* 3.1 背景建立. */
void CHC_SetupBG(GMM *pGmm, const uchar *src, int step, int num)
{
	pGmm->UpdateModel(src, step, num);
}

/* 3.2 前景检测. */
int CHC_ForegroundDetect(GMM *pGmm, const uchar *src, int step, const uchar *mask)
{
	return pGmm->Detect(src, step, mask);
}

/* 3.3  删除高斯背景建模对象. */
void CHC_ReleaseGMModel(GMM** pGmm)
{
	delete *pGmm; *pGmm = NULL;
}

/* 3.4 获取检测二值结果的团块分类之后的结果. */
const unsigned int* CHC_GetBlobRst(GMM *pGmm)
{
	return pGmm->BlobRst();
}


// 4, 跟踪.

/* 4.0 创建跟踪对象 */
TriDimTracker* CHC_CreateTDTModel(int h, int w, int maxObjNum)
{
	return new TriDimTracker(h, w, maxObjNum);
}

/* 4.1 删除跟踪对象. */
void CHC_ReleaseTDTModel(TriDimTracker **pTdt)
{
	delete *pTdt; *pTdt = NULL;
}

/* 4.2 跟踪处理. */
void CHC_TrackProc(TriDimTracker *pTdt, const uchar *src, const GMM *pGmm, int num, const Calibration *pCbr, const TriDimModel *pTdm)
{
	pTdt->MultiTrack(src, pGmm->BlobRst(), num, pCbr, pTdm);
}

/* 4.3 得到存放轨迹的数组中所包含的位置个数. */
int CHC_GetCellNum(const TriDimTracker *pTdt)
{
	return pTdt->GetMaxObjNum();
}

/* 4.4 得到当前轨迹条数. */
int CHC_GetTraceNum(const TriDimTracker *pTdt)
{
	return pTdt->GetCurObjNum();
}

/* 4.5 轨迹数组中第i个位置上是否存放了轨迹. */
int CHC_IsExistTrace(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetTracePtr(i);
}

/* 4.6 轨迹数组中第i个位置上的轨迹的标识. */
int CHC_GetTraceIdentity(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetIdentity(i);
}

/* 4.7 轨迹数组中第i个位置上的轨迹的状态. */
int CHC_GetTraceStatus(const TriDimTracker *pTdt, int i)
{
	return int(pTdt->GetStatus(i));
}

/* 4.8 轨迹数组中第i个位置上的轨迹的外框和质心信息. */
void CHC_GetTraceGeoMsg(const TriDimTracker *pTdt, int i, int *msg)
{
	const Block block = pTdt->GetGeoMsg(i);

	*(msg + 0) = block.cx;
	*(msg + 1) = block.cy;

	*(msg + 2) = block.l;
	*(msg + 3) = block.t;
	*(msg + 4) = block.r;
	*(msg + 5) = block.b;
}

/* 4.9 轨迹数组中第i个位置上的轨迹的分类信息. */
int CHC_GetTraceKind(const TriDimTracker *pTdt, int i)
{
	return int(pTdt->GetKind(i));
}

/* 4.10 轨迹数组中第i个位置上的轨迹的方向信息. */
double CHC_GetTraceDirection(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetDirection(i);
}

/* 4.11 轨迹数组中第i个位置上的轨迹的团块面积. */
int CHC_GetTraceBlockArea(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetArea(i);
}

/* 4.12 轨迹数组中第i个位置上的轨迹的真实速率. */
double CHC_GetTraceSpeed(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetSpeed(i);
}

/* 4.13 轨迹数组中第i个位置上的轨迹的真实高度. */
double CHC_GetTraceHeight(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetRealHeight(i);
}
#include "algo.h"
#include "TriDimModel.h"
#include "calibration.h"
#include "GMModel.h"
#include "3dTrack.h"
#include "NewTrack.h"



// 2, 人体三维模型建立.

/* 2.0 建立人体三维模型对象. */
TriDimModel* CHC_CreateTDModel( int h,
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
                                int hTop ) {
  return new TriDimModel( h, w, subH, subW, pCbr->GetTMat(), pCbr->GetITMat(), sampNum, rLow, rMid, rTop, hLow, hMid, hTop );
}

/* 2.1 删除人体三维模型对象. */
void CHC_ReleaseTDModel( TriDimModel **pTdm ) {
  delete * pTdm;
  *pTdm = NULL;
}



// 4, 跟踪.

/* 4.0 创建跟踪对象 */
TriDimTracker* CHC_CreateTDTModel( int h, int w, int maxObjNum ) {
  return new TriDimTracker( h, w, maxObjNum );
}

/* 4.1 删除跟踪对象. */
void CHC_ReleaseTDTModel( TriDimTracker **pTdt ) {
  delete * pTdt;
  *pTdt = NULL;
}

/* 4.2 跟踪处理. */
void CHC_TrackProc( TriDimTracker *pTdt, const uchar *src, const GMM *pGmm, int num, const Calibration *pCbr, const TriDimModel *pTdm ) {
  pTdt->MultiTrack( src, pGmm->BlobRst(), num, pCbr, pTdm );
}

/* 4.3 得到存放轨迹的数组中所包含的位置个数. */
int CHC_GetCellNum( const TriDimTracker *pTdt ) {
  return pTdt->GetMaxObjNum();
}

/* 4.4 得到当前轨迹条数. */
int CHC_GetTraceNum( const TriDimTracker *pTdt ) {
  return pTdt->GetCurObjNum();
}

/* 4.5 轨迹数组中第i个位置上是否存放了轨迹. */
int CHC_IsExistTrace( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetTracePtr( i );
}

/* 4.6 轨迹数组中第i个位置上的轨迹的标识. */
int CHC_GetTraceIdentity( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetIdentity( i );
}

/* 4.7 轨迹数组中第i个位置上的轨迹的状态. */
int CHC_GetTraceStatus( const TriDimTracker *pTdt, int i ) {
  return int( pTdt->GetStatus( i ) );
}

/* 4.8 轨迹数组中第i个位置上的轨迹的外框和质心信息. */
void CHC_GetTraceGeoMsg( const TriDimTracker *pTdt, int i, int *msg ) {
  const Block block = pTdt->GetGeoMsg( i );
  
  *( msg + 0 ) = block.w;
  *( msg + 1 ) = block.h;
  
  *( msg + 2 ) = block.l;
  *( msg + 3 ) = block.t;
  *( msg + 4 ) = block.r;
  *( msg + 5 ) = block.b;
}

/* 4.9 轨迹数组中第i个位置上的轨迹的分类信息. */
int CHC_GetTraceKind( const TriDimTracker *pTdt, int i ) {
  return int( pTdt->GetKind( i ) );
}

/* 4.10 轨迹数组中第i个位置上的轨迹的方向信息. */
double CHC_GetTraceDirection( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetDirection( i );
}

/* 4.11 轨迹数组中第i个位置上的轨迹的团块面积. */
int CHC_GetTraceBlockArea( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetArea( i );
}

/* 4.12 轨迹数组中第i个位置上的轨迹的真实速率. */
double CHC_GetTraceSpeed( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetSpeed( i );
}

/* 4.13 轨迹数组中第i个位置上的轨迹的真实高度. */
double CHC_GetTraceHeight( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetRealHeight( i );
}

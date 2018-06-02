
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include "macro.h"
#include "_MILTrack.h"
#include "direct.h"
#include "cap.h"
#include "window.h"
#include "sort.h"
#include "utime.h"
#include "chalg.h"
#include "debug.h"
#include "inifile.h"

#ifdef _DEBUG_DQ
#else
#define _M  1000 //候选特征数(候选分类器数)
#define _K  100 //选出特征数(选出分类器数)
#define _num_neg  65//负样本数
#define _num_pos  45//正样本数
#define _num_all (_num_neg+_num_pos) //正样本数
#define _num_candidate_result 500 //搜索样本数,要>=num_fix_cr
real _MIU1[ _M ];
real _DELTA1[ _M ]; //高斯分布均值和标准差
real _MIU2[ _M ];
real _DELTA2[ _M ];
real _L[ _M ];
int _L_ind[ _M ];
real _SelectMIU1[ _K ];
real _SelectDELTA1[ _K ];
real _SelectMIU2[ _K ];
real _SelectDELTA2[ _K ];
MILHAAR _SelectKFeatures[ _K ];
MILPOINT _YX_all[ _num_all ];
MILPOINT _YX_candidate[ _num_candidate_result ];
real __h_candidateWeakClf[ _M*_num_all ];
MILPOINT _YX_all[ _num_all ] = {
#include "F:\\public\\bin\\MILTrack\\数据\\sample.txt"
                               };
MILPOINT _YX_candidate[ _num_candidate_result ] = {
#include "F:\\public\\bin\\MILTrack\\数据\\candidate.txt"
    };
MILHAAR _MFeatures[ _M ] = {
#include "F:\\public\\bin\\MILTrack\\数据\\MF.txt"
                           };

#endif


MILTRACK* CPM_NewMilTrack(
  int M,  // = 1000, //候选特征数(候选分类器数)
  int K,  // = 100, //选出特征数(选出分类器数)
  int num_neg,  // = 65,//负样本数
  int num_pos,  // = 45,//正样本数
  //int num_all, // = 110, //正样本数
  int num_candidate_result, // = 500, //搜索样本数,要>=num_fix_cr
  //int num_replace_clk = 5,//用于替换的分类器(特征)数
  int beita1, // = 50,//随机产生负样本用到的距离1
  int beita2, // = 80,//随机产生负样本用到的距离2
  int gama, //  = 5, //随机产生正样本用到的距离
  int s, // = 30,//候选样本区域,要>=gama,且最好保证s > (num_fix_cr/10)/2
  int num_fix_cr, // = 300,//要在原位置附近固定的样本数,目前取值为100、200和300，见函数randomSelectSample_for_result.m
  real v // = 0.95,//学习率,0.85~1,v越小特征均值和标准差就更新得越快，v = 1则不更新特征均值和标准差，对于表面不变或者变化很缓慢的对象v应该取到接近1
) {
  MILTRACK * pmt = MALLOCSET( MILTRACK, 1, 0 );
  assert( K <= M && "K must less M!" );
  pmt->M = M; // = 1000; //候选特征数(候选分类器数)
  pmt->K = K; // = 100; //选出特征数(选出分类器数)
  pmt->num_neg = num_neg; // = 65;//负样本数
  pmt->num_pos = num_pos; // = 45;//正样本数
  //int num_all; // = 110; //正样本数
  pmt->num_candidate_result = num_candidate_result; // = 500; //搜索样本数;要>=num_fix_cr
  //int num_replace_clk = 5;//用于替换的分类器(特征)数
  pmt->beita1 = beita1; // = 50;//随机产生负样本用到的距离1
  pmt->beita2 = beita2; // = 80;//随机产生负样本用到的距离2
  pmt->gama = gama; //  = 5; //随机产生正样本用到的距离
  pmt->s = s; // = 30;//候选样本区域;要>=gama;且最好保证s > (num_fix_cr/10)/2
  pmt->num_fix_cr = num_fix_cr; // = 300;//要在原位置附近固定的样本数;目前取值为100、200和300，见函数randomSelectSample_for_result.m
  pmt->v = v;
  pmt->Y1 = pmt->X1 = pmt->sample_h = pmt->sample_w = 0;
  
#ifdef _DEBUG_DQ
  
  pmt->_h_candidateWeakClf = MALLOC( real, pmt->M * ( pmt->num_neg + pmt->num_pos ) );
  pmt->MFeatures = MALLOC( MILHAAR, pmt->M );
  pmt->YX_all = MALLOC( MILPOINT, ( pmt->num_neg + pmt->num_pos ) );
  pmt->YX_candidate = MALLOC( MILPOINT, pmt->num_candidate_result );
  pmt->L = MALLOC( real, pmt->M );
  pmt->L_ind = MALLOC( int, pmt->M );
#else
  
  pmt->M = _M; // = 1000, //候选特征数(候选分类器数)
  pmt->K = _K; // = 100, //选出特征数(选出分类器数)
  pmt->num_neg = _num_neg; // = 65,//负样本数
  pmt->num_pos = _num_pos; // = 45,//正样本数
  //int num_all, // = 110, //正样本数
  
  pmt->_h_candidateWeakClf = __h_candidateWeakClf;
  pmt->MFeatures = _MFeatures;
  pmt->YX_all = _YX_all;
  pmt->YX_candidate = _YX_candidate;
  pmt->MIU1 = _MIU1;
  pmt->DELTA1 = _DELTA1;
  pmt->MIU2 = _MIU2;
  pmt->DELTA2 = _DELTA2;
  pmt->SelectKFeatures = _SelectKFeatures;
  pmt->SelectMIU1 = _SelectMIU1;
  pmt->SelectDELTA1 = _SelectDELTA1;
  pmt->SelectMIU2 = _SelectMIU2;
  pmt->SelectDELTA2 = _SelectDELTA2;
  pmt->L = _L;
  pmt->L_ind = _L_ind;
#endif
  
  {
    int i;
    FOREACH_DO( pmt->M, i, pmt->MFeatures[ i ].MIU1 = pmt->MFeatures[ i ].DELTA1 = pmt->MFeatures[ i ].MIU2 = pmt->MFeatures[ i ].DELTA2 = 0.f );
  }
  return pmt;
}

int CPM_DelMilTrack( MILTRACK* pmt ) {
  if ( pmt ) {
    SAFEFREE( pmt->_h_candidateWeakClf );
    SAFEFREE( pmt->YX_all );
    SAFEFREE( pmt->YX_candidate );
    SAFEFREE( pmt->MFeatures );
    SAFEFREE( pmt->L );
    SAFEFREE( pmt->L_ind );
    SAFEFREE( pmt );
  }
  return 0;
}


real maxreal( int n, real* f ) {
  real maxval = -10000000000.f;
  int i;
  for ( i = 0; i < n; ++i ) {
    if ( f[ i ] > maxval ) {
      maxval = f[ i ];
    }
  }
  return maxval;
}

int MilTrackUpdate( MILTRACK* pmt, int pic_h, int pic_w, unsigned char* I, int step, int t ) {
  int i, j, m;
  int tempnum_neg = pmt->num_neg;
  int tempnum_pos = pmt->num_pos;
  int totalnum = tempnum_neg + tempnum_pos;
  int Y1 = pmt->Y1, X1 = pmt->X1, sample_h = pmt->sample_h, sample_w = pmt->sample_w;
  //////////////////////////////////////////
  // step3:随机产生样本 //
  //////////////////////////////////////////
#ifdef _DEBUG_DQ
  
  int ret;
  ret = randomSelectSample( X1, Y1, sample_w, sample_h, pic_w, pic_h, pmt->beita1, pmt->beita2, pmt->gama,
                            tempnum_neg, tempnum_pos, -1, pmt->YX_all ); //YX_neg,YX_pos只记录样本矩形框的左上角坐标，若第一帧和后面帧一样都不只用一个正样本，则此处是用-1代替原来的t
                            
  if ( 0 == ret )
    return 0;
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\YX_all_%d.txt" ), "%3d, ", ( tempnum_neg + tempnum_pos ), 2, ( int* ) ( pmt->YX_all ), 2, 1 );
#endif
  
  // step4:计算每一个样本对应的M个候选弱分类器 //
  ////////////////////////////////////////////////////////////////////////////////////
  candidateWeakClf( tempnum_neg, tempnum_pos, pmt->YX_all, sample_w, sample_h, pmt->MFeatures,
                    pmt->M, I, pic_h, pic_w, step, pmt->v, t, pmt->_h_candidateWeakClf );
                    
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\MIU1_%d.txt" ), "%23.15f, ", pmt->M, 1, pmt->MIU1, 1, 1 );
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\MIU2_%d.txt" ), "%23.15f, ", pmt->M, 1, pmt->MIU2, 1, 1 );
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\DELTA1_%d.txt" ), "%23.15f, ", pmt->M, 1, pmt->DELTA1, 1, 1 );
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\DELTA2_%d.txt" ), "%23.15f, ", pmt->M, 1, pmt->DELTA2, 1, 1 );
  // step5:从M个候选弱分类器中选择K个最佳弱分类器
  {
    //归一化，防止h_candidateWeakClf中元素过大时，使1/(1+exp(-x))为0
    //h_candidateWeakClf = h_candidateWeakClf/max(max(h_candidateWeakClf));
    real maxval = maxreal( pmt->M * totalnum, pmt->_h_candidateWeakClf );
    FOREACH_DO( pmt->M * totalnum, i, pmt->_h_candidateWeakClf[ i ] /= maxval );
  }
  //负样本包标记为0,正样本包标记为1
  //Label_y = zeros(tempnum_neg + 1,1);
  //Label_y(end,1) = 1;
#define Label_y   (i>=tempnum_neg)
  
#define h_candidateWeakClf(y, x) pmt->_h_candidateWeakClf[y*pmt->M+x]
  
  //L = zeros(M,1);//M个错误率(损失函数)
  for ( m = 0; m < pmt->M; ++m ) { //对于每个特征
    real temp_multi = 1.;
    real sumerr = 0.;
    //real P_main[num_neg+1];
    //存储每个样本包的概率,tempnum_neg + 1为总包数，每一个负样本单独做为一个包，所有正样本做为一个包
    
    for ( j = 0; j < tempnum_pos; ++j ) { //所有正样本做为一个包
      real temp_pos_p = 1. / ( 1. + exp( -h_candidateWeakClf( tempnum_neg + j, m ) ) );
      temp_multi *= ( 1 - temp_pos_p );
    }
    sumerr = log( 1. - temp_multi + 0.000000001 );
    
    // FOREACH_DO(num_all, i, sumerr += (Label_y *log(P_main[i] + 0.000000001) + (1-Label_y) *log(1-P_main[i] + 0.000000001)));
    for ( i = 0; i < tempnum_neg; ++i ) { //每一个负样本单独做为一个包
      real temp_neg_p = 1. / ( 1. + exp( -h_candidateWeakClf( i, m ) ) ); //采用S型函数(sigmoid)计算概率,保证输出范围为[0,1]
      real a = log( 1. - temp_neg_p + 0.000000001 );
      sumerr += a;
    }
    
    pmt->L[ m ] = fabs( sumerr ); //损失函数，即错误率
  }
  //将M个错误率从小到大排列
  {
    for ( j = 0; j < pmt->M; ++j ) {
      pmt->L_ind[ j ] = j;
    }
#ifdef _USE_LBP_FEATURE
    //选择L从小到大排序后对应的前K个特征(包括位置、尺度、类型)
#define INDEX_LESS(a, b)  \
    ((pmt->MFeatures[a].Kind<pmt->MFeatures[b].Kind) || \
    ((pmt->MFeatures[a].Kind==pmt->MFeatures[b].Kind) && (pmt->L[a]) < (pmt->L[b])))
    
    QSORT( pmt->L_ind, pmt->L_ind + pmt->M, INDEX_LESS, int );
    {
      int* ind = MALLOC( int, pmt->M );
      int k = 0, cnt[ 9 ] = {0};
      memcpy( ind, pmt->L_ind, pmt->M * sizeof( int ) );
      for ( j = 0; j < pmt->M; ) {
        assert( pmt->MFeatures[ j ].Kind < 9 );
        for ( k = 0; j < pmt->M && k < 9; ++k ) {
          for ( ; cnt[ k ] < pmt->M && pmt->MFeatures[ ind[ cnt[ k ] ] ].Kind != k; ) {
            ++cnt[ k ];
          }
          if ( cnt[ k ] < pmt->M ) {
            pmt->L_ind[ j++ ] = ind[ cnt[ k ] ++ ];
          }
        }
      }
      SAFEFREE( ind );
    }
#undef INDEX_LESS
    
#else
    //选择L从小到大排序后对应的前K个特征(包括位置、尺度、类型)
#define INDEX_LESS(a, b)  ((pmt->L[a]) < (pmt->L[b]))
    
    QSORT( pmt->L_ind, pmt->L_ind + pmt->M, INDEX_LESS, int );
#undef INDEX_LESS
    
#endif
    
  }
  return 1;
}

int CPM_MilTrackSelectObject( MILTRACK* pmt, int pic_h, int pic_w, unsigned char* I, int step,
                              int Y1, int X1, int sample_h, int sample_w ) {
  if ( sample_h <= 0 || sample_w <= 0 ) {
    return 0;
  }
#ifdef _DEBUG_DQ
  pmt->Y1 = Y1;
  pmt->X1 = X1;
  pmt->sample_h = sample_h;
  pmt->sample_w = sample_w;
  randomMFeatures( sample_w, sample_h, pmt->M, pmt->MFeatures ); //随机产生M个特征(类型、位置、尺度)
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\MFeatures_%d.txt" ), "%3d, ", pmt->M, 5, ( int* ) ( pmt->MFeatures ), 5, 1 );
#else
  
  int pos[ 4 ] = {
#include "F:\\public\\bin\\MILTrack\\pos.txt"
                 };
  
  pmt->X1 = X1 = pos[ 0 ];
  pmt->Y1 = Y1 = pos[ 1 ];
  pmt->sample_w = sample_w = pos[ 2 ];
  pmt->sample_h = sample_h = pos[ 3 ];
#endif
  
  return MilTrackUpdate( pmt, pic_h, pic_w, I, step, 0 );
}

int CPM_MilTrack( MILTRACK* pmt, int pic_h, int pic_w, unsigned char* I, int step, int* Y1, int* X1 ) {
  MILPOINT result;
  
  int ret;
  if ( pmt->sample_h <= 0 || pmt->sample_w <= 0 ) {
    return 0;
  }
#ifdef _DEBUG_DQ
  
  ret = randomSelectSample_for_result( pmt->X1, pmt->Y1, pmt->sample_w, pmt->sample_h, pic_w, pic_h, pmt->s,
                                       pmt->num_candidate_result, pmt->num_fix_cr, pmt->YX_candidate );
  if ( 0 == ret )
    return 0;
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\YX_candidate_%d.txt" ), "%3d, ", pmt->num_candidate_result, 2, ( int* ) ( pmt->YX_candidate ), 2, 1 );
#endif
  
  result = compute_reult( pmt->num_candidate_result, pmt->YX_candidate, pmt->sample_w, pmt->sample_h,
                          pmt->K, pmt->MFeatures, pmt->L_ind, I, pic_h, pic_w, step );
  *X1 = pmt->X1 = result.x;
  *Y1 = pmt->Y1 = result.y;
  return MilTrackUpdate( pmt, pic_h, pic_w, I, step, 1 );
}

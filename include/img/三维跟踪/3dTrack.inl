#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "3dTrack.h"


// 09.28. 增加和轨迹方向相关的变量和代码.
// 09.27. 分裂情况增加判断条件: 如果是车辆, 马上分裂处理.


// 常数阈值.
const int FADE_CONFIRM = 1; // 当前未使用, 消失的轨迹立即删除.
const int NORM_CONFIRM = 15;
const int COMB_NEW_THD = 10;
const int SPLT_CONFIRM = 5;
const int EDGE_CONFIRM = 3;


// 跟踪类成员函数.

double TriDimTracker::MoveDirection( int preX, int preY, int curX, int curY ) {
  const double pi = 3.1415926;
  
  if ( curX == preX ) {
    // 目标静止.
    if ( curY == preY ) {
      return 0.0;
    }
    else {
      // 目标垂直向上运动.
      if ( curY > preY ) {
        return 90.0;
      }
      
      // 目标垂直向下运动.
      else {
        return 270.0;
      }
    }
  }
  else if ( curY == preY ) {
    // 目标水平向右运动.
    if ( curX > preX ) {
      return 0.0;
    }
    
    // 目标水平向左运动.
    else {
      return 180.0;
    }
  }
  
  // 目标运动方向不在水平和竖直方向上.
  else {
    double angle = atan( double( curY - preY ) / double( curX - preX ) );
    
    if ( curX < preX ) {
      angle += pi;
    }
    else if ( curY < preY && curX > preX ) {
      angle += ( pi + pi );
    }
    
    return 180.0 * angle / pi;
  }
}

void TriDimTracker::AnalyzeMem( int **norm, int **emerge, int **fade, int **combine, int **split, int num ) {
  *norm = new int[ m_CurObjNum * num ];
  *combine = new int[ m_CurObjNum * num + num ];
  *split = new int[ m_CurObjNum * num + m_CurObjNum ];
  *emerge = new int[ num ];
  *fade = new int[ m_CurObjNum ];
  
  if ( NULL == *norm || NULL == *combine || NULL == *split || NULL == *emerge || NULL == *fade ) {
    exit( -1 );
  }
  
  memset( *norm, 0, m_CurObjNum * num * sizeof( int ) );
  memset( *combine, 0, ( m_CurObjNum * num + num ) * sizeof( int ) );
  memset( *split, 0, ( m_CurObjNum * num + m_CurObjNum ) * sizeof( int ) );
  memset( *emerge, 0, num * sizeof( int ) );
  memset( *fade, 0, m_CurObjNum * sizeof( int ) );
}

void TriDimTracker::DistMatrix( const int *map, uchar **binList, int num, uchar *relation ) {
  uchar * bin = new uchar[ m_SceneH * m_SceneW ];
  
  if ( NULL == bin ) {
    exit( -1 );
  }
  
  memset( relation, 0, m_CurObjNum * num * sizeof( uchar ) );
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + *( map + i ) );
    
    assert( NULL != pt );
    
    Real2Bin( pt->real, bin, m_SceneH, m_SceneW );
    
    for ( int j = 0; j < num; ++j ) {
      if ( BinImgInter( bin, *( binList + j ), m_SceneH, m_SceneW ) > pt->area / 20 ) {
        *( relation + i * num + j ) = 1;
      }
    }
  }
  
  delete [] bin;
  bin = NULL;
}

void TriDimTracker::AnalyzeDistMat( const int *map,
                                    const uchar *relation,
                                    int num,
                                    int *norm,
                                    int *emerge,
                                    int *fade,
                                    int *combine,
                                    int *split ) {
  int i, j, k, sum;
  
  // 单轨迹对应单检测结果.
  for ( i = 0; i < m_CurObjNum; ++i ) {
    for ( j = 0; j < num; ++j ) {
      if ( *( relation + i * num + j ) > 0 ) {
        for ( k = j + 1, sum = 0; k < num; ++k ) {
          sum += *( relation + i * num + k );
        }
        
        if ( 0 == sum ) {
          for ( k = 0, sum = 0; k < m_CurObjNum; ++k ) {
            sum += *( relation + k * num + j );
          }
          
          if ( sum == 1 ) {
            *( norm + i * num + j ) = 1;
          }
        }
        
        break;
      }
    }
  }
  
  // 消失, 分裂.
  for ( i = 0; i < m_CurObjNum; ++i ) {
    for ( j = 0, sum = 0; j < num; ++j ) {
      sum += *( relation + i * num + j );
    }
    
    if ( 0 == sum ) {
      *( fade + i ) = 1;
    }
    else if ( sum > 1 ) {
      int * p = split + i * ( num + 1 );
      
      *( p + 0 ) = sum;
      
      for ( j = 0, k = 1; j < num; ++j ) {
        if ( *( relation + i * num + j ) > 0 ) {
          *( p + k++ ) = j;
        }
      }
    }
  }
  
  // 新目标, 轨迹合并.
  for ( i = 0; i < num; ++i ) {
    for ( j = 0, sum = 0; j < m_CurObjNum; ++j ) {
      sum += *( relation + j * num + i );
    }
    
    if ( sum == 0 ) {
      *( emerge + i ) = 1;
    }
    else if ( sum > 1 ) {
      int * p = combine + i * ( m_CurObjNum + 1 );
      
      *( p + 0 ) = sum;
      
      for ( j = 0, k = 1; j < m_CurObjNum; ++j ) {
        if ( *( relation + j * num + i ) > 0 ) {
          *( p + k++ ) = j;
        }
      }
    }
  }
}

void TriDimTracker::ProcFade( const int *map, const int *fade, const Calibration *pCbr ) {
  assert( NULL != map && NULL != fade && NULL != pCbr );
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    if ( 1 == *( fade + i ) ) {
      TrackTarget *&pt = *( m_TargetList + *( map + i ) );
      
      assert( NULL != pt );
      
      pt->s = ST_DELETE;
    }
  }
}

int TriDimTracker::ProcEmerge( const uchar *src,
                               const int *emerge,
                               uchar **bin,
                               const int *rect,
                               const double *feet,
                               const int *area,
                               const int *center,
                               const int *feeti,
                               const double *len,
                               const double *compact,
                               int num ) {
  int loc, inc = 0;
  
  for ( int i = 0; i < num; ++i ) {
    // 判定为新出现的团块.
    if ( 1 == *( emerge + i ) ) {
      // 查找空闲轨迹指针.
      loc = NullPtr();
      
      // 轨迹数目已达到最大处理数目, 无法增加轨迹.
      if ( 0 > loc ) {
        return inc;
      }
      
      // 增加轨迹.
      else {
        // 轨迹信息初始化.
        InitTrace( loc,             // 轨迹在轨迹指针中的索引.
                   center + i * 2,  // 质心像素坐标.
                   rect + i * 4,    // 团块外框坐标.
                   feet + i * 2,    // 团块底部世界坐标.
                   *( area + i ),      // 团块面积.
                   feeti + i * 2,   // 团块底部像素坐标.
                   *( len + i ),       // 团块实际高度.
                   *( compact + i ) ); // 团块紧实度.
                   
        // 获取目标二值图对应位置的颜色信息.
        ColorMsg( src, *( bin + i ), m_SceneH, m_SceneW, ( *( m_TargetList + loc ) ) ->real );
        
        // 更新轨迹变动条数.
        ++inc;
      }
    }
  }
  
  return inc;
}

void TriDimTracker::ProcOne2One( const uchar *src,
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
                                 int num ) {
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    for ( int j = 0; j < num; ++j ) {
      if ( 1 == *( norm + i * num + j ) ) {
        TrackTarget *&pt = *( m_TargetList + *( map + i ) );
        
        assert( NULL != pt && ST_DELETE != pt->s );
        
        // 正常状态, 增加得分(置信度).
        if ( ST_NORMAL == pt->s ) {
          ++( pt->score );
        }
        
        // 新轨迹, 判定是否达到转换为正常状态的条件.
        else if ( ST_EMERGE == pt->s ) {
          ++( pt->score );
          
          if ( NORM_CONFIRM < pt->score ) {
            pt->s = ST_NORMAL;
            pt->score = 1;
          }
        }
        
        // 正在消失中的轨迹.
        else if ( ST_FADING == pt->s ) {
          pt->s = ST_EMERGE;
          pt->score = 1;
        }
        
        // 出现在合并或分裂情况中的轨迹.
        else if ( ST_COMBINE == pt->s || ST_SPLIT == pt->s ) {
          pt->s = ST_NORMAL;
          pt->score = 1;
        }
        
        // 即将被删除的轨迹.
        else {
          assert( ST_DELETE == pt->s );
          continue;
        }
        
        // 彩色信息更新为当前帧对应的二值结果位置颜色信息.
        ColorMsg( src, *( bin + j ), m_SceneH, m_SceneW, pt->real );
        
        // 面积信息.
        pt->area = *( area + j );
        
        // 方向信息(总在质心信息更新之前).
        pt->direction = MoveDirection( pt->rect.w, pt->rect.h, *( center + j * 2 + 1 ), *( center + j * 2 + 0 ) );
        
        // 质心信息.
        pt->rect.h = *( center + j * 2 + 0 );
        pt->rect.w = *( center + j * 2 + 1 );
        
        // 矩形信息.
        pt->rect.l = *( rect + j * 4 + 0 );
        pt->rect.t = *( rect + j * 4 + 1 );
        pt->rect.r = *( rect + j * 4 + 2 );
        pt->rect.b = *( rect + j * 4 + 3 );
        
        // 底部图像坐标.
        *( pt->fti + 0 ) = *( feeti + j * 2 + 0 );
        *( pt->fti + 1 ) = *( feeti + j * 2 + 1 );
        
        // 底部世界坐标.
        *( pt->ftw + 0 ) = *( feet + j * 2 + 0 );
        *( pt->ftw + 1 ) = *( feet + j * 2 + 1 );
        
        // 实际高度.
        pt->len = *( len + j );
        
        // 紧实度.
        pt->cmt = *( compact + j );
        
        // 预测参数更新.
        pt->pred.update( pCbr->GetTMat(), pt->ftw );
        
        // 计算实际速率, 应该放在预测状态更新之后.
        pt->speed = pt->pred.GetSpeed();
        
        // 预测.
        pt->pred.predict();
      }
    }
  }
}

int TriDimTracker::ProcSplit( const uchar *src,
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
                              int num ) {
  int loc, inc = 0;
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    if ( 1 < *( split + i * ( num + 1 ) + 0 ) ) {
      TrackTarget *&pt = *( m_TargetList + *( map + i ) );
      
      assert( NULL != pt );
      
      // 状态处理.
      if ( ST_DELETE == pt->s ) {
        continue;
      }
      
      if ( ST_EMERGE == pt->s || ST_FADING == pt->s ) {
        pt->s = ST_DELETE;
      }
      else {
        // 分裂状态得分未达到阈值, 仍作为一个整体处理.
        if ( ( ST_SPLIT == pt->s && SPLT_CONFIRM > ++( pt->score ) && pt->attri != VEHICLE ) || ST_NORMAL == pt->s || ST_COMBINE == pt->s ) {
          // 正常和合并状态处理.
          if ( ST_SPLIT != pt->s ) {
            pt->s = ST_SPLIT;
            pt->score = 1;
          }
          
          // 整体二值图信息内存.
          uchar *comb = new uchar[ m_SceneH * m_SceneW ];
          
          if ( NULL == comb ) {
            exit( -1 );
          }
          
          memset( comb, 0, m_SceneH * m_SceneW * sizeof( uchar ) );
          
          // 获取二值图组合信息.
          for ( int j = 0; j < *( split + i * ( num + 1 ) + 0 ); ++j ) {
            const int pos = *( split + i * ( num + 1 ) + ( j + 1 ) );
            
            for ( int k = 0; k < m_SceneH * m_SceneW; ++k ) {
              *( comb + k ) |= *( *( bin + pos ) + k );
            }
          }
          
          // 颜色信息.
          ColorMsg( src, comb, m_SceneH, m_SceneW, pt->real );
          
          // 团块信息.
          BinMap<uchar> tmpBlock( comb, 1, m_SceneH, m_SceneW );
          
          // 面积信息.
          pt->area = *( tmpBlock.GetArea() + 0 );
          
          // 方向信息.
          pt->direction = MoveDirection( pt->rect.w, pt->rect.h, *( tmpBlock.GetCentroid() + 1 ), *( tmpBlock.GetCentroid() + 0 ) );
          
          // 质心信息.
          pt->rect.h = *( tmpBlock.GetCentroid() + 0 );
          pt->rect.w = *( tmpBlock.GetCentroid() + 1 );
          
          // 矩形信息.
          pt->rect.l = *( tmpBlock.GetRect() + 0 );
          pt->rect.t = *( tmpBlock.GetRect() + 1 );
          pt->rect.r = *( tmpBlock.GetRect() + 2 );
          pt->rect.b = *( tmpBlock.GetRect() + 3 );
          
          // 实际长度, 紧实度, 底部世界坐标, 底部图像坐标.
          pCbr->TriDimInfo( comb,
                            m_SceneH,
                            m_SceneW,
                            pt->area,
                            tmpBlock.GetRect(),
                            &( pt->len ),
                            &( pt->cmt ),
                            pt->ftw,
                            pt->fti );
                            
          // 预测信息更新.
          pt->pred.update( pCbr->GetTMat(), pt->ftw );
          
          // 计算实际速率, 应该放在预测状态更新之后.
          pt->speed = pt->pred.GetSpeed();
          
          // 预测.
          pt->pred.predict();
          
          // 释放内存.
          delete [] comb;
          comb = NULL;
        }
        
        // 分裂状态确认, 分裂团块全部当作新团块处理.
        else {
          // 删除原轨迹并将分裂后的第一块的信息生成轨迹由该指针指向, 该轨迹沿用原轨迹ID.
          int index = *( split + i * ( num + 1 ) + 1 );
          
          // 保留原ID.
          int oldID = pt->label;
          
          delete pt;
          pt = NULL;
          
          pt = new TrackTarget( *( len + index ),             // 目标实际高度.
                                *( compact + index ),         // 目标紧实度.
                                feet + index * 2,          // 目标底部世界坐标.
                                feeti + index * 2,         // 目标底部像素坐标.
                                *( center + index * 2 + 0 ),  // 目标质心像素坐标.
                                *( center + index * 2 + 1 ),
                                *( rect + index * 4 + 0 ),    // 目标外框坐标.
                                *( rect + index * 4 + 1 ),
                                *( rect + index * 4 + 2 ),
                                *( rect + index * 4 + 3 ),
                                *( area + index ),            // 目标面积.
                                oldID,                     // 保留原ID.
                                m_SceneH,
                                m_SceneW );
                                
          // 检查轨迹创建是否成功.
          if ( NULL == pt )
            exit( -1 );
            
          pt->pred.predict();
          
          // 颜色信息.
          ColorMsg( src, *( bin + index ), m_SceneH, m_SceneW, pt->real );
          
          // 对其他分裂后的团块新建轨迹.
          for ( int j = 1; j < *( split + i * ( num + 1 ) + 0 ); ++j ) {
            index = *( split + i * ( num + 1 ) + ( j + 1 ) );
            
            // 获取空闲轨迹指针.
            loc = NullPtr();
            
            // 轨迹数目已达到最大处理数目, 无法增加轨迹.
            if ( 0 > loc ) {
              return inc;
            }
            
            // 增加轨迹.
            else {
              InitTrace( loc,                 // 轨迹索引.
                         center + index * 2,  // 质心坐标.
                         rect + index * 4,    // 目标外框.
                         feet + index * 2,    // 目标底部世界坐标.
                         *( area + index ),      // 目标面积.
                         feeti + index * 2,   // 目标底部像素坐标.
                         *( len + index ),       // 目标实际高度.
                         *( compact + index ) ); // 目标紧实度.
                         
              // 目标颜色信息.
              ColorMsg( src, *( bin + index ), m_SceneH, m_SceneW, ( *( m_TargetList + loc ) ) ->real );
              
              ++inc;
            }
          }
        }
      }
    }
  }
  
  return inc;
}

void TriDimTracker::ProcMerge( const uchar *src,
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
                               int num ) {
  uchar * rgb = new uchar[ m_SceneH * m_SceneW * 3 ];
  uchar *tmpRGB = new uchar[ m_SceneH * m_SceneW * 3 ];
  uchar *tmpBin = new uchar[ m_SceneH * m_SceneW ];
  
  if ( NULL == rgb || NULL == tmpRGB || NULL == tmpBin ) {
    exit( -1 );
  }
  
  for ( int i = 0; i < num; ++i ) {
    // 当前检测结果团块由多条轨迹合并而成.
    if ( 1 < *( combine + i * ( m_CurObjNum + 1 ) + 0 ) ) {
      // 所有合并团块的状态都为EMERGE时删除所有合并团块对应轨迹.
      for ( int j = 0, sum_new = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
        TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
        
        if ( pt->s == ST_EMERGE ) {
          ++sum_new;
        }
      }
      
      if ( sum_new == *( combine + i * ( m_CurObjNum + 1 ) + 0 ) ) {
        for ( j = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
          TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
          
          pt->s = ST_DELETE;
        }
        
        continue;
      }
      
      // 合并团块对应的彩色信息.
      ColorMsg( src, *( bin + i ), m_SceneH, m_SceneW, rgb );
      
      for ( j = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
        // 获取发生合并的轨迹指针.
        TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
        
        assert( NULL != pt );
        
        // 状态处理.
        if ( ST_DELETE == pt->s ) {
          continue;
        }
        
        if ( ST_FADING == pt->s ) {
          pt->s = ST_DELETE;
        }
        
        // 在团块中寻找最匹配的位置.
        else {
          // 处于场景边缘的轨迹状态置为删除.
          if ( TraceOnBorder( pt, EDGE_CONFIRM ) > 0 || ( pt->s == ST_EMERGE && pt->score < COMB_NEW_THD ) ) {
            pt->s = ST_DELETE;
          }
          else {
            // 状态信息.
            if ( ST_COMBINE == pt->s ) {
              ++( pt->score );
            }
            else {
              pt->s = ST_COMBINE;
              pt->score = 1;
            }
            
            // 计算搜索范围.
            double minFW[ 2 ], maxFW[ 2 ];
            
            int minFI[ 2 ], maxFI[ 2 ], ratio;
            
            if ( pt->speed > 400 ) {
              ratio = 7;
            }
            else if ( 200 < pt->speed && pt->speed <= 400 ) {
              ratio = 5;
            }
            else {
              ratio = 3;
            }
            
            maxFW[ 0 ] = pt->pred.GetPredX() + ratio * sqrt( pt->pred.GetVX() );
            maxFW[ 1 ] = pt->pred.GetPredY() + ratio * sqrt( pt->pred.GetVY() );
            
            minFW[ 0 ] = pt->pred.GetPredX() - ratio * sqrt( pt->pred.GetVX() );
            minFW[ 1 ] = pt->pred.GetPredY() - ratio * sqrt( pt->pred.GetVY() );
            
            DimTrans32( pCbr->GetTMat(), maxFW, maxFI );
            DimTrans32( pCbr->GetTMat(), minFW, minFI );
            
            int minx, maxx, miny, maxy;
            
            if ( minFI[ 0 ] < maxFI[ 0 ] ) {
              minx = minFI[ 0 ];
              maxx = maxFI[ 0 ];
            }
            else {
              minx = maxFI[ 0 ];
              maxx = minFI[ 0 ];
            }
            
            if ( minFI[ 1 ] < maxFI[ 1 ] ) {
              miny = minFI[ 1 ];
              maxy = maxFI[ 1 ];
            }
            else {
              miny = maxFI[ 1 ];
              maxy = minFI[ 1 ];
            }
            
            // 位置匹配.
            double curDist = 0.0;
            double minDist = 9e99;
            
            int offsetX = 0;
            int offsetY = 0;
            
            int R, G, B;
            
            bool tag = false;
            
            for ( int m = miny; m <= maxy; ++m ) {
              for ( int n = minx; n <= maxx; ++n ) {
                if ( m > 0 && m < m_SceneH - 1 && n > 0 && n < m_SceneW - 1 ) {
                  curDist = 0;
                  
                  Move( pt->real, m_SceneH, m_SceneW, n - pt->fti[ 0 ], m - pt->fti[ 1 ], tmpRGB );
                  
                  // 得到移动后团块的二值图信息.
                  Real2Bin( tmpRGB, tmpBin, m_SceneH, m_SceneW );
                  
                  if ( BinImgInter( tmpBin, *( bin + i ), m_SceneH, m_SceneW ) >= pt->area / 2 ) {
                    for ( int y = 0; y < m_SceneH; ++y ) {
                      for ( int x = 0; x < m_SceneW; ++x ) {
                        if ( *( rgb + y * m_SceneW * 3 + x * 3 + 0 ) != 0 ||
                             *( rgb + y * m_SceneW * 3 + x * 3 + 1 ) != 0 ||
                             *( rgb + y * m_SceneW * 3 + x * 3 + 2 ) != 0 ) {
                          B = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 0 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 0 );
                          G = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 1 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 1 );
                          R = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 2 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 2 );
                        }
                        else {
                          B = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 0 ) + 255;
                          G = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 1 ) + 255;
                          R = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 2 ) + 255;
                        }
                        
                        curDist += ( B * B + G * G + R * R );
                      }
                    }
                    
                    if ( curDist < minDist ) {
                      tag = true;
                      minDist = curDist;
                      
                      offsetX = n - pt->fti[ 0 ];
                      offsetY = m - pt->fti[ 1 ];
                    }
                  }
                }
              }
            }
            
            if ( !tag ) {
              pt->s = ST_DELETE;
              continue;
            }
            
            // 方向信息.
            pt->direction = MoveDirection( pt->rect.w, pt->rect.h, pt->rect.w + offsetX, pt->rect.h + offsetY );
            
            // 外框, 质心.
            pt->rect.w += offsetX;
            pt->rect.h += offsetY;
            
            pt->rect.l += offsetX;
            pt->rect.t += offsetY;
            pt->rect.r += offsetX;
            pt->rect.b += offsetY;
            
            // 目标底部像素坐标.
            pt->fti[ 0 ] += offsetX;
            pt->fti[ 1 ] += offsetY;
            
            // 目标底部世界坐标.
            DimTrans23( pCbr->GetITMat(), pt->fti, pt->ftw );
            
            // 颜色信息移动.
            ColorMove( pt, offsetX, offsetY );
            
            // 预测参数更新.
            pt->pred.update( pCbr->GetTMat(), pt->ftw );
            
            // 计算实际速率, 应该放在预测状态更新之后.
            pt->speed = pt->pred.GetSpeed();
            
            // 预测.
            pt->pred.predict();
          }
        }
      }
    }
  }
  
  delete [] tmpBin;
  tmpBin = NULL;
  delete [] tmpRGB;
  tmpRGB = NULL;
  delete [] rgb;
  rgb = NULL;
}

void TriDimTracker::InitTrace( int loc,
                               const int *center,
                               const int *rect,
                               const double *feet,
                               int area,
                               const int *feeti,
                               double len,
                               double compact ) {
  TrackTarget *&pt = *( m_TargetList + loc );
  
  pt = new TrackTarget( len,
                        compact,
                        feet,
                        feeti,
                        *( center + 0 ),
                        *( center + 1 ),
                        *( rect + 0 ),
                        *( rect + 1 ),
                        *( rect + 2 ),
                        *( rect + 3 ),
                        area,
                        NewID(),
                        m_SceneH,
                        m_SceneW );
                        
  if ( NULL == pt )
    exit( -1 );
    
  pt->pred.predict();
}

void TriDimTracker::ColorMove( TrackTarget *&pt, int offsetX, int offsetY ) {
  assert( NULL != pt );
  
  uchar *rgb = new uchar[ m_SceneH * m_SceneW * 3 ];
  
  if ( NULL == rgb ) {
    exit( -1 );
  }
  
  // 颜色信息移动.
  Move( pt->real, m_SceneH, m_SceneW, offsetX, offsetY, rgb );
  memcpy( pt->real, rgb, m_SceneH * m_SceneW * 3 * sizeof( uchar ) );
  
  delete [] rgb;
  rgb = NULL;
}

void TriDimTracker::MultiTrack( const uchar *src, const unsigned int *block, int num, const Calibration *pCbr, const TriDimModel *pTDM ) {
  // 保存轨迹条数信息.
  int preObjNum = m_CurObjNum;
  
  // 三维信息内存分配.
  double *pHeight = new double[ num ];
  double *compact = new double[ num ];
  double *pFeetWP = new double[ num * 2 ];
  
  int *pFeetIP = new int[ num * 2 ];
  int *map = new int[ m_CurObjNum ];
  
  uchar *relation = new uchar[ m_CurObjNum * num ];
  
  if ( NULL == pHeight || NULL == compact || NULL == pFeetWP || NULL == pFeetIP || NULL == map || NULL == relation ) {
    exit( -1 );
  }
  
  int *normal, *emerge, *fade, *combine, *split;
  
  AnalyzeMem( &normal, &emerge, &fade, &combine, &split, num );
  
  // 获取团块信息.
  BinMap<unsigned int> geoMsg( block, num, m_SceneH, m_SceneW );
  
  // 分块获取团块三维相关信息.
  for ( int i = 0; i < num; ++i ) {
    pCbr->TriDimInfo( *( geoMsg.GetBinPtr() + i ),
                      m_SceneH,
                      m_SceneW,
                      *( geoMsg.GetArea() + i ),
                      geoMsg.GetRect() + i * 4,
                      pHeight + i,
                      compact + i,
                      pFeetWP + i * 2,
                      pFeetIP + i * 2 );
  }
  
  // 轨迹位置与顺序数组映射关系.
  TargetMap( map );
  
  // 生成距离矩阵.
  DistMatrix( map, geoMsg.GetBinPtr(), num, relation );
  
  // 分析距离矩阵.
  AnalyzeDistMat( map, relation, num, normal, emerge, fade, combine, split );
  
  // 处理单轨迹对应单检测团块结果的情况.
  ProcOne2One( src,
               map,
               normal,
               geoMsg.GetBinPtr(),
               geoMsg.GetRect(),
               pFeetWP,
               geoMsg.GetArea(),
               geoMsg.GetCentroid(),
               pFeetIP,
               pHeight,
               compact,
               pCbr,
               num );
               
  // 处理轨迹团块出现分裂的情况.
  preObjNum += ProcSplit( src,
                          map,
                          split,
                          geoMsg.GetBinPtr(),
                          geoMsg.GetRect(),
                          pFeetWP,
                          geoMsg.GetArea(),
                          geoMsg.GetCentroid(),
                          pFeetIP,
                          pHeight,
                          compact,
                          pCbr,
                          num );
                          
  // 处理轨迹消失的情况.
  ProcFade( map, fade, pCbr );
  
  // 处理出现新轨迹的情况.
  preObjNum += ProcEmerge( src,
                           emerge,
                           geoMsg.GetBinPtr(),
                           geoMsg.GetRect(),
                           pFeetWP,
                           geoMsg.GetArea(),
                           geoMsg.GetCentroid(),
                           pFeetIP,
                           pHeight,
                           compact,
                           num );
                           
  // 处理多轨迹合并的情况.
  ProcMerge( src,
             map,
             combine,
             geoMsg.GetBinPtr(),
             geoMsg.GetRect(),
             pFeetWP,
             geoMsg.GetArea(),
             geoMsg.GetCentroid(),
             pFeetIP,
             pHeight,
             compact,
             pCbr,
             num );
             
  // 释放分析距离矩阵使用的内存.
  FreeAnalyzeMem( &normal, &emerge, &fade, &combine, &split );
  
  // 内存释放.
  delete [] relation;
  relation = NULL;
  delete [] map;
  map = NULL;
  delete [] pHeight;
  pHeight = NULL;
  delete [] compact;
  compact = NULL;
  delete [] pFeetWP;
  pFeetWP = NULL;
  delete [] pFeetIP;
  pFeetIP = NULL;
  
  // 场景边缘的轨迹直接删除.
  for ( i = 0; i < m_MaxObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + i );
    
    if ( NULL != pt && TraceOnBorder( pt, EDGE_CONFIRM ) > 0 ) {
      pt->s = ST_DELETE;
    }
  }
  
  // 释放需要删除的轨迹的内存空间.
  preObjNum += DelInvalid();
  
  // 轨迹分类.
  TraceClassify( pTDM );
  
  // 更新当前轨迹条数.
  m_CurObjNum = preObjNum;
}

void TriDimTracker::TraceClassify( const TriDimModel *pTDM ) {
  for ( int i = 0; i < m_MaxObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + i );
    
    if ( NULL != pt && ST_DELETE != pt->s ) {
      const int r = int( pt->fti[ 1 ] * pTDM->GetHSubNum() / m_SceneH );
      const int c = int( pt->fti[ 0 ] * pTDM->GetWSubNum() / m_SceneW );
      
      assert( -1 < r && r < pTDM->GetHSubNum() && -1 < c && c < pTDM->GetWSubNum() );
      
      const int size = pTDM->GetModelSize( r, c );
      
      if ( pt->area < 1.5 * size && pt->area > 0.5 * size && pt->len >= 100 && pt->len <= 195 ) {
        pt->attri = PERSON;
      }
      else if ( pt->speed < 200 && pt->area >= 1.5 * size && pt->area <= 5 * size ) {
        pt->attri = CROWD;
      }
      else if ( ( pt->speed > 200 && pt->area > 1.5 * size ) || ( pt->area > 5 * size ) ) {
        pt->attri = VEHICLE;
      }
      else {
        pt->attri = UNKNOW;
      }
    }
  }
}

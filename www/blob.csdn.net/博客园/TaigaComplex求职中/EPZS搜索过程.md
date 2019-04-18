# EPZS搜索过程 - TaigaComplex求职中 - 博客园







# [EPZS搜索过程](https://www.cnblogs.com/TaigaCon/p/3900394.html)





EPZS(Enhance Predictive Zonal Search) 增强预测区域搜索，是一种整像素运动估计的搜索算法。

EPZS采用的是相关性较高的预测方法。这里的相关性较高是指，更多地根据已有的条件，来进行运动向量的预测（如采用相邻块的mv作为当前搜索块的mv进行预测）。



# Search Set

在搜索范围内的所有mv，可以被归结为集合$S$，EPZS会从$S$中按照其算法的规律来选择特定的子集$S’$。$S’$可以包含以下几种搜索方式。



#### 1.$S_1$，mvp，(0, 0)

$mv_{prev} ,  mv(0, 0)$

![](https://images0.cnblogs.com/blog/421096/201410/170000022636240.jpg)





#### 2.$S_2$，当前块的相邻块mv，包括ABCD四个mv，co-located块mv，做mv_scale，即对于POC的距离进行比例增减。

$mv_a,mv_b,mv_c,mv_d,mv_{co}$

![](https://images0.cnblogs.com/blog/421096/201410/170024489355460.jpg)



#### 3.$S_3$，这种方式基于第0个参考图像的mv，当前参考图像的mv需要由ref0的mv以POC的距离做mv_scale后得到

$mv_{refn} = {mv_{ref0}} \cdot {\frac{POC_{cur}-POC_{n}}{POC_{cur}-POC_{0}}}$



![](https://images0.cnblogs.com/blog/421096/201410/170023318262702.jpg)


#### 4.$S_4$，以mvp或者（0, 0）为中心，呈正方形窗口的mv集合

$mv_{window}$



![](https://images0.cnblogs.com/blog/421096/201410/170025288738859.jpg)



#### 5.$S_5$，由于运动有可能不是匀速地运动，而是以相同加速度来进行，因此可以用前两张编码图像的mv计算出当前mv

$mv_{cur} = mv_{t-1} + (mv_{t-1} - mv_{t-2})$



![](https://images0.cnblogs.com/blog/421096/201410/170027180446912.jpg)



#### 6.$S_6$，co-located块的相邻mv，需做mv_scale

$mv_{cola},mv_{colb},mv_{colc},mv_{cold}$



![](https://images0.cnblogs.com/blog/421096/201410/170028467323908.jpg)



#### 7.$S_7$，Memory Prediction即采用同一块位置中，上一个块类型（Block type - 1）做预测得到的mv，采用其相邻mv，做mv_scale

$mv_{preva},mv_{prevb},mv_{prevc},mv_{prevd}$



![](https://images0.cnblogs.com/blog/421096/201410/170030065449923.jpg)



#### 8.$S_8$，如果采用的是HME（Hierarchical Motion Estimation）分层运动估计的编码方式，那么应该选择该次HME运动估计的参考图像，从参考图像中选择co-located块，以及co-located块周边的块（带col块共9个块）的mv作为候选mv

$mv_{hme\_co},mv_{hme\_surround\_of\_co}$



![](https://images0.cnblogs.com/blog/421096/201410/170031168881249.jpg)



#### 9.$S_9$，上层块mv，做mv_scale

$mv_{4 \times 4} = mv_{8 \times 4},mv_{8 \times 4} = mv{8 \times 8},…$



![](https://images0.cnblogs.com/blog/421096/201410/170035529664324.jpg)



从如上的mv集合明显看出，基本上，如果当前mv是由参考图像已有mv预测出来的情况，都需要进行mv_scale，以此提高mv预测的准确性。



在预测完所有mv的可能位置后，得到mv集合$S$，由于此处进行的是整像素预测，所以需要对mv求整。接下来需要在$S$中求出mv_cost最小的mv，当然，mv需要在search range内。



# Termination

Termination用于判断当前mv预测是否满足停止条件（mv_cost < stopCriterion）,满足则代表当前mv预测已经足够准确，可以退出。
- 在求出第一个子集$S_1$的mv_cost后，进行第一次Termination，记为$T_1$。$T_1$是由当前块类型的像素点个数（如16x16像素点个数为256）与lambda联合组成。JM18.6中$T_1$，即stopCriterion的值为如下形式:

$stopCriterion = (N_{pixel} \times \frac{3}{4}) \times 2^{LAMBDA\_ACCURACY\_BITS} + LAMBDA\_DIST $

- 如果第一个停止条件不能满足，则接下来需要对所有的mv集合进行预测，从中选出mv_cost最小的mv。在得到最小mv后，进行第二次Termination，记为$T_2$。$T_2$由3块类型的相邻块中最小SAD，用$stopCriterion_{T_1}$修正计算得来。
- 首先定义上界与下界:

$\begin{align*}
Upper &= (N_{pixel} \times 3) \times 2^{LAMBDA\_ACCURACY\_BITS} + LAMBDA\_DIST \\
Lower &= (N_{pixel} \times \frac{1}{4}) \times 2^{LAMBDA\_ACCURACY\_BITS} + LAMBDA\_DIST 
\end{align*}$

- 上下界修正:

$tmpStopCriterion = Mid( Min(SAD_A,SAD_B,SAD_C), Upper, Lower)$

- StopCriterion修正:

$stopCriterion = \frac{(Max(tmpStopCriterion, stopCriterion) \times 8 + stopCriterion)}{8} + LAMBDA\_DIST $


- 如果第二个停止条件仍然不满足，接下来进行最后的小区域搜索，搜索方式有菱形，正方形，大菱形等。如果搜索得到最佳mv为菱形（或正方形）的中心点，则停止搜索，得到最终的最佳整数mv。可以参考http://www.cnblogs.com/TaigaCon/p/3788984.html



相对于UMHS，EPZS更多地考虑了运动的相关性，基本上采用的都是用已得到的mv预测当前mv，这种方式在规律运动图像的预测中会比UMHS更有效率。但是UMHS的预测更加考虑周全，对搜索范围进行了更细密的搜索，一旦碰上无规律运动图像，UMHS预测可以得到比EPZS更准确的预测效果。因此，根据图像性质来选择不同的搜索算法将能更好地提升编码质量。



jm18.6:
/*!
***********************************************************************
* \brief
*    FAST Motion Estimation using EPZS
*    AMT/HYC
***********************************************************************
*/
distblk                                            //  ==> minimum motion cost after search
EPZS_motion_estimation (Macroblock * currMB,     // <--  current Macroblock
                          MotionVector * pred_mv,  // <--  motion vector predictor in sub-pel units
                          MEBlock * mv_block,      // <--  motion vector information
                          distblk min_mcost,       // <--  minimum motion cost (cost for center or huge value)
                          int lambda_factor        // <--  lagrangian parameter for determining motion cost
                          )
{
  Slice *currSlice = currMB->p_Slice;
  VideoParameters *p_Vid = currMB->p_Vid;
  InputParameters *p_Inp = currMB->p_Inp;
  EPZSParameters *p_EPZS = currSlice->p_EPZS;
  PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  int blocktype = mv_block->blocktype;

  int list = mv_block->list;
  int cur_list = list + currMB->list_offset;
  short ref = mv_block->ref_idx;
  MotionVector *mv = &mv_block->mv[list];
  SearchWindow *searchRange = &mv_block->searchRange;
  int mapCenter_x = searchRange->max_x - mv->mv_x;
  int mapCenter_y = searchRange->max_y - mv->mv_y;
  StorablePicture *ref_picture = currSlice->listX[cur_list][ref];

  distblk lambda_dist = weighted_cost(lambda_factor, 2);
  distblk stopCriterion = p_EPZS->medthres[blocktype] + lambda_dist;
  distblk *prevSad = &p_EPZS->distortion[cur_list][blocktype - 1][mv_block->pos_x2];

  MotionVector *p_motion = NULL; //only work for EPZSSpatialMem

  EPZSStructure *searchPatternF = p_EPZS->searchPattern;
  uint16 **EPZSMap = &p_EPZS->EPZSMap[mapCenter_y];
  uint16 *EPZSPoint = &p_EPZS->EPZSMap[searchRange->max_y][searchRange->max_x];

  MotionVector center = pad_MVs (*mv, mv_block);
  MotionVector pred = pad_MVs (*pred_mv, mv_block);
  MotionVector tmp = *mv, cand = center;

  ++p_EPZS->BlkCount;
  if (p_EPZS->BlkCount == 0)
    ++p_EPZS->BlkCount;

  if (p_Inp->EPZSSpatialMem)
  {
#if EPZSREF
    p_motion = &p_EPZS->p_motion[cur_list][ref][blocktype - 1][mv_block->block_y][mv_block->pos_x2];
#else
    p_motion = &p_EPZS->p_motion[cur_list][blocktype - 1][mv_block->block_y][mv_block->pos_x2];
#endif
  }

  // Clear EPZSMap
  // memset(EPZSMap[0],FALSE,searcharray*searcharray);
  // Check median candidate;
  //p_EPZS->EPZSMap[0][0] = p_EPZS->BlkCount;
  *EPZSPoint = p_EPZS->BlkCount;

  //--- initialize motion cost (cost for motion vector) and check ---
  //(0,0)作为mv
  min_mcost = mv_cost (p_Vid, lambda_factor, &cand, &pred);

  //--- add residual cost to motion cost ---
  min_mcost += mv_block->computePredFPel (ref_picture, mv_block, DISTBLK_MAX - min_mcost, &cand);

  // Additional threshold for ref>0
  if ((ref > 0 && currSlice->structure == FRAME) && (*prevSad < distblkmin (p_EPZS->medthres[blocktype] + lambda_dist, min_mcost))) 
  {//满足返回条件
#if EPZSREF
    if (p_Inp->EPZSSpatialMem)
#else 
    if (p_Inp->EPZSSpatialMem && ref == 0)
#endif 
    {
      *p_motion = tmp;
    }
    return min_mcost;
  }

  //! If p_EPZS->medthres satisfied, then terminate, otherwise generate Predictors
  //! Condition could be strengthened by consideration distortion of adjacent partitions.
  if (min_mcost > stopCriterion)
  {
    SPoint *p_EPZS_point = p_EPZS->predictor->point;
    Boolean checkMedian = FALSE;
    distblk second_mcost = DISTBLK_MAX;
    distblk mcost;
    int prednum = 5;
    int conditionEPZS;
    MotionVector tmp2 = {0, 0}, tmv;
    int pos;
    short invalid_refs = 0;

    stopCriterion = EPZSDetermineStopCriterion (p_EPZS, prevSad, mv_block, lambda_dist);

    if (min_mcost < (stopCriterion >> 1))
    {
#if EPZSREF
      if (p_Inp->EPZSSpatialMem)
#else 
      if (p_Inp->EPZSSpatialMem && ref == 0)
#endif 
      {
        *p_motion = tmp;
      }

      return min_mcost;
    }

    //! Add Spatial Predictors in predictor list.
    //! Scheme adds zero, left, top-left, top, top-right. Note that top-left adds very little
    //! in terms of performance and could be removed with little penalty if any.
    //当前块相邻mv,做mv_scale
    invalid_refs = EPZS_spatial_predictors (p_EPZS, mv_block, 
      list, currMB->list_offset, ref, motion);



    //当前类型的block_type(8x4)的上一个block_type(8x8),相同位置留下来的mv,采用其相邻mv
    /*
     *      
     *   +--------+----+----+                 
     *   |        |    B    |                 
     *   |        |____A____|                 
     *   |        |         |                 
     *   |        |         |                 
     *   |        +----+----+                 
     *   |                  |                 
     *   |                  |                 
     *   |                  |                 
     *   |                  |                 
     *   +------------------+                 
     *    
     *   B : 8x4    A 8x8
     *     macroblock                      
     *                                     
     *memory_mv_b = surrounding_of_mv_a
     *采用mv_a的相邻mv
     *
     */
    if (p_Inp->EPZSSpatialMem)
      EPZS_spatial_memory_predictors (p_EPZS, mv_block, cur_list, &prednum, ref_picture->size_x >> 2);
   
    //if (p_Inp->HMEEnable == 1 && p_Inp->EPZSUseHMEPredictors == 1 && blocktype == 4)
    //if (p_Inp->HMEEnable == 1 && p_Inp->EPZSUseHMEPredictors == 1 && (currSlice->slice_type == P_SLICE || currSlice->slice_type == SP_SLICE || p_Inp->EnableReorderBslice) )
    //如果采用HME(Hierarchical Motion Estimate),即B帧的参考图像选择方式有所不同
    //选择此时的分层参考图像(Hierarchical ref pic)对应块的相邻mv
    if (p_Inp->HMEEnable == 1 && p_Inp->EPZSUseHMEPredictors == 1)
      EPZS_hierarchical_predictors (p_EPZS, mv_block, &prednum, ref_picture, currSlice);

#if (MVC_EXTENSION_ENABLE)
    if ( p_Inp->EPZSTemporal[currSlice->view_id] && blocktype < 5 ) 
#else
    // Temporal predictors
    //colocate块的相邻块mv
    if (p_Inp->EPZSTemporal && blocktype < 5)
#endif
    {
      EPZS_temporal_predictors (currMB, ref_picture, p_EPZS, mv_block, &prednum, stopCriterion, min_mcost);
    }

    //! Window Size Based Predictors
    //! Basically replaces a Hierarchical ME concept and helps escaping local minima, or
    //! determining large motion variations.
    //! Following predictors can be adjusted further (i.e. removed, conditioned etc)
    //! based on distortion, correlation of adjacent MVs, complexity etc. These predictors
    //! and their conditioning could also be moved after all other predictors have been
    //! tested. Adaptation could also be based on type of material and coding mode (i.e.
    //! field/frame coding,MBAFF etc considering the higher dependency with opposite parity field
    //conditionEPZS = ((min_mcost > stopCriterion)
    // && (p_Inp->EPZSFixed > 1 || (p_Inp->EPZSFixed && currSlice->slice_type == P_SLICE)));
    //conditionEPZS = ((ref == 0) && (blocktype < 5) && (min_mcost > stopCriterion)
    //&& (p_Inp->EPZSFixed > 1 || (p_Inp->EPZSFixed && currSlice->slice_type == P_SLICE)));
    //conditionEPZS = ((min_mcost > stopCriterion) && ((ref < 2 && blocktype < 4)
    conditionEPZS = (p_Inp->EPZSFixed == 3 && (currMB->mb_x == 0 || currMB->mb_y == 0))
      || ((min_mcost > 3 * stopCriterion) && ((ref < 2 && blocktype < 4) || (ref < 1 && blocktype == 4)
      || ((currSlice->structure != FRAME || currMB->list_offset)
      && ref < 3))
      && (p_Inp->EPZSFixed > 1 || (p_Inp->EPZSFixed && currSlice->slice_type == P_SLICE)));

    //正方形窗口mv
    if (conditionEPZS)
      EPZSWindowPredictors (mv, p_EPZS->predictor, &prednum, 
      (p_Inp->EPZSAggressiveWindow != 0) || ((blocktype < 5) && (invalid_refs > 2) && (ref < 1 + (currSlice->structure != FRAME || currMB->list_offset)))
      ? p_EPZS->window_predictor_ext : p_EPZS->window_predictor);

    //! Blocktype/Reference dependent predictors.
    //! Since already mvs for other blocktypes/references have been computed, we can reuse
    //! them in order to easier determine the optimal point. Use of predictors could depend
    //! on cost,
    //conditionEPZS = (ref == 0 || (ref > 0 && min_mcost > stopCriterion) || currSlice->structure != FRAME || currMB->list_offset);
    conditionEPZS = (ref == 0 || (ref > 0 && min_mcost > 2 * stopCriterion));

    //上层宏块mv,做mv_scale
    if (conditionEPZS && currMB->mbAddrX != 0 && p_Inp->EPZSBlockType)
      EPZSBlockTypePredictorsMB (currSlice, mv_block, p_EPZS_point, &prednum);

    //! Check all predictors
    
    //循环内,对所有的cand mv(S1,S2,S3,S4)做cost.取出最佳mv
    for (pos = 0; pos < prednum; ++pos)
    {
      tmv = p_EPZS_point[pos].motion;
      set_integer_mv(&tmv);

      //if (((iabs (tmv.mv_x - mv->mv_x) > searchRange->max_x || iabs (tmv.mv_y - mv->mv_y) > searchRange->max_y)) && (tmv.mv_x || tmv.mv_y))
      if ((iabs (tmv.mv_x - mv->mv_x) - searchRange->max_x <= 0) && (iabs (tmv.mv_y - mv->mv_y) - searchRange->max_y <= 0))
      {
        EPZSPoint = &EPZSMap[tmv.mv_y][mapCenter_x + tmv.mv_x];
        if (*EPZSPoint != p_EPZS->BlkCount)
        {
          *EPZSPoint = p_EPZS->BlkCount;
          cand = pad_MVs (tmv, mv_block);

          //--- set motion cost (cost for motion vector) and check ---
          mcost = mv_cost (p_Vid, lambda_factor, &cand, &pred);

          if (mcost < second_mcost)
          {
            mcost += mv_block->computePredFPel (ref_picture, mv_block, second_mcost - mcost, &cand);

            //--- check if motion cost is less than minimum cost ---
            if (mcost < min_mcost)
            {
              tmp2 = tmp;
              tmp = tmv;
              second_mcost = min_mcost;
              min_mcost = mcost;
              checkMedian = TRUE;
            }
            //else if (mcost < second_mcost && (tmp.mv_x != tmv.mv_x || tmp.mv_y != tmv.mv_y))
            else if (mcost < second_mcost)
            {
              tmp2 = tmv;
              second_mcost = mcost;
              checkMedian = TRUE;
            }
          }
        }
      }
    }

    //! Refine using EPZS pattern if needed
    //! Note that we are using a conservative threshold method. Threshold
    //! could be tested after checking only a certain number of predictors
    //! instead of the full set. Code could be easily modified for this task.
    if (min_mcost > stopCriterion)
    {
      const int mv_range = 10;
      int patternStop = 0, pointNumber = 0, checkPts, nextLast = 0;
      int totalCheckPts = 0, motionDirection = 0;

      //! Adapt pattern based on different conditions.
      if (p_Inp->EPZSPattern != 0)
      {
        if ((min_mcost < stopCriterion + ((3 * p_EPZS->medthres[blocktype]) >> 1)))
        {
          if ((tmp.mv_x == 0 && tmp.mv_y == 0) 
            || (iabs (tmp.mv_x - mv->mv_x) < (mv_range) && iabs (tmp.mv_y - mv->mv_y) < (mv_range)))
            searchPatternF = p_Vid->sdiamond;
          else
            searchPatternF = p_Vid->square;
        }
        else if (blocktype > 4 || (ref > 0 && blocktype != 1))
          searchPatternF = p_Vid->square;
        else
          searchPatternF = p_EPZS->searchPattern;
      }

      //! center on best predictor
      center = tmp;

      for (;;)
      {
        totalCheckPts = searchPatternF->searchPoints;
        do
        {//循环对得到的最佳mv是做小范围搜索:square,dimond,etc.
          checkPts = totalCheckPts;
          do
          {//循环对小范围进行各个像素点的搜索
            tmv = add_MVs (center, &(searchPatternF->point[pointNumber].motion));

            if (((iabs (tmv.mv_x - mv->mv_x) - searchRange->max_x) <= 0) && ((iabs (tmv.mv_y - mv->mv_y) - searchRange->max_y) <= 0))
            {
              //EPZSMap用于记录当前点是否被搜索过,一旦搜索过,则不再搜索,这样的话就不会跟//! Check all predictors重复了
              EPZSPoint = &EPZSMap[tmv.mv_y][mapCenter_x + tmv.mv_x];
              if (*EPZSPoint != p_EPZS->BlkCount)
              {
                *EPZSPoint = p_EPZS->BlkCount;
                cand = pad_MVs (tmv, mv_block);

                mcost = mv_cost (p_Vid, lambda_factor, &cand, &pred);

                if (mcost < min_mcost)
                {
                  mcost += mv_block->computePredFPel (ref_picture, mv_block, min_mcost - mcost, &cand);

                  if (mcost < min_mcost)
                  {
                    tmp = tmv;
                    min_mcost = mcost;
                    motionDirection = pointNumber;
                  }
                }
              }
            }
            ++pointNumber;
            if (pointNumber >= searchPatternF->searchPoints)
              pointNumber -= searchPatternF->searchPoints;
            checkPts--;
          }
          while (checkPts > 0);
          
          //对得到的最佳mv,如果是center(第一次的center是mvp),则停止当前循环
          if (nextLast || ((tmp.mv_x == center.mv_x) && (tmp.mv_y == center.mv_y)))
          {
            patternStop = searchPatternF->stopSearch;
            searchPatternF = searchPatternF->nextpattern;
            totalCheckPts = searchPatternF->searchPoints;
            nextLast = searchPatternF->nextLast;
            motionDirection = 0;
            pointNumber = 0;
          }
          else
          {
            totalCheckPts = searchPatternF->point[motionDirection].next_points;
            pointNumber = searchPatternF->point[motionDirection].start_nmbr;
            center = tmp;
          }
        }
        while (patternStop != 1);

        if ((ref > 0) && (currSlice->structure == FRAME) 
          && ((4 * *prevSad < min_mcost) || ((3 * *prevSad < min_mcost) && (*prevSad <= stopCriterion))))
        {
          *mv = tmp;
#if EPZSREF
          if (p_Inp->EPZSSpatialMem)
#else  
          if (p_Inp->EPZSSpatialMem && ref == 0)
#endif  
          {
            *p_motion = tmp;
          }

          return min_mcost;
        }

        //! Check Second best predictor with EPZS pattern
        conditionEPZS = (checkMedian == TRUE)
          && ((currSlice->slice_type == P_SLICE) || (blocktype < 5))
          && (min_mcost > stopCriterion) && (p_Inp->EPZSDual > 0);

        if (!conditionEPZS)
          break; 
        //如果上方条件有多于0个不满足的，则退出循环
        //退出条件1:  EPZSDual = 0 ,即循环只需要执行一次
        //        2:  cost足够小了
        //        3:  blocktypc = 5,6,7
        //        4:  不为P_SLICE
        //        5:  checkMedian = false,即由于EPZSDual,而执行完了第二次

        pointNumber = 0;
        patternStop = 0;
        motionDirection = 0;
        nextLast = 0;

        if ((tmp.mv_x == 0 && tmp.mv_y == 0) || (tmp.mv_x == mv->mv_x && tmp.mv_y == mv->mv_y))
        {
          if (iabs (tmp.mv_x - mv->mv_x) < (mv_range) && iabs (tmp.mv_y - mv->mv_y) < (mv_range))
            searchPatternF = p_Vid->sdiamond;
          else
            searchPatternF = p_Vid->square;
        }
        else
          searchPatternF = p_EPZS->searchPatternD;

        //! Second best. Note that following code is identical as for best predictor.
        //由于EPZSDual > 0,开启第二次循环,以原点(当前mv_block所在坐标)为中心,开始搜索
        center = tmp2;
        checkMedian = FALSE;
      }
    }
  }

  if ((ref == 0) || (*prevSad > min_mcost))
    *prevSad = min_mcost;
#if EPZSREF
  if (p_Inp->EPZSSpatialMem)
#else  
  if (p_Inp->EPZSSpatialMem && ref == 0)
#endif  
  {
    *p_motion = tmp;
    //printf("value %d %d %d %d\n", p_motion->mv_x, p_motion->mv_y, p_motion[cur_list][ref][0][0][0].mv_x, p_motion[list + list_offset][ref][0][0][0].mv_y);
    //printf("xxxxx %d %d %d %d\n", p_motion->mv_x, p_motion->mv_y, p_motion[cur_list][ref][blocktype - 1][mv_block->block_y][pic_pix_x2].mv_x, p_motion[cur_list][ref][blocktype - 1][mv_block->block_y][pic_pix_x2].mv_y);
  }

  *mv = tmp;

  return min_mcost;
}
View Code















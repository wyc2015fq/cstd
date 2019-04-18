# h.264 Bi-Predictive Motion Search - TaigaComplex求职中 - 博客园







# [h.264 Bi-Predictive Motion Search](https://www.cnblogs.com/TaigaCon/p/4510457.html)





在做B帧的运动预测时，有两组参考图像列表（refList0, refList1），需要进行分别前向预测、后向预测。传统的预测方式是：
- 
对refList0进行前向预测，得到最佳前向ref与mv。

- 
对refList1进行后向预测，得到最佳后向ref与mv。

- 
对比三组ref与mv（前向、后向、前向+后向），从中选择最佳的ref与mv。




如果最终得到的是前向+后向的方式，那这就是典型的B帧预测方式。不过这种方式会存在以下的问题：
- 
refList0,refList1的最佳ref与mv是分开进行预测的，即在计算cost时，预测块取值采用的是单独的前向预测块与单独的后向预测块。

- 
对比三组ref与mv时，预测块取值为前向预测块与后向预测块之和的一半。

- 
在进行残差编码时，预测块取值为前向预测块与后向预测块之和的一半。




![](https://images0.cnblogs.com/blog/421096/201505/172057440732449.jpg)



可以看到在运动预测时，预测块的取值方式与最后的残差编码的取值方式不同，那么这种运动预测所得到的mv能否在计算残差编码时使得残差最小？答案不能肯定，因为运动预测时的预测块值与残差编码的预测块值不是一样的。不过对于这种B帧预测的情况，肯定有更好的运动预测方式：令运动预测时的预测块的预测方式与残差编码的预测块的预测方式一致。



# Bi-predictive motion search

双向预测运动搜索（Bi-predictive motion search）相对于上述传统类型的运动预测方式，主要的改进是在计算cost时，预测块的取值为前向预测块与后向预测块的一半。



![](https://images0.cnblogs.com/blog/421096/201505/172057455735707.jpg)

这种运动估计方式，既保持了运动估计与残差编码的一致性，又充分考虑到了前后参考图像的耦合性。

在传统的运动估计中，由于前、后向估计是分开的，但是在残差编码时需要对前、后预测块进行组合，因此如果前、后参考帧关联性很强的话，这种单独的预测方式所得到的组合预测块（combination predicted block）并可能非最佳的预测块，如下图：



![](https://images0.cnblogs.com/blog/421096/201505/172057466832380.png)![](https://images0.cnblogs.com/blog/421096/201505/172057473086737.png)

（注：左：前后预测块的像素直方图；右：当前块的像素直方图）



采用双向预测运动搜索后，即使原本在单向预测时被判定为不是最佳的运动向量，如果前、后参考帧的关联性很强的话，也可能由于前后向结合搜索从而得到与当前块差距最小的组合预测块（combination predicted block）

![](https://images0.cnblogs.com/blog/421096/201505/172057481674165.png)![](https://images0.cnblogs.com/blog/421096/201505/172057493708866.png)

（注：左：前后预测块的像素直方图；右：当前块的像素直方图）





JM18.6实现

首先，是在B slice下才会进行双向预测运动搜索。

既然需要对两个参考图像同时进行预测，那么这两个参考图像应该如何选择呢？在JM18.6中，只对两个参考图像列表的ref0执行这种双向预测运动搜索，并且在进行搜索时，两个参考图像都是ref0，即ref0List[0],ref1List[0]。



/*!
 ***********************************************************************
 * \brief
 *    Block motion search
 ***********************************************************************
 */
distblk                                         //!< minimum motion cost after search
BlockMotionSearch (Macroblock *currMB,      //!< Current Macroblock
                   MEBlock   *mv_block,     //!< Motion estimation information block
                   int       mb_x,          //!< x-coordinate inside macroblock
                   int       mb_y,          //!< y-coordinate inside macroblock
                   int*      lambda_factor) //!< lagrangian parameter for determining motion cost
{
  ...

  // Bipred ME consideration: returns minimum bipred cost
  if (is_bipred_enabled(p_Vid, blocktype) && (ref == 0)) 
  {
    BiPredBlockMotionSearch(currMB, mv_block, &pred, mb_x, mb_y, lambda_factor);
  }

  return min_mcost;
}



/*!
************************************************************************
* \brief
*    UMHEXBipredIntegerPelBlockMotionSearch: fast pixel block motion search for bipred mode
*    this algrithm is called UMHexagonS(see JVT-D016),which includes
*    four steps with different kinds of search patterns
* \author
*   Main contributors: (see contributors.h for copyright, address and affiliation details)
*   - Zhibo Chen         <chenzhibo@tsinghua.org.cn>
*   - JianFeng Xu        <fenax@video.mdc.tsinghua.edu.cn>
*   - Xiaozhong Xu       <xxz@video.mdc.tsinghua.edu.cn>
* \date   :
*   2006.1
************************************************************************
*/
distblk                                                //  ==> minimum motion cost after search
UMHEXBipredIntegerPelBlockMotionSearch (Macroblock *currMB,      // <--  current Macroblock
                                        int       list,          // <--  current reference list
                                        MotionVector *pred_mv1,  // <--  motion vector predictor (x|y) in sub-pel units
                                        MotionVector *pred_mv2,  // <--  motion vector predictor (x|y) in sub-pel units
                                        MotionVector  *mv1,      // <--> in: search center (x|y) / out: motion vector (x|y) - in sub-pel units
                                        MotionVector *mv2,       // <--> in: search center (x|y) 
                                        MEBlock *mv_block,       // <--  motion vector information
                                        int       search_range,  // <--  1-d search range in sub-pel units
                                        distblk       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
                                        int       lambda_factor  // <--  lagrangian parameter for determining motion cost
                                        )
{
  ...
  //maybe wrong,should swap ref1 and ref2 ,not listX[list][ref] and listX[list][0] 
  //ok, only ref == 0 comes into this function
  StorablePicture *ref_picture1 = currSlice->listX[list + currMB->list_offset][ref];
  StorablePicture *ref_picture2 = currSlice->listX[list == 0 ? 1 + currMB->list_offset: currMB->list_offset][ 0 ];

  ...
}





由于有两个参考图像，但是在进行一次运动搜索时，不可能对两个参考图像同时进行搜索，因此轮询对两张参考图像进行搜索。流程如下：
- 
假设当前参考图像为ref0List[0]，那么令ref0List[0]（前向）由单向运动搜索已经得到的mv作为该参考图像的候选mv，并且在此次搜索过程中保持不变。

- 
Ref1List[0]为本次搜索过程的搜索对象。先求出后向mvp，用于运动搜索。

- 
采用某种算法（如：UMH）进行运动搜索，与普通的运动搜索的不同只有在计算cost时同时用了ref0List[0]的候选mv与ref1List[0]的候选mv。

- 
得到此次运动搜索的ref1List[0]的最佳mv后，保存起来，交换ref0List[0]与ref1List[1]，刚刚得到的最佳mv为下一次运动搜索的固定mv。

- 
3与4轮询几次后即可得到最终结果。




![](https://images0.cnblogs.com/blog2015/421096/201505/172121228874003.jpg)





/*!
 ***********************************************************************
 * \brief
 *    Bi-predictive motion search
 ***********************************************************************
 */
static distblk BiPredBlockMotionSearch(Macroblock   *currMB,      //!< Current Macroblock
                                       MEBlock      *mv_block,
                                       MotionVector *pred_mv,     //!< current list motion vector predictor
                                       int           mb_x,            //!< x-coordinate inside macroblock
                                       int           mb_y,            //!< y-coordinate inside macroblock
                                       int*          lambda_factor)   //!< lagrangian parameter for determining motion cost
{
  VideoParameters *p_Vid     = currMB->p_Vid;
  InputParameters *p_Inp     = currMB->p_Inp;
  Slice           *currSlice = currMB->p_Slice;
  int         list = mv_block->list;
  int         i, j;
  short       bipred_type = list ? 0 : 1;
  MotionVector ***** bipred_mv = currSlice->bipred_mv[bipred_type];
  distblk     min_mcostbi = DISTBLK_MAX;
  MotionVector *mv = &mv_block->mv[list];         //1.用从单向运动搜索得到的mv作为候选mv
  MotionVector bimv, tempmv;
  MotionVector pred_mv1, pred_mv2, pred_bi;
  MotionVector *bi_mv1 = NULL, *bi_mv2 = NULL;
  short       iterlist = (short) list;  
  int         block_x   = (mb_x>>2);
  int         block_y   = (mb_y>>2);
  int         blocktype = mv_block->blocktype;
  int         bsx       = mv_block->blocksize_x;
  int         bsy       = mv_block->blocksize_y;
  //PixelPos    block[4];  // neighbor blocks
  
  //get_neighbors(currMB, mv_block->block, mb_x, mb_y, bsx);

  if (p_Inp->SearchMode[p_Vid->view_id] == UM_HEX)
  {
    p_Vid->p_UMHex->bipred_flag = 1;
    //2.求出后（前）向mvp
    UMHEXSetMotionVectorPredictor(currMB, &pred_bi, p_Vid->enc_picture->mv_info, 0, list ^ 1, mb_x, mb_y, bsx, bsy, mv_block);
  }
  else
    currMB->GetMVPredictor (currMB, mv_block->block, &pred_bi, 0, p_Vid->enc_picture->mv_info, list ^ 1, mb_x, mb_y, bsx, bsy);

  if ((p_Inp->SearchMode[p_Vid->view_id] != EPZS) || (p_Inp->EPZSSubPelGrid == 0))
  {
    mv->mv_x = ((mv->mv_x  + 2) >> 2) * 4;
    mv->mv_y = ((mv->mv_y  + 2) >> 2) * 4;
    bimv.mv_x = ((pred_bi.mv_x  + 2) >> 2) * 4;
    bimv.mv_y = ((pred_bi.mv_y  + 2) >> 2) * 4;
  }
  else
  {
    bimv = pred_bi;
  }

  //Bi-predictive motion Refinements
  for (mv_block->iteration_no = 0; mv_block->iteration_no <= p_Inp->BiPredMERefinements; mv_block->iteration_no++)
  {
    if (mv_block->iteration_no & 0x01)           //4.参考图像交换
    {
      pred_mv1  = *pred_mv;
      pred_mv2  = pred_bi;
      bi_mv1    = mv;
      bi_mv2    = &bimv;
      iterlist  = (short) list;
    }
    else
    {
      pred_mv1  = pred_bi;
      pred_mv2  = *pred_mv;
      bi_mv1    = &bimv;
      bi_mv2    = mv;
      iterlist = (short) (list ^ 1);
    }

    tempmv = *bi_mv1;

    PrepareBiPredMEParams(currSlice, mv_block, mv_block->ChromaMEEnable, iterlist, currMB->list_offset, mv_block->ref_idx);
    // Get bipred mvs for list iterlist given previously computed mvs from other list
    min_mcostbi = currMB->BiPredME (currMB, iterlist, 
      &pred_mv1, &pred_mv2, bi_mv1, bi_mv2, mv_block, 
      (p_Inp->BiPredMESearchRange[p_Vid->view_id] <<2)>>mv_block->iteration_no, min_mcostbi, lambda_factor[F_PEL]);

    if (mv_block->iteration_no > 0 && (tempmv.mv_x == bi_mv1->mv_x) && (tempmv.mv_y == bi_mv1->mv_y))
    {
      break;
    }
  }

  if (!p_Inp->DisableSubpelME[p_Vid->view_id])
  {
    if (p_Inp->BiPredMESubPel)
    {
      if ( !p_Vid->start_me_refinement_hp )
        min_mcostbi = DISTBLK_MAX;
      PrepareBiPredMEParams(currSlice, mv_block, mv_block->ChromaMEEnable, iterlist, currMB->list_offset, mv_block->ref_idx);

      min_mcostbi =  currMB->SubPelBiPredME (currMB, mv_block, iterlist, &pred_mv1, &pred_mv2, bi_mv1, bi_mv2, min_mcostbi, lambda_factor);
    }

    if (p_Inp->BiPredMESubPel==2)
    {
      if ( !p_Vid->start_me_refinement_qp )
        min_mcostbi = DISTBLK_MAX;
      PrepareBiPredMEParams(currSlice, mv_block, mv_block->ChromaMEEnable, iterlist ^ 1, currMB->list_offset, mv_block->ref_idx);

      min_mcostbi =  currMB->SubPelBiPredME (currMB, mv_block, iterlist ^ 1, &pred_mv2, &pred_mv1, bi_mv2, bi_mv1, min_mcostbi, lambda_factor);
    }
  }

  clip_mv_range(p_Vid, 0, bi_mv1, Q_PEL);
  clip_mv_range(p_Vid, 0, bi_mv2, Q_PEL);

  for (j=block_y; j < block_y + (bsy>>2); j++)
  {
    for (i=block_x ; i < block_x + (bsx>>2); i++)
    {
      bipred_mv[iterlist    ][(short) mv_block->ref_idx][blocktype][j][i] = *bi_mv1;
      bipred_mv[iterlist ^ 1][(short) mv_block->ref_idx][blocktype][j][i] = *bi_mv2;
    }
  }


  return min_mcostbi;
}



#define SEARCH_ONE_PIXEL_BIPRED                                                                         \
if((iabs(cand.mv_x - center2.mv_x) >> 2) < search_range && (iabs(cand.mv_y - center2.mv_y) >> 2) < search_range)     \
{                                                                                                       \
  if(!p_UMHex->McostState[((cand.mv_y - center2.mv_y) >> 2) + search_range][((cand.mv_x-center2.mv_x) >> 2)+search_range])         \
  {                                                                                                     \
    mcost  = mv_cost (p_Vid, lambda_factor, ¢er1, &pred1);                                          \
    mcost += mv_cost (p_Vid, lambda_factor, &cand, &pred2);                                             \
    if(mcost<min_mcost)                                                                                   \
    {                                                                                                     \
      mcost  += mv_block->computeBiPredFPel(ref_picture1, ref_picture2,                                 \ //3.组合两个ref来进行cost计算
      mv_block, min_mcost - mcost, ¢er1, &cand);                                                \
      p_UMHex->McostState[((cand.mv_y - center2.mv_y) >> 2) + search_range][((cand.mv_x - center2.mv_x) >> 2) + search_range] = 1; \
      if (mcost < min_mcost)                                                                            \
      {                                                                                                 \
        best = cand;                                                                                    \
        min_mcost = mcost;                                                                              \
      }                                                                                                 \
    }                                                                                                   \
  }                                                                                                     \
}  





在JM18.6中，采用这种运动搜索方式需要满足以下几种条件：1. Bslice, 2. 输入参数指定了BiPredMotionEstimation, 3. 分割模式被指定了bipred_enabled, 4. Ref = 0。













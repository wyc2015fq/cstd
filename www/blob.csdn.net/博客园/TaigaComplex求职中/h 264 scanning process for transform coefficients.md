# h.264 scanning process for transform coefficients - TaigaComplex求职中 - 博客园







# [h.264 scanning process for transform coefficients](https://www.cnblogs.com/TaigaCon/p/5245108.html)





宏块在经过变换、量化后，得到大小为4x4或者8x8的矩阵，矩阵中的数据被称为transform coefficient levels。这些level在后面会被用于熵编码，因此我们需要把矩阵按照一定顺序进行扫描，得到数字序列。



扫描顺序在帧与场会有所不同

4x4块矩阵的扫描顺序如下

![image](https://images2015.cnblogs.com/blog/421096/201603/421096-20160305163709612-1514347895.png)![image](https://images2015.cnblogs.com/blog/421096/201603/421096-20160305163711580-1613413051.png)

**Zig-zag scan(Frame)                          Field scan**



8x8块矩阵的扫描顺序如下

![image](https://images2015.cnblogs.com/blog/421096/201603/421096-20160305163713221-2025134918.png)![image](https://images2015.cnblogs.com/blog/421096/201603/421096-20160305163717799-96063283.png)



**Zig-zag scan(Frame)                                  Field scan            **



在实际的代码处理上（JM），对变换系数的扫描过程是包含在量化过程中的，因为对矩阵内的元素的量化也是逐个进行的，因此就可以按照对变换系数扫描的顺序取出矩阵内的元素进行量化，后续就能直接对这些transform coefficient level序列进行熵编码。
int quant_4x4_normal(Macroblock *currMB, int **tblock, struct quant_methods *q_method)
{ 
  VideoParameters *p_Vid = currMB->p_Vid;
  QuantParameters *p_Quant = p_Vid->p_Quant;
  Slice *currSlice = currMB->p_Slice;
  Boolean is_cavlc = (Boolean) (currSlice->symbol_mode == CAVLC);
  
  int   block_x = q_method->block_x;
  int  qp = q_method->qp;
  int*  ACL = &q_method->ACLevel[0];
  int*  ACR = &q_method->ACRun[0];  
  LevelQuantParams **q_params_4x4 = q_method->q_params;
  const byte (*pos_scan)[2] = q_method->pos_scan;
  const byte *c_cost = q_method->c_cost;
  int *coeff_cost = q_method->coeff_cost;
  
  
  LevelQuantParams *q_params = NULL;
  int i,j, coeff_ctr;
  
  int *m7;
  int scaled_coeff;
  
  int   level, run = 0;
  int   nonzero = FALSE;
  int   qp_per = p_Quant->qp_per_matrix[qp];
  int   q_bits = Q_BITS + qp_per;
  const byte *p_scan = &pos_scan[0][0];

  // Quantization
  // 4x4 block matrix has 16 coefficients
  for (coeff_ctr = 0; coeff_ctr < 16; ++coeff_ctr)
  {
    //scanning positions (Zig-zag scan or Field scan)
    i = *p_scan++;  // horizontal position
    j = *p_scan++;  // vertical position
    
    //block_x,block_y here is the position of a block on a Macroblock with the unit of pixel
    m7 = &tblock[j][block_x + i];

    if (*m7 != 0)
    {
      q_params = &q_params_4x4[j][i];
      scaled_coeff = iabs (*m7) * q_params->ScaleComp;
      level = (scaled_coeff + q_params->OffsetComp) >> q_bits;

      if (level != 0)
      {
        if (is_cavlc)
          level = imin(level, CAVLC_LEVEL_LIMIT);

        *coeff_cost += (level > 1) ? MAX_VALUE : c_cost[run];

        level  = isignab(level, *m7);
        *m7     = rshift_rnd_sf(((level * q_params->InvScaleComp) << qp_per), 4);
        // inverse scale can be alternative performed as follows to ensure 16bit
        // arithmetic is satisfied.
        // *m7 = (qp_per<4) ? rshift_rnd_sf((level*q_params->InvScaleComp),4-qp_per) : (level*q_params->InvScaleComp)<<(qp_per-4);
        *ACL++ = level;
        *ACR++ = run;
        // reset zero level counter
        run    = 0;
        nonzero = TRUE;
      }
      else
      {
        *m7 = 0;
        ++run;
      }
    }
    else
    {
      ++run;
    }
  }

  *ACL = 0;

  return nonzero;
}













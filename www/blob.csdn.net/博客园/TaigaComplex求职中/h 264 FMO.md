# h.264 FMO - TaigaComplex求职中 - 博客园







# [h.264 FMO](https://www.cnblogs.com/TaigaCon/p/3605762.html)





在H.264之前的标准中，比如H.263，其比特流中的数据是按照一个宏块接一个宏块的方式排列的，一旦发生丢包，很多相邻宏块信息都会丢失，很难进行错误隐藏处理。在H.264中加入了一项新特性：把宏块在比特流中的数据按照一定的映射规则进行排列，而不一定按照原本的光栅扫描顺序排列，这种方称为灵活的宏块重拍FMO(Flexible Macroblock Ordering)。

FMO是基于组的方式将宏块集合起来，把一帧内的宏块划分到不同的条带组（slice group）。当然在最后也需要对条带组按光栅扫描顺序划分条带（slice）。在h.264中，当slice group被设为大于1时才使用FMO。在代码端看来，FMO时，主要是对映射表（数组）的设置，如下第一个表：



对一帧进行FMO：
|0|1|2|0|1|2|0|1|2|0|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|



得到slice group有三个：
|0|0|0|0|0|0|0|0|0|
|----|----|----|----|----|----|----|----|----|
|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|||||
|1|1|1|1|1|1|1|1|1|
|----|----|----|----|----|----|----|----|----|
|1|1|1|1|1|1|1|1|1|
|1|1|1|1|1|1|1|1|1|
|1|1|1|1|1|1|1|1|1|
|2|2|2|2|2|2|2|2|2|
|----|----|----|----|----|----|----|----|----|
|2|2|2|2|2|2|2|2|2|
|2|2|2|2|2|2|2|2|2|
|2|2|2|2||||||

然后再对每个slice group按照光栅扫描顺序进行排序，分割出slice，下面为分割slice group 0（假设slice长度为16）

slice 1 in slice group 0: 
|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|

slice 2 in slice group 0:
|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|





FMO在h.264中有7种（FMO0 ~ FMO6）

#### FMO0

FMO0，交织映射。交织映射是指不同的slice group交替出现。

如下设置了slice group 0 = 10，slice group 1 = 15，slice group 2 = 18
|0|0|0|0|0|0|0|0|0|0|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|1|1|1|1|1|1|1|1|1|1|
|1|1|1|2|2|2|2|2|2|2|2|
|2|2|2|2|2|2|2|2|2|2|0|
|0|0|0|0|0|0|0|0|0|1|1|
|1|1|1|1|1|1|1|1|1|1|1|
|1|1|2|2|2|2|2|2|2|2|2|
|2|2|2|2|2|2|2|2|2|0|0|
|0|0|0|0|0|0|0|0|1|1|1|

JM code如下：
/*!
 ************************************************************************
 * \brief
 *    Generate interleaved slice group map type MapUnit map (type 0)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType0MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  unsigned iGroup, j;
  unsigned i = 0;
  do
  {
    for( iGroup = 0; 
    (iGroup <= pps->num_slice_groups_minus1) && (i < PicSizeInMapUnits); 
    i += pps->run_length_minus1[iGroup++] + 1)
    {
      for( j = 0; j <= pps->run_length_minus1[ iGroup ] && i + j < PicSizeInMapUnits; j++ )
        MapUnitToSliceGroupMap[i+j] = iGroup;
    }
  }
  while( i < PicSizeInMapUnits );
}




#### FMO1

FMO1，分散映射。每个宏块相邻的宏块都不在同一组。x方向上的宏块按照slice group序号递增分配，y方向第一个宏块按照0，slice group numbers/2交替分配。
|0|1|2|0|1|2|0|1|2|0|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|
|1|2|0|1|2|0|1|2|0|1|2|
|0|1|2|0|1|2|0|1|2|0|1|

 JM code 如下：
 
/*!
 ************************************************************************
 * \brief
 *    Generate dispersed slice group map type MapUnit map (type 1)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType1MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  unsigned i;
  for( i = 0; i < PicSizeInMapUnits; i++ )
  {
    MapUnitToSliceGroupMap[i] = ((i%img->PicWidthInMbs)+(((i/img->PicWidthInMbs)*(pps->num_slice_groups_minus1+1))/2))
      %(pps->num_slice_groups_minus1+1);
  }
}




#### FMO2

FMO2，前后景映射。以整个帧作为背景，该模式可以通过（左上角坐标，右下角坐标）的方式指定前景，每个前景都为一个slice group，最先指定的前景可以覆盖后来指定的前景。

假设有

slice group 0（3,4）（8,8）

slice group 1（1,2）（6,6）
|2|2|2|2|2|2|2|2|2|2|2|
|----|----|----|----|----|----|----|----|----|----|----|
|2|2|2|2|2|2|2|2|2|2|2|
|2|1|1|1|1|1|1|2|2|2|2|
|2|1|1|1|1|1|1|2|2|2|2|
|2|1|1|0|0|0|0|0|0|2|2|
|2|1|1|0|0|0|0|0|0|2|2|
|2|1|1|0|0|0|0|0|0|2|2|
|2|2|2|0|0|0|0|0|0|2|2|
|2|2|2|0|0|0|0|0|0|2|2|

 JM code如下：
/*!
 ************************************************************************
 * \brief
 *    Generate foreground with left-over slice group map type MapUnit map (type 2)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType2MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  int iGroup;
  unsigned i, x, y;
  unsigned yTopLeft, xTopLeft, yBottomRight, xBottomRight;
  
  for( i = 0; i < PicSizeInMapUnits; i++ )
    MapUnitToSliceGroupMap[ i ] = pps->num_slice_groups_minus1;
  
  for( iGroup = pps->num_slice_groups_minus1 - 1 ; iGroup >= 0; iGroup-- ) 
  {
    yTopLeft = pps->top_left[ iGroup ] / img->PicWidthInMbs;
    xTopLeft = pps->top_left[ iGroup ] % img->PicWidthInMbs;
    yBottomRight = pps->bottom_right[ iGroup ] / img->PicWidthInMbs;
    xBottomRight = pps->bottom_right[ iGroup ] % img->PicWidthInMbs;
    for( y = yTopLeft; y <= yBottomRight; y++ )
      for( x = xTopLeft; x <= xBottomRight; x++ )
        MapUnitToSliceGroupMap[ y * img->PicWidthInMbs + x ] = iGroup;
  }
}




#### FMO3

FMO3，环形扫描映射。在JM中，环形扫描映射把一帧分为两个slice group，一个是以一帧为背景，另一个是以环作为的前景（其实环可以扩展到更多的group）。环的起点是帧的中心位置，通过指定环的运动方向（顺时针或逆时针）以及环的长度即可得到前景。

以下为顺时针，环长度为32
|1|1|1|1|1|1|1|1|1|1|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|1|1|1|1|1|1|1|1|1|1|
|1|1|1|20|21|22|23|24|25|1|1|
|1|1|1|19|6|7|8|9|26|1|1|
|1|1|1|18|5|0|1|10|27|1|1|
|1|1|1|17|4|3|2|11|28|1|1|
|1|1|1|16|15|14|13|12|29|1|1|
|1|1|1|1|1|1|1|31|30|1|1|
|1|1|1|1|1|1|1|1|1|1|1|



 JM code 如下：
/*!
 ************************************************************************
 * \brief
 *    Generate box-out slice group map type MapUnit map (type 3)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType3MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  unsigned i, k;
  int leftBound, topBound, rightBound, bottomBound;
  int x, y, xDir, yDir;
  int mapUnitVacant;
  
  unsigned mapUnitsInSliceGroup0 = min((pps->slice_group_change_rate_minus1 + 1) * img->slice_group_change_cycle, PicSizeInMapUnits);
  
  for( i = 0; i < PicSizeInMapUnits; i++ )
    MapUnitToSliceGroupMap[ i ] = 2;
  
  x = ( img->PicWidthInMbs - pps->slice_group_change_direction_flag ) / 2;
  y = ( img->PicHeightInMapUnits - pps->slice_group_change_direction_flag ) / 2;
  
  leftBound   = x;
  topBound    = y;
  rightBound  = x;
  bottomBound = y;
  
  xDir =  pps->slice_group_change_direction_flag - 1;
  yDir =  pps->slice_group_change_direction_flag;
  
  for( k = 0; k < PicSizeInMapUnits; k += mapUnitVacant ) 
  {
    mapUnitVacant = ( MapUnitToSliceGroupMap[ y * img->PicWidthInMbs + x ]  ==  2 );
    if( mapUnitVacant )
      MapUnitToSliceGroupMap[ y * img->PicWidthInMbs + x ] = ( k >= mapUnitsInSliceGroup0 );
    
    if( xDir  ==  -1  &&  x  ==  leftBound ) 
    {
      leftBound = max( leftBound - 1, 0 );
      x = leftBound;
      xDir = 0;
      yDir = 2 * pps->slice_group_change_direction_flag - 1;
    }
    else 
      if( xDir  ==  1  &&  x  ==  rightBound ) 
      {
        rightBound = min( rightBound + 1, (int)img->PicWidthInMbs - 1 );
        x = rightBound;
        xDir = 0;
        yDir = 1 - 2 * pps->slice_group_change_direction_flag;
      }
      else
        if( yDir  ==  -1  &&  y  ==  topBound ) 
        {
          topBound = max( topBound - 1, 0 );
          y = topBound;
          xDir = 1 - 2 * pps->slice_group_change_direction_flag;
          yDir = 0;
        }
        else 
          if( yDir  ==  1  &&  y  ==  bottomBound ) 
          {
            bottomBound = min( bottomBound + 1, (int)img->PicHeightInMapUnits - 1 );
            y = bottomBound;
            xDir = 2 * pps->slice_group_change_direction_flag - 1;
            yDir = 0;
          }
          else
          {
            x = x + xDir;
            y = y + yDir;
          }
  }
  
}




#### FMO4

FMO4，光栅扫描映射。该模式只支持两个slice group，按照光栅扫描顺序来分组，方向有正向与反向之分。

如下为反向FMO4(始于右下角):
|1|1|1|1|1|1|1|1|1|1|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|1|1|1|1|1|1|1|1|1|1|
|1|1|1|1|1|1|1|1|1|1|1|
|1|1|1|1|1|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|0|0|
|0|0|0|0|0|0|0|0|0|0|0|

 JM code 如下：
/*!
 ************************************************************************
 * \brief
 *    Generate raster scan slice group map type MapUnit map (type 4)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType4MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  
  unsigned mapUnitsInSliceGroup0 = min((pps->slice_group_change_rate_minus1 + 1) * img->slice_group_change_cycle, PicSizeInMapUnits);
  unsigned sizeOfUpperLeftGroup = pps->slice_group_change_direction_flag ? ( PicSizeInMapUnits - mapUnitsInSliceGroup0 ) : mapUnitsInSliceGroup0;
  
  unsigned i;
  
  for( i = 0; i < PicSizeInMapUnits; i++ )
    if( i < sizeOfUpperLeftGroup )
      MapUnitToSliceGroupMap[ i ] = pps->slice_group_change_direction_flag;
    else
      MapUnitToSliceGroupMap[ i ] = 1 - pps->slice_group_change_direction_flag;
    
}




#### FMO5

FMO5，擦式扫描映射。仅支持两个slice group，扫描方式为纵向，也有正反两个方向

如下为正向FMO5（始于左上角）：
|0|0|0|1|1|1|1|1|1|1|1|
|----|----|----|----|----|----|----|----|----|----|----|
|0|0|0|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|
|0|0|1|1|1|1|1|1|1|1|1|

 JM code 如下：
/*!
 ************************************************************************
 * \brief
 *    Generate wipe slice group map type MapUnit map (type 5)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
*/
static void FmoGenerateType5MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  
  unsigned mapUnitsInSliceGroup0 = min((pps->slice_group_change_rate_minus1 + 1) * img->slice_group_change_cycle, PicSizeInMapUnits);
  unsigned sizeOfUpperLeftGroup = pps->slice_group_change_direction_flag ? ( PicSizeInMapUnits - mapUnitsInSliceGroup0 ) : mapUnitsInSliceGroup0;
  
  unsigned i,j, k = 0;
  
  for( j = 0; j < img->PicWidthInMbs; j++ )
    for( i = 0; i < img->PicHeightInMapUnits; i++ )
      if( k++ < sizeOfUpperLeftGroup )
        MapUnitToSliceGroupMap[ i * img->PicWidthInMbs + j ] = 1 - pps->slice_group_change_direction_flag;
      else
        MapUnitToSliceGroupMap[ i * img->PicWidthInMbs + j ] = pps->slice_group_change_direction_flag;
      
}




#### FMO6

FMO6，显示控制映射。可以在配置文件中自由地指定每个宏块所属的slice group。
|0|1|0|2|1|1|0|2|1|1|1|
|----|----|----|----|----|----|----|----|----|----|----|
|1|2|0|0|0|1|0|2|0|1|0|
|3|5|4|1|0|0|3|5|4|0|1|
|0|0|5|5|3|1|0|2|5|0|1|
|4|1|0|0|1|1|1|1|4|4|2|
|0|2|2|3|2|3|2|1|5|2|3|
|0|1|2|0|1|1|2|0|5|5|5|
|3|2|1|4|1|4|4|0|3|3|2|
|4|2|3|5|0|0|1|4|1|2|3|

顺带一提JM支持一帧中最多为8个slice group

JM code 如下：
/*!
 ************************************************************************
 * \brief
 *    Generate explicit slice group map type MapUnit map (type 6)
 *
 * \param img
 *    Image Parameter to be used for map generation
 * \param pps
 *    Picture Parameter set to be used for map generation
 ************************************************************************
 */
static void FmoGenerateType6MapUnitMap (ImageParameters * img, pic_parameter_set_rbsp_t * pps )
{
  unsigned i;
  for (i=0; i<PicSizeInMapUnits; i++)
  {
    MapUnitToSliceGroupMap[i] = pps->slice_group_id[i];
  }
}
















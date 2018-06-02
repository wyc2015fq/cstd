#include "Utility.h"
/// <summary>
/// 将数据截断在Byte数据类型内，开发时间：2014.12.8
/// </summary>
/// <param name="Value">需要裁减的原始数据。</param>
/// <remarks> 参考：http://www.cnblogs.com/zyl910/archive/2012/03/12/noifopex1.html。</remarks>
/// <remarks> 用位掩码做饱和处理.用带符号右移生成掩码，不要写成宏定义，会因为参数类型而出现问题，且这样写系统会自动内联，效率上也比宏定义高一些。</remarks>
unsigned char ClampToByte(int Value)
{
  return ((Value | ((signed int)(255 - Value) >> 31)) & ~((signed int)Value >> 31));
}
/// <summary>
/// 返回0到1之间的随机数，开发时间：2014.12.8
/// </summary>
double Rand()
{
  return (double)rand() / (RAND_MAX + 1.0);
}
/// <summary>
/// 按照指定的边缘模式计算扩展后各坐标的有理值
/// </summary>
/// <param name="Width">矩阵的宽度。</param>
/// <param name="Height">矩阵的高度。</param>
/// <param name="Left">左侧需要扩展的坐标。</param>
/// <param name="Right">右侧需要扩展的坐标。</param>
/// <param name="Top">顶部需要扩展的坐标。</param>
/// <param name="Bottom">底部需要扩展的坐标。</param>
/// <param name="Edge">处理边缘的方式。</param>
/// <param name="RowPos">保存行方向的有理坐标值。</param>
/// <param name="ColPos">保存列方向的有理坐标值。</param>
/// <returns>返回函数是否执行成功。</returns>
IS_RET GetValidCoordinate(int Width, int Height, int Left, int Right, int Top, int Bottom, EdgeMode Edge, TMatrix** Row, TMatrix** Col)
{
  if ((Left < 0) || (Right < 0) || (Top < 0) || (Bottom < 0)) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  IS_RET Ret = IS_CreateMatrix(Width + Left + Right, 1, IS_DEPTH_32S, 1, Row);
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  Ret = IS_CreateMatrix(1, Height + Top + Bottom, IS_DEPTH_32S, 1, Col);
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  int X, Y, XX, YY, *RowPos = (int*)(*Row)->Data, *ColPos = (int*)(*Col)->Data;
  for (X = -Left; X < Width + Right; X++) {
    if (X < 0) {
      if (Edge == EdgeMode_Tile) {                                                   //重复边缘像素
        RowPos[X + Left] = 0;
      }
      else {
        XX = -X;
        while (XX >= Width) {
          XX -= Width;  // 做镜像数据
        }
        RowPos[X + Left] = XX;
      }
    }
    else if (X >= Width) {
      if (Edge == EdgeMode_Tile) {
        RowPos[X + Left] = Width - 1;
      }
      else {
        XX = Width - (X - Width + 2);
        while (XX < 0) {
          XX += Width;
        }
        RowPos[X + Left] = XX;
      }
    }
    else {
      RowPos[X + Left] = X;
    }
  }
  for (Y = -Top; Y < Height + Bottom; Y++) {
    if (Y < 0) {
      if (Edge == EdgeMode_Tile) {
        ColPos[Y + Top] = 0 ;
      }
      else {
        YY = -Y;
        while (YY >= Height) {
          YY -= Height;
        }
        ColPos[Y + Top] = YY;
      }
    }
    else if (Y >= Height) {
      if (Edge == EdgeMode_Tile) {
        ColPos[Y + Top] = Height - 1;
      }
      else {
        YY = Height - (Y - Height + 2);
        while (YY < 0) {
          YY += Height;
        }
        ColPos[Y + Top] = YY;
      }
    }
    else {
      ColPos[Y + Top] = Y;
    }
  }
  return IS_RET_OK;
}
IS_RET __stdcall GetExpandImage(TMatrix* Src, TMatrix** Dest, int Left, int Right, int Top, int Bottom, EdgeMode Edge)
{
  if (Src == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Depth != IS_DEPTH_8U || Left < 0 || Right < 0 || Top < 0 || Bottom < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int X, Y, SrcWidth, SrcHeight, SrcStride, DstWidth, DstHeight, DstStride, Channel;
  unsigned char* LinePS, *LinePD;
  SrcWidth = Src->Width;
  SrcHeight = Src->Height;
  DstWidth = SrcWidth + Left + Right;
  DstHeight = SrcHeight + Top + Bottom;
  if (*Dest != NULL) {
    if ((*Dest)->Data == NULL) {
      return IS_RET_ERR_NULLREFERENCE;
    }
    if ((*Dest)->Depth != IS_DEPTH_8U || (*Dest)->Width != DstWidth || (*Dest)->Height != DstHeight) {
      return IS_RET_ERR_ARGUMENTOUTOFRANGE;
    }
  }
  else {
    IS_RET Ret = IS_CreateMatrix(DstWidth, DstHeight, Src->Depth, Src->Channel, Dest);
    if (Ret != IS_RET_OK) {
      return Ret;
    }
  }
  SrcStride = Src->WidthStep;
  DstStride = (*Dest)->WidthStep;
  Channel = Src->Channel;
  TMatrix* Row, *Col;
  GetValidCoordinate(SrcWidth, SrcHeight, Left, Right, Top, Bottom, Edge, &Row, &Col);            //      获取坐标偏移量
  int* RowPos = (int*)Row->Data, *ColPos = (int*)Col->Data;
  for (Y = 0; Y < SrcHeight; Y++) {
    LinePD = (*Dest)->Data + (Y + Top) * DstStride;
    LinePS = Src->Data + Y * SrcStride;
    for (X = 0; X < Left; X++) {
      memcpy(LinePD, LinePS + RowPos[X] * Channel, Channel);                                                          //      左侧的像素
      LinePD += Channel;                                                                                                                                      //      移动像素
    }
    memcpy(LinePD, LinePS, SrcWidth * Channel);                                                                                             //      中间的像素
    LinePD += SrcWidth * Channel;                                                                                                                   //      移动像素
    for (X = Left + SrcWidth; X < Left + SrcWidth + Right; X++) {
      memcpy(LinePD, LinePS + RowPos[X] * Channel, Channel);                                                          //      右侧的像素
      LinePD += Channel;
    }
  }
  for (Y = 0; Y < Top; Y++) {
    memcpy((*Dest)->Data + Y * DstStride, (*Dest)->Data + (Top + ColPos[Y]) * DstStride, DstStride);                        //      行行直接拷贝
  }
  for (Y = Top + SrcHeight; Y < Top + SrcHeight + Bottom; Y++) {
    memcpy((*Dest)->Data + Y * DstStride, (*Dest)->Data + (Top + ColPos[Y]) * DstStride, DstStride);
  }
  IS_FreeMatrix(&Row);
  IS_FreeMatrix(&Col);
  return IS_RET_OK;
}


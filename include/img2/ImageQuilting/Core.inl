#include "Core.h"



void* IS_AllocMemory(unsigned int Size, bool ZeroMemory)
{
  void* Ptr = malloc(Size);               //      如果是手机或者其他情况，请将此函数换为malloc之类的，在PC上用它主要是实现内存分配的字节对齐。
  if (Ptr != NULL)
    if (ZeroMemory == true) {
      memset(Ptr, 0, Size);
    }
  return Ptr;
}
void IS_FreeMemory(void* Ptr)
{
  if (Ptr != NULL) {
    free(Ptr);
  }
}
/// <summary>
/// 根据矩阵元素的类型来获取一个元素实际占用的字节数
/// </summary>
/// <param name="Depth">根据矩阵元素的类型。</param>
/// <remarks> 只作为内部使用。</remarks>
int IS_ELEMENT_SIZE(int Depth)
{
  int Size;
  switch (Depth) {
  case IS_DEPTH_8U:
    Size = sizeof(unsigned char);
    break;
  case IS_DEPTH_8S:
    Size = sizeof(char);
    break;
  case IS_DEPTH_16S:
    Size = sizeof(short);
    break;
  case IS_DEPTH_32S:
    Size = sizeof(int);
    break;
  case IS_DEPTH_32F:
    Size = sizeof(float);
    break;
  case IS_DEPTH_64F:
    Size = sizeof(double);
    break;
  default:
    Size = 0;
    break;
  }
  return Size;
}
/// <summary>
/// 创建新的矩阵数据。
/// </summary>
/// <param name="Width">矩阵的宽度。</param>
/// <param name="Height">矩阵的高度。</param>
/// <param name="Depth">矩阵的元素类型。</param>
/// <param name="Channel">矩阵的通道数。</param>
/// <param name="Matrix">返回的矩阵对象。</param>
/// <returns>返回值为0表示成功，其他表示失败。</returns>
IS_RET IS_CreateMatrix(int Width, int Height, int Depth, int Channel, TMatrix** Matrix)
{
  if (Width < 1 || Height < 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Depth != IS_DEPTH_8U && Depth != IS_DEPTH_8S && Depth != IS_DEPTH_16S && Depth != IS_DEPTH_32S && Depth != IS_DEPTH_32F && Depth != IS_DEPTH_64F) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Channel != 1 && Channel != 2 && Channel != 3 && Channel != 4) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  *Matrix = (TMatrix*)IS_AllocMemory(sizeof(TMatrix));                                    //      真的不相信这个函数还有分配不成功的时刻
  (*Matrix)->Width = Width;
  (*Matrix)->Height = Height;
  (*Matrix)->Depth = Depth;
  (*Matrix)->Channel = Channel;
  (*Matrix)->WidthStep = WIDTHBYTES(Width * Channel * IS_ELEMENT_SIZE(Depth));
  (*Matrix)->Data = (unsigned char*)IS_AllocMemory((*Matrix)->Height * (*Matrix)->WidthStep, true);
  if ((*Matrix)->Data == NULL) {
    IS_FreeMemory(*Matrix);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  (*Matrix)->Reserved = 0;
  return IS_RET_OK;
}
/// <summary>
/// 释放创建的矩阵。
/// </summary>
/// <param name="Matrix">需要释放的矩阵对象。</param>
/// <returns>返回值为0表示成功，其他表示失败。</returns>
IS_RET IS_FreeMatrix(TMatrix** Matrix)
{
  if ((*Matrix) == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if ((*Matrix)->Data == NULL) {
    IS_FreeMemory((*Matrix));
    return IS_RET_ERR_OUTOFMEMORY;
  }
  else {
    IS_FreeMemory((*Matrix)->Data);                 //      注意释放的顺序
    IS_FreeMemory((*Matrix));
    return IS_RET_OK;
  }
}
/// <summary>
/// 克隆现有的矩阵。
/// </summary>
/// <param name="Src">被克隆的矩阵对象。</param>
/// <param name="Dest">克隆后的矩阵对象。</param>
/// <returns>返回值为0表示成功，其他表示失败。</returns>
IS_RET IS_CloneMatrix(TMatrix* Src, TMatrix** Dest)
{
  if (Src == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, Src->Channel, Dest);
  if (Ret == IS_RET_OK) {
    memcpy((*Dest)->Data, Src->Data, (*Dest)->Height * (*Dest)->WidthStep);
  }
  return Ret;
}


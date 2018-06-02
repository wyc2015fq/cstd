//////////////////////////////////////////////////////////////////
//																//
//		用途 : RLE 压缩算法	(PCX, TGA图像)						//
//		创建 : [Foolish] / 2000-9-6								//
//		更新 : 2002-11-29										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_RLE_COMPRESS_H__
#define		__FOO_RLE_COMPRESS_H__
#include <windows.h>
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

//====================================================================
//	RLE - 压缩算法
//====================================================================
//////////////////////////////////////////////////////////////////////
//	功  能 ：RLE压 / 解压缩 ( PCX 图像 )							//
//	参  数 ：														//
//	返回值 ：写入OutBuffer中的字节数								//
//	说  明 ：压缩时OutBuffer至少两倍InBuffer大小					//
//////////////////////////////////////////////////////////////////////
int  RLE_PCX_Encode (BYTE * InBuffer, int nInSize, BYTE * OutBuffer) ;
int  RLE_PCX_Decode (BYTE * InBuffer, int nInSize, BYTE * OutBuffer) ;
//////////////////////////////////////////////////////////////////////
//	功  能 ：RLE压 / 解压缩 ( PCX, TGA 图像 )						//
//																	//
//	参  数 ：iColorBit 为像素颜色位数								//
//					PCX为 ：1, 4, 8, 24								//
//					TGA为 ：8, 16, 24, 32							//
//			 iNumPixel 为写入OutBuffer中的像素个数					//
//																	//
//	返回值 ：EncodeLine 返回编码后 OutBuffer 的指针					//
//			 DecodeLine 返回解码后 InBuffer 的指针					//
//																	//
//	说  明 ：PCX图像 ：1,4,8位色位图按字节处理，24位色分成3通道处理	//
//			 TGA图像 ：支持8, 16, 24, 32色, 不支持15位色			//
//			 压缩时OutBuffer至少两倍InBuffer大小					//
//////////////////////////////////////////////////////////////////////
BYTE * RLE_TGA_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;

//===================================================================
#ifdef __cplusplus
}
#endif
#endif
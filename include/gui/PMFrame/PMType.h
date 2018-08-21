//********************************************************************************
//文件：PMType.h
//作者：feiren
//时间：2011-8-20
//用途：封装基本类型对象
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMTYPE_H__
#define __PMTYPE_H__

#include "PMJson.h"



#pragma once

#define PMVoid		void
#define PMBool		bool
#define PMChar		char
#define PMWchar		wchar_t
#define PMByte		unsigned char
#define PMShort		short
#define PMUnshort	unsigned short
#define PMInt		int
#define PMUnint		unsigned int
#define PMLong		long
#define PMUnlong	unsigned long
#define PMDouble	double
#define PMColor		COLORREF
#define PMBitmap	HBITMAP 


#define PMSuccess	1
#define PMFail		-1
#define PMError		-1
#define PMNone		0
#define PMTrue		true
#define PMFalse		false

#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))

typedef TPoint<PMInt> IntPoint;
typedef TPoint3D<PMInt> IntPoint3D;
typedef TPoint<PMDouble> DoublePoint;
typedef vector<IntPoint> IntPoints;

typedef TSize<PMInt> IntSize;
typedef TSize<PMDouble> DoubleSize;
typedef vector<IntSize> IntSizes;

typedef TRect<PMInt> IntRect;
typedef TRect<PMDouble> DoubleRect;
typedef vector<IntRect> IntRects;

typedef TCoord<PMInt> IntCoord;
typedef TCoord<PMDouble> DoubleCoord;

typedef std::map<std::string, std::string>	MapString;
typedef std::vector<std::string>			VectorString;
typedef std::pair<std::string, std::string> PairString;
typedef std::vector<PairString>				VectorStringPairs;
typedef std::vector<PMInt>					PMInts;
typedef std::vector<PMDouble>				PMDoubles;
typedef std::map<IntPoint, PMInt>			MapPoint;
typedef std::map<IntPoint3D, PMInt>			MapPoint3D;
typedef std::map<PMInt, PMInt>				MapInt;
typedef std::list<void*>					PMTasks;

typedef char int8;
typedef short int16;
typedef int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;

typedef unsigned int Char;
void Assign(IntPoint &sourcePoint,const POINT &targetPoint);
void Assign(IntSize &sourceSize,const SIZE &targetSize);
void Assign(IntRect &sourceRect,const RECT &targetRect);
void Assign(RECT &sourceRect,const IntRect &targetRect);
IntRect CreateRect(const IntSize &sourceSize);
IntRect CreateRect(RECT &source);
IntSize CreateSize(SIZE &size);
enum ArrowState
{
	ARROW_NONE,
	ARROW_FORWARD,
	ARROW_REVERSE,
	ARROW_BOTH,
};

#define MAX_DATA  1
#define MID_DATA  0
#define MIN_DATA  -1

#define CARET_TIMMER_TICC 500
#define SYSTEM_TIMMER_TICC 50
#define GIF_TIMMER_TICC 50
#endif
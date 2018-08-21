///////////////////////////////////////////////////////////////////////////
//  Copyright(c) 1999-2015, zyzx, All Rights Reserved
//	Created: 2015/03/13
//
/// @file	 lui.h
/// @brief	 LUI基础头文件
/// @author  李绍良[lsl](zyzx),email:zyzx_lsl@163.com
/// @version 0.1
/// @history 修改历史
///  \n lsl	2015/03/13	0.1	创建文件
/// @warning 没有警告
///////////////////////////////////////////////////////////////////////////
#ifndef __LUI_H__
#define __LUI_H__


#if defined(WIN32)
	#if defined(LUI_BUILD_DLL)
		#define LUI_API extern "C"	__declspec(dllexport)
	#else
		#define LUI_API extern "C"	__declspec(dllimport)
	#endif
#else
	#ifdef __cplusplus
		#define LUI_API	extern "C"
	#else
		#define LUI_API	extern
	#endif
#endif

typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;

typedef int				int_t;
typedef unsigned int	uint_t;


typedef struct LUI_CANVAS
{
	int_t cx;
	int_t cy;
	int_t bpp;
	int_t pitch;
	int_t len;

	unsigned char *pData;
}LUI_CANVAS;

typedef struct LUI_GRAPHICS
{
	typedef int_t (*ScreenSize)(int_t *pCx, int_t *pCy);
	typedef int_t (*CreateCanvas)(LUI_CANVAS *ptr, int_t cx, int_t cy, int_t bpp);
	typedef int_t (*DestroyCanvas)(LUI_CANVAS *ptr);

	typedef void (*DrawPixel)(LUI_CANVAS *ptr, int_t x, int_t y, uint32 color);
	typedef void (*DrawLine)(LUI_CANVAS *ptr, int_t x1, int_t y1, int_t x2, int_t y2, uint32 color, int_t width);
	typedef void (*DrawRect)(LUI_CANVAS *ptr, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool fill, int_t border);
	typedef void (*DrawImage)(LUI_CANVAS *ptr, const unsigned char *pImage, int_t x, int_t y, int_t width, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha);
	typedef void (*DrawImageID)(LUI_CANVAS *ptr, int_t bmpid, int_t x, int_t y, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha);
	typedef int_t (*DrawText)(LUI_CANVAS *ptr, int_t zoom, int_t size, const char *pStr, int_t len, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool border, uint32 color_border);
	typedef bool (*TextSize)(int_t size, int_t zoom, const char *pStr, int_t len, int_t &cx, int_t &cy);

	typedef void (*Refresh)(int_t x, int_t y, int_t cx, int_t cy);
	typedef void (*Clear)(LUI_CANVAS *ptr, int_t pitch, int_t x, int_t y, int_t cx, int_t cy);

}LUI_GRAPHICS;



//rgb1555
#define LUI_RGB1555(r, g, b) \
					((uint16) \
					(0x8000 | \
					(((r & 0xFF) >> 3) << 10) | \
					(((g & 0xFF) >> 3) << 5) | \
					((b & 0xFF) >> 3)))

#define LUI_R1555(color) ((color & 0x7C00) >> 7)
#define LUI_G1555(color) ((color & 0x3E0) >> 2)
#define LUI_B1555(color) ((color & 0x1F) << 3)

//rgba8888
#define LUI_RGBA(a, r, g, b) \
				((uint32) \
				(((a) & 0xFF) << 24) | \
				(((r) & 0xFF) << 16) | \
				(((g) & 0xFF) << 8) | \
				((b) & 0xFF))

#define LUI_RGB8888(r, g, b) GUI_RGBA(0xFF, r, g, b)

#define LUI_R8888(color) ((color >> 16) & 0xFF)
#define LUI_G8888(color) ((color >> 8) & 0xFF)
#define LUI_B8888(color) (color & 0xFF)

#ifdef __LUI_BIT32__
	#define LUI_RGB		LUI_RGB8888
	#define LUI_R		LUI_R8888
	#define LUI_G		LUI_G8888
	#define LUI_B		LUI_B8888
#else
	#define LUI_RGB		LUI_RGB1555
	#define LUI_R		LUI_R1555
	#define LUI_G		LUI_G1555
	#define LUI_B		LUI_B1555
#endif


typedef struct LRECT
{
	int_t l;	//left;
	int_t t;	//top;
	int_t r;	//right;
	int_t b;	//bottom;
}LRECT;

typedef struct LCOLOR
{
	uint32 outline;			//外框颜色
	uint32 inner;			//内框颜色
	uint32 bg;				//背景颜色
	uint32 fore;			//前景颜色
}LCOLOR;

typedef struct LMSG
{
	uint_t hwnd;
	uint_t vmsg;
	uint_t exmsg;
	uint_t lparam;
	uint_t wparam;
}LMSG;


typedef struct LUI_WND
{
	uint_t	type;			//[WND_TYPE]
	uint_t	id;				//当前窗口id
	uint_t	style;			//样式
	uint_t	status;			//显示、使能状态等

	int_t	fontsize;		//文本字体大小

	LRECT	rect;			//相对父窗口区域
}LUI_WND;


#endif // __LUI_H__
//end

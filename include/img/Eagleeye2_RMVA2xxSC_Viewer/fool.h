//////////////////////////////////////////////////////////////////
//		邮箱 : crazybit@263.net									//
//		主页 : http://crazybit.topcities.com/					//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef __FOO_HEADER_H_
#define __FOO_HEADER_H_
#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "vfw32.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "msimg32.lib") // AlphaBlend func.
#pragma comment (lib, "zlib.lib")
#pragma comment (lib, "png.lib")
#pragma comment (lib, "Jpeg.lib")
#pragma comment (lib, "Tiff.lib")
//#pragma comment (lib, "flib.lib")		// To Use FLibrary

//=============================================================================

#include ".\Include\StdDefine.h"
#include ".\Include\fooBit.h"

//	常用Win32函数
#include ".\Include\fooSys.h"
#include ".\Include\C_x86_CPU.h"
#include ".\Include\C_Process.h"
#include ".\Include\C_Memory.h"
#include ".\Include\C_Disk.h"
#include ".\Include\C_File.h"
#include ".\Include\C_Network.h"

//	一些类
#include ".\Include\TrayIcon.h"
#include ".\Include\FileFind.h"
#include ".\Include\Folder.h"
#include ".\Include\MemMapFile.h"
#include ".\Include\ToolHelp.h"
#include ".\Include\Reg.h"
#include ".\Include\Time.h"

//	压缩算法
#include ".\Include\Compress\Huffman.h"
#include ".\Include\Compress\Rle.h"
#include ".\Include\Compress\Lzw.h"

//	图像处理
#include ".\Include\Pic\Bitmap.h"
#include ".\Include\Pic\Pic.h"
#include ".\Include\Pic\Effect.h"
#include ".\Include\Pic\Image.h"

//#include ".\WinApi\fooWave.h"

#endif
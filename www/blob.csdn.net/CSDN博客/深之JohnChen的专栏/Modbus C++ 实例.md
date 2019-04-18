# Modbus C++ 实例 - 深之JohnChen的专栏 - CSDN博客

2017年09月14日 18:20:47[byxdaz](https://me.csdn.net/byxdaz)阅读数：943


//公共函数类

CommonFunction.h

```cpp
//公共函数类

#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include <string.h>
#include <ctype.h>
using namespace std;

typedef struct tagFileInfo 
{
	string strFileName;			//文件名
	string strPathFileName;		//全路径文件名
}FileInfo;

//CRC16校验
unsigned short CRC16( unsigned char* puchMsg, unsigned short usDataLen);

//crc32实现函数
unsigned int CRC32( unsigned char *buf, unsigned int len);

//获取两种字符之间的数据
void GeDataListByStartStopSplitFlag(char *pContent,std::list<int> & listId,char cStartFlag='{',char cStopFlag='}');

//获取分割数据（通过分隔符）
void GeDataListBySplitFlag(char *pContent,std::list<std::string> & listId,char *pSplitFlag=",");

//字符串去掉左右空格
char *trim(char *str);

/*
功能说明：获取程序版本号
参数说明：pVersion,字符串，表示版本号(比如：1.0.0.1)
返回值：BOOL
*/
BOOL GetVersion(HINSTANCE hInstance,char *pVersion);

//-------------------------------------------------------------------------------------
// 功能：组建绝对路径
// 参数：pBuffer[in]:缓冲区		nLength[in,out]:缓冲区长度
//		  pAttachName[in]:文件名前缀
//		  pAttachDir[in]:附加目录(入"TempFile\\",如无"\\"会自动追加)
// 返回：成功与否
//-------------------------------------------------------------------------------------
BOOL ConstructFileName(char* pBuffer,int& nLength,const char* pAttachName,const char* pAttachDir);

//功能：组建绝对路径[目录自动生成]
BOOL ConstructFileNameEx(char* pBuffer,int& nLength,const char* pAttachName,const char* pAttachDir);

//查找某目录下文件和文件夹
void FindFiles(char * lpPath,list<FileInfo> & listFile,bool bFindSubFolder = false);

//分割字符
void StringSplit(char *pStr,int nSplitFlag,list<string> & listSplit);

//获取低字节
unsigned char GetShortLowByte(unsigned short usData);

//获取高字节
unsigned char GetShortHighByte(unsigned short usData);
```

CommonFunction.cpp

```cpp
#include "CommonFunction.h"
#include <winver.h>
#include <Dbghelp.h>

#pragma comment(lib,"version.lib ")
#pragma comment(lib,"Dbghelp.lib")

//CRC16校验
unsigned short CRC16( unsigned char* puchMsg, unsigned short usDataLen)
{
	static unsigned char auchCRCHi[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	} ;

	static unsigned char auchCRCLo[] =
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40
	};

	unsigned char uchCRCHi = 0xFF ;
	unsigned char uchCRCLo = 0xFF ;
	unsigned char uIndex ;
	while( usDataLen-- )
	{
		uIndex = uchCRCHi ^ *puchMsg++ ;
		uchCRCHi = uchCRCLo ^ auchCRCHi[ uIndex ] ;
		uchCRCLo = auchCRCLo[ uIndex ] ;
	}
	return ( uchCRCHi << 8 | uchCRCLo ) ;
}

static unsigned int   g_CRC32[256] = {0};
static char   g_init = 0;
//初始化表
static void init_table()
{
	unsigned int   i = 0,j = 0;
	unsigned int   crc = 0;
	for(i = 0;i < 256;i++)
	{
		crc = i;
		for(j = 0;j < 8;j++)
		{
			if(crc & 1)
			{
				crc = (crc >> 1) ^ 0xEDB88320;
			}
			else
			{
				crc = crc >> 1;
			}
		}
		g_CRC32[i] = crc;
	}
}

//crc32实现函数
unsigned int CRC32( unsigned char *buf, unsigned int len)
{
	unsigned int ret = 0xFFFFFFFF;
	int   i;
	if(!g_init)
	{
		init_table();
		g_init = 1;
	}
	for(i = 0; i < len;i++)
	{
		ret = g_CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
	}
	ret = ~ret;
	return ret;
}

//获取两种字符之间的数据
void GeDataListByStartStopSplitFlag(char *pContent,std::list<int> & listId,char cStartFlag,char cStopFlag)
{
	listId.clear();
	if(pContent != NULL)
	{
		char *p = pContent;
		char *q = NULL;
		char *qn = NULL;
		int nId = 0;
		char szId[32] = {0};
		while(true)
		{
			q = strchr(p,cStartFlag);
			if(q != NULL)
			{
				qn = strchr(q+1,cStopFlag);
				if(qn != NULL)
				{
					memset(szId,0,sizeof(szId));
					memcpy(szId,q+1,qn-q-1);
					nId = atoi(szId);
					listId.push_back(nId);
					p = qn+1;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
}

//获取分割数据（通过分隔符）
void GeDataListBySplitFlag(char *pContent,std::list<std::string> & listId,char *pSplitFlag)
{
	listId.clear();
	if(pContent != NULL)
	{
		int nSplitFlagLength = strlen(pSplitFlag);
		char *p = pContent;
		char *q = NULL;
		char *qn = NULL;
		int nId = 0;
		char szId[128] = {0};
		while(true)
		{
			q = strstr(p,pSplitFlag);
			if(q != NULL)
			{
				memset(szId,0,sizeof(szId));
				memcpy(szId,p,q-p);
				listId.push_back(szId);
				//下一个
				p = q+nSplitFlagLength;
			}
			else
			{
				if(p != '\0')
				{
					memset(szId,0,sizeof(szId));
					strcpy(szId,p);
					listId.push_back(szId);
				}
				break;
			}
		}
	}
}

char *trim(char *str)
{
	char *p = str;
	char *p1;
	if(p)
	{
		p1 = p + strlen(str) - 1;
		while(*p && isspace(*p)) p++;
		while(p1 > p && isspace(*p1)) *p1-- = '/0';
	}
	return p;
}

//功能说明：获取程序版本号
BOOL GetVersion(HINSTANCE hInstance,char *pVersion)
{
	if(NULL == pVersion)
		return FALSE;

	//获取版本号
	DWORD dwSize,dwHandle; 
	BYTE *byData = NULL,*lpVers = NULL;  
	char sTitle[MAX_PATH] = {0};  
	char sAppName[MAX_PATH] = {0}; 
	unsigned int uLen = 0;
	GetModuleFileName(hInstance,sAppName,MAX_PATH);    
	dwSize=GetFileVersionInfoSize(sAppName,&dwHandle);  
	byData=new BYTE[dwSize+10]; 
	memset(byData,0,sizeof(BYTE)*(dwSize+10));
	GetFileVersionInfo(sAppName,0,dwSize,byData); 
	BOOL bSucess = VerQueryValue(byData,TEXT("\\StringFileInfo\\080403a8\\ProductVersion"),(void **)&lpVers,&uLen);//080403a8为中文  
	memcpy(pVersion,lpVers,uLen);
	if(byData!=NULL)
	{
		delete []byData;
		byData = NULL;
	}
	if(!bSucess)
		return FALSE;

	return TRUE;
}

//-------------------------------------------------------------------------------------
// 功能：组建绝对路径
// 参数：pBuffer[in]:缓冲区		nLength[in,out]:缓冲区长度
//		  pAttachName[in]:文件名前缀
//		  pAttachDir[in]:附加目录(入"TempFile\\",如无"\\"会自动追加)
// 返回：成功与否
//-------------------------------------------------------------------------------------
BOOL ConstructFileName(char* pBuffer,int& nLength,const char* pAttachName,const char* pAttachDir)
{
	char szBuffer[512]={0};
	DWORD dwPathLength = ::GetModuleFileName(NULL,szBuffer,512);
	if(0 == dwPathLength)
		return FALSE;

	char* pTarget = ::strrchr(szBuffer,'\\')+1;
	pTarget[0] = 0;

	if(NULL != pAttachDir)
	{
		::strcat(szBuffer,pAttachDir);
		pTarget = ::strrchr((char*)pAttachDir,'\\');
		if(NULL == pTarget)
			::strcat(szBuffer,"\\");
	}

	if(NULL != pAttachName)
		::strcat(szBuffer,pAttachName);

	int nRealLength = ::strlen(szBuffer);
	if(nRealLength > nLength)
		return FALSE;

	::strcpy_s(pBuffer,nLength,szBuffer);
	nLength = nRealLength;
	return TRUE;
}

//功能：组建绝对路径[目录自动生成]
BOOL ConstructFileNameEx(char* pBuffer,int& nLength,const char* pAttachName,const char* pAttachDir)
{
	//	if(NULL == pAttachName)
	//		return FALSE;

	char szBuffer[512]={0};
	DWORD dwPathLength = ::GetModuleFileName(NULL,szBuffer,512);
	if(0 == dwPathLength)
		return FALSE;

	char* pTarget = ::strrchr(szBuffer,'\\')+1;
	pTarget[0] = 0;

	if(NULL != pAttachDir)
	{
		::strcat(szBuffer,pAttachDir);
		pTarget = ::strrchr((char*)pAttachDir,'\\');
		if(NULL == pTarget)
			::strcat(szBuffer,"\\");

		MakeSureDirectoryPathExists(szBuffer);
	}

	if(NULL != pAttachName)
		::strcat(szBuffer,pAttachName);

	int nRealLength = ::strlen(szBuffer);
	if(nRealLength > nLength)
		return FALSE;

	::strcpy_s(pBuffer,nLength,szBuffer);
	nLength = nRealLength;
	return TRUE;
}

//查找某目录下文件和文件夹
void FindFiles(char * lpPath,list<FileInfo> & listFile,bool bFindSubFolder)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind,lpPath);
	strcat(szFind,"*.*");
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return;

	FileInfo fiFileInfo;
	char szFile[MAX_PATH] = {0};
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{
				strcpy(szFile,lpPath);
				strcat(szFile,"");
				strcat(szFile,FindFileData.cFileName);
				if(bFindSubFolder)	//搜索子文件夹
				{
					FindFiles(szFile,listFile,bFindSubFolder);
				}
			}
		}
		else
		{
			strcpy(szFile,lpPath);
			strcat(szFile,"");
			strcat(szFile,FindFileData.cFileName);
			fiFileInfo.strFileName = FindFileData.cFileName;
			fiFileInfo.strPathFileName = szFile;
			listFile.push_back(fiFileInfo);
		}

		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}

//分割字符
void StringSplit(char *pStr,int nSplitFlag,list<string> & listSplit)
{
	listSplit.clear();

	if(NULL == pStr)
		return;

	if('\0' == nSplitFlag)
	{
		listSplit.push_back(pStr);
	}
	else
	{
		char *p = NULL;
		char *q = pStr;
		char szTmp[128] = {0};
		while(true)
		{
			p = strchr(q,nSplitFlag);
			memset(szTmp,0,sizeof(szTmp));
			if(p != NULL)
			{
				memcpy(szTmp,q,p-q);
				listSplit.push_back(szTmp);
				q = p+1;
				if(q == NULL)
				{
					break;
				}
			}
			else
			{
				strcpy(szTmp,q);
				listSplit.push_back(szTmp);
				break;
			}
		}
	}
}

//获取低字节
unsigned char GetShortLowByte(unsigned short usData)
{
	return (usData & 0x00ff);
}

//获取高字节
unsigned char GetShortHighByte(unsigned short usData)
{
	return (usData & 0xff00) >> 8;
}
```

//串口类

SerialPort.h

```cpp
/*
Module : SerialPort.h
Purpose: Interface for an C++ wrapper class for serial ports

Copyright (c) 1999 - 2017 by PJ Naughter. (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

///////////////////// Macros / Structs etc ////////////////////////////////////

#pragma once

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#ifndef CSERIALPORT_EXT_CLASS
#define CSERIALPORT_EXT_CLASS
#endif //#ifndef CSERIALPORT_EXT_CLASS

#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_(X)
#endif //#ifndef _Out_writes_bytes_

#ifndef __out_data_source
#define __out_data_source(X)
#endif //#ifndef __out_data_source

#ifndef _Out_writes_bytes_to_opt_
#define _Out_writes_bytes_to_opt_(X,Y)
#endif //#ifndef _Out_writes_bytes_to_opt_

#ifndef _Out_writes_bytes_opt_
#define _Out_writes_bytes_opt_(X)
#endif //#ifndef _Out_writes_bytes_opt_

#ifndef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_(X)
#endif //#ifndef _In_reads_bytes_opt_

#ifndef _In_
#define _In_
#endif //#ifndef _In_

#ifndef _In_z_
#define _In_z_
#endif //#ifndef _In_z_

#ifndef _Inout_opt_
#define _Inout_opt_
#endif //#ifndef _Inout_opt_

#ifndef _Out_opt_
#define _Out_opt_
#endif //#ifndef _Out_opt_

#ifndef _Out_
#define _Out_
#endif //#ifndef _Out_

#ifndef _Inout_
#define _Inout_
#endif //#ifndef _Inout_

#ifndef _In_opt_
#define _In_opt_
#endif //#ifndef _In_opt_

////////////////////////// Includes ///////////////////////////////////////////

#include <sal.h>

#ifndef CSERIALPORT_MFC_EXTENSIONS
#include <exception>
#include <string>
#endif //#ifndef CSERIALPORT_MFC_EXTENSIONS

/////////////////////////// Classes ///////////////////////////////////////////

#ifdef CSERIALPORT_MFC_EXTENSIONS
class CSERIALPORT_EXT_CLASS CSerialException : public CException
#else
class CSERIALPORT_EXT_CLASS CSerialException : public std::exception
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS
{
public:
//Constructors / Destructors
  CSerialException(_In_ DWORD dwError);

//Methods
#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

#if _MSC_VER >= 1700
  virtual BOOL GetErrorMessage(_Out_writes_z_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError,	_Out_opt_ PUINT pnHelpContext = NULL) const;
  virtual BOOL GetErrorMessage(_Out_writes_z_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext = NULL);
#elif _MSC_VER >= 1500
  virtual BOOL GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError,	_Out_opt_ PUINT pnHelpContext = NULL) const;
  virtual BOOL GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext = NULL);
#else	
  virtual BOOL GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext = NULL) const;
  virtual BOOL GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext = NULL);
#endif //#if _MSC_VER >= 1700

#ifdef CSERIALPORT_MFC_EXTENSIONS
  CString GetErrorMessage();
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

//Data members
  DWORD m_dwError;
};

class CSERIALPORT_EXT_CLASS CSerialPort
{
public:
//Enums
  enum FlowControl
  {
    NoFlowControl,
    CtsRtsFlowControl,
    CtsDtrFlowControl,
    DsrRtsFlowControl,
    DsrDtrFlowControl,
    XonXoffFlowControl
  };

  enum Parity
  {    
    NoParity = 0,
    OddParity = 1,
    EvenParity = 2,
    MarkParity = 3,
    SpaceParity = 4
  };

  enum StopBits
  {
    OneStopBit,
    OnePointFiveStopBits,
    TwoStopBits
  };

//Constructors / Destructors
  CSerialPort();
  virtual ~CSerialPort();

//General Methods
  void Open(_In_ int nPort, _In_ DWORD dwBaud = 9600, _In_ Parity parity = NoParity, _In_ BYTE DataBits = 8, 
            _In_ StopBits stopBits = OneStopBit, _In_ FlowControl fc = NoFlowControl, _In_ BOOL bOverlapped = FALSE);
  void Open(_In_z_ LPCTSTR pszPort, _In_ DWORD dwBaud = 9600, _In_ Parity parity = NoParity, _In_ BYTE DataBits = 8, 
            _In_ StopBits stopBits = OneStopBit, _In_ FlowControl fc = NoFlowControl, _In_ BOOL bOverlapped = FALSE);
  void Close();
  void Attach(_In_ HANDLE hComm);
  HANDLE Detach();
  operator HANDLE() const { return m_hComm; };
  BOOL IsOpen() const { return m_hComm != INVALID_HANDLE_VALUE; };

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
  void Dump(_In_ CDumpContext& dc) const;
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

//Reading / Writing Methods
  DWORD Read(_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead);
  void  Read(_Out_writes_bytes_to_opt_(dwNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _In_ OVERLAPPED& overlapped, _Inout_opt_ DWORD* lpNumberOfBytesRead = NULL);
  void  ReadEx(_Out_writes_bytes_opt_(dwNumberOfBytesToRead) __out_data_source(FILE) LPVOID lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
  DWORD Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite);
  void  Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _In_ OVERLAPPED& overlapped, _Out_opt_ DWORD* lpNumberOfBytesWritten = NULL);
  void  WriteEx(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) LPCVOID lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
  void  TransmitChar(_In_ char cChar);
  void  GetOverlappedResult(_In_ OVERLAPPED& overlapped, _Out_ DWORD& dwBytesTransferred, _In_ BOOL bWait);
  void  CancelIo();
  DWORD BytesWaiting();

//Configuration Methods
  void GetConfig(_Out_ COMMCONFIG& config);
  static void GetDefaultConfig(_In_ int nPort, _Out_ COMMCONFIG& config);
  static void GetDefaultConfig(_In_z_ LPCTSTR pszPort, _Out_ COMMCONFIG& config);
  void SetConfig(_In_ COMMCONFIG& Config);
  static void SetDefaultConfig(_In_ int nPort, _In_ COMMCONFIG& config);
  static void SetDefaultConfig(_In_z_ LPCTSTR pszPort, _In_ COMMCONFIG& config);

//Misc RS232 Methods
  void ClearBreak();
  void SetBreak();
  void ClearError(_Out_ DWORD& dwErrors);
  void GetStatus(_Out_ COMSTAT& stat);
  void GetState(_Out_ DCB& dcb);
  void SetState(_In_ DCB& dcb);
  void Escape(_In_ DWORD dwFunc);
  void ClearDTR();
  void ClearRTS();
  void SetDTR();
  void SetRTS();
  void SetXOFF();
  void SetXON();
  void GetProperties(_Inout_ COMMPROP& properties);
  void GetModemStatus(_Out_ DWORD& dwModemStatus); 

//Timeouts
  void SetTimeouts(_In_ COMMTIMEOUTS& timeouts);
  void GetTimeouts(_Out_ COMMTIMEOUTS& timeouts);
  void Set0Timeout();
  void Set0WriteTimeout();
  void Set0ReadTimeout();

//Event Methods
  void SetMask(_In_ DWORD dwMask);
  void GetMask(_Out_ DWORD& dwMask);
  void WaitEvent(_Inout_ DWORD& dwMask);
  BOOL WaitEvent(_Inout_ DWORD& dwMask, _Inout_ OVERLAPPED& overlapped);
  
//Queue Methods
  void Flush();
  void Purge(_In_ DWORD dwFlags);
  void TerminateOutstandingWrites();
  void TerminateOutstandingReads();
  void ClearWriteBuffer();
  void ClearReadBuffer();
  void Setup(_In_ DWORD dwInQueue, _In_ DWORD dwOutQueue);

//Static methods
  static void ThrowSerialException(_In_ DWORD dwError = 0);

protected:
//Member variables
  HANDLE m_hComm; //Handle to the comms port
};

#endif //#ifndef __SERIALPORT_H__
```

SerialPort.cpp

```cpp
/*
Module : SerialPort.cpp
Purpose: Implementation for an C++ wrapper class for serial ports
Created: PJN / 31-05-1999
History: PJN / 03-06-1999 1. Fixed problem with code using CancelIo which does not exist on 95.
                          2. Fixed leaks which can occur in sample app when an exception is thrown
         PJN / 16-06-1999 1. Fixed a bug whereby CString::ReleaseBuffer was not being called in 
                             CSerialException::GetErrorMessage
         PJN / 29-09-1999 1. Fixed a simple copy and paste bug in CSerialPort::SetDTR
         PJN / 08-05-2000 1. Fixed an unreferrenced variable in CSerialPort::GetOverlappedResult in VC 6
         PJN / 10-12-2000 1. Made class destructor virtual
         PJN / 15-01-2001 1. Attach method now also allows you to specify whether the serial port
                          is being attached to in overlapped mode
                          2. Removed some ASSERTs which were unnecessary in some of the functions
                          3. Updated the Read method which uses OVERLAPPED IO to also return the bytes
                          read. This allows calls to WriteFile with a zeroed overlapped structure (This
                          is required when dealing with TAPI and serial communications)
                          4. Now includes copyright message in the source code and documentation.
         PJN / 24-03-2001 1. Added a BytesWaiting method
         PJN / 04-04-2001 1. Provided an overriden version of BytesWaiting which specifies a timeout
         PJN / 23-04-2001 1. Fixed a memory leak in DataWaiting method
         PJN / 01-05-2002 1. Fixed a problem in Open method which was failing to initialize the DCB 
                          structure incorrectly, when calling GetState. Thanks to Ben Newson for this fix.
         PJN / 29-05-2002 1. Fixed an problem where the GetProcAddress for CancelIO was using the
                          wrong calling convention
         PJN / 07-08-2002 1. Changed the declaration of CSerialPort::WaitEvent to be consistent with the
                          rest of the methods in CSerialPort which can operate in "OVERLAPPED" mode. A note
                          about the usage of this: If the method succeeds then the overlapped operation
                          has completed synchronously and there is no need to do a WaitForSingle/MultipleObjects.
                          If any other unexpected error occurs then a CSerialException will be thrown. See
                          the implementation of the CSerialPort::DataWaiting which has been rewritten to use
                          this new design pattern. Thanks to Serhiy Pavlov for spotting this inconsistency.
         PJN / 20-09-2002 1. Addition of an additional ASSERT in the internal _OnCompletion function.
                          2. Addition of an optional out parameter to the Write method which operates in 
                          overlapped mode. Thanks to Kevin Pinkerton for this addition.
         PJN / 10-04-2006 1. Updated copyright details.
                          2. Addition of a CSERIALPORT_EXT_CLASS and CSERIALPORT_EXT_API macros which makes 
                          the class easier to use in an extension dll.
                          3. Removed derivation of CSerialPort from CObject as it was not really needed.
                          4. Fixed a number of level 4 warnings in the sample app.
                          5. Reworked the overlapped IO methods to expose the LPOVERLAPPED structure to client 
                          code.
                          6. Updated the documentation to use the same style as the web site.
                          7. Did a spell check of the HTML documentation.
                          8. Updated the documentation on possible blocking in Read/Ex function. Thanks to 
                          D Kerrison for reporting this issue.
                          9. Fixed a minor issue in the sample app when the code is compiled using /Wp64
         PJN / 02-06-2006 1. Removed the bOverlapped as a member variable from the class. There was no 
                          real need for this setting, since the SDK functions will perform their own 
                          checking of how overlapped operations should
                          2. Fixed a bug in GetOverlappedResult where the code incorrectly checking against
                          the error ERROR_IO_PENDING instead of ERROR_IO_INCOMPLETE. Thanks to Sasho Darmonski
                          for reporting this bug.
                          3. Reviewed all TRACE statements for correctness.
         PJN / 05-06-2006 1. Fixed an issue with the creation of the internal event object. It was incorrectly
                          being created as an auto-reset event object instead of a manual reset event object.
                          Thanks to Sasho Darmonski for reporting this issue.
         PJN / 24-06-2006 1. Fixed some typos in the history list. Thanks to Simon Wong for reporting this.
                          2. Made the class which handles the construction of function pointers at runtime a
                          member variable of CSerialPort
                          3. Made AfxThrowSerialPortException part of the CSerialPort class. Thanks to Simon Wong
                          for reporting this.
                          4. Removed the unnecessary CSerialException destructor. Thanks to Simon Wong for 
                          reporting this.
                          5. Fixed a minor error in the TRACE text in CSerialPort::SetDefaultConfig. Again thanks
                          to Simon Wong for reporting this. 
                          6. Code now uses new C++ style casts rather than old style C casts where necessary. 
                          Again thanks to Simon Wong for reporting this.
                          7. CSerialException::GetErrorMessage now uses the strsafe functions. This does mean 
                          that the code now requires the Platform SDK if compiled using VC 6.
         PJN / 25-06-2006 1. Combined the functionality of the CSerialPortData class into the main CSerialPort class.
                          2. Renamed AfxThrowSerialPortException to ThrowSerialPortException and made the method
                          public.
         PJN / 05-11-2006 1. Minor update to stdafx.h of sample app to avoid compiler warnings in VC 2005.
                          2. Reverted the use of the strsafe.h header file. Instead now the code uses the VC 2005
                          Safe CRT and if this is not available, then we fail back to the standard CRT.
         PJN / 25-01-2007 1. Minor update to remove strsafe.h from stdafx.h of the sample app.
                          2. Updated copyright details.
         PJN / 24-12-2007 1. CSerialException::GetErrorMessage now uses the FORMAT_MESSAGE_IGNORE_INSERTS flag. For more information please see Raymond
                          Chen's blog at http://blogs.msdn.com/oldnewthing/archive/2007/11/28/6564257.aspx. Thanks to Alexey Kuznetsov for reporting this
                          issue.
                          2. Simplified the code in CSerialException::GetErrorMessage somewhat.
                          3. Optimized the CSerialException constructor code.
                          4. Code now uses newer C++ style casts instead of C style casts.
                          5. Reviewed and updated all the TRACE logging in the module
                          6. Replaced all calls to ZeroMemory with memset
         PJN / 30-12-2007 1. Updated the sample app to clean compile on VC 2008
                          2. CSerialException::GetErrorMessage now uses Checked::tcsncpy_s if compiled using VC 2005 or later.
         PJN / 18-05-2008 1. Updated copyright details.
                          2. Changed the actual values for Parity enum so that they are consistent with the Parity define values in the Windows SDK 
                          header file WinBase.h. This avoids the potential issue where you use the CSerialPort enum parity values in a call to the
                          raw Win32 API calls. Thanks to Robert Krueger for reporting this issue.
         PJN / 21-06-2008 1. Code now compiles cleanly using Code Analysis (/analyze)
                          2. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          3. The code now only supports VC 2005 or later. 
                          4. CSerialPort::Read, Write, GetOverlappedResult & WaitEvent now throw an exception irrespective of whether the last error
                          is ERROR_IO_PENDING or not
                          5. Replaced all calls to ZeroMemory with memset
         PJN / 04-07-2008 1. Provided a version of the Open method which takes a string instead of a numeric port number value. This allows the code
                          to support some virtual serial port packages which do not use device names of the form "COM%d". Thanks to David Balazic for 
                          suggesting this addition.
         PJN / 25-01-2012 1. Updated copyright details.
                          2. Updated sample app and class to compile cleanly on VC 2010 and later.
         PJN / 28-02-2015 1. Updated sample project settings to more modern default values.
                          2. Updated copyright details.
                          3. Reworked the CSerialPort and CSerialPortException classes to optionally compile without MFC. By default 
                          the classes now use STL classes and idioms but if you define CSERIALPORT_MFC_EXTENSIONS the classes will 
                          revert back to the MFC behaviour.
                          4. Remove logic to use GetProcAddress to access CancelIO functionality.
                          5. Updated the code to clean compile on VC 2013
                          6. Added SAL annotations to all the code
                          7. Addition of a GetDefaultConfig method which takes a string
                          8. Addition of a SetDefaultConfig method which takes a string
         PJN / 26-04-2015 1. Removed unnecessary inclusion of WinError.h
                          2. Removed the CSerialPort::DataWaiting method as it depends on the port being open in overlapped mode. Instead client code
                          can simply call CSerialPort::WaitEvent directly themselves. Removing this method also means that the CSerialPort::m_hEvent
                          handle has not also been removed.
                          3. The CSerialPort::WriteEx method has been reworked to expose all the parameters of the underlying WriteFileEx API. This
                          rework also fixes a memory leak in WriteEx which can sometimes occur. This reworks also means that the 
                          CSerialPort::_OnCompletion and CSerialPort::_OnCompletion methods have been removed. Thanks to Yufeng Huang for reporting 
                          this issue.
                          4. The CSerialPort::ReadEx method has been reworked to expose all the parameters of the underlying ReadFileEx API. This
                          rework also fixes a memory leak in ReadEx which can sometimes occur. This reworks also means that the 
                          CSerialPort::_OnCompletion and CSerialPort::_OnCompletion methods have been removed. Thanks to Yufeng Huang for reporting 
                          this issue.
         PJN / 17-12-2015 1. Verified the code compiles cleanly on VC 2015.
         PJN / 22-05-2016 1. Updated copyright details.
                          2. Fixed some typos in SerialPort.h, SerialPort.cpp and SerialPort.htm where CSERIALPORT_MFC_EXTENSTIONS was being used 
                          instead of CSERIALPORT_MFC_EXTENSIONS. Thanks to Nicholas Buse for reporting this issue.
         PJN / 16-08-2017 1. Updated copyright details.
                          2. Updated declarations of CSerialException::GetErrorMessage to be consistent with CException base class implementations
                          3. Fixed up the SAL annotations on CSerialPort::GetConfig. Thanks to Wang Jinhai for reporting this issue.

Copyright (c) 1996 - 2017 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

///////////////////////////////// Includes ////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

#ifndef __ATLBASE_H__
#pragma message("To avoid this message, please put atlbase.h in your pre compiled header (normally stdafx.h)")
#include <atlbase.h>
#endif //#ifndef __ATLBASE_H__

///////////////////////////////// Defines /////////////////////////////////////

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
#define new DEBUG_NEW
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

//////////////////////////////// Implementation ///////////////////////////////

#if _MSC_VER >= 1700
BOOL CSerialException::GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext) const
#elif _MSC_VER >= 1500
BOOL CSerialException::GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext) const
#else
BOOL CSerialException::GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext) const
#endif //#if _MSC_VER >= 1700
{
  //Validate our parameters
  ATLASSERT(lpszError != NULL);
    
  if (pnHelpContext != NULL)
    *pnHelpContext = 0;
    
  //What will be the return value from this function (assume the worst)
  BOOL bSuccess = FALSE;

  LPTSTR lpBuffer;
  DWORD dwReturn = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL,  m_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
                                 reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

  if (dwReturn == 0)
    *lpszError = _T('\0');
  else
  {
    bSuccess = TRUE;
    Checked::tcsncpy_s(lpszError, nMaxError, lpBuffer, _TRUNCATE);  
    LocalFree(lpBuffer);
  }

  return bSuccess;
}

#if _MSC_VER >= 1700
BOOL CSerialException::GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext)
#elif _MSC_VER >= 1500
BOOL CSerialException::GetErrorMessage(_Out_z_cap_(nMaxError) LPTSTR lpszError, _In_ UINT nMaxError, _Out_opt_ PUINT pnHelpContext)
#else
BOOL CSerialException::GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpszError, __in UINT nMaxError, __out_opt PUINT pnHelpContext)
#endif //#if _MSC_VER >= 1700
{
  //Call the const version of GetErrorMessage
  return const_cast<const CSerialException*>(this)->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
CString CSerialException::GetErrorMessage()
{
  CString rVal;
  LPTSTR pstrError = rVal.GetBuffer(4096);
  GetErrorMessage(pstrError, 4096, NULL);
  rVal.ReleaseBuffer();
  return rVal;
}
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

CSerialException::CSerialException(_In_ DWORD dwError) : m_dwError(dwError)
{
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
void CSerialException::Dump(_In_ CDumpContext& dc) const
{
  CObject::Dump(dc);

  dc << _T("m_dwError = ") << m_dwError << _T("\n");
}
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

CSerialPort::CSerialPort() : m_hComm(INVALID_HANDLE_VALUE)
{
}

CSerialPort::~CSerialPort()
{
  Close();
}

void CSerialPort::ThrowSerialException(_In_ DWORD dwError)
{
  if (dwError == 0)
    dwError = ::GetLastError();

  ATLTRACE(_T("Warning: throwing CSerialException for error %d\n"), dwError);
#ifdef CSERIALPORT_MFC_EXTENSIONS
  CSerialException* pException = new CSerialException (dwError);
  THROW(pException);
 #else
  CSerialException e(dwError);
  throw e;
 #endif //#ifdef CSERIALPORT_MFC_EXTENSIONS
}

#ifdef CSERIALPORT_MFC_EXTENSIONS
#ifdef _DEBUG
void CSerialPort::Dump(CDumpContext& dc) const
{
  dc << _T("m_hComm = ") << m_hComm << _T("\n");
}
#endif //#ifdef _DEBUG
#endif //#ifdef CSERIALPORT_MFC_EXTENSIONS

void CSerialPort::Open(_In_z_ LPCTSTR pszPort, _In_ DWORD dwBaud, _In_ Parity parity, _In_ BYTE DataBits, _In_ StopBits stopBits, _In_ FlowControl fc, _In_ BOOL bOverlapped)
{
  Close(); //In case we are already open

  //Call CreateFile to open the comms port
  m_hComm = CreateFile(pszPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, bOverlapped ? FILE_FLAG_OVERLAPPED : 0, NULL);
  if (m_hComm == INVALID_HANDLE_VALUE)
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Open, Failed to open the comms port, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  //Get the current state prior to changing it
  DCB dcb;
  dcb.DCBlength = sizeof(DCB);
  GetState(dcb);

  //Setup the baud rate
  dcb.BaudRate = dwBaud; 

  //Setup the Parity
  switch (parity)
  {
    case EvenParity:  
    {
      dcb.Parity = EVENPARITY;
      break;
    }
    case MarkParity:  
    {
      dcb.Parity = MARKPARITY;
      break;
    }
    case NoParity:    
    {
      dcb.Parity = NOPARITY;
      break;
    }
    case OddParity:   
    {
      dcb.Parity = ODDPARITY;
      break;
    }
    case SpaceParity: 
    {
      dcb.Parity = SPACEPARITY;
      break;
    }
    default:          
    {
      ATLASSERT(FALSE);            
      break;
    }
  }

  //Setup the data bits
  dcb.ByteSize = DataBits;

  //Setup the stop bits
  switch (stopBits)
  {
    case OneStopBit:           
    {
      dcb.StopBits = ONESTOPBIT; 
      break;
    }
    case OnePointFiveStopBits: 
    {
      dcb.StopBits = ONE5STOPBITS;
      break;
    }
    case TwoStopBits:          
    {
      dcb.StopBits = TWOSTOPBITS;
      break;
    }
    default:                   
    {
      ATLASSERT(FALSE);
      break;
    }
  }

  //Setup the flow control 
  dcb.fDsrSensitivity = FALSE;
  switch (fc)
  {
    case NoFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsRtsFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case CtsDtrFlowControl:
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrRtsFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case DsrDtrFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = TRUE;
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
      dcb.fOutX = FALSE;
      dcb.fInX = FALSE;
      break;
    }
    case XonXoffFlowControl:
    {
      dcb.fOutxCtsFlow = FALSE;
      dcb.fOutxDsrFlow = FALSE;
      dcb.fOutX = TRUE;
      dcb.fInX = TRUE;
      dcb.XonChar = 0x11;
      dcb.XoffChar = 0x13;
      dcb.XoffLim = 100;
      dcb.XonLim = 100;
      break;
    }
    default:
    {
      ATLASSERT(FALSE);
      break;
    }
  }
  
  //Now that we have all the settings in place, make the changes
  SetState(dcb);
}

void CSerialPort::Open(_In_ int nPort, _In_ DWORD dwBaud, _In_ Parity parity, _In_ BYTE DataBits, _In_ StopBits stopBits, _In_ FlowControl fc, _In_ BOOL bOverlapped)
{
  //Form the string version of the port number
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("\\\\.\\COM%d"), nPort);

  //Delegate the work to the other version of Open
  Open(szPort, dwBaud, parity, DataBits, stopBits, fc, bOverlapped);
}

void CSerialPort::Close()
{
  if (IsOpen())
  {
    //Close down the comms port
    CloseHandle(m_hComm);
    m_hComm = INVALID_HANDLE_VALUE;
  }
}

void CSerialPort::Attach(_In_ HANDLE hComm)
{
  Close();

  //Validate our parameters, now that the port has been closed
  ATLASSERT(m_hComm == INVALID_HANDLE_VALUE);

  m_hComm = hComm;  
}

HANDLE CSerialPort::Detach()
{
  //What will be the return value from this function
  HANDLE hComm = m_hComm;

  m_hComm = INVALID_HANDLE_VALUE;

  return hComm;
}

DWORD CSerialPort::Read(_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwBytesRead = 0;
  if (!ReadFile(m_hComm, lpBuffer, dwNumberOfBytesToRead, &dwBytesRead, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Read, Failed in call to ReadFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return dwBytesRead;
}

void CSerialPort::Read(_Out_writes_bytes_to_opt_(dwNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) void* lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _In_ OVERLAPPED& overlapped, _Inout_opt_ DWORD* lpNumberOfBytesRead)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ReadFile(m_hComm, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead, &overlapped))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Read, Failed in call to ReadFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ReadEx(_Out_writes_bytes_opt_(dwNumberOfBytesToRead) __out_data_source(FILE) LPVOID lpBuffer, _In_ DWORD dwNumberOfBytesToRead, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ReadFileEx(m_hComm, lpBuffer, dwNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ReadEx, Failed in call to ReadFileEx, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

DWORD CSerialPort::Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwBytesWritten = 0;
  if (!WriteFile(m_hComm, lpBuffer, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Write, Failed in call to WriteFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return dwBytesWritten;
}

void CSerialPort::Write(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) const void* lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _In_ OVERLAPPED& overlapped, _Out_opt_ DWORD* lpNumberOfBytesWritten)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WriteFile(m_hComm, lpBuffer, dwNumberOfBytesToWrite, lpNumberOfBytesWritten, &overlapped))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Write, Failed in call to WriteFile, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::WriteEx(_In_reads_bytes_opt_(dwNumberOfBytesToWrite) LPCVOID lpBuffer, _In_ DWORD dwNumberOfBytesToWrite, _Inout_ LPOVERLAPPED lpOverlapped, _In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WriteFileEx(m_hComm, lpBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WriteEx, Failed in call to WriteFileEx, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetOverlappedResult(_In_ OVERLAPPED& overlapped, _Out_ DWORD& dwBytesTransferred, _In_ BOOL bWait)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!::GetOverlappedResult(m_hComm, &overlapped, &dwBytesTransferred, bWait))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetOverlappedResult, Failed in call to GetOverlappedResult, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::CancelIo()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!::CancelIo(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("Failed in call to CancelIO, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

DWORD CSerialPort::BytesWaiting()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  //Check to see how many characters are unread
  COMSTAT stat;
  GetStatus(stat);
  return stat.cbInQue;
}

void CSerialPort::TransmitChar(_In_ char cChar)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!TransmitCommChar(m_hComm, cChar))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::TransmitChar, Failed in call to TransmitCommChar, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetConfig(_Out_ COMMCONFIG& config)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetCommConfig(m_hComm, &config, &dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetConfig, Failed in call to GetCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetConfig(_In_ COMMCONFIG& config)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetCommConfig(m_hComm, &config, dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetConfig, Failed in call to SetCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetBreak()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommBreak(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetBreak, Failed in call to SetCommBreak, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearBreak()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ClearCommBreak(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ClearBreak, Failed in call to SetCommBreak, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearError(_Out_ DWORD& dwErrors)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!ClearCommError(m_hComm, &dwErrors, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::ClearError, Failed in call to ClearCommError, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetDefaultConfig(_In_ int nPort, _Out_ COMMCONFIG& config)
{
  //Create the device name as a string
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("COM%d"), nPort);

  return GetDefaultConfig(szPort, config);
}

void CSerialPort::GetDefaultConfig(_In_z_ LPCTSTR pszPort, _Out_ COMMCONFIG& config)
{
  DWORD dwSize = sizeof(COMMCONFIG);
  if (!GetDefaultCommConfig(pszPort, &config, &dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetDefaultConfig, Failed in call to GetDefaultCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetDefaultConfig(_In_ int nPort, _In_ COMMCONFIG& config)
{
  //Create the device name as a string
  TCHAR szPort[12];
  _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("COM%d"), nPort);

  return SetDefaultConfig(szPort, config);
}

void CSerialPort::SetDefaultConfig(_In_z_ LPCTSTR pszPort, _In_ COMMCONFIG& config)
{
  DWORD dwSize = sizeof(COMMCONFIG);
  if (!SetDefaultCommConfig(pszPort, &config, dwSize))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetDefaultConfig, Failed in call to SetDefaultCommConfig, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetStatus(_Out_ COMSTAT& stat)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  DWORD dwErrors;
  if (!ClearCommError(m_hComm, &dwErrors, &stat))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetStatus, Failed in call to ClearCommError, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetState(_Out_ DCB& dcb)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommState(m_hComm, &dcb))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetState, Failed in call to GetCommState, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetState(_In_ DCB& dcb)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommState(m_hComm, &dcb))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetState, Failed in call to SetCommState, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Escape(_In_ DWORD dwFunc)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!EscapeCommFunction(m_hComm, dwFunc))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Escape, Failed in call to EscapeCommFunction, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::ClearDTR()
{
  Escape(CLRDTR);
}

void CSerialPort::ClearRTS()
{
  Escape(CLRRTS);
}

void CSerialPort::SetDTR()
{
  Escape(SETDTR);
}

void CSerialPort::SetRTS()
{
  Escape(SETRTS);
}

void CSerialPort::SetXOFF()
{
  Escape(SETXOFF);
}

void CSerialPort::SetXON()
{
  Escape(SETXON);
}

void CSerialPort::GetProperties(_Inout_ COMMPROP& properties)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommProperties(m_hComm, &properties))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetProperties, Failed in call to GetCommProperties, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetModemStatus(_Out_ DWORD& dwModemStatus)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommModemStatus(m_hComm, &dwModemStatus))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetModemStatus, Failed in call to GetCommModemStatus, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetMask(_In_ DWORD dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommMask(m_hComm, dwMask))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetMask, Failed in call to SetCommMask, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetMask(_Out_ DWORD& dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommMask(m_hComm, &dwMask))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetMask, Failed in call to GetCommMask, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Flush()
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!FlushFileBuffers(m_hComm))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Flush, Failed in call to FlushFileBuffers, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Purge(_In_ DWORD dwFlags)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!PurgeComm(m_hComm, dwFlags))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Purge, Failed in call to PurgeComm, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::TerminateOutstandingWrites()
{
  Purge(PURGE_TXABORT);
}

void CSerialPort::TerminateOutstandingReads()
{
  Purge(PURGE_RXABORT);
}

void CSerialPort::ClearWriteBuffer()
{
  Purge(PURGE_TXCLEAR);
}

void CSerialPort::ClearReadBuffer()
{
  Purge(PURGE_RXCLEAR);
}

void CSerialPort::Setup(_In_ DWORD dwInQueue, _In_ DWORD dwOutQueue)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetupComm(m_hComm, dwInQueue, dwOutQueue))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::Setup, Failed in call to SetupComm, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::SetTimeouts(_In_ COMMTIMEOUTS& timeouts)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!SetCommTimeouts(m_hComm, &timeouts))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::SetTimeouts, Failed in call to SetCommTimeouts, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::GetTimeouts(_Out_ COMMTIMEOUTS& timeouts)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!GetCommTimeouts(m_hComm, &timeouts))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::GetTimeouts, Failed in call to GetCommTimeouts, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

void CSerialPort::Set0Timeout()
{
  COMMTIMEOUTS Timeouts;
  memset(&Timeouts, 0, sizeof(Timeouts));
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0WriteTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.WriteTotalTimeoutMultiplier = 0;
  Timeouts.WriteTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::Set0ReadTimeout()
{
  COMMTIMEOUTS Timeouts;
  GetTimeouts(Timeouts);
  Timeouts.ReadIntervalTimeout = MAXDWORD;
  Timeouts.ReadTotalTimeoutMultiplier = 0;
  Timeouts.ReadTotalTimeoutConstant = 0;
  SetTimeouts(Timeouts);
}

void CSerialPort::WaitEvent(_Inout_ DWORD& dwMask)
{
  //Validate our parameters
  ATLASSERT(IsOpen());

  if (!WaitCommEvent(m_hComm, &dwMask, NULL))
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }
}

BOOL CSerialPort::WaitEvent(_Inout_ DWORD& dwMask, _Inout_ OVERLAPPED& overlapped)
{
  //Validate our parameters
  ATLASSERT(IsOpen());
  ATLASSERT(overlapped.hEvent != NULL);

  BOOL bSuccess = WaitCommEvent(m_hComm, &dwMask, &overlapped);
  if (!bSuccess)
  {
    DWORD dwLastError = GetLastError();
    ATLTRACE(_T("CSerialPort::WaitEvent, Failed in call to WaitCommEvent, Error:%u\n"), dwLastError);
    ThrowSerialException(dwLastError);
  }

  return bSuccess;
}
```

SerialPortWrapper.h

```cpp
//串口业务处理类

#pragma once

#include "CommonFunction.h"
#include "SerialPort.h"
#include <list>

//错误码定义
typedef enum ErrorCode{
		ErrorCode_Successed				=			0x0000,				//成功
		ErrorCode_CrcFail				=			0x0001,				//crc错误
		ErrorCode_ReadTimeout			=			0x0002,				//read timeout
		ErrorCode_WriteTimeout			=			0x0003,				//write timeout
};

//数据回调函数
typedef BOOL (CALLBACK* ResultStatusCallback)(unsigned short usRequireCode,int nErrorCode,unsigned char* pBuffer, unsigned int uiBufferLength, unsigned int uiUserData1, unsigned int uiUserData2);

//串口发送数据结构
typedef struct tagSerialPortDataProcess{
	unsigned short usRequireCode;		//请求业务代号
	unsigned char ucLevel;				//优先级（1-100），1 > ... > 100
	unsigned char ucCircle;				//是否循环处理
	unsigned int  uiCircleMillseconds;	//循环时间（毫秒）
	DWORD dwStartTime;					//开始时间
	unsigned char *pSendBuffer;			//发送数据缓冲区
	int   nSendBufferSize;				//发送数据缓冲区大小
	ResultStatusCallback pCallBack;		//结果回调函数
	unsigned int uiCallbackUserData1;	//结果回调函数用户数据1
	unsigned int uiCallbackUserData2;	//结果回调函数用户数据2
}SerialPortDataProcess;

class CSerialPortWrapper
{
public:
	CSerialPortWrapper(void);
	virtual ~CSerialPortWrapper(void);

	//插入业务数据
	void InsertBusinessSerialPortData(SerialPortDataProcess spdpSerialPortDataProcess);

	//开始工作
	BOOL StartWork(LPCTSTR pszPort,DWORD dwBaud = 9600,CSerialPort::Parity parity = CSerialPort::NoParity,BYTE DataBits = 8, CSerialPort::StopBits stopBits = CSerialPort::OneStopBit,CSerialPort::FlowControl fc = CSerialPort::NoFlowControl, BOOL bOverlapped = FALSE);

	//停止工作
	BOOL StopWork();

	//暂停工作(bPauseRunning为TRUE时，交出线程控制权)
	BOOL PauseWork(BOOL bPauseRunning);

	//直接写数据
	DWORD Write(unsigned char *pBuffer,int nBufferSize);

	//直接读数据
	DWORD Read(unsigned char *pBuffer,int nBufferSize);
	

	//串口数据处理线程
	static unsigned int WINAPI CommonDataProcessThreadProc(LPVOID pv);
	unsigned int CommonDataProcessThreadProcInner(LPVOID pv);

protected:
	BOOL IsPauseWork();
	BOOL IsOpenCommFail();

	CSerialPort m_spSerialPort;
	BOOL   m_bOpenCommFail;
	HANDLE m_hThreadComm;
	BOOL   m_bRunning;
	BOOL   m_bPauseRunning;
	BOOL   m_bPauseRunningAck;
	std::list<SerialPortDataProcess> m_listSerialPortData;
	CRITICAL_SECTION m_cs;
};
```

SerialPortWrapper.cpp

```cpp
#include "StdAfx.h"
#include "SerialPortWrapper.h"
#include <process.h>

CSerialPortWrapper::CSerialPortWrapper(void)
{
	m_hThreadComm = NULL;
	m_bRunning = FALSE;
	m_bPauseRunning = FALSE;
	m_bPauseRunningAck = FALSE;
	m_bOpenCommFail = FALSE;
	InitializeCriticalSection(&m_cs);
}

CSerialPortWrapper::~CSerialPortWrapper(void)
{
	DeleteCriticalSection(&m_cs);
}

//插入业务数据
void CSerialPortWrapper::InsertBusinessSerialPortData(SerialPortDataProcess spdpSerialPortDataProcess)
{
	EnterCriticalSection(&m_cs);
	m_listSerialPortData.push_back(spdpSerialPortDataProcess);
	LeaveCriticalSection(&m_cs);
}

//开始工作
BOOL CSerialPortWrapper::StartWork(LPCTSTR pszPort,DWORD dwBaud,CSerialPort::Parity parity,BYTE DataBits, CSerialPort::StopBits stopBits,CSerialPort::FlowControl fc, BOOL bOverlapped)
{
	try
	{
		m_spSerialPort.Open(pszPort,dwBaud,parity,DataBits,stopBits,fc,bOverlapped);
		COMMTIMEOUTS timeouts = {0};
		m_spSerialPort.GetTimeouts(timeouts);
		timeouts.ReadTotalTimeoutConstant = 100;
		timeouts.WriteTotalTimeoutConstant = 100;
		m_spSerialPort.SetTimeouts(timeouts);
	}
	catch (...)
	{
		m_bOpenCommFail = TRUE;
		return FALSE;
	}

	//开启线程
	m_bRunning = TRUE;
	m_bPauseRunning = FALSE;
	m_bPauseRunningAck = FALSE;
	m_hThreadComm = (HANDLE)_beginthreadex(NULL, 0, CommonDataProcessThreadProc, this, 0, NULL);

	return TRUE;
}

//停止工作
BOOL CSerialPortWrapper::StopWork()
{
	m_bRunning = FALSE;
	m_bPauseRunning = FALSE;
	m_bPauseRunningAck = FALSE;
	if(m_hThreadComm != NULL)
	{
		WaitForSingleObject(m_hThreadComm,1000);
		CloseHandle(m_hThreadComm);
		m_hThreadComm = NULL;
	}
	//释放空间
	std::list<SerialPortDataProcess>::iterator iter,iterEnd;
	iter = m_listSerialPortData.begin();
	iterEnd = m_listSerialPortData.end();
	for(iter; iter!=iterEnd; iter++)
	{
		if(iter->pSendBuffer != NULL)
		{
			delete [] iter->pSendBuffer;
			iter->pSendBuffer = NULL;
		}
	}
	m_listSerialPortData.clear();
	if(!m_bOpenCommFail)
	{
		m_spSerialPort.ClearWriteBuffer();
		m_spSerialPort.ClearReadBuffer();
		m_spSerialPort.Flush();
		m_spSerialPort.CancelIo();
		m_spSerialPort.Close();
	}

	return TRUE;
}

//暂停工作
BOOL CSerialPortWrapper::PauseWork(BOOL bPauseRunning)
{
	m_bPauseRunning = bPauseRunning;
	return TRUE;
}

BOOL CSerialPortWrapper::IsPauseWork()
{
	return m_bPauseRunning;
}

BOOL CSerialPortWrapper::IsOpenCommFail()
{
	return m_bOpenCommFail;
}

//直接写数据
DWORD CSerialPortWrapper::Write(unsigned char *pBuffer,int nBufferSize)
{
	DWORD dRet = 0;
	if(!IsPauseWork())
	{
		//暂停工作
		PauseWork(TRUE);
		while(true)
		{
			if(!m_bRunning)
				break;

			if(m_bPauseRunningAck)
				break;

			Sleep(100);
		}

		 dRet = m_spSerialPort.Write(pBuffer,nBufferSize);

		//恢复工作
		PauseWork(FALSE);
	}
	else
	{
		 dRet = m_spSerialPort.Write(pBuffer,nBufferSize);
	}

	return dRet;
}

//直接读数据
DWORD CSerialPortWrapper::Read(unsigned char *pBuffer,int nBufferSize)
{
	DWORD dRet = 0;
	if(!IsPauseWork())
	{
		//暂停工作
		PauseWork(TRUE);
		while(true)
		{
			if(!m_bRunning)
				break;

			if(m_bPauseRunningAck)
				break;

			Sleep(100);
		}

		dRet = m_spSerialPort.Read(pBuffer,nBufferSize);

		//恢复工作
		PauseWork(FALSE);
	}
	else
	{
		dRet = m_spSerialPort.Read(pBuffer,nBufferSize);
	}

	return dRet;
}

//串口数据处理线程
unsigned int CSerialPortWrapper::CommonDataProcessThreadProc(LPVOID pv)
{
	CSerialPortWrapper *pSerialPortWrapper = (CSerialPortWrapper *)pv;
	pSerialPortWrapper->CommonDataProcessThreadProcInner(pv);

	return 0;
}
unsigned int CSerialPortWrapper::CommonDataProcessThreadProcInner(LPVOID pv)
{
	BYTE ucOutputBuffer[1024] = {0};
	int  nOutputBufferSize = 1024;
	DWORD dwWritedByte = 0;
	int  nWriteDataWaitTimeout = 50;					//写数据等待超时时间
	DWORD dwReadedByte = 0,dwTotalReadedByte = 0;
	int nTimes = 0;
	while(true)
	{
		if(!m_bRunning)
			break;

		if(m_bPauseRunning)
		{
			Sleep(500);
			m_bPauseRunningAck = TRUE;
			continue;
		}

		m_bPauseRunningAck = FALSE;
		EnterCriticalSection(&m_cs);
		//优先级、超时时间 在后续版本处理，本版本暂时使用普通处理
		std::list<SerialPortDataProcess>::iterator iter,iterEnd;
		iter = m_listSerialPortData.begin();
		iterEnd = m_listSerialPortData.end();
		for(iter; iter!=iterEnd; iter++)
		{
			dwWritedByte = m_spSerialPort.Write(iter->pSendBuffer,iter->nSendBufferSize);
			Sleep(nWriteDataWaitTimeout);
			//获取数据
			dwReadedByte = 0;
			dwTotalReadedByte = 0;
			nTimes = 0;
			while(true)
			{
				dwReadedByte = m_spSerialPort.Read(ucOutputBuffer+dwTotalReadedByte,nOutputBufferSize);
				dwTotalReadedByte = dwReadedByte;
				nTimes++;
				if(dwReadedByte > 4)
				{
					bool bValid = true;
					//校验crc
					unsigned short usCrcTmp = CRC16(ucOutputBuffer,dwTotalReadedByte-2);
					if(!(GetShortHighByte(usCrcTmp) == ucOutputBuffer[dwTotalReadedByte-2] && GetShortLowByte(usCrcTmp) == ucOutputBuffer[dwTotalReadedByte-1]))
					{
						//crc不合法
						bValid = false;
					}

					//回调数据
					if(iter->pCallBack != NULL)
					{
						if(bValid)
						{
							iter->pCallBack(iter->usRequireCode,ErrorCode_Successed,ucOutputBuffer,dwTotalReadedByte,iter->uiCallbackUserData1,iter->uiCallbackUserData2);
						}
						else
						{
							iter->pCallBack(iter->usRequireCode,ErrorCode_CrcFail,ucOutputBuffer,dwTotalReadedByte,iter->uiCallbackUserData1,iter->uiCallbackUserData2);
						}
					}	
					break;
				}
				else
				{
					if(nTimes > 3)
					{
						//回调数据
						if(iter->pCallBack != NULL)
						{
							iter->pCallBack(iter->usRequireCode,ErrorCode_ReadTimeout,ucOutputBuffer,dwTotalReadedByte,iter->uiCallbackUserData1,iter->uiCallbackUserData2);
						}
						break;
					}

				}
			}
		}

		//删除非循环请求数据
		iter = m_listSerialPortData.begin();
		while(iter != m_listSerialPortData.end())
		{
			if(iter->ucCircle == 0)
			{
				if(iter->pSendBuffer != NULL)
				{
					delete [] iter->pSendBuffer;
					iter->pSendBuffer = NULL;
				}
				iter = m_listSerialPortData.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		LeaveCriticalSection(&m_cs);

		Sleep(200);
	}
	return 0;
}
```

//调用代码

```cpp
//头文件
CSerialPortWrapper m_spwSerialPortWrapper;
//结果数据回调函数
static  BOOL WINAPI CommonRequrieResultStatusCallback(unsigned short usRequireCode,int nErrorCode,unsigned char* pBuffer, unsigned int uiBufferLength, unsigned int uiUserData1, unsigned int uiUserData2);
BOOL CommonRequrieResultStatusCallbackInner(unsigned short usRequireCode,int nErrorCode,unsigned char* pBuffer, unsigned int uiBufferLength, unsigned int uiUserData1, unsigned int uiUserData2);

//实现文件
//开始工作
m_spwSerialPortWrapper.StartWork("COM3",9600,CSerialPort::EvenParity,8,CSerialPort::OneStopBit);

//停止工作
m_spwSerialPortWrapper.StopWork();

//发送指令
	//比如：获取0寄存器地址，10个通道的数据
	BYTE ucInputBuffer[128] = {0};
	int  nInputBufferSize = 0;
	ucInputBuffer[0] = 0x01;
	ucInputBuffer[1] = 0x03;
	nInputBufferSize +=2;
	int nRegisterAddr = 0;
	int nRegisterNum = 10;
	ucInputBuffer[nInputBufferSize] = GetShortHighByte(nRegisterAddr);
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = GetShortLowByte(nRegisterAddr);
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = GetShortHighByte(nRegisterNum);
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = GetShortLowByte(nRegisterNum);
	nInputBufferSize++;
	unsigned short usCrc = CRC16(ucInputBuffer,nInputBufferSize);
	ucInputBuffer[nInputBufferSize] = GetShortHighByte(usCrc);
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = GetShortLowByte(usCrc);
	nInputBufferSize++;

	SerialPortDataProcess spdpSerialPortDataProcess = {0};
	spdpSerialPortDataProcess.usRequireCode = 1;
	spdpSerialPortDataProcess.ucCircle = 1;
	spdpSerialPortDataProcess.nSendBufferSize = nInputBufferSize;
	spdpSerialPortDataProcess.pSendBuffer = new unsigned char[nInputBufferSize];
	spdpSerialPortDataProcess.pCallBack = CommonRequrieResultStatusCallback;
	spdpSerialPortDataProcess.uiCallbackUserData1 = (int)this;
	spdpSerialPortDataProcess.uiCallbackUserData2 = 0;
	memcpy(spdpSerialPortDataProcess.pSendBuffer,ucInputBuffer,nInputBufferSize);
	m_spwSerialPortWrapper.InsertBusinessSerialPortData(spdpSerialPortDataProcess);
	
	//写数据
	//比如：将5寄存器地址的值修改成99
	BYTE ucInputBuffer[128] = {0};
	int  nInputBufferSize = 0;
	BYTE ucOutputBuffer[256] = {0};
	int  nOutputBufferSize = 256;
	ucInputBuffer[0] = 0x01;
	ucInputBuffer[1] = 0x06;
	nInputBufferSize +=2;
	int nRegisterAddr = 5;
	int nRegisterValues = 99;
	ucInputBuffer[nInputBufferSize] = (nRegisterAddr & 0xff00) >> 8;
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = nRegisterAddr & 0x00ff;
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = (nRegisterValues & 0xff00) >> 8;
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = nRegisterValues & 0x00ff;
	nInputBufferSize++;
	unsigned short usCrc = CRC16(ucInputBuffer,nInputBufferSize);
	ucInputBuffer[nInputBufferSize] = (usCrc & 0xff00) >> 8;
	nInputBufferSize++;
	ucInputBuffer[nInputBufferSize] = usCrc & 0x00ff;
	nInputBufferSize++;
	DWORD dwWritedByte = m_spwSerialPortWrapper.Write(ucInputBuffer,nInputBufferSize);
	
	
	
	//结果数据回调函数
BOOL CModbusTestDlg::CommonRequrieResultStatusCallback(unsigned short usRequireCode,int nErrorCode,unsigned char* pBuffer, unsigned int uiBufferLength, unsigned int uiUserData1, unsigned int uiUserData2)
{
	CModbusTestDlg *pDlg = (CModbusTestDlg *)uiUserData1;
	if(pDlg != NULL)
	{
		pDlg->CommonRequrieResultStatusCallbackInner(usRequireCode,nErrorCode,pBuffer,uiBufferLength,uiUserData1,uiUserData2);
	}

	return TRUE;
}
BOOL CModbusTestDlg::CommonRequrieResultStatusCallbackInner(unsigned short usRequireCode,int nErrorCode,unsigned char* pBuffer, unsigned int uiBufferLength, unsigned int uiUserData1, unsigned int uiUserData2)
{
	if(nErrorCode == ErrorCode_Successed)
	{
		if(uiBufferLength > 0)
		{
			//显示数据
			CString strTmp = "";
			char szHex[3] = {0};
			for(int n=0;n<uiBufferLength;n++)
			{
				sprintf(szHex,"%02X",pBuffer[n]);
				strTmp +=szHex;
				if(n != uiBufferLength-1)
				{
					strTmp +=" ";
				}
			}
			TRACE(strTmp);
			TRACE("\n\r");
		}
	}

	return TRUE;
}
```

[代码下载](http://download.csdn.net/download/byxdaz/9982915)


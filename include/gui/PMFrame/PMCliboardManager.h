//********************************************************************************
//文件：PMCliboardManager.h
//作者：feiren
//时间：2011-8-20
//用途：剪贴板控制器，封装基本剪贴板操作
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMCLIBOARDMANAGER_H__
#define __PMCLIBOARDMANAGER_H__
#include "PMSpaceAlgorithm.h"
#include "ximage.h"

#pragma once

class PMCliboardManager
{
public:
	PMCliboardManager(void);
	~PMCliboardManager(void);
	static PMCliboardManager* GetSingleton();
	PMVoid Initialize(HWND hWnd);
	PMBool IsGRValid();
	PMBool IsEDValid();	
	PMVoid CLEAR();
	
	PMVoid SetText		(const string &data);
	PMVoid SetImage		(TBuffer &buffer);
	PMVoid SetImage		(HANDLE hDIB);
	PMVoid SetGrid		(TBuffer &buffer);
	PMVoid SetEditor	(TBuffer &buffer);
	PMVoid Copy();

	PMBool GetText		(string &data);
	PMBool GetImage		(TBuffer &buffer);
	PMBool GetImage		(CxImage *&image);
	PMBool GetImage		(TVecStrings &imagePaths);
	PMBool GetGrid		(TBuffer &buffer);
	PMBool GetEditor	(TBuffer &buffer);
private:
	PMVoid Free();
	HWND mHWnd;
	PMChar *mImage;
	PMInt   mImageSize;
	HANDLE  mHDIB;
	PMChar *mGrid;
	PMInt   mGridSize;
	PMChar *mEditor;
	PMInt   mEditorSize;
	string  mText;
	
	PMUnint mCF_Image;
	PMUnint mCF_Grid;
	PMUnint mCF_Editor;

	PMInt   mMax_Text_Size;
	TStringSpliter   mText_Extends;
	TStringSpliter   mPicture_Extends;
};

#endif
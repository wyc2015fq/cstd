//********************************************************************************
//文件：PMScreenGDI.h
//作者：feiren
//时间：2011-8-20
//用途：封装基本的GDI显示方法、lafer算法、全局枚举对象
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMPICTUREMANAGER_H__
#define __PMPICTUREMANAGER_H__

#include "PMScreenGDI.h"
#pragma once

struct PMImage 
{
	string mName;
	PMInt  mWidth;
	PMInt  mHeight;
	CxImage* mImage;
	PMVoid PushJson(TJSON &jnFather);
};
struct PMImages 
{
	vector<PMImage> mImages;
	PMVoid PushJson(TJSON &jnFather);
};
struct GifData
{
	GifData(){Clear();}	
	GifData(PMChar *MD5,IntRect &Rect):mTicc(0),mDelay(0),mImage(NULL),mbVisible(false){mMD5 = MD5;mRect = Rect;}	
	PMBool	operator == (GifData const& obj)const{return mRect==obj.mRect&&(mMD5==obj.mMD5||0==strcmp(mMD5,obj.mMD5));}
	bool operator <(GifData const& obj)const
	{
		PMInt Result = strcmp(mMD5,obj.mMD5);
		if(Result<0)return true;
		if(0==Result)
		{
			if(mRect.left<obj.mRect.left)return true;
			if(mRect.left==obj.mRect.left)
			{
				if(mRect.top<obj.mRect.top)return true;
				if(mRect.top==obj.mRect.top)
				{
					if(mRect.right<obj.mRect.right)return true;
					if(mRect.right==obj.mRect.right)return mRect.bottom<obj.mRect.bottom;
				}
			}
		}
		return false;
	}
	PMVoid    Clear()
	{
		mTicc  = 0;
		mDelay = 0;	
		mIndex = 0;
		mCount = 0;
		mMD5   = NULL;
		mbVisible = false;
		mState = 0;
		mbIcon = false;
		mRect.clear()  ;
		mImage = NULL;
	}
	PMInt     mTicc;
	PMInt     mDelay;	
	PMInt	  mIndex;
	PMInt	  mCount;
	PMChar	* mMD5;
	PMBool	  mbVisible;
	PMInt	  mState;
	PMBool	  mbIcon;
	IntRect   mRect;
	CxImage * mImage;
};
typedef map<string,CxImage*> CxImages;
typedef map<PMInt,vector<GifData>> PMGifs;
class PMPictureManager
{
public:
	PMPictureManager(void);
	virtual ~PMPictureManager(void);
	static PMPictureManager *GetSingleton();
	PMVoid	SetJson			(TJSON *jnPictures);
	TJSON  *GetImageData	(const string &MD5);
	CxImage *At				(const string &MD5);
	PMVoid	Release();
	PMInt	RegisterGifImage(PMChar *MD5,PMBool bIcon = false);		
	PMVoid	Draw			(HDC hdc,PMInt ID,IntRect &rect,PMInt state=0);
	PMVoid	DrawCenter		(HDC hdc,PMInt ID,IntRect &rect,PMInt state=0);
	PMVoid  Draw			(HDC hdc,const string &MD5,IntRect &rect,PMInt state=0);
	PMVoid  DrawCenter		(HDC hdc,const string &MD5,IntRect &rect,PMInt state=0);
	PMBool	TimerEvent		(PMInt nGifGroup,HDC hdc);	
	PMVoid	SetGifGroup		(PMInt nGifGroup,PMBool bReset = true);
	PMVoid	DeleteGifGroup	(PMInt nGifGroup);
	PMInt	CreateImageRoup	();	
	PMVoid	DrawEditImage	(HDC hdc);
	PMVoid  DrawImageFrame	(HDC pdc, RECT &ImageBound);
	PMVoid	AddPicture		(TJSON &jnPictures);
	PMVoid	AddPicture		(TVecStrings &imagePaths,PMImages &images);
	string	AddPicture		(CxImage *image);
	string  AddPicture		(TBuffer &buffer);
	TBuffer GetData			(const string &name);
private:	
	RECT	GetCenterRect	(IntRect &rect,PMInt pWidth,PMInt pHeight);		
	CxImages mImages;
	PMGifs   mGifImages;
	PMInt	 mCurrentGifGroup;
	TJSON   *mJnPictures;
	GifData  mEditImage;
	PMInt	 mImageGroupCount;
};

#endif
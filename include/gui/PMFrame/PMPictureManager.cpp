#include "StdAfx.h"
#include "PMPictureManager.h"

#define SELECTDARKNESS -50

PMPictureManager g_PictureManager;


PMPictureManager::PMPictureManager(void):mImageGroupCount(0),mJnPictures(NULL),mCurrentGifGroup(0)
{
}

PMPictureManager::~PMPictureManager(void)
{
	Release();
}

PMPictureManager * PMPictureManager::GetSingleton()
{
	return &g_PictureManager;
}

CxImage * PMPictureManager::At( const string &MD5 )
{
	CxImages::iterator it = mImages.find(MD5);
	if(it!=mImages.end())return it->second;
	TJSON* jnImage = GetImageData(MD5);
	if(jnImage)
	{
		//json格式："MD5":[0,x.x.x.x..x.x..x.x.dsa]
		TBuffer imageBuffer = jnImage->At(1).ToBinary();
		CxImage *NewImage   = new CxImage;
		NewImage->SetRetreiveAllFrames(true);
		NewImage->Decode((BYTE*)imageBuffer.buffer,imageBuffer.useLen,jnImage->At(0).ToInt());
		mImages[MD5] = NewImage;	
		return NewImage;
	}
	return NULL;
}

TJSON  * PMPictureManager::GetImageData( const string &MD5 )
{
	TJSON* jnImage = mJnPictures->Find(MD5);
	return jnImage;
}

PMVoid PMPictureManager::SetJson( TJSON *jnPictures )
{
	mJnPictures = jnPictures;
}

PMVoid PMPictureManager::Release()
{
	for (CxImages::iterator it = mImages.begin() ; it != mImages.end(); it++)
	{
		delete it->second;
	}
	mImages.clear();
}

PMVoid PMPictureManager::DrawCenter( HDC hdc,const string &MD5,IntRect &rect,PMInt state )
{
	CxImage *image = At(MD5);
	if(image)
	{
		RECT nRect = GetCenterRect(rect,image->GetWidth(),image->GetHeight());
		if(state==0) //普通状态
		{
			image->Draw(hdc,nRect);
		}
		else if(state==1)//选中状态
		{
			CxImage DarknessImage;
			DarknessImage.Copy(*image);
			DarknessImage.Light(0,SELECTDARKNESS);
			DarknessImage.Draw(hdc,nRect);				
		}else//编辑状态
		{			
			mEditImage.mImage = image;
			mEditImage.mRect  = nRect;
		}
	}		
}

PMVoid PMPictureManager::DrawCenter( HDC hdc,PMInt ID,IntRect &rect,PMInt state )
{
	GifData &gif   = mGifImages[mCurrentGifGroup][ID];
	
	gif.mRect = rect;
	CxImage *image = gif.mImage->GetFrame(gif.mIndex);
	RECT nRect = GetCenterRect(rect,gif.mImage->GetWidth(),gif.mImage->GetHeight());
	if(state==0) //普通状态
	{
		image->Draw(hdc,nRect);
	}
	else if(state==1)   //选中状态
	{
		CxImage DarknessImage;
		DarknessImage.Copy(*image);
		DarknessImage.Light(0,SELECTDARKNESS);
		DarknessImage.Draw(hdc,nRect);	
	}
	else//编辑状态
	{
		mEditImage.mImage = image;
		mEditImage.mRect  = nRect;
	}
	gif.mbVisible  = true;
	gif.mState	   = state;
}

PMVoid PMPictureManager::Draw( HDC hdc,const string &MD5,IntRect &rect,PMInt state )
{
	CxImage *image = At(MD5);
	if(image)
	{		
		RECT nRect = rect.ToRECT();
		if(state==0) //普通状态
		{
			image->Draw(hdc,nRect);
		}
		else  if(state==1) //选中状态
		{
			CxImage DarknessImage;
			DarknessImage.Copy(*image);
			DarknessImage.Light(0,SELECTDARKNESS);
			DarknessImage.Draw(hdc,nRect);	
		}else//编辑状态
		{
			mEditImage.mImage = image;
			mEditImage.mRect  = rect;
		}
	}	
}

PMVoid PMPictureManager::Draw( HDC hdc,PMInt ID ,IntRect &rect,PMInt state)
{
	GifData &gif  = mGifImages[mCurrentGifGroup][ID];
	gif.mRect = rect;
	CxImage *image = gif.mImage->GetFrame(gif.mIndex);
	if(state==0) //普通状态
	{
		image->Draw(hdc,rect.ToRECT());
	}
	else if(state==1)  //选中状态
	{
		CxImage DarknessImage;
		DarknessImage.Copy(*image);
		DarknessImage.Light(0,SELECTDARKNESS);
		DarknessImage.Draw(hdc,rect.ToRECT());	
	}
	else//编辑状态
	{
		mEditImage.mImage = image;
		mEditImage.mRect  = rect;
		//image->Draw(hdc,rect.ToRECT());
	}
	gif.mbVisible  = true;
	gif.mState	   = state;
}

PMVoid PMPictureManager::SetGifGroup( PMInt nGifGroup ,PMBool bReset)
{
	mCurrentGifGroup = nGifGroup;
	//将所有GIF图片初始化为不可见
	if(bReset)
	{
		PMGifs::iterator it = mGifImages.find(nGifGroup);
		if(it != mGifImages.end())
		{
			vector<GifData> &gifs = it->second;
			for(vector<GifData>::iterator iter = gifs.begin(); iter != gifs.end(); iter++)
			{
				iter->mbVisible = false;
			}
		}
	}
}

PMVoid PMPictureManager::DeleteGifGroup( PMInt nGifGroup )
{
	mGifImages.erase(nGifGroup);
}

PMBool PMPictureManager::TimerEvent( PMInt nGifGroup,HDC hdc)
{	
	PMBool Result = false;
	static PMInt LastTicc = GetTickCount(),CurrentTicc;
	CurrentTicc  = GetTickCount() - LastTicc;
	if(CurrentTicc>0)
	{		
		map<PMInt,vector<GifData>>::iterator it = mGifImages.find(nGifGroup);
		if(it != mGifImages.end())
		{
			vector<GifData>::iterator iter = it->second.begin();
			vector<GifData>::iterator iterEnd = it->second.end();
			GifData *imageData;
			CxImage *image;
			for (;iter!=iterEnd;iter++)
			{
				imageData = &(*iter);
				imageData->mTicc += CurrentTicc;
				if(imageData->mTicc > imageData->mDelay)
				{
					imageData->mIndex++;
					if (imageData->mIndex>=imageData->mCount) imageData->mIndex=0;
					imageData->mTicc = 0;
					image = imageData->mImage->GetFrame(imageData->mIndex);
					imageData->mDelay= max(1,image->GetFrameDelay())*10;
					if(imageData->mbVisible)
					{
						RECT   rect;	
						if(imageData->mbIcon)
						{
							rect = GetCenterRect(imageData->mRect,imageData->mImage->GetWidth(),imageData->mImage->GetHeight());
						}else
						{
							rect = imageData->mRect.ToRECT();					
						}
						if(imageData->mState==0)//普通状态
						{
							image->Draw(hdc,rect);
						}
						else if(imageData->mState==1)//选中状态 
						{
							CxImage DarknessImage;
							DarknessImage.Copy(*image);
							DarknessImage.Light(0,SELECTDARKNESS);
							DarknessImage.Draw(hdc,rect);							
						}
						else//编辑状态
						{
							mEditImage.mImage = image;
							mEditImage.mRect  = rect;							
						}
						Result = true;
					}
				}
			}
			DrawEditImage(hdc);
		}
		LastTicc = GetTickCount();
	}
	return Result;
}

PMInt PMPictureManager::CreateImageRoup()
{
	mImageGroupCount++;
	return mImageGroupCount;
}

PMInt PMPictureManager::RegisterGifImage( PMChar *MD5,PMBool bIcon )
{
	PMBool bGif = false;	
	CxImages::iterator it = mImages.find(MD5);
	if(it!=mImages.end()&&it->second->GetType()==CXIMAGE_FORMAT_GIF)
	{
		bGif = true;		
	}else
	{
		TJSON* jnImage = GetImageData(MD5);
		if(jnImage&&jnImage->At(0).ToInt() == CXIMAGE_FORMAT_GIF)
		{
			bGif = true;
		}
	}
	if(bGif)
	{
		CxImage *image = At(MD5);
		if(image->GetNumFrames()>1)
		{
			GifData data;
			vector<GifData> &gifs = mGifImages[mCurrentGifGroup];
			data.mMD5   = MD5;		
			data.mbIcon = bIcon;
			data.mCount = image->GetNumFrames();
			data.mIndex = 0;
			data.mImage = image;
			data.mTicc  = 0;		
			data.mDelay = image->GetFrame(0)->GetFrameDelay();
			data.mbVisible = false;
			gifs.push_back(data);
			return gifs.size()-1;
		}
	}
	return -1;
}

RECT PMPictureManager::GetCenterRect( IntRect &rect,PMInt pWidth,PMInt pHeight )
{
	PMInt rWidth  = rect.width();
	PMInt rHeight = rect.height();
	RECT  dRect;
	PMBool bCalcWidth = true;
	PMBool bCalcHeight= true;
	//图标显示，需要按比例计算新的宽高及位置
	if(rWidth<=pWidth && rHeight<=pHeight)
	{
		PMDouble scale  = (PMDouble)pWidth/pHeight;
		PMInt    nWidth = (PMInt)(rHeight*scale);
		if(nWidth < rWidth)
		{
			dRect.top     = rect.top;
			dRect.bottom  = rect.bottom;
			dRect.left    = rect.left  + (rWidth - nWidth)/2;
			dRect.right   = rect.right + nWidth;
		}else
		{
			dRect.left    = rect.left;
			dRect.right   = rect.right;
			nWidth		  = (PMInt)(rWidth / scale);
			dRect.top     = rect.top  + (rHeight - nWidth)/2;
			dRect.bottom  = dRect.top + nWidth;
		}
		bCalcWidth	  = false;
		bCalcHeight	  = false;
	}else if(rWidth<=pWidth)
	{
		dRect.left    = rect.left;
		dRect.right   = rect.right;
		pHeight		  = ((PMInt)((PMDouble)pHeight/pWidth*rWidth));
		bCalcWidth	  = false;
	}else if(rHeight<=pHeight)
	{
		dRect.top     = rect.top;
		dRect.bottom  = rect.bottom;
		pWidth		  = ((PMInt)((PMDouble)pWidth/pHeight*rHeight));
		bCalcHeight	  = false;
	}
	
	if(bCalcWidth)
	{
		dRect.left    = rect.left  + (rWidth - pWidth)/2;
		dRect.right   = dRect.left + pWidth;
	}	
	if(bCalcHeight)
	{
		dRect.top     = rect.top  + (rHeight - pHeight)/2;
		dRect.bottom  = dRect.top + pHeight;
	}
	return dRect;
}

PMVoid PMPictureManager::DrawImageFrame(HDC pdc, RECT &ImageBound )
{
	const PMInt RADIUS  = 4;
	const PMInt ERADIUS = 5;
	HPEN   hPen		= CreatePen(0,1,RGB(77,115,153));
	HBRUSH hBr		= CreateSolidBrush(RGB(209,248,250));
	HBRUSH hNullBr  =(HBRUSH)GetStockObject(NULL_BRUSH);
	HPEN   hoPen	=(HPEN)  SelectObject(pdc,hPen);
	HBRUSH hoBr		=(HBRUSH)SelectObject(pdc,hNullBr);
	Rectangle(pdc,ImageBound.left,ImageBound.top,ImageBound.right,ImageBound.bottom);
	SelectObject(pdc,hBr);
	Ellipse(pdc,ImageBound.left-ERADIUS,ImageBound.top-ERADIUS,ImageBound.left+ERADIUS,ImageBound.top+ERADIUS);
	Ellipse(pdc,ImageBound.right-ERADIUS,ImageBound.top-ERADIUS,ImageBound.right+ERADIUS,ImageBound.top+ERADIUS);
	Ellipse(pdc,ImageBound.left-ERADIUS,ImageBound.bottom-ERADIUS,ImageBound.left+ERADIUS,ImageBound.bottom+ERADIUS);
	Ellipse(pdc,ImageBound.right-ERADIUS,ImageBound.bottom-ERADIUS,ImageBound.right+ERADIUS,ImageBound.bottom+ERADIUS);
	PMInt THCent = (ImageBound.left + ImageBound.right)/2;
	PMInt BHCent = (ImageBound.top  + ImageBound.bottom)/2;
	Rectangle(pdc,THCent-RADIUS,ImageBound.top-RADIUS,THCent+RADIUS,ImageBound.top+RADIUS);
	Rectangle(pdc,THCent-RADIUS,ImageBound.bottom-RADIUS,THCent+RADIUS,ImageBound.bottom+RADIUS);
	Rectangle(pdc,ImageBound.left-RADIUS,BHCent-RADIUS,ImageBound.left+RADIUS,BHCent+RADIUS);
	Rectangle(pdc,ImageBound.right-RADIUS,BHCent-RADIUS,ImageBound.right+RADIUS,BHCent+RADIUS);

	SelectObject(pdc,hoPen);
	SelectObject(pdc,hBr);
	DeleteObject(hPen);
	DeleteObject(hBr);
}

PMVoid PMPictureManager::DrawEditImage( HDC hdc )
{
	if(mEditImage.mImage)
	{
		RECT   rect = mEditImage.mRect.ToRECT();	;				
		mEditImage.mImage->Draw(hdc,rect);	
		DrawImageFrame(hdc,rect);
		mEditImage.Clear();
	}	
}

PMVoid PMPictureManager::AddPicture( TJSON &jnPictures )
{
	TJSON* jnImage;
	string name;
	set<string> names;
	for (TJSON::iterator it = jnPictures.begin(); it != jnPictures.end(); it++)
	{
		jnImage = &(*it);
		name = jnImage->GetName();
		if (!mJnPictures->Find(name))
		{
			mJnPictures->Insert(name,jnImage);
			names.insert(name);			
		}
	}
	for (set<string>::iterator iter = names.begin(); iter != names.end(); iter++)
	{
		jnPictures.DeleteItem(*iter,false);
	}
}

string PMPictureManager::AddPicture( CxImage *image )
{
	PMByte *data = NULL;
	PMInt   size = 0;	
	string  name;
	TMD5     expMD5;
	image->Encode(data,size,CXIMAGE_FORMAT_PNG);
	name =expMD5.MD5Buffer((PMChar*)data,size);
	if (mJnPictures->Find(name))
	{
		free(data);
		delete image;
	}else
	{
		TBuffer buffer;
		buffer.buffer  = (PMChar*)data;
		buffer.size    = size;
		buffer.useLen  = size;
		TJSON &jnImage = (*mJnPictures)[name];
		jnImage[0]     = CXIMAGE_FORMAT_PNG;
		jnImage.Push(buffer);
		mImages[name]  = image;
	}
	return name;
}

string PMPictureManager::AddPicture( TBuffer &buffer )
{
	TJSON jnPicture ;
	jnPicture.FromString(buffer.buffer);
	string name = jnPicture.begin().GetName();
	if (mJnPictures->Find(name))
	{
		jnPicture.Clear();
	}else
	{
		mJnPictures->Insert(name,&(*jnPicture.begin()));
		jnPicture.RemoveAll();
		jnPicture.Clear();	
	}
	buffer.Clear();
	return name;
}

PMVoid PMPictureManager::AddPicture( TVecStrings &imagePaths,PMImages &images )
{
	PMImage image;
	TMD5    expMD5;
	CxImage *img;
	for (TVecStrings::iterator it = imagePaths.begin(); it != imagePaths.end(); it++)
	{
		image.mName = expMD5.MD5File(it->c_str());
		if(!mJnPictures->Find(image.mName))//找到
		{
			TBuffer buffer;
			string  fileEx		  = TFile::GetFileEx(it->c_str());
			PMUnint imageType	  = CxImage::GetTypeIdFromName(fileEx.c_str());
			buffer.Read(it->c_str());
			img = new CxImage;
			img->SetRetreiveAllFrames(true);
			img->Decode((BYTE*)buffer.buffer,buffer.useLen,imageType);
			TJSON &jnImage		  = (*mJnPictures)[image.mName];
			jnImage[0]			  = imageType;
			jnImage.Push(buffer);
			mImages[image.mName]  = img;
		}else
		{
			img = At(image.mName);
		}
		image.mWidth  = img->GetWidth();
		image.mHeight = img->GetHeight();
		image.mImage  = img;
		images.mImages.push_back(image);
	}
}

TBuffer PMPictureManager::GetData( const string &name )
{
	TJSON *json = mJnPictures->Find(name);
	if(json)
	{
		TJSON jnPicture;
		jnPicture.Insert(name,json);
		TBuffer buffer = jnPicture.Print(false);
		jnPicture.RemoveAll();
		jnPicture.Clear();
		return buffer;
	}
	return TBuffer();
}


PMVoid PMImage::PushJson( TJSON &jnFather )
{
	TJSON &jnImage = jnFather.Push();
	jnImage.Push(mName);
	jnImage.Push(mWidth);
	jnImage.Push(mHeight);
}

PMVoid PMImages::PushJson( TJSON &jnFather )
{
	for (vector<PMImage>::iterator it = mImages.begin(); it != mImages.end(); it++)
	{
		it->PushJson(jnFather);
	}
}

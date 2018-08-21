#include "StdAfx.h"
#include "PMEdit.h"


PMInt PMEdit::GetContentJsonIndex( TJSON &jnContent )
{
	PMInt index = jnContent.GetChildCount() - 1;
	TJSON *jnItem;
	PMInt  type;
	while (index>=0)
	{
		jnItem = &jnContent.At(index);
		type   = GetType(jnItem);
		if(type == CONTENT_PICTURE || type == CONTENT_TEXT)
		{
			return index;
		}
		index--;
	}
	return -1;
}

PMInt PMEdit::GetContentJsonIndex( PMInt iIndex )
{
	PMInt index  = iIndex;
	PMInt size   = mJnData->GetChildCount();
	PMInt type;
	PMInt nCount = 0;
	while (index<size)
	{
		type   = GetType(&mJnData->At(index));
		if(type == CONTENT_PICTURE || type == CONTENT_TEXT)
		{			
			return nCount;
		}
		nCount++;
		index++;
	}
	return nCount;
}

TJSON* PMEdit::GetLastCommand()
{
	TJSON *Result = NULL;
	if(mUndoList.size()>0&&mRedoList.empty())
	{
		Result = mUndoList.back();
		if (Result->At(0).ToInt()!=PMEDITOR_COMMAND_CHARACTERMERGE || mCaret.mLocation!=Result->At(2).At(0))
		{			
			Result = NULL;
		}
	}
	return Result;
}

#define SETPOSISIONTOLD(LD) if(LD.Empty())\
{\
	if(LDBegin)\
{\
	LD = *LDBegin;\
}else\
{\
	LDBegin = &LD;\
	SetBeginPosition(LD);\
}\
}else if(mLastLD.mJnIndex == -1)\
{\
	if(LDEnd)\
{\
	LD = *LDEnd;\
}else\
{\
	LDEnd = &LD;\
	SetEndPosition(LD);\
}\
}else\
{\
	LDS.push_back(&LD);\
}

PMBool PMEdit::HasPosition(Positions &LDS,PMInt jnIndex)
{
	Positions::iterator  it;
	for (it = LDS.begin(); it != LDS.end(); it++)
	{
		if((*it)->mJnIndex <= jnIndex)
		{
			return true;
		}
	}
	return false;
}

PMVoid PMEdit::ReCalcLinePosition(Positions &LDS,ECLine *Line)
{
	ECElements::iterator itElementB = Line->mElements.begin();
	ECElements::iterator itElementE = Line->mElements.end();
	Positions::iterator  it;
	ECElement *Element = NULL;
	PMInt	   iLen    = 0;
	Position  *post    = NULL;
	ECLine    *l	   = NULL;

	for (; itElementB!=itElementE; itElementB++)
	{
		Element = &(*itElementB);
		if (Element->mType == CONTENT_TEXT)
		{
			for (it = LDS.begin(); it != LDS.end();)
			{
				post = (*it);
				if(post->mJnIndex <= Element->mJnIndex )
				{
					if(post->mOriOffset + post->mOffset <= Element->mOffset + Element->mCount)
					{
						post->mJnIndex = Element->mJnIndex;
						SetFont(Element->mJnFont);
						post->Assign(Element);
						post->mRow        = Line->mRow;
						post->mCol        = itElementB - Line->mElements.begin();
						post->mOffset     = post->mOffset - Element->mOffset;
						if(Element->mIntID == TOKEN_ENTER && post->mOffset == Element->mCount)//下一行
						{
							l = mEDLines[Line->mRow+1];
							post->mPositionX  = 0;
							post->mPositionY  = l->mY + l->mHeight;
						}else
						{
							post->mPositionX  = Element->mBoundary.left + GetStringWidth(Element->mStringID,post->mOffset);
							post->mPositionY  = Element->mBoundary.bottom;
						}
						post->mFontHeight = mFontHeight;
						it = LDS.erase(it);
						if(LDS.empty()||!HasPosition(LDS,Line->mElements.back().mJnIndex))return;
					}else
					{
						it++;
					}	
				}else
				{
					it++;
				}
			}
		}else//picture
		{
			for (it = LDS.begin(); it != LDS.end();)
			{
				post = (*it);
				if(post->mJnIndex <= Element->mJnIndex)
				{
					post->mJnIndex = Element->mJnIndex;
					SetFont(Element->mJnFont);
					post->Assign(Element);
					post->mRow        = Line->mRow;
					post->mCol        = itElementB - Line->mElements.begin();					
					if(post->mOffset) post->mPositionX  = Element->mBoundary.right;
					else post->mPositionX  = Element->mBoundary.left;
					post->mPositionY  = Element->mBoundary.bottom;
					post->mFontHeight = mFontHeight;
					it = LDS.erase(it);
					if(LDS.empty()||!HasPosition(LDS,Line->mElements.back().mJnIndex))return;
				}else
				{
					it++;
				}
			}
		}
	}
}
PMVoid PMEdit::ReCalcPosition()
{
	Positions LDS;
	Position *LDBegin = NULL;
	Position *LDEnd   = NULL;
	SETPOSISIONTOLD(mCaret.mLocation);
	SETPOSISIONTOLD(mLastLD);
	if(mSelection.mType!=SELECTED_NONE)
	{
		SETPOSISIONTOLD(mSelection.mFrom);
		SETPOSISIONTOLD(mSelection.mTo);
	}
	if (!LDS.empty())
	{
		Positions::iterator  it;
		ECLines::iterator    itLineB = mEDLines.begin();
		ECLines::iterator    itLineE = mEDLines.end();
		Position  *post    = NULL;
		ECLine    *Line    = NULL;
		PMInt      jnIndex = 0;
		//1、预处理
		for (it = LDS.begin(); it != LDS.end(); it++)
		{
			post = *it;
			post->mOffset    = post->mOriOffset + post->mOffset;
			post->mOriOffset = 0;
		}
		for (; itLineB!=itLineE; itLineB++)
		{
			Line = *itLineB;
			if(!(Line->mElements.empty())&&HasPosition(LDS,Line->mElements.back().mJnIndex))
			{
				ReCalcLinePosition(LDS,Line);
				if(LDS.empty())break;
			}
		}

		if (!LDS.empty())
		{
			Position LD;
			if(LDEnd==NULL)
			{
				SetEndPosition(LD);
				LDEnd = &LD;
			}
			for (it = LDS.begin(); it != LDS.end(); it++)
			{
				**it = *LDEnd;
			}
		}
	}
	CalcSelectionBound();
}

PMVoid PMEdit::PushOldPosition( TJSON &jnFather )
{
	mCaret.mLocation.ToJson(jnFather.Push());
	mLastLD.ToJson(jnFather.Push());
	jnFather.Push(mSelection.mType);
	mSelection.mFrom.ToJson(jnFather.Push());
	mSelection.mTo.ToJson(jnFather.Push());
}

PMVoid PMEdit::PopOldPosition( TJSON &jnFather )
{
	mCaret.mLocation.FromJson(jnFather.At(0));
	mLastLD.FromJson(jnFather.At(1));
	mSelection.mType = jnFather.At(2).ToInt();
	mSelection.mFrom.FromJson(jnFather.At(3));
	mSelection.mTo.FromJson(jnFather.At(4));
}

PMVoid PMEdit::CharacterMergeCommand( PMChar *data )
{	
	TJSON jnData;
	switch (mSelection.mType)
	{
	case -1://全选		
	case 1://范围选择
		jnData.Push(data);
		EditContent(jnData,PMEDITOR_COMMAND_EDIT);	
		break;
	case 2://图片编辑
	case 0://没有选择
		{
			TJSON *jnLastCmd = GetLastCommand();
			if (jnLastCmd)
			{
				TJSON &jnNew	  = jnLastCmd->At(2);//获取范围节点
				TJSON &jnCaret    = jnNew.At(0);
				TJSON &jnNewData  = jnNew.At(1);
				TJSON &jnOffset   = jnCaret.At(3);
				jnOffset		  = jnOffset.ToInt()+strlen(data);//新的位置
				if(jnNewData.GetChildCount()==3)
				{
					TJSON &jnData = jnNewData.At(1);
					jnData = jnData.ToString() + data;
				}else
				{
					TJSON &jnData = jnNewData.At(0);
					jnData = jnData.ToString() + data;
				}			
				mCaret.mLocation.mOffset += strlen(data);
				mLastLD = mCaret.mLocation;
				Refresh(false);
				if(!ScrollToPosition(mCaret.mLocation))Show(true);
			}else
			{
				jnData.Push(data);
				EditContent(jnData,PMEDITOR_COMMAND_CHARACTERMERGE);					
			}			
		}		
		break;
	}			
}

PMVoid PMEdit::SymbolAddCommand( PMChar *data )
{	
	TJSON jnData;
	jnData.Push(data);
	EditContent(jnData,PMEDITOR_COMMAND_EDIT);		
}

PMVoid PMEdit::CommandBack()
{	
	if(!mEDLines.empty())
	{
		TJSON jnData;
		switch (mSelection.mType)
		{
		case -1://全选	
		case 1://范围选择
			EditContent(jnData,PMEDITOR_COMMAND_EDIT);		
			break;
		case 2://图片编辑
		case 0://没有选择
			EditDelete(0);	
			break;
		}	
	}
}

PMVoid PMEdit::CommandDelete()
{	
	if(!mEDLines.empty())
	{
		TJSON jnData;
		switch (mSelection.mType)
		{
		case -1://全选	
		case 1://范围选择
			EditContent(jnData,PMEDITOR_COMMAND_EDIT);		
			break;
		case 2://图片编辑
		case 0://没有选择
			EditDelete(1);	
			break;
		}	
	}
}

PMVoid PMEdit::CommandCut()
{
	DoCopy();
	CommandDelete();
}

PMVoid PMEdit::CommandPaste()
{
	if(mCliboard->IsEDValid())
	{
		TBuffer pack;
		string  Text;	
		TJSON   jnData;
		CxImage*image = NULL;
		TVecStrings vImageFiles;
		if (mCliboard->GetEditor(pack))//json
		{
			TJSON jnEditor;
			jnEditor.FromString(pack.buffer);
			TJSON &jnPictures = jnEditor[PICTURE_SECTION];
			mPictureManager->AddPicture(jnPictures);
			TJSON &Content	  = jnEditor[EDCONTENT_SECTION];

			TJSON *jnFont = mCaret.mLocation.mJnFont;
			if (Content.At(0).Equal(jnFont))//去除重复的字体
			{
				jnData.RangeCopy(Content,1);
				TJSON::DeleteJSON(&Content.At(0));
			}else
			{
				jnData.RangeCopy(Content);
			}
			
			Content.RemoveAll();
			jnEditor.Clear();
		}else if(mCliboard->GetText(Text))  //Text
		{
			jnData.Push(Text);
		}else if(mCliboard->GetImage(pack))//picture
		{
			string name		 = mPictureManager->AddPicture(pack);
			image			 = mPictureManager->At(name);
			TJSON &jnPicture = jnData.Push();
			jnPicture.Push() = name;
			jnPicture.Push() = image->GetWidth();
			jnPicture.Push() = image->GetHeight();			
		}else if(mCliboard->GetImage(image))//picture
		{
			TJSON &jnPicture = jnData.Push();
			TJSON &jnName	 = jnPicture.Push() ;
			jnPicture.Push() = image->GetWidth();
			jnPicture.Push() = image->GetHeight();
			jnName			 = mPictureManager->AddPicture(image);
		}else if(mCliboard->GetImage(vImageFiles))  //Text
		{
			PMImages images;
			mPictureManager->AddPicture(vImageFiles,images);
			images.PushJson(jnData);
		}
		if(!jnData.Empty())
		{
			EditContent(jnData,PMEDITOR_COMMAND_EDIT);			
		}
	}
}

PMVoid PMEdit::SetLDData(Position &LD,TJSON &jnContent,ECElement *ElementFrom,PMInt iOffset,PMInt iIndex)
{
	PMInt    iContent;
	if(jnContent.Empty())//删除
	{
		LD.mJnIndex = ElementFrom->mJnIndex;
		LD.mOffset  = iOffset;
	}
	else//添加
	{
		iContent    = GetContentJsonIndex(jnContent);
		if(iContent==-1)//内容全都是属性
		{
			LD.mJnIndex = ElementFrom->mJnIndex + jnContent.GetChildCount() + iIndex;
		}else//具有能显示的内容
		{
			TJSON   *jnItem      = &jnContent.At(iContent);
			LD.mJnIndex = ElementFrom->mJnIndex + iContent + iIndex;
			if (GetType(jnItem)==CONTENT_PICTURE)
			{
				LD.mOffset = 1;
			}else
			{
				LD.mOffset = strlen(jnItem->ToCString());
			}
		}
	}
}

PMVoid PMEdit::AddFont( TJSON &jnNewData,ECElement *ElementTo,TJSON *jnOldData )
{
	if (ElementTo)
	{
		TJSON *jnFont = NULL;
		PMInt  i = jnNewData.GetChildCount()-1;
		PMBool bDelete = false;
		while (i>=0)
		{
			jnFont = &jnNewData.At(i);
			if(GetType(jnFont)==CONTENT_TEXTATTRIBUTE)//新数据最后一个字体属性是否与下一个字体属性匹配
			{
				break;
			}
			i--;
		}
		if(jnOldData)
		{
			PMInt j = jnOldData->GetChildCount()-1;
			while (j>=0)
			{
				if(&jnOldData->At(j)==ElementTo->mJnFont)//删除的对象里面有遗留的下一个的字体属性，需要补齐
				{
					bDelete = true;
					break;
				}
				j--;
			}
		}
		if ((i>=0 && *ElementTo->mJnFont!=*jnFont)||bDelete)
		{
			jnNewData.Push(*ElementTo->mJnFont);
		}
	}
}

PMVoid PMEdit::EditContent( TJSON &jnContent,PMInt iCommand )
{
	TJSON *jnCmd       = CreateCommand(iCommand);//PMEDITOR_COMMAND_RANGEPASTE
	TJSON &jnOld	   = jnCmd->Push();			//获取范围节点
	TJSON &jnNew	   = jnCmd->Push();			//获取范围节点
	TJSON &jnOldPost   = jnOld.Push();
	TJSON &jnNewPost   = jnNew.Push();
	TJSON &jnOldData   = jnOld.Push();
	TJSON &jnNewData   = jnNew.Push();
	Position LD;
	PushOldPosition(jnOldPost);

	switch (mSelection.mType)
	{
	case -1://全选		
		{
			LD.mJnIndex = -1;						//全选			
			LD.ToJson(jnNewPost);
			jnOldData.RangeCopy(*mJnData,0);		//  data copy
			jnOld.Push(0);							//6 from Index
			jnNewData.RangeCopy(jnContent);
		}
		break;
	case 1://范围选择
		{
			ECElement *ElementFrom = GetElement(mSelection.mFrom);//必须要有
			ECElement *ElementTo   = GetElement(mSelection.mTo);  //可以为NULL
			ECElement *ElementPre  = NULL;
			ECElement *ElementNext = NULL;
			Position   LDPre	   = mSelection.mFrom;
			Position   LDNext	   = mSelection.mTo;
			PMInt      iFrom,iTo;
			PMBool	   bCalc       = true;
			if (ElementFrom->mType==CONTENT_PICTURE)
			{
				if (mSelection.mFrom.mOffset==0)//包括自己
				{
					iFrom = ElementFrom->mJnIndex;
					SetLDData(LD,jnContent,ElementFrom,0,0);	
					ElementPre = GetPreElement(LDPre);
				}else//包括在下一个数据
				{
					iFrom = ElementFrom->mJnIndex + 1;
					SetLDData(LD,jnContent,ElementFrom,1,1);
					bCalc = false;
				}		
			}else if(ElementFrom->mType == CONTENT_TEXT)
			{
				PMChar *strFrom		= ElementFrom->mStringID - ElementFrom->mOffset;
				PMInt   lenFrom     = strlen(strFrom);
				PMInt   nOffsetFrom = mSelection.mFrom.mOriOffset + mSelection.mFrom.mOffset;
				if (nOffsetFrom==0)//完全
				{
					iFrom = ElementFrom->mJnIndex;
					SetLDData(LD,jnContent,ElementFrom,0,0);	
					ElementPre = GetPreElement(LDPre);
				}else if(nOffsetFrom==lenFrom)//完全没有
				{
					iFrom = ElementFrom->mJnIndex + 1;	
					SetLDData(LD,jnContent,ElementFrom,lenFrom,1);	
					bCalc = false;
				}else
				{
					bCalc = false;
					iFrom = ElementFrom->mJnIndex;
					AssignSubStr(jnNewData.Push(),strFrom,nOffsetFrom);	
					SetLDData(LD,jnContent,ElementFrom,nOffsetFrom,1);					
				}
			}
			if (ElementTo)
			{
				jnNewData.RangeCopy(jnContent);
				if (ElementTo->mType==CONTENT_PICTURE)
				{			
					if (mSelection.mTo.mOffset==1)//包括图片
					{
						iTo = ElementTo->mJnIndex;
						ElementNext = GetNextElement(LDNext);
					}else//不包括图片
					{
						iTo = ElementTo->mJnIndex - 1;
						ElementNext = ElementTo;
					}
					if(bCalc&&(HasFont(jnContent)||jnContent.Empty()))//处理前面字体
					{
						if((!ElementPre||ElementPre->mFontIndex!=ElementFrom->mFontIndex)&&
							(!ElementNext||ElementFrom->mFontIndex!=ElementNext->mFontIndex))//判断是否需要删除前面的字体属性
						{
							iFrom = ElementFrom->mFontIndex;
							LD.mJnIndex += iFrom -  ElementFrom->mJnIndex;
						}						
						jnOldData.RangeCopy(*mJnData,iFrom,iTo+1);
						AddFont(jnNewData,ElementTo,&jnOldData);//处理后面字体
					}else
					{
						jnOldData.RangeCopy(*mJnData,iFrom,iTo+1);
						AddFont(jnNewData,ElementTo,&jnOldData);//处理后面字体
					}
					
				}else if(ElementTo->mType == CONTENT_TEXT)
				{
					PMChar *strTo	  = ElementTo->mStringID - ElementTo->mOffset;
					PMInt   lenTo     = strlen(strTo);
					PMInt   nOffsetTo = mSelection.mTo.mOriOffset + mSelection.mTo.mOffset;
					if (nOffsetTo==0)
					{
						if(bCalc&&(HasFont(jnContent)||jnContent.Empty()))//处理前面字体
						{
							if((!ElementPre||ElementPre->mFontIndex!=ElementFrom->mFontIndex)&&
								(ElementFrom->mFontIndex!=ElementTo->mFontIndex))//判断是否需要删除前面的字体属性
							{
								iFrom = ElementFrom->mFontIndex;
								LD.mJnIndex += iFrom -  ElementFrom->mJnIndex;
							}	
						}
						iTo = ElementTo->mJnIndex - 1;
						jnOldData.RangeCopy(*mJnData,iFrom,iTo+1);
						AddFont(jnNewData,ElementTo,&jnOldData);
					}else if(nOffsetTo==lenTo)
					{						
						ElementNext = GetNextElement(LDNext);
						if(bCalc&&(HasFont(jnContent)||jnContent.Empty()))//处理前面字体
						{							
							if((!ElementPre||ElementPre->mFontIndex!=ElementFrom->mFontIndex)&&
								(!ElementNext||ElementFrom->mFontIndex!=ElementNext->mFontIndex))//判断是否需要删除前面的字体属性
							{
								iFrom = ElementFrom->mFontIndex;
								LD.mJnIndex += iFrom -  ElementFrom->mJnIndex;
							}	
						}
						iTo = ElementTo->mJnIndex;
						jnOldData.RangeCopy(*mJnData,iFrom,iTo+1);
						AddFont(jnNewData,ElementNext,&jnOldData);
					}else
					{
						if(bCalc&&(HasFont(jnContent)||jnContent.Empty()))//处理前面字体
						{
							if((!ElementPre||ElementPre->mFontIndex!=ElementFrom->mFontIndex)&&
								(ElementFrom->mFontIndex!=ElementTo->mFontIndex))//判断是否需要删除前面的字体属性
							{
								iFrom = ElementFrom->mFontIndex;
								LD.mJnIndex += iFrom -  ElementFrom->mJnIndex;
							}	
						}
						iTo = ElementTo->mJnIndex;
						jnOldData.RangeCopy(*mJnData,iFrom,iTo+1);
						AddFont(jnNewData,ElementTo,&jnOldData);
						jnNewData.Push() = strTo + nOffsetTo;					
					}
				}			
				jnOld.Push(iFrom);//6
			}else
			{
				if(bCalc&&(HasFont(jnContent)||jnContent.Empty()))
				{
					if((!ElementPre||ElementPre->mFontIndex!=ElementFrom->mFontIndex))//判断是否需要删除前面的字体属性
					{
						iFrom = ElementFrom->mFontIndex;
					}
				}
				LD.mJnIndex = -1;						//全选	
				jnNewData.RangeCopy(jnContent);
				jnOldData.RangeCopy(*mJnData,iFrom);//copy到最后
				jnOld.Push(iFrom);//6		
			}
		}
		break;
	case 2://图片编辑
	case 0://没有选择
		{
			//添加数据，不用处理前面的字体属性
			ECElement *Element     = GetElement(mCaret.mLocation);//必须要有
			if (Element)
			{
				if (Element->mType == CONTENT_PICTURE)//图片类型
				{
					if (mCaret.mLocation.mOffset==0)//在图片前面插入
					{
						if(IsFont(Element->mJnIndex-1)&&HasFont(jnContent))//图片前面json是字体属性,并插入内容有字体
						{
							jnOld.Push(Element->mJnIndex-1);
							SetLDData(LD,jnContent,Element,0,-1);
							jnNewData.RangeCopy(jnContent);
						}else
						{
							jnOld.Push(Element->mJnIndex);
							SetLDData(LD,jnContent,Element,0,0);
							jnNewData.RangeCopy(jnContent);
							AddFont(jnNewData,Element);
						}	
					}else//在图片后面插入
					{
						jnOld.Push(Element->mJnIndex+1);
						SetLDData(LD,jnContent,Element,1,1);	
						jnNewData.RangeCopy(jnContent);
						AddFont(jnNewData,Element);
					}
				}else if(Element->mType == CONTENT_TEXT)
				{
					PMChar *p	   = Element->mStringID - Element->mOffset;
					PMInt   len    = strlen(p);
					PMInt   offset = mCaret.mLocation.mOriOffset + mCaret.mLocation.mOffset;
					if(offset == 0)
					{
						if(IsFont(Element->mJnIndex-1)&&HasFont(jnContent))//文字前面json是字体属性,并插入内容有字体
						{
							jnOld.Push(Element->mJnIndex-1);
							SetLDData(LD,jnContent,Element,0,-1);
							jnNewData.RangeCopy(jnContent);
						}else
						{
							jnOld.Push(Element->mJnIndex); 
							jnNewData.RangeCopy(jnContent);
							SetLDData(LD,jnContent,Element,0,0);
							AddFont(jnNewData,Element);
						}
					}else if(offset == len)
					{
						jnOld.Push(Element->mJnIndex+1); 
						jnNewData.RangeCopy(jnContent);
						SetLDData(LD,jnContent,Element,len,1);
						AddFont(jnNewData,Element);
					}else
					{
						jnOldData.Push(&mJnData->At(Element->mJnIndex));
						jnOld.Push(Element->mJnIndex); 
						AssignSubStr(jnNewData.Push(),p,offset);
						jnNewData.RangeCopy(jnContent);
						AddFont(jnNewData,Element);
						jnNewData.Push() = p + offset;
						SetLDData(LD,jnContent,Element,offset,1);
					}
				}
			}else//在最后面插入
			{
				if(IsFont(mJnData->GetChildCount()-1)&&HasFont(jnContent))
				{
					jnOldData.Push(&mJnData->At(mJnData->GetChildCount()-1));
					jnOld.Push(mJnData->GetChildCount()-1); 
				}else
				{
					jnOld.Push(mJnData->GetChildCount()); 
				}
				LD.mJnIndex = -1;						//全选	
				jnNewData.RangeCopy(jnContent);
			}
		}
		break;
	}	
	LD.ToJson(jnNewPost);
	AddExcute(jnCmd);
	jnContent.RemoveAll();
	jnContent.Clear();
}

PMBool PMEdit::IsFont( PMInt iIndex )
{
	if(iIndex<0||iIndex>=mJnData->GetChildCount())return false;
	return GetType(&mJnData->At(iIndex))==CONTENT_TEXTATTRIBUTE;
}

PMBool PMEdit::HasFont( TJSON &jnContent )
{
	if(jnContent.Empty())return false;
	return GetType(&jnContent.At(0))==CONTENT_TEXTATTRIBUTE;
}

PMVoid PMEdit::ControlDelete()
{
	if (mSelection.mType == SELECTED_NONE)
	{
		PMInt      Type     = TOKEN_NONE;
		Position   LD		= GetNextIdenticalPosition(mCaret.mLocation,Type);
		if(mCaret.mLocation!=LD)
		{
			Position   NewLD   = mCaret.mLocation;
			ECElement *EF	   = GetElement(NewLD);
			ECElement *ET	   = GetElement(LD);
			if(EF)
			{
				TJSON     *jnNew     = new TJSON;
				TJSON     *jnOld     = new TJSON;
				PMInt	   iFrom;
				PMInt	   iTo       = LD.mJnIndex + 1;
				ECElement *ElementTo = ET;
				if(ET&&(ET->mType==CONTENT_PICTURE||*(ET->mStringID+LD.mOffset)==NULL))
				{
					Position PO = LD;
					ElementTo = GetNextElement(PO);
				}
				if(EF->mCount==NewLD.mOffset)//完整的在下一个Element里面
				{
					EF = GetNextElement(NewLD);
				}
				iFrom = EF->mJnIndex;
				if(NewLD.mOffset + NewLD.mOriOffset != 0)
				{
					AssignSubStr(jnNew->Push(),(EF->mStringID-EF->mOffset),NewLD.mOffset + NewLD.mOriOffset);
					jnOld->RangeCopy(*mJnData,iFrom,iTo);
					AddFont(*jnNew,ElementTo,jnOld);
				}else
				{
					DealWithFontData(NewLD,iFrom,ElementTo);
					NewLD.mJnIndex = iFrom;
					jnOld->RangeCopy(*mJnData,iFrom,iTo);
					AddFont(*jnNew,ElementTo,jnOld);
				}
				if(ET&&ET->mType==CONTENT_TEXT&&*(ET->mStringID+LD.mOffset)!=NULL)jnNew->Push(ET->mStringID+LD.mOffset);
				EditContent( NewLD,jnNew,jnOld,iFrom );
			}
		}
	}else
	{
		CommandDelete();
	}
}

PMVoid PMEdit::ControlBack()
{
	if (mSelection.mType == SELECTED_NONE)
	{
		PMInt      Type     = TOKEN_NONE;
		Position   LD		= GetPreIdenticalPosition(mCaret.mLocation,Type);
		if(mCaret.mLocation!=LD)
		{
			Position   NewLD   = LD;
			Position   LDTO	   = mCaret.mLocation;
			ECElement *EF	   = GetElement(NewLD);
			ECElement *ET	   = GetElement(LDTO);
			if(LDTO.mOffset==0)
			{
				ET = GetPreElement(LDTO);
			}
			if(EF)
			{
				TJSON     *jnNew     = new TJSON;
				TJSON     *jnOld     = new TJSON;
				PMInt	   iFrom;
				PMInt	   iTo       = LDTO.mJnIndex + 1;
				ECElement *ElementTo = ET;
				if(ET&&(ET->mType==CONTENT_PICTURE||*(ET->mStringID+LDTO.mOffset)==NULL))
				{
					Position PO = LDTO;
					ElementTo = GetNextElement(PO);
				}
				iFrom = EF->mJnIndex;
				if(NewLD.mOffset + NewLD.mOriOffset != 0)
				{
					AssignSubStr(jnNew->Push(),(EF->mStringID-EF->mOffset),NewLD.mOffset + NewLD.mOriOffset);
					jnOld->RangeCopy(*mJnData,iFrom,iTo);
					AddFont(*jnNew,ElementTo,jnOld);
				}else
				{
					DealWithFontData(NewLD,iFrom,ElementTo);
					NewLD.mJnIndex = iFrom;
					jnOld->RangeCopy(*mJnData,iFrom,iTo);
					AddFont(*jnNew,ElementTo,jnOld);
				}
				if(ET&&ET->mType==CONTENT_TEXT&&*(ET->mStringID+LDTO.mOffset)!=NULL)jnNew->Push(ET->mStringID+LDTO.mOffset);
				EditContent( NewLD,jnNew,jnOld,iFrom );
			}
		}
	}else
	{
		CommandBack();
	}
}

PMVoid PMEdit::EditContent( Position &LD,TJSON *jnNewContent,TJSON *jnOldContent,PMInt iFrom )
{
	TJSON *jnCmd       = CreateCommand(PMEDITOR_COMMAND_EDIT);
	TJSON &jnOld	   = jnCmd->Push();			//获取范围节点
	TJSON &jnNew	   = jnCmd->Push();			//获取范围节点
	TJSON &jnOldPost   = jnOld.Push();
	TJSON &jnNewPost   = jnNew.Push();
	jnOld.Push(jnOldContent);
	jnNew.Push(jnNewContent);
	PushOldPosition(jnOldPost);
	LD.ToJson(jnNewPost);
	jnOld.Push(iFrom);
	AddExcute(jnCmd);
}

PMVoid PMEdit::DealWithFontData( Position LD,PMInt &iFrom )
{
	Position   LD1		   = LD;
	ECElement *Element     = GetElement(LD);
	ECElement *PreElement  = GetPreElement(LD);
	ECElement *NextElement = GetNextElement(LD1);
	if((!PreElement||PreElement->mFontIndex!=Element->mFontIndex)&&(!NextElement||NextElement->mFontIndex!=Element->mFontIndex))
	{
		iFrom = Element->mFontIndex;
	}
}

PMVoid PMEdit::DealWithFontData( Position LD,PMInt &iFrom,ECElement *ElementTo)
{
	ECElement *Element     = GetElement(LD);
	ECElement *PreElement  = GetPreElement(LD);
	if((!PreElement||PreElement->mFontIndex!=Element->mFontIndex)&&(!ElementTo||ElementTo->mFontIndex!=Element->mFontIndex))
	{
		iFrom = Element->mFontIndex;
	}
}

PMVoid PMEdit::EditDelete( PMInt iDirection)
{
	TJSON *jnCmd       = CreateCommand(PMEDITOR_COMMAND_DELETE);//PMEDITOR_COMMAND_RANGEPASTE
	TJSON &jnOld	   = jnCmd->Push();			//获取范围节点
	TJSON &jnNew	   = jnCmd->Push();			//获取范围节点
	TJSON &jnOldPost   = jnOld.Push();
	TJSON &jnNewPost   = jnNew.Push();
	TJSON &jnOldData   = jnOld.Push();
	TJSON &jnNewData   = jnNew.Push();
	Position LD        = mCaret.mLocation;
	PMInt	 nDelete   = 0;
	PMInt    iFrom	   = mCaret.mLocation.mJnIndex;
	PMInt    iTo	   = iFrom + 1;
	PMBool   bCalc	   = true;
	PushOldPosition(jnOldPost);
	if(mSelection.mType == SELECTED_PICTUREEDIT)//删除选择图片
	{
		iFrom = mSelection.mFrom.mJnIndex;
		iTo	  = iFrom + 1;
		DealWithFontData(mSelection.mFrom,iFrom);
		jnOldData.RangeCopy(*mJnData,iFrom,iTo);
		jnOld.Push(iFrom);					//2
		LD.mJnIndex   = iFrom;
		LD.mOriOffset = 0;
		LD.mOffset    = 0;
	}else
	{
		ECElement *Element = GetElement(LD);
		if (iDirection==0)//往前删除
		{
			if (!Element)//在前面一个对象里面
			{
				iTo     = LD.mJnIndex + 1;
				Element = GetPreElement(LD);
				iFrom   = LD.mJnIndex;
				
			}else if(LD.mOffset==0)
			{
				Element = GetPreElement(LD);
				iFrom   = LD.mJnIndex;
				iTo		= iFrom + 1;
			}
			if(!Element) goto LBError;
			if (Element->mType == CONTENT_TEXT)
			{
				
				PMChar *strData   = Element->mStringID - Element->mOffset;
				PMInt   len		  = strlen(strData);
				PMInt   SecondLen = LD.mOriOffset + LD.mOffset;
				nDelete = PMSpaceAlgorithm::GetLastWordSize(strData,SecondLen);
				LD.mOffset -= nDelete;
				if(SecondLen - nDelete > 0)
				{
					bCalc = false;
					AssignSubStr(jnNewData.Push(),strData,SecondLen - nDelete);
				}
				if(SecondLen < len)
				{
					bCalc = false;
					jnNewData.Push() = strData + SecondLen ;
				}
			}else
			{
				LD.mOffset    = 0;
				LD.mOriOffset = 0;
			}
		}else//往后删除
		{
			if(Element&&LD.mOffset==Element->mCount)
			{
				Element = GetNextElement(LD);
				iFrom   = LD.mJnIndex;
				iTo     = iFrom + 1;
			}
			if(!Element) goto LBError;
			if (Element->mType == CONTENT_TEXT)
			{
				PMChar *strData   = Element->mStringID - Element->mOffset;
				PMInt   len		  = strlen(strData);
				PMInt   SecondLen = LD.mOriOffset + LD.mOffset;
				PMChar  *p		  = strData + SecondLen;
				if (*p<0||('\n'==*p&&'\r'==*(p+1))||('\r'==*p&&'\n'==*(p+1)))nDelete = 2;
				else nDelete = 1;					
				if(SecondLen>0)
				{
					bCalc = false;
					AssignSubStr(jnNewData.Push(),strData,SecondLen);
				}
				if(SecondLen + nDelete < len)
				{
					bCalc = false;
					jnNewData.Push() = strData + SecondLen + nDelete;
				}
			}else
			{
				LD.mOffset    = 0;
				LD.mOriOffset = 0;
			}
		}
		if(bCalc)
		{
			DealWithFontData(LD,iFrom);
			LD.mJnIndex = iFrom;
		}
		jnOldData.RangeCopy(*mJnData,iFrom,iTo);
		jnOld.Push(iFrom);
	}
	LD.ToJson(jnNewPost);
	AddExcute(jnCmd);
	return;
LBError:
	TJSON::DeleteJSON(jnCmd);
}

PMVoid PMEdit::OnSelectedIdentical()
{
	PMInt    Type   = TOKEN_NONE;
	Position LDTo   = GetNextIdenticalPosition(mCaret.mLocation,Type);
	Position LDFrom = GetPreIdenticalPosition (mCaret.mLocation,Type);
	if(LDFrom!=LDTo)
	{
		mSelection.mType = SELECTED_SCOPE;
		mSelection.mFrom = LDFrom;
		mSelection.mTo   = LDTo;
		CalcSelectionBound();
		SetCaret(LDTo,false,true);
		mLastLD			 = LDFrom;
		if(!ScrollToPosition(LDTo))Show(true);
	}
	mMouseState = PMEDITOR_MOUSE_DEFAULT;
}

PMEdit::Position PMEdit::GetNextIdenticalPosition( Position LD,PMInt &Type )
{
	Position   Result   = LD;
	ECElement *Element  = GetElement(LD);
	Type = TOKEN_NONE;
	if(!Element)return Result;
	if (LD.mOffset==Element->mCount)//在后面面一个对象里面
	{
		Element = GetNextElement(LD);
	}
	if (Element)
	{
		ECElement   *ValidElement = Element;
		Result   = LD;
		if (Element->mType == CONTENT_TEXT)//连续删除文字
		{
			TTextWordLex lex;
			TTexToken   *token;
			lex.SetData(Element->GetString()+LD.mOffset);
			token = lex.GetToken();
			Type  = token->type;
LBRoot:
			while (Type == token->type)
			{
				Result       = LD;
				ValidElement = Element;
				token        = lex.GetToken();
			}
			Element->Restore();
			if(token->type == TOKEN_NONE)
			{				
				Element = GetNextElement(LD);
				if(Element&&Element->mType == CONTENT_TEXT)
				{						
					lex.SetData(Element->GetString());
					token = lex.GetToken();
					goto LBRoot;
				}
			}
			if(!Element||Element->mType != CONTENT_TEXT)//末尾结束、非文字结束
			{
				Result.mOffset = ValidElement->mCount;
				Result.mPositionX = ValidElement->mBoundary.right;
			}else//不相同结束,计算偏移量
			{
				if(ValidElement!=Element)
				{
					Result.mOffset = ValidElement->mCount;
					if(ValidElement->mIntID == TOKEN_ENTER)
					{
						Result.mPositionX = 0;
						Result.mPositionY = mEDLines[Result.mRow+1]->mY + mEDLines[Result.mRow+1]->mHeight;
					}else
					{
						Result.mPositionX = ValidElement->mBoundary.right;
					}
				}else
				{
					SetFont(ValidElement->mJnFont);
					Result.mOffset = token->p - ValidElement->mStringID;
					Result.mPositionX = ValidElement->mBoundary.left + GetStringWidth(ValidElement->mStringID,Result.mOffset);
				}
			}
		}else//连续删除图片
		{
			Element = GetNextElement(LD);
			while(Element&&Element->mType == CONTENT_PICTURE)
			{
				ValidElement = Element;
				Result  = LD;
				Element = GetNextElement(LD);
			}
			Result.mOffset    = 1;
			Result.mPositionX = ValidElement->mBoundary.right;
			Type = TOKEN_PICTURE;
		}
	}
	return Result;
}

PMEdit::Position PMEdit::GetPreIdenticalPosition( Position LD,PMInt &Type )
{
	Position   Result   = LD;
	if(!IsBeginPosition(LD))
	{
		vector<TTexToken> tokens;
		ECElement *Element  = GetElement(LD);
		if(!Element||LD.mOffset==0)
		{
			Element = GetPreElement(LD);
		}
		if(Element)
		{
			ECElement *ValidElement = Element;
			Result   = LD;
			if(Element->mType == CONTENT_TEXT)
			{
				TTextWordLex lex;
				TTexToken *token;
				PMInt i;
				Element->mC = *(Element->mStringID + LD.mOffset);
				*(Element->mStringID + LD.mOffset) = 0;
				lex.SetData(Element->mStringID);
				token = lex.GetToken();
				while (token->type != TOKEN_NONE)
				{
					tokens.push_back(*token);
					token = lex.GetToken();
				} 
				*(Element->mStringID + LD.mOffset) = Element->mC;
				if(Type==TOKEN_NONE)
				{
					Type = tokens.back().type;
				}else if(Type != tokens.back().type)//不用查找了，已经和后面的类型不一样
				{
					return Result;
				}
LBLOOP:			
				for (i = tokens.size()-1; i>=0; i--)
				{
					if(tokens[i].type!=Type)
					{
						i++;
						goto LBEND;
					}else 
					{
						ValidElement = Element;
						Result = LD;
					}
				}
				Element = GetPreElement(LD);
				if(Element&&Element->mType == CONTENT_TEXT)
				{
					tokens.clear();
					lex.SetData(Element->GetString());
					token = lex.GetToken();
					while (token->type != TOKEN_NONE)
					{
						tokens.push_back(*token);
						token = lex.GetToken();
					} 
					Element->Restore();
					goto LBLOOP;
				}
LBEND:
				if(!Element||Element->mType != CONTENT_TEXT)//末尾结束、非文字结束
				{
					Result.mOffset = 0;
					Result.mPositionX = ValidElement->mBoundary.left;
				}else//不相同结束,计算偏移量
				{
					if(i!=tokens.size())
					{
						SetFont(ValidElement->mJnFont);
						Result.mOffset = tokens[i].p - ValidElement->mStringID;
						Result.mPositionX = ValidElement->mBoundary.left + GetStringWidth(ValidElement->mStringID,Result.mOffset);
					}else
					{
						Result.mOffset = 0;
						Result.mPositionX = ValidElement->mBoundary.left;
					}
				}
			}else 
			{
				if(Type==TOKEN_PICTURE||Type==TOKEN_NONE)
				{
					Element = GetPreElement(LD);
					while(Element&&Element->mType == CONTENT_PICTURE)
					{
						ValidElement = Element;
						Result  = LD;
						Element = GetPreElement(LD);
					}
					Result.mOffset = 0;
					Result.mPositionX = ValidElement->mBoundary.left;
				}
			}
		}
	}
	return Result;
}
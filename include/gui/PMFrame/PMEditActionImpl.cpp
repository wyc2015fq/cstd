#include "StdAfx.h"
#include "PMEdit.h"

const PMChar *COMMONCHARACTER  = " ,./;'[]<>?:\"{}~`!@#$%^&*()+|-=\\";
//******************************************************************
//一、******************下面是事件响应方法************************
//******************************************************************
PMVoid PMEdit::SetSelection( Position &LD )
{
	if (LD==mLastLD)
	{
		mSelection.mType = SELECTED_NONE;
	}
	else if(LD>mLastLD)
	{
		mSelection.mType = SELECTED_SCOPE;//-1全选、0没有选择、1范围、2图片编辑
		mSelection.mFrom = mLastLD;
		mSelection.mTo   = LD;
	}else
	{
		mSelection.mType = SELECTED_SCOPE;//-1全选、0没有选择、1范围、2图片编辑
		mSelection.mFrom = LD;
		mSelection.mTo   = mLastLD;
	}
	CalcSelectionBound();
	SetCaret(LD,false);
}

PMBool PMEdit::ScrollToPosition( Position &LD )
{
	IntRect  ReteRect = GetRetePosition(mReteRect);
	ECLine *Line = mEDLines[LD.mRow];
	if(Line->mHeight>=ReteRect.height())
	{
		return ScrollToLocation(LD.mPositionX,LD.mPositionY);
	}else if(Line->mY<=ReteRect.top)//上面
	{
		return ScrollToLocation(LD.mPositionX,Line->mY);
	}else
	{
		return ScrollToLocation(LD.mPositionX,LD.mPositionY);
	}
}

PMBool PMEdit::ScrollToLocation( PMInt x,PMInt y )
{
	IntRect  ReteRect = GetRetePosition(mReteRect);
	PMBool	 bHChange = false;
	PMBool	 bVChange = false;
	//按照正行显示原则
	if (!ReteRect.inside(x,y))//需要重新计算
	{
		PMInt  di = ReteRect.GetDirection(x,y);
		PMInt  iHRange = 0, iVRange = 0;
		switch(di)
		{
		case DIRECT_LEFT:  
			iHRange = x - mVisibleX;
			if (HScrollView(iHRange))
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_RIGHT: //只需要计算
			iHRange = x - mReteRect.width() - mVisibleX ;
			if (HScrollView(iHRange))
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_TOP:  
			iVRange = y - mVisibleY;
			if (VScrollView(iVRange))
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_BOTTON:	
			iVRange = y - mReteRect.height() - mVisibleY ;
			if (VScrollView(iVRange))
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_RIGHT|DIRECT_TOP:
			iHRange = x - mReteRect.width() - mVisibleX ;
			iVRange = y - mVisibleY ;
			bHChange= HScrollView(iHRange);
			bVChange= VScrollView(iVRange);
			if (bHChange || bVChange)
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_LEFT|DIRECT_TOP:
			iHRange = x - mVisibleX ;
			iVRange = y - mVisibleY ;
			bHChange= HScrollView(iHRange);
			bVChange= VScrollView(iVRange);
			if (bHChange || bVChange)
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_LEFT|DIRECT_BOTTON:	
			iHRange = x - mVisibleX ;
			iVRange = y - mReteRect.height() - mVisibleY;
			bHChange= HScrollView(iHRange);
			bVChange= VScrollView(iVRange);
			if (bHChange || bVChange)
			{
				Show(true);
				return true;
			}
			break;
		case DIRECT_RIGHT|DIRECT_BOTTON:
			iHRange = x - mReteRect.width() - mVisibleX ;
			iVRange = y - mReteRect.height() - mVisibleY;
			bHChange= HScrollView(iHRange);
			bVChange= VScrollView(iVRange);
			if (bHChange || bVChange)
			{
				Show(true);
				return true;
			}
			break;
		}
	}
	return false;
}

PMVoid PMEdit::OnHScrollView( PMInt iChange )
{
	if(HScrollView(iChange))
	{
		Show(true);
	}
}

PMVoid PMEdit::OnVScrollView( PMInt iChange )
{
	if(VScrollView(iChange))
	{
		Show(true);
	}
}

PMVoid PMEdit::OnMouseSelectOutside( TInputData *EventData )
{
	PMInt  di =mReteRect.GetDirection(EventData->mMouseX,EventData->mMouseY);
	PMInt  iHRange = 0, iVRange = 0;
	PMBool bChange = false;
	switch(di)
	{
	case DIRECT_LEFT:  
		iHRange = EventData->mMouseX - mReteRect.left;
		if (HorizonSelected(EventData,iHRange))
		{
			Show(true);
		}
		break;
	case DIRECT_RIGHT: //只需要计算
		iHRange = EventData->mMouseX - mReteRect.right;
		if (HorizonSelected(EventData,iHRange))
		{
			Show(true);
		}
		break;
	case DIRECT_TOP:  
		iVRange = EventData->mMouseY - mReteRect.top;
		if (VerticalSelected(EventData,iVRange))
		{
			Show(true);
		}
		break;
	case DIRECT_BOTTON:	
		iVRange = EventData->mMouseY - mReteRect.bottom;
		if (VerticalSelected(EventData,iVRange))
		{
			Show(true);
		}
		break;
	case DIRECT_RIGHT|DIRECT_TOP:
		iHRange = EventData->mMouseX - mReteRect.right;
		iVRange = EventData->mMouseY - mReteRect.top;
		bChange = HorizonSelected(EventData,iHRange)||VerticalSelected(EventData,iVRange);
		if (bChange)
		{
			Show(true);
		}
		break;
	case DIRECT_LEFT|DIRECT_TOP:
		iHRange = EventData->mMouseX - mReteRect.left;
		iVRange = EventData->mMouseY - mReteRect.top;
		bChange = HorizonSelected(EventData,iHRange)||VerticalSelected(EventData,iVRange);
		if (bChange)
		{
			Show(true);
		}
		break;
	case DIRECT_LEFT|DIRECT_BOTTON:	
		iHRange = EventData->mMouseX - mReteRect.left;
		iVRange = EventData->mMouseY - mReteRect.bottom;
		bChange = HorizonSelected(EventData,iHRange)||VerticalSelected(EventData,iVRange);
		if (bChange)
		{
			Show(true);
		}
		break;
	case DIRECT_RIGHT|DIRECT_BOTTON:
		iHRange = EventData->mMouseX - mReteRect.right;
		iVRange = EventData->mMouseY - mReteRect.bottom;
		bChange = HorizonSelected(EventData,iHRange)||VerticalSelected(EventData,iVRange);
		if (bChange)
		{
			Show(true);
		}
		break;
	}
}

PMBool PMEdit::VerticalSelected( TInputData *EventData,PMInt iChange )
{
	PMBool Result = false;
	IntPoint	MousePoint = GetRetePosition(EventData->mMouseX,EventData->mMouseY);
	Position LD = GetPosition(MousePoint.x,MousePoint.y,-1);
	if (mLastLD!=LD)
	{
		OnSelectionAdd(LD);		
		Result = true;
	}
	if(VScrollView(iChange))
	{		
		Result = true;
	}
	return Result;
}

PMBool PMEdit::HorizonSelected( TInputData *EventData,PMInt iChange )
{
	PMBool Result = false;	
	IntPoint	MousePoint = GetRetePosition(EventData->mMouseX,EventData->mMouseY);
	Position LD = GetPosition(MousePoint.x,MousePoint.y,-1);
	if (mLastLD!=LD)
	{
		OnSelectionAdd(LD);		
		Result = true;
	}
	if(HScrollView(iChange))
	{		
		Result = true;	
	}
	return Result;
}

PMBool PMEdit::HScrollView( PMInt iChange )
{
	if(mSize.width > mReteWidth || mVisibleX != 0)
	{		
		PMInt delta	   = mVisibleX;
		PMInt maxWidth = mSize.width - mReteWidth ;//多留一点空白地方
		mVisibleX += iChange;
		if(mVisibleX < 0) mVisibleX = 0;
		else if(mVisibleX > maxWidth) mVisibleX = maxWidth;
		delta = mVisibleX - delta;
		if(delta!=0)
		{
			return true;
		}	
	}
	return false;
}

PMBool PMEdit::VScrollView( PMInt iChange )
{
	if(mSize.height >mContentHeight || mVisibleY!=0)
	{		
		PMInt delta	    = mVisibleY;
		PMInt maxHeight = mSize.height - mContentHeight ;//多留一点空白地方
		mVisibleY+=iChange;
		if(mVisibleY < 0) mVisibleY=0;
		else if(mVisibleY > maxHeight) mVisibleY = maxHeight;
		delta = mVisibleY - delta;
		if(delta!=0)
		{
			GetIterStartLine();
			return true;
		}	
	}
	return false;
}

PMVoid PMEdit::OnPictureResize( TInputData *EventData )
{
	const PMInt MinAdge = 8;
	DrawPictureEditFrame();
	switch(mPictureResizeArea.mIndex)
	{
	case 1:
		{
			mPictureResizeArea.mNewBoundary.left = mPictureResizeArea.mBoundary.left + EventData->mMouseX - EventData->mOldMouseX;
			mPictureResizeArea.mNewBoundary.top  = mPictureResizeArea.mBoundary.top  + EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.left>=mPictureResizeArea.mNewBoundary.right-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.left = mPictureResizeArea.mNewBoundary.right - MinAdge;
			}
			if (mPictureResizeArea.mNewBoundary.top >=mPictureResizeArea.mNewBoundary.bottom-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.top  = mPictureResizeArea.mNewBoundary.bottom - MinAdge;
			}
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));	
		}
		break;
	case 2:
		{			
			mPictureResizeArea.mNewBoundary.top    = mPictureResizeArea.mBoundary.top + EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.top>=mPictureResizeArea.mNewBoundary.bottom-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.top = mPictureResizeArea.mNewBoundary.bottom - MinAdge;
			}
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		break;
	case 3:
		{
			mPictureResizeArea.mNewBoundary.right     = mPictureResizeArea.mBoundary.right + EventData->mMouseX - EventData->mOldMouseX;
			mPictureResizeArea.mNewBoundary.top       = mPictureResizeArea.mBoundary.top   + EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.right<=mPictureResizeArea.mNewBoundary.left+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.right = mPictureResizeArea.mNewBoundary.left + MinAdge;
			}
			if (mPictureResizeArea.mNewBoundary.top  >=mPictureResizeArea.mNewBoundary.bottom-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.top   = mPictureResizeArea.mNewBoundary.bottom - MinAdge;
			}
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		break;
	case 4:
		{
			mPictureResizeArea.mNewBoundary.left     =  mPictureResizeArea.mBoundary.left  + EventData->mMouseX - EventData->mOldMouseX;		
			if (mPictureResizeArea.mNewBoundary.left>=mPictureResizeArea.mNewBoundary.right-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.left = mPictureResizeArea.mNewBoundary.left+MinAdge;
			}
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		break;
	case 5:
		{
			mPictureResizeArea.mNewBoundary.right     =  mPictureResizeArea.mBoundary.right + EventData->mMouseX - EventData->mOldMouseX;		
			if (mPictureResizeArea.mNewBoundary.right<=mPictureResizeArea.mNewBoundary.left+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.right = mPictureResizeArea.mNewBoundary.right - MinAdge;
			}
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));	
		}
		break;
	case 6:
		{
			mPictureResizeArea.mNewBoundary.left       = mPictureResizeArea.mBoundary.left   + EventData->mMouseX - EventData->mOldMouseX;
			mPictureResizeArea.mNewBoundary.bottom     = mPictureResizeArea.mBoundary.bottom + EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.left  >= mPictureResizeArea.mNewBoundary.right-MinAdge)
			{
				mPictureResizeArea.mNewBoundary.left   = mPictureResizeArea.mNewBoundary.right - MinAdge;
			}
			if (mPictureResizeArea.mNewBoundary.bottom<=mPictureResizeArea.mNewBoundary.top+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.bottom = mPictureResizeArea.mNewBoundary.top + MinAdge;
			}	
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));	
		}
		break;
	case 7:
		{
			mPictureResizeArea.mNewBoundary.bottom     =mPictureResizeArea.mBoundary.bottom +  EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.bottom<=mPictureResizeArea.mNewBoundary.top+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.bottom = mPictureResizeArea.mNewBoundary.top + MinAdge;
			}	
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		break;
	case 8:
		{
			mPictureResizeArea.mNewBoundary.right      = mPictureResizeArea.mBoundary.right   + EventData->mMouseX - EventData->mOldMouseX;
			mPictureResizeArea.mNewBoundary.bottom     = mPictureResizeArea.mBoundary.bottom  + EventData->mMouseY - EventData->mOldMouseY;			
			if (mPictureResizeArea.mNewBoundary.right <= mPictureResizeArea.mNewBoundary.left+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.right   = mPictureResizeArea.mNewBoundary.left + MinAdge;
			}
			if (mPictureResizeArea.mNewBoundary.bottom<=mPictureResizeArea.mNewBoundary.top+MinAdge)
			{
				mPictureResizeArea.mNewBoundary.bottom = mPictureResizeArea.mNewBoundary.top + MinAdge;
			}	
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));	
		}
		break;
	}
	DrawPictureEditFrame();
}

PMVoid PMEdit::OnPictureResizeCompleted( TInputData *EventData )
{
	DrawPictureEditFrame();//取消反向框
	if(mPictureResizeArea.mBoundary!=mPictureResizeArea.mNewBoundary)
	{
		TJSON *jnCmd     = CreateCommand(PMEDITOR_COMMAND_PICTURERESIZE);
		TJSON &jnOld	 = jnCmd->Push();//获取范围节点
		TJSON &jnNew	 = jnCmd->Push();//获取范围节点
		jnOld.Push(GETPOSITIONJSON(mSelection.mFrom));
		jnOld.Push(mPictureResizeArea.mBoundary.width());
		jnOld.Push(mPictureResizeArea.mBoundary.height());
		jnNew.Push(mPictureResizeArea.mNewBoundary.width());
		jnNew.Push(mPictureResizeArea.mNewBoundary.height());
		AddExcute(jnCmd);
	}
}

PMVoid PMEdit::OnIconAreaClick( Position &LD )
{

}

PMVoid PMEdit::OnLineSelected( TInputData *EventData )
{
	Position PB ;
	Position PE ;
	Position LD = GetPosition(EventData);
	if(LD.mPositionY>mLastLD.mPositionY)
	{
		PB = GetLineBeginLocation(mLastLD);
		PE = GetLineEndLocation(LD);
		mLastLD = PB;
		SetCaret(PE,false,true);		
	}else if(LD.mPositionY<mLastLD.mPositionY)
	{
		PB = GetLineBeginLocation(LD);
		PE = GetLineEndLocation(mLastLD);
		mLastLD = PE;
		SetCaret(PB,false,true);
	}else
	{
		PB = GetLineBeginLocation(LD);
		PE = GetLineEndLocation(LD);
		mLastLD = PB;
		SetCaret(PE,false,true);
	}
	mSelection.Clear();
	if(PE!=PB)
	{			
		mSelection.mType = SELECTED_SCOPE;
		mSelection.mFrom = PB;
		mSelection.mTo   = PE;
		CalcSelectionBound();
	}
	if(!ScrollToPosition(mCaret.mLocation))Show(true);
}

PMVoid PMEdit::OnLNAreaClick( Position &LD,TInputData *EventData )
{
	mMouseState = PMEDITOR_MOUSE_LINESELECT;
	Position PB ;
	Position PE ;
	if (EventData->IsShiftPress())
	{
		if(LD.mPositionY>mLastLD.mPositionY)
		{
			PB = GetLineBeginLocation(mLastLD);
			PE = GetLineEndLocation(LD);
			mLastLD = PB;
			SetCaret(PE,false,true);
		}else if(LD.mPositionY<mLastLD.mPositionY)
		{
			PB = GetLineBeginLocation(LD);
			PE = GetLineEndLocation(mLastLD);
			mLastLD = PE;
			SetCaret(PB,false,true);
		}else
		{
			PB = GetLineBeginLocation(LD);
			PE = GetLineEndLocation(LD);
			mLastLD = PB;
			SetCaret(PE,false,true);
		}
	}else
	{
		PB = GetLineBeginLocation(LD);
		PE = GetLineEndLocation(LD);
		mLastLD = PB;
		SetCaret(PE,false,true);
	}
	mSelection.Clear();
	if(PE!=PB)
	{			
		mSelection.mType = SELECTED_SCOPE;
		mSelection.mFrom = PB;
		mSelection.mTo   = PE;
		CalcSelectionBound();
	}
	if(!ScrollToPosition(mCaret.mLocation))Show(true);
}

PMVoid PMEdit::OnReteAreaClick( Position &LD,TInputData *EventData )
{
	if(EventData->IsShiftPress())
	{
		OnSelectionAdd(LD);
	}else
	{				
		IntPoint   pt = GetRetePosition(EventData->mMouseX,EventData->mMouseY);
		ECElement *Element = GetElement(LD);
		if(LD.mType == CONTENT_PICTURE && (Element && Element->mBoundary.inside(pt)))//选择当时的方位,-2行号位置，-1图标位置，0编辑器为空，1中间，2上，3左，4右，5下
		{
			OnPictureClick(LD);	
		}else
		{
			mSelection.Clear();
			SetCaret(LD);	
			mMouseState = PMEDITOR_MOUSE_SELECTED;
		}		
	}
	Show(true);
}

PMVoid PMEdit::OnPictureClick( Position &LD )
{
	mMouseState = PMEDITOR_MOUSE_PICTUREEDIT;
	ECElement *Element = GetElement(LD);
	//设置图片为编辑状态
	mSelection.mType = SELECTED_PICTUREEDIT;
	mSelection.mFrom = LD;
	mSelection.mTo	 = LD;
	mSelection.mFrom.mOffset   = 0;
	mSelection.mTo.mOffset     = 1;
	mSelection.mFrom.mPositionX= Element->mBoundary.left; 
	mSelection.mTo.mPositionX  = Element->mBoundary.right;
	mSelection.mTopBound = Element->mBoundary;
	mSelection.mMidBound.clear();
	mSelection.mBotBound.clear();
	SetCaret(LD);
}

PMVoid PMEdit::OnSelectionAdd( Position &LD )
{
	if(mMouseState == PMEDITOR_MOUSE_PICTUREEDIT && mLastLD==LD)
	{		
	}else  if(mMouseState == PMEDITOR_MOUSE_PICTUREEDIT && mLastLD!=LD)
	{
		mMouseState = PMEDITOR_MOUSE_SELECTED;
		mSelection.mType = SELECTED_SCOPE;
		if(LD>mLastLD)
		{
			mSelection.mFrom = mLastLD;
			mSelection.mTo   = LD;
		}else
		{
			mSelection.mFrom = LD;
			mSelection.mTo   = mLastLD;
		}
	}
	else if (LD==mLastLD)
	{
		mSelection.mType = SELECTED_NONE;
	}
	else if(LD>mLastLD)
	{
		mMouseState = PMEDITOR_MOUSE_SELECTED;
		mSelection.mType = SELECTED_SCOPE;//-1全选、0没有选择、1范围、2图片编辑
		mSelection.mFrom = mLastLD;
		mSelection.mTo   = LD;
	}else
	{
		mMouseState = PMEDITOR_MOUSE_SELECTED;
		mSelection.mType = SELECTED_SCOPE;//-1全选、0没有选择、1范围、2图片编辑
		mSelection.mFrom = LD;
		mSelection.mTo   = mLastLD;
	}
	CalcSelectionBound();
	SetCaret(LD,false);
}

PMVoid PMEdit::DisplayCaret()
{
	if (mCaret.mVisible&&mCaret.mbState)
	{
		IntPoint Loc = GetActualPosition(mCaret.mLocation.mPositionX,mCaret.mLocation.mPositionY);
		RECT R ;
		R.left	  = Loc.x;
		R.right   = Loc.x+1;
		R.top	  = Loc.y - mCaret.mLocation.mFontHeight;
		R.bottom  = Loc.y ;
		if(mReteRect.inside(R))InvertRect(mHdc,&R);
	}
}

PMVoid PMEdit::DrawCaret()
{
	if (mCaret.mVisible)
	{
		mCaret.mbState = !mCaret.mbState;
		IntPoint Loc = GetActualPosition(mCaret.mLocation.mPositionX,mCaret.mLocation.mPositionY);
		RECT R ;
		R.left	  = Loc.x;
		R.right   = Loc.x+1;
		R.top	  = Loc.y - mCaret.mLocation.mFontHeight;
		R.bottom  = Loc.y ;
		if(mReteRect.inside(R))InvertRect(mHdc,&R);	
	}
}

PMVoid PMEdit::HideCaret( HDC pdc )
{
	if (mCaret.mVisible&&mCaret.mbState)
	{
		IntPoint Loc = GetActualPosition(mCaret.mLocation.mPositionX,mCaret.mLocation.mPositionY);
		RECT R ;
		R.left	  = Loc.x;
		R.right   = Loc.x+1;
		R.top	  = Loc.y - mCaret.mLocation.mFontHeight;
		R.bottom  = Loc.y ;
		if(mReteRect.inside(R))InvertRect(mHdc,&R);
		mCaret.mbState = false;
	}
}

PMVoid PMEdit::OnContentDrag( Position &LD )
{
	mMouseState = PMEDITOR_MOUSE_BEGINDRAG;
}

PMVoid PMEdit::OnKeyChar( TInputData *EventData )
{
	PMChar c = EventData->mKey[0];
	//具有替换、插入两个特性
	if ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_')//英文字符,多一个会合并
	{
		CharacterMergeCommand(EventData->mKey);		
	}else if(c<0||strchr(COMMONCHARACTER,c))//中文字符、符号
	{
		SymbolAddCommand(EventData->mKey);
	}else
	{
		switch (EventData->mKey[0])
		{		
		case TInputData::VLK_BACK:   //删除
			{
				CommandBack();
			}
			break;
		case TInputData::VLK_DELETE: //删除
			{
				CommandDelete();
			}
			break;
		case TInputData::VLK_TAB:    //切换焦点  
			{
				SymbolAddCommand(EventData->mKey);
			}
			break;
		case TInputData::VLK_RETURN: //ENTER回车 
			{
				EventData->mKey[0]=0x0D;//回车
				EventData->mKey[1]=0x0A;//换行
				EventData->mKey[2]=0x00;//结束符号
				SymbolAddCommand(EventData->mKey);
			}
			break;		
		}
	}
}

//******************************************************************
//二、******************下面是直接的操作方法************************
//******************************************************************
PMVoid PMEdit::UnExcute( TJSON &pAction )
{
	mRedoList.push_back(&pAction);	
	switch(pAction.At(0).ToInt())
	{
	case PMEDITOR_COMMAND_CHARACTERMERGE:
	case PMEDITOR_COMMAND_DELETE:
	case PMEDITOR_COMMAND_EDIT:
		UnDoEdit(pAction);
		break;
	case PMEDITOR_COMMAND_PICTURERESIZE:
		UnDoPictureResize(pAction);
		break;
	case PMEDITOR_COMMAND_DRAG:
		break;
	}
}


PMVoid PMEdit::Excute( TJSON &pAction )
{
	mUndoList.push_back(&pAction);	
	switch(pAction.At(0).ToInt())
	{
	case PMEDITOR_COMMAND_CHARACTERMERGE:
	case PMEDITOR_COMMAND_DELETE:
	case PMEDITOR_COMMAND_EDIT:
		DoEdit(pAction);
		break;
	case PMEDITOR_COMMAND_PICTURERESIZE:
		DoPictureResize(pAction);
		break;
	case PMEDITOR_COMMAND_DRAG:
		break;
	}
}

PMVoid PMEdit::ClearCommandList( TJSONS &cmdList )
{
	PMBool bRedoList =  &cmdList == &mRedoList;
	TJSONS::iterator it = cmdList.begin();
	TJSONS::iterator itEnd = cmdList.end();
	TJSON *pAction;
	for (;it!=itEnd;it++)
	{
		pAction =*it;
		switch(pAction->At(0).ToInt())
		{
		case PMEDITOR_COMMAND_CHARACTERMERGE:
		case PMEDITOR_COMMAND_DELETE:
		case PMEDITOR_COMMAND_EDIT:
			if(bRedoList)
				pAction->At(1).At(1).RemoveAll();
			else 
				pAction->At(2).At(1).RemoveAll();
			break;
		case PMEDITOR_COMMAND_PICTURERESIZE:
			{
				PMUnint index = 0;
				pAction->At(1).DeleteItem(index,false);
			}			
			break;
		case PMEDITOR_COMMAND_DRAG:
			break;
		}
		TJSON::DeleteJSON(pAction);
	}
	cmdList.clear();

}

//1、有回退操作的方法
PMVoid PMEdit::DoEdit( TJSON &pAction )
{
	TJSON &jnOld = pAction.At(1);
	TJSON &jnNew = pAction.At(2);
	PMInt  iFrom = jnOld.At(2).ToInt();
	mJnData->RangeReplace(iFrom,jnOld.At(1),jnNew.At(1));	
	//2、计算新的json
	mSelection.Clear();
	mCaret.mLocation.FromJson(jnNew.At(0));	
	mCaret.mbState = true;
	mLastLD		   = mCaret.mLocation;
	PMInputManager::GetSingleton()->ResetCaretTimmer(mHWnd);
	Refresh(false);
	if(!ScrollToPosition(mCaret.mLocation))Show(true);
}

PMVoid PMEdit::DoPictureResize( TJSON &pAction )
{
	TJSON &jnOld = pAction.At(1);
	TJSON &jnNew = pAction.At(2);
	TJSON &jnPicture = jnOld.At(0);
	jnPicture.At(1)  = jnNew.At(0);
	jnPicture.At(2)  = jnNew.At(1);
	Refresh(true);
}

PMVoid PMEdit::UnDoEdit( TJSON &pAction )
{
	TJSON &jnOld = pAction.At(1);
	TJSON &jnNew = pAction.At(2);
	PMInt  iFrom = jnOld.At(2).ToInt();
	mJnData->RangeReplace(iFrom,jnNew.At(1),jnOld.At(1));	
	//2、计算新的json
	PopOldPosition(jnOld.At(0));
	mLastLD		   = mCaret.mLocation;
	mCaret.mbState = true;
	PMInputManager::GetSingleton()->ResetCaretTimmer(mHWnd);
	Refresh(false);
	if(!ScrollToPosition(mCaret.mLocation))Show(true);
}

PMVoid PMEdit::UnDoPictureResize( TJSON &pAction )
{
	TJSON &jnOld = pAction.At(1);
	TJSON &jnNew = pAction.At(2);
	TJSON &jnPicture = jnOld.At(0);
	jnPicture.At(1)  = jnOld.At(1);
	jnPicture.At(2)  = jnOld.At(2);
	Refresh(true);
}

//3、无回退的方法
PMVoid PMEdit::DoSelectAll()
{
	if (!mJnData->Empty())
	{
		mSelection.mType = SELECTED_ALL;
		SetBeginPosition(mLastLD);
		SetEndPosition(mCaret.mLocation);	
		mSelection.mFrom = mLastLD;
		mSelection.mTo   = mCaret.mLocation;
		Show(true);
	}	
}

PMVoid PMEdit::DoCopy()
{
	TBuffer pack;
	switch (mSelection.mType)
	{
	case SELECTED_ALL://全选	
		{
			string  Text = GetText(mJnData);			
			GetPack(pack);
			mCliboard->SetText(Text);
			mCliboard->SetEditor(pack);
			mCliboard->Copy();
		}
		break;
	case SELECTED_SCOPE://范围选择
		{
			string Text = GetSelectedText();
			GetSelectedScopePack(pack);
			mCliboard->SetText(Text);
			mCliboard->SetEditor(pack);
			mCliboard->Copy();
		}		
		break;
	case SELECTED_PICTUREEDIT://图片编辑
		{
			ECElement *Element = GetElement(mSelection.mFrom);
			string   name  = Element->mStringID;
			pack		   = mPictureManager->GetData(name);
			CxImage *image = mPictureManager->At(name);
			mCliboard->SetImage(pack);
			mCliboard->SetImage(image->CopyToHandle());
			mCliboard->Copy();
		}
		break;
	}	
}

PMVoid PMEdit::DoEscape()
{
	if (mMouseState == PMEDITOR_MOUSE_PICTURERESIZE)
	{
		DrawPictureEditFrame();
		mMouseState = PMEDITOR_MOUSE_DEFAULT;
		SetCursor(LoadCursor(NULL, IDC_IBEAM));	
	}
}

PMVoid PMEdit::DoDown()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetDownLocation(mSelection.mTo),true,false);
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		case SELECTED_NONE:
			SetCaret(GetDownLocation(mCaret.mLocation),true,false);
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetDownLocation(mCaret.mLocation),true,false);
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		}
	}
}

PMVoid PMEdit::DoUp()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetUpLocation(mSelection.mFrom),true,false);
			mSelection.Clear();
			ScrollToPosition(mCaret.mLocation);		
			break;
		case SELECTED_NONE:
			SetCaret(GetUpLocation(mCaret.mLocation),true,false);
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetUpLocation(mCaret.mLocation),true,false);
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);
			break;
		}
	}
}

PMVoid PMEdit::DoRight()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(mSelection.mTo);
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);
			break;
		case SELECTED_NONE:			
			SetCaret(GetNextLocation(mCaret.mLocation));
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetNextLocation(mCaret.mLocation));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		}
	}
}

PMVoid PMEdit::DoLeft()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(mSelection.mFrom);
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);		
			break;
		case SELECTED_NONE:
			SetCaret(GetPreLocation(mCaret.mLocation));
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetPreLocation(mCaret.mLocation));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		}
	}
}

PMVoid PMEdit::DoPageDown()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		PMInt detaY = GetPageDownDetaY();
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetPageDownLocation(mSelection.mTo,detaY),true,false);
			mSelection.Clear();
			ScrollToPageDownLocaltion(detaY,true);		
			break;
		case SELECTED_NONE:
			SetCaret(GetPageDownLocation(mCaret.mLocation,detaY),true,false);
			ScrollToPageDownLocaltion(detaY,false);
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetPageDownLocation(mCaret.mLocation,detaY),true,false);
			mSelection.Clear();
			ScrollToPageDownLocaltion(detaY,true);	
			break;
		}
	}
}

PMVoid PMEdit::DoPageUp()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		PMInt detaY = GetPageUpDetaY();
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetPageUpLocation(mSelection.mFrom,detaY),true,false);
			mSelection.Clear();
			ScrollToPageUpLocaltion(detaY,true);		
			break;
		case SELECTED_NONE:
			SetCaret(GetPageUpLocation(mCaret.mLocation,detaY),true,false);
			ScrollToPageUpLocaltion(detaY,false);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetPageUpLocation(mCaret.mLocation,detaY),true,false);
			mSelection.Clear();
			ScrollToPageUpLocaltion(detaY,true);	
			break;
		}
	}
}

PMInt PMEdit::GetPageDownDetaY()
{
	PMInt iSize  = mEDLines.size();
	PMInt iLine  = 0;
	if(iLine>=iSize||mEDLines.empty())return 0;
	PMInt Result = 0;	
	PMInt y      = 0;
	if(mCaret.mLocation.mPositionY>=mVisibleY && mCaret.mLocation.mPositionY <= mVisibleY + mContentHeight)//当前光标可视
	{
		y        = mVisibleY + mContentHeight;
		iLine    = mVisibleLine;
	}else
	{
		y        = mCaret.mLocation.mPositionY + mContentHeight;
		iLine    = mCaret.mLocation.mRow;
	}
	while (iLine<iSize)
	{
		if(mEDLines[iLine]->mY + mEDLines[iLine]->mHeight >= y)
		{
			Result = mEDLines[iLine]->mY - mVisibleY;
			break;
		}
		iLine++;
	}
	if(iLine>=iSize)
	{
		Result = mEDLines[iSize-1]->mY + mEDLines[iSize-1]->mHeight - mVisibleY + RESERVECONTENT;
	}
	return Result;
}

PMVoid PMEdit::ScrollToPageDownLocaltion(PMInt detaY,PMBool bShow)
{
	PMBool ResultV = false,ResultH = false;
	ResultV = VScrollView(detaY);	
	if(mCaret.mLocation.mPositionX<=mVisibleX)
	{
		ResultH = HScrollView(mCaret.mLocation.mPositionX-mVisibleX);
	}else if(mCaret.mLocation.mPositionX>=mVisibleX+mContentWidth)
	{
		ResultH = HScrollView(mCaret.mLocation.mPositionX-mVisibleX-mContentWidth);
	}
	if(ResultV||ResultH||bShow)Show(true);	
}

PMInt PMEdit::GetPageUpDetaY()
{
	PMInt iLine  = 0;	
	PMInt Result = 0;	
	PMInt y      = 0;
	if(mCaret.mLocation.mPositionY>=mVisibleY && mCaret.mLocation.mPositionY <= mVisibleY + mContentHeight)//当前光标可视
	{
		y        = mVisibleY - mContentHeight;
		iLine    = mVisibleLine;
	}else
	{
		y        = mCaret.mLocation.mPositionY - mContentHeight;
		iLine    = mCaret.mLocation.mRow;
	}
	if(iLine<=0||mEDLines.empty())return 0;
	while (iLine>=0)
	{
		if(mEDLines[iLine]->mY <= y)
		{
			Result = mVisibleY - mEDLines[iLine]->mY - mEDLines[iLine]->mHeight;
			break;
		}
		iLine--;
	}
	if(iLine<0)
	{
		Result = mVisibleY;
	}
	return Result;
}

PMVoid PMEdit::ScrollToPageUpLocaltion(PMInt DeltaY,PMBool bShow)
{
	PMBool   ResultV = false,ResultH = false;
	ResultV = VScrollView(-DeltaY);
	if(mCaret.mLocation.mPositionX<=mVisibleX)
	{
		ResultH = HScrollView(mCaret.mLocation.mPositionX-mVisibleX);
	}else if(mCaret.mLocation.mPositionX>=mVisibleX+mContentWidth)
	{
		ResultH = HScrollView(mCaret.mLocation.mPositionX-mVisibleX-mContentWidth);
	}
	if(ResultV||ResultH||bShow)Show(true);	
}

PMEdit::Position PMEdit::GetPageUpLocation( Position &LD,PMInt DeltaY )
{
	if(LD.mRow>0)
	{
		return GetPosition(mCaret.mLastX,LD.mPositionY - DeltaY ,-1);
	}
	return LD;
}

PMEdit::Position PMEdit::GetPageDownLocation( Position &LD,PMInt DeltaY )
{
	if((PMUnint)LD.mRow<mEDLines.size()-1)
	{
		return GetPosition(mCaret.mLastX,LD.mPositionY + DeltaY ,-1);//- 2*mFontHeight
	}
	return LD;
}

PMVoid PMEdit::DoHome()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetLineBeginLocation(mSelection.mFrom));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);		
			break;
		case SELECTED_NONE:
			SetCaret(GetLineBeginLocation(mCaret.mLocation));
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetLineBeginLocation(mCaret.mLocation));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		}
	}
}

PMVoid PMEdit::DoEnd()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		switch(mSelection.mType)//-1全选、0没有选择、1范围、2图片编辑
		{
		case SELECTED_ALL:
		case SELECTED_SCOPE:
			SetCaret(GetLineEndLocation(mSelection.mTo));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);		
			break;
		case SELECTED_NONE:
			SetCaret(GetLineEndLocation(mCaret.mLocation));
			ScrollToPosition(mCaret.mLocation);	
			break;
		case SELECTED_PICTUREEDIT:
			SetCaret(GetLineEndLocation(mCaret.mLocation));
			mSelection.Clear();
			if(!ScrollToPosition(mCaret.mLocation))Show(true);	
			break;
		}
	}
}

PMVoid PMEdit::DoInsert()
{

}

PMVoid PMEdit::DoSave()
{
	mjnFile->Write("d:\\EditorData.json",true);
}

PMEdit::Position PMEdit::GetPreLocation( Position &LD )
{
	Position Result = LD;
	ECElement *Element = GetElement(Result);
	if (!Element||Result.mOffset==0)//在前面一个对象里面
	{
		Element = GetPreElement(Result);
	}
	if(Element)
	{
		if (Element->mType == CONTENT_TEXT)
		{
			if(Element->mIntID == TOKEN_ENTER)//往下一行移动
			{
				SetFont(Element->mJnFont);
				Result.mOffset        = 0; 
				Result.mPositionX     = Element->mBoundary.left;
				Result.mPositionY     = Element->mBoundary.bottom;
			}else
			{
				PMInt   nMove	  = PMSpaceAlgorithm::GetLastWordSize(Element->mStringID,Result.mOffset);
				SetFont(Element->mJnFont);
				Result.mOffset       -= nMove; 
				Result.mPositionX     = Element->mBoundary.left + GetStringWidth(Element->mStringID,Result.mOffset);
			}
		}else
		{
			Result.mOffset    = 0;
			Result.mOriOffset = 0;
			Result.mPositionX = Element->mBoundary.left;
		}
	}
	return Result;
}

PMEdit::Position PMEdit::GetNextLocation( Position &LD )
{
	Position Result = LD;
	ECElement *Element = GetElement(Result);
	if (Element&&Result.mOffset==Element->mCount)//在后面面一个对象里面
	{
		Element = GetNextElement(Result);
	}
	if(Element)//已经是最前面的一个对象了
	{
		if (Element->mType == CONTENT_TEXT)
		{			
			if(Element->mIntID == TOKEN_ENTER)//往下一行移动
			{
				ECLine *Line = mEDLines[Result.mRow+1];
				SetFont(Element->mJnFont);
				Result.mOffset        = Element->mCount; 
				Result.mPositionX     = 0;
				Result.mPositionY     = Line->mY + Line->mHeight;
			}else
			{
				PMInt nMove;
				if (*(Element->mStringID + Result.mOffset)<0)nMove = 2;
				else nMove = 1;	
				SetFont(Element->mJnFont);
				Result.mOffset       += nMove; 
				Result.mPositionX     = Element->mBoundary.left + GetStringWidth(Element->mStringID,Result.mOffset);
			}
		}else
		{
			Result.mOffset    = 1;
			Result.mOriOffset = 0;
			Result.mPositionX = Element->mBoundary.right;
		}
	}
	return Result;
}

PMEdit::Position PMEdit::GetUpLocation( Position &LD )
{
	if(LD.mRow>0)
	{
		ECLine *Line = mEDLines[LD.mRow];
		return GetPosition(mCaret.mLastX,LD.mPositionY - Line->mHeight,-1);
	}
	return LD;
}

PMEdit::Position PMEdit::GetDownLocation( Position &LD )
{
	if((PMUnint)LD.mRow<mEDLines.size()-1)
	{
		return GetPosition(mCaret.mLastX,LD.mPositionY + 2,-1);
	}
	return LD;
}

PMEdit::Position PMEdit::GetLineBeginLocation( Position &LD )
{
	return GetPosition(0,LD.mPositionY,-1);
}

PMEdit::Position PMEdit::GetLineEndLocation( Position &LD )
{
	ECLine *Line = mEDLines[LD.mRow];
	return GetPosition(Line->mWidth,LD.mPositionY,-1);
}

PMVoid PMEdit::ControlHome()
{
	mSelection.Clear();
	SetBeginPosition(mLastLD);
	SetCaret(mLastLD,false,true);
	ScrollToPosition(mLastLD);
}

PMVoid PMEdit::ControlEnd()
{
	mSelection.Clear();
	SetEndPosition(mLastLD);
	SetCaret(mLastLD,false,true);
	ScrollToPosition(mLastLD);
}

PMVoid PMEdit::ShiftDown()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetDownLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::ShiftUp()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetUpLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::ShiftRight()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetNextLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::ShiftLeft()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetPreLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::ShiftPageDown()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		PMInt detaY = GetPageDownDetaY();
		Position LD = GetPageDownLocation(mCaret.mLocation,detaY);
		SetSelection(LD);
		ScrollToPageDownLocaltion(detaY,true);
	}
}

PMVoid PMEdit::ShiftPageUp()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		PMInt detaY = GetPageUpDetaY();
		Position LD = GetPageUpLocation(mCaret.mLocation,detaY);
		SetSelection(LD);
		ScrollToPageUpLocaltion(detaY,true);
	}
}

PMVoid PMEdit::ShiftHome()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetLineBeginLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::ShiftEnd()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		Position LD = GetLineEndLocation(mCaret.mLocation);
		SetSelection(LD);
		if(!ScrollToPosition(mCaret.mLocation))Show(true);
	}
}

PMVoid PMEdit::AltRight()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		IntRect RT;
		if(ExcuteForward(RT))MoveView(RT);	
	}
}

PMVoid PMEdit::AltLeft()
{
	if (mMouseState==PMEDITOR_MOUSE_DEFAULT)
	{
		IntRect RT;
		if(ExcuteBackward(RT))MoveView(RT);
	}
}

PMVoid PMEdit::MoveView( const IntRect &RT )
{
	PMBool bCalc = false;
	Position LD  = GetPosition(RT.left,RT.top,-1);	
	SetCaret(LD);
	if(!(mVisibleX == RT.right && mVisibleY == RT.bottom && mSelection.mType == SELECTED_NONE))
	{
		mSelection.Clear();
		if(mVisibleX != RT.right )
		{			
			mVisibleX = RT.right;	
		}
		if(LD.mPositionX<=mVisibleX||LD.mPositionX>=mVisibleX + mReteWidth)
		{
			mVisibleX = LD.mPositionX;
		}
		if(mVisibleY != RT.bottom)
		{
			mVisibleY = RT.bottom;
			bCalc = true;
		}
		if(LD.mPositionY<=mVisibleY||LD.mPositionY>=mVisibleY + GetTotalHeight())
		{
			mVisibleY = LD.mPositionY;
			bCalc = true;
		}
		if(bCalc)GetIterStartLine();	
		Show(true);
	}
}
